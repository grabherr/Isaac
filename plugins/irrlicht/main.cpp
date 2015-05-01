/** Irrlicht server


Note that the Terrain Renderer in Irrlicht is based on Spintz'
GeoMipMapSceneNode, lots of thanks go to him. DeusXL provided a new elegant
simple solution for building larger area on small heightmaps -> terrain
smoothing.

In the beginning there is nothing special. We include the needed header files
and create an event listener to listen if the user presses a key: The 'W' key
switches to wireframe mode, the 'P' key to pointcloud mode, and the 'D' key
toggles between solid and detail mapped material.
*/
#include <irrlicht.h>
#include "driverChoice.h"
#include <math.h>

#include "util/StreamComm.h"
#include "graphics/Messages.h"
#include "base/ThreadHandler.h"

#define IPI 3.141592653589793

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};


double Dist(const core::vector3df & a, const core::vector3df & b) {
  double d = 0.;
  d += (a.X - b.X) * (a.X - b.X);
  d += (a.Y - b.Y) * (a.Y - b.Y);
  d += (a.Z - b.Z) * (a.Z - b.Z);
  //std::cout << a.X << " " << b.X << " " << a.Y << " " << b.Y << " " << a.Z << " " << b.Z << std::endl;
  return sqrt(d);
}


class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(scene::ISceneNode* terrain, scene::ISceneNode* skybox, scene::ISceneNode* skydome) :
		Terrain(terrain), Skybox(skybox), Skydome(skydome), showBox(true), showDebug(false)
	{
		Skybox->setVisible(showBox);
		Skydome->setVisible(!showBox);
                for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                        KeyIsDown[i] = false;
	}
       // This is used to check whether a key is being held down
        virtual bool IsKeyDown(EKEY_CODE keyCode) const
        {
                return KeyIsDown[keyCode];
        }

	bool OnEvent(const SEvent& event)
	{
                if (event.EventType == irr::EET_KEY_INPUT_EVENT)
                        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		// check if user presses the key 'W' or 'D'
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_KEY_Q: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_WIREFRAME,
						!Terrain->getMaterial(0).Wireframe);
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
				return true;
			case irr::KEY_KEY_P: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD,
						!Terrain->getMaterial(0).PointCloud);
				Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
				return true;
			case irr::KEY_KEY_C: // toggle detail map
				Terrain->setMaterialType(
					Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
					video::EMT_DETAIL_MAP : video::EMT_SOLID);
				return true;
			case irr::KEY_KEY_V: // toggle skies
				showBox=!showBox;
				Skybox->setVisible(showBox);
				Skydome->setVisible(!showBox);
				return true;
			case irr::KEY_KEY_X: // toggle debug information
				showDebug=!showDebug;
				Terrain->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
				return true;
			case irr::KEY_KEY_Z: // toggle debug information
			        exit(0);
				return true;
			default:
				break;
			}
		}

		return false;
	}

private:
	scene::ISceneNode* Terrain;
	scene::ISceneNode* Skybox;
	scene::ISceneNode* Skydome;
	bool showBox;
	bool showDebug;
        bool KeyIsDown[KEY_KEY_CODES_COUNT];

};


//=================================================================
class AnimModel
{
public:
  AnimModel() {
    m_pModel = NULL;
    m_speed = 20.;
  }
  const StreamCoordinates & GetCoords() const {return  m_coords;}
  void SetCoords(const StreamCoordinates &c) {
    m_coords = c;
    m_pModel->setPosition(core::vector3df(m_coords[0], m_coords[1], m_coords[2]));
  }

  void SetDirection(const StreamCoordinates &c) {
    core::vector3df dir;
    SphereCoordinates sp = c.AsSphere();
    dir.Y = 360.*sp.phi()/2./IPI;
    dir.X = dir.Z = 0.;
    cout << "Direction: " << c[0] << " " << c[1] << " " << c[2] << " -> " << dir.Y << endl;
    m_pModel->setRotation(dir);
  }

  const string & GetType() const {return m_type;}
  const string & GetName() const {return m_name;}

  void SetName(const string & n) {m_name = n;}
  void SetType(const string & t) {m_type = t;}

  void SetAnimation(const string & anim, double speed) {
    if (anim != m_anim || speed != m_speed) {
      m_pModel->setMD2Animation(anim.c_str());
      m_pModel->setAnimationSpeed(speed);      
    }
    m_speed = speed;
    m_anim = anim;
  }

  scene::IAnimatedMeshSceneNode* m_pModel;
 
private:
  
  string m_type;
  string m_name;
  StreamCoordinates m_coords;
  string m_anim;
  double m_speed;
};



//========================================================================
class IrrlichtServer
{
public:
  IrrlichtServer();
  ~IrrlichtServer() {
    if (receiver != NULL)
      delete receiver;
  }

  void AddCamera(double x, double y, double z);
  void AddTerrain();
  void AddFairy();
  void AddLamp();
  void AddSceneNodes();

  void Run();
  void ProcessMessage(const string & type, DataPacket & d);
  
  void SetCameraPosition(const StreamCoordinates & c);
  void SetCameraRotation(const StreamCoordinates & c);
  void SetCameraTilt(double angle);

  void GetCameraPosition(StreamCoordinates & c);
  void GetCameraRotation(StreamCoordinates & c);
  void GetCameraTilt(StreamCoordinates & c);

protected:
  int GetModelIndex(const string & name) {
    int i;
    for (i=0; i<m_anim.isize(); i++) {
      if (m_anim[i].GetName() == name)
	return i;
    }
    return -1;
  }

  scene::ICameraSceneNode* camera;
  IrrlichtDevice* device;
  scene::ITerrainSceneNode* terrain;

  video::IVideoDriver* driver;
  scene::ISceneManager* smgr;
  gui::IGUIEnvironment* env;
  scene::IAnimatedMeshSceneNode* fairy;
  video::SMaterial material;
  
  svec<AnimModel> m_anim;

  MyEventReceiver * receiver;

  double xp;
  double yp;
  double zp;

  //ThreadHandler m_threadHandler;
  //SharedData m_data;
  StreamCommTransmitter * m_pTrans;
  StreamCommReceiver * m_pRec; 

};


IrrlichtServer::IrrlichtServer()
{

  m_pTrans = GetTransmitter("localhost", DEFPORT+1);
  m_pRec = GetReceiver(DEFPORT);

  camera = NULL;
  terrain = NULL;
  receiver = NULL;
  driver = NULL;
  smgr = NULL;
  env = NULL;
  fairy = NULL;
  

  xp = yp = zp = 0.;

  // ask user for driver
  video::E_DRIVER_TYPE driverType=video::EDT_OPENGL; /*driverChoiceConsole()*/
  std::cout << "Using driver type " << driverType << std::endl;
  if (driverType==video::EDT_COUNT)
    exit(-1);
  
  // create device with full flexibility over creation parameters
  // you can add more parameters if desired, check irr::SIrrlichtCreationParameters
  irr::SIrrlichtCreationParameters params;
  params.DriverType=driverType;
  //params.Fullscreen=true;
  
  params.WindowSize=core::dimension2d<u32>(640, 480);
  //params.WindowSize=core::dimension2d<u32>(1024, 768);
  
  //params.WindowSize=core::dimension2d<u32>(1680, 1050);
  
  device = createDeviceEx(params);
  //device->maximizeWindow();
  
  if (device == 0)
    exit(-1); // could not create selected driver.
  
  /*
    First, we add standard stuff to the scene: A nice irrlicht engine
    logo, a small help text, a user controlled camera, and we disable
    the mouse cursor.
  */
  driver = device->getVideoDriver();
  smgr = device->getSceneManager();
  env = device->getGUIEnvironment();

  
  std::cout << "Start communication thread..." << std::endl;
  string init = "";
  //m_threadHandler.AddThread(new IrrThread(&m_data), init); 
  //m_threadHandler.Feed(0, init);

  std::cout << "Constructor done " << std:: endl;
}

void IrrlichtServer::AddCamera(double x, double y, double z)
{
  std::cout << "Start AddCamera " << std:: endl;
  xp = 2700*2;
  yp = 255*2;
  zp = 2600*2;
  
  // add camera
  std::cout << "1" << std:: endl;
  camera = smgr->addCameraSceneNodeFPS(0,100.0f,1.2f);
  std::cout << "2" << std:: endl;
  camera->setPosition(core::vector3df(2700*2-2000,255*2+1300,2600*2-900));
  std::cout << "3" << std:: endl;
  camera->setRotation(core::vector3df(0., 270, 0.));
  
  
  std::cout << "4" << std:: endl;
  camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
  std::cout << "5" << std:: endl;
  camera->setFarValue(42000.0f);
  
  // disable mouse cursor
  std::cout << "6" << std:: endl;
  device->getCursorControl()->setVisible(false);
  std::cout << "AddCamera done " << std:: endl;

}

void IrrlichtServer::AddTerrain()
{

  // add terrain scene node
  terrain = smgr->addTerrainSceneNode(
				      "/home/manfred/Work/Models/Terrain/terrain-heightmap-empty.bmp",
				      /*"../../media/terrain-heightmap.bmp",*/
				      0,					// parent node
				      -1,					// node id
				      core::vector3df(0.f, 0.f, 0.f),		// position
				      core::vector3df(0.f, 0.f, 0.f),		// rotation
				      core::vector3df(40.f, 4.4f, 40.f),	// scale
				      video::SColor ( 255, 255, 255, 255 ),	// vertexColor
				      5,					// maxLOD
				      scene::ETPS_17,				// patchSize
				      4					// smoothFactor
				      );
  
  terrain->setMaterialFlag(video::EMF_LIGHTING, false);
  
  //terrain->setMaterialTexture(0,
  //		driver->getTexture("../../media/terrain-texture.jpg"));
  terrain->setMaterialTexture(0,
			      driver->getTexture("/home/manfred/Work/Bots/Images/sweden1.jpg"));
  terrain->setMaterialTexture(1,
			      driver->getTexture("../../media/detailmap3.jpg"));
  
  terrain->setMaterialType(video::EMT_DETAIL_MAP);
  
  terrain->scaleTexture(1.0f, 20.0f);
  
  
  
  /*
    To be able to do collision with the terrain, we create a triangle selector.
    If you want to know what triangle selectors do, just take a look into the
    collision tutorial. The terrain triangle selector works together with the
    terrain. To demonstrate this, we create a collision response animator
    and attach it to the camera, so that the camera will not be able to fly
    through the terrain.
  */
  
  // create triangle selector for the terrain	
  scene::ITriangleSelector* selector
    = smgr->createTerrainTriangleSelector(terrain, 0);
  terrain->setTriangleSelector(selector);
  
  // create collision response animator and attach it to the camera
  scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
									  selector, camera, core::vector3df(60,100,60),
									  core::vector3df(0,0,0),
									  core::vector3df(0,50,0));
  selector->drop();
  camera->addAnimator(anim);
  anim->drop();
  
  /* If you need access to the terrain data you can also do this directly via the following code fragment.
   */
  scene::CDynamicMeshBuffer* buffer = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
  terrain->getMeshBufferForLOD(*buffer, 0);
  video::S3DVertex2TCoords* data = (video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();
  // Work on data or get the IndexBuffer with a similar call.
  buffer->drop(); // When done drop the buffer again.
  
  
  /*
    To make the user be able to switch between normal and wireframe mode,
    we create an instance of the event receiver from above and let Irrlicht
    know about it. In addition, we add the skybox which we already used in
    lots of Irrlicht examples and a skydome, which is shown mutually
    exclusive with the skybox by pressing 'S'.
  */
  
  // create skybox and skydome
  driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
  
  scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
						     /*driver->getTexture("../../media/irrlicht2_up.jpg"),
						       driver->getTexture("../../media/irrlicht2_dn.jpg"),
						       driver->getTexture("../../media/irrlicht2_lf.jpg"),
						       driver->getTexture("../../media/irrlicht2_rt.jpg"),
						       driver->getTexture("../../media/irrlicht2_ft.jpg"),
						       driver->getTexture("../../media/irrlicht2_bk.jpg"));*/
						     
						     driver->getTexture("/home/manfred/Work/Bots/Images/bluesky_up.jpg"),
						     driver->getTexture("/home/manfred/Work/Bots/Images/bluesky_dn.jpg"),
						     driver->getTexture("/home/manfred/Work/Bots/Images/bluesky_lf.jpg"),
						     driver->getTexture("/home/manfred/Work/Bots/Images/bluesky_rt.jpg"),
						     driver->getTexture("/home/manfred/Work/Bots/Images/bluesky_ft.jpg"),
						     driver->getTexture("/home/manfred/Work/Bots/Images/bluesky_bk.jpg"));
  
  
  scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"),16,8,0.95f,2.0f);
  
  driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
  
  // create event receiver
  receiver = new MyEventReceiver(terrain, skybox, skydome);
  device->setEventReceiver(receiver);
  std::cout << "AddTerrain done " << std:: endl;

}

void IrrlichtServer::AddFairy()
{
  //=========================================================================================
  /* Add 1 animated hominid, which we can pick using a ray-triangle intersection.
     They all animate quite slowly, to make it easier to see that accurate triangle
     selection is being performed. */
  
  
  // Add an MD2 node, which uses vertex-based animation.
  fairy = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/faerie.md2"),
					 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
  //fairy->setPosition(core::vector3df(-90,-15,-140)); // Put its feet on the floor.
  fairy->setPosition(core::vector3df(xp, 100, zp)); // Put its feet on the floor.

  // Around axes
  fairy->setRotation(core::vector3df(0., 180, 0));

  fairy->setScale(core::vector3df(3.6f)); // Make it appear realistically scaled
  fairy->setMD2Animation(scene::EMAT_POINT);
  fairy->setAnimationSpeed(20.f);
  //fairy->setAnimationSpeed(70.f);
  material.setTexture(0, driver->getTexture("../../media/faerie2.bmp"));
  material.Lighting = false;
  material.NormalizeNormals = true;
  fairy->getMaterial(0) = material;
  std::cout << "AddFairy done " << std:: endl;
}


void IrrlichtServer::AddSceneNodes()
{
  scene::IMeshSceneNode* elm1 = 0;
  
  // Add an MD2 node, which uses vertex-based animation.
  elm1 = smgr->addMeshSceneNode(smgr->getMesh("/home/manfred/Work/Bots/Models/Elm/Elm5.3ds"),
				0, IDFlag_IsPickable | IDFlag_IsHighlightable);
  //fairy->setPosition(core::vector3df(-90,-15,-140)); // Put its feet on the floor.
  elm1->setPosition(core::vector3df(700, 0, 2900)); // Put its feet on the floor.
  elm1->setScale(core::vector3df(1.1f, 1.1f, 1.1f)); // Make it appear realistically scaled
  
  material.setTexture(0, driver->getTexture("/home/manfred/Work/Bots/Models/Elm/iTrees-02_Bark-WillowOld-02.jpg"));
  material.setTexture(1, driver->getTexture("/home/manfred/Work/Bots/Models/Elm/itrees-02-leaf_002.jpg"));
  material.Lighting = false;
  material.NormalizeNormals = true;
  elm1->getMaterial(0) = material;
  
  /*
  scene::IMeshSceneNode* tree7 = 0;
  
  // Add an MD2 node, which uses vertex-based animation.
  tree7 = smgr->addMeshSceneNode(smgr->getMesh("/home/manfred/Work/Bots/Models/Tree7/Tree.3DS"),
				 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
  //fairy->setPosition(core::vector3df(-90,-15,-140)); // Put its feet on the floor.
  tree7->setPosition(core::vector3df(xp-500, yp-3100, zp)); // Put its feet on the floor.
  tree7->setScale(core::vector3df(1.1f, 1.1f, 1.1f)); // Make it appear realistically scaled
  
  material.setTexture(0, driver->getTexture("/home/manfred/Work/Bots/Models/Tree7/itrees-02-leaf_002.jpg"));
  //material.setTexture(1, driver->getTexture("/home/manfred/Work/Bots/Models/Elm/iTrees-02_Bark-WillowOld-02.jpg"));
  material.Lighting = false;
  material.NormalizeNormals = true;
  tree7->getMaterial(0) = material;*/
  std::cout << "AddSceneNodes done " << std:: endl;
}  
  



void IrrlichtServer::AddLamp()
{
  // =======================================================================================
  // Add a lamp to the scene
  /*
    scene::IMeshSceneNode* lamp = 0;
    
    // Add an MD2 node, which uses vertex-based animation.
    lamp = smgr->addMeshSceneNode(smgr->getMesh("../../media/lamp1.3ds"),
    0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    //fairy->setPosition(core::vector3df(-90,-15,-140)); // Put its feet on the floor.
    lamp->setPosition(core::vector3df(xp, yp, zp)); // Put its feet on the floor.
    lamp->setScale(core::vector3df(0.1f, 0.1f, 0.1f)); // Make it appear realistically scaled
    
    material.setTexture(0, driver->getTexture("../../media/Tile.bmp"));
    material.Lighting = false;
    material.NormalizeNormals = true;
    lamp->getMaterial(0) = material;
  */
  // =======================================================================================
}

void IrrlichtServer::ProcessMessage(const string & type, DataPacket & d)
{
  if (type == MSG_ANIMNODE_ADD) {
    AnimatedSceneNode m;
    m.FromPacket(d);
    const StreamCoordinates & coords = m.GetCoordinates();
    const StreamCoordinates & dir = m.GetDirection();

    AnimModel anim;

    anim.m_pModel = smgr->addAnimatedMeshSceneNode(smgr->getMesh(m.GetModel().c_str()),
					   0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    anim.SetCoords(coords);
    anim.SetDirection(dir);
    //anim.m_pModel->setPosition(core::vector3df(coords[0], coords[1], coords[2])); // Put its feet on the floor.

    anim.m_pModel->setScale(core::vector3df(3.6f)); // Make it appear realistically scaled
    if (m.GetAnimation() != "") {
      anim.SetAnimation(m.GetAnimation(), m.GetAnimationSpeed());
    }
    material.setTexture(0, driver->getTexture(m.GetTexture().c_str()));
    material.Lighting = false;
    material.NormalizeNormals = true;
    anim.m_pModel->getMaterial(0) = material;
    anim.SetName(m.GetName());
    m_anim.push_back(anim);
  }
  if (type ==  MSG_NODE_ADD) {
    SceneNode sn;
    sn.FromPacket(d);
    const StreamCoordinates & coords = sn.GetCoordinates();

    scene::IMeshSceneNode* elm1 = 0;
  
    // Add an MD2 node, which uses vertex-based animation.
    elm1 = smgr->addMeshSceneNode(smgr->getMesh(sn.GetMesh().c_str()),
				  0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    elm1->setPosition(core::vector3df(coords[0], coords[1], coords[2])); // Put its feet on the floor.
    elm1->setScale(core::vector3df(1.1f, 1.1f, 1.1f)); // Make it appear realistically scaled
    
    material.setTexture(0, driver->getTexture(sn.GetTexture1().c_str()));
    material.setTexture(1, driver->getTexture(sn.GetTexture2().c_str()));
    material.Lighting = false;
    material.NormalizeNormals = true;
    elm1->getMaterial(0) = material;
    cout << "Added NODE." << endl;

  }
  if (type ==  MSG_ANIMNODE_UPDATE) {
    AnimatedSceneNode m;
    m.FromPacket(d);
    const StreamCoordinates & coords = m.GetCoordinates();
    const StreamCoordinates & dir = m.GetDirection();
    cout << "Update model " << m.GetName() << endl;
    int index = GetModelIndex(m.GetName());
    if (index >= 0) {
      m_anim[index].SetCoords(coords);
      m_anim[index].SetDirection(dir);
      //m_anim[index].m_pModel->setPosition(core::vector3df(coords[0], coords[1], coords[2]));
      if (m.GetAnimation() != "") {
	m_anim[index].SetAnimation(m.GetAnimation(), m.GetAnimationSpeed());
      }
    } else {
      std::cout << "ERROR, Model not found. " << std::endl;
    }
    
  }
  if (type ==  MSG_NODE_UPDATE) {
    
  }
}

void IrrlichtServer::Run()
{
  
  //=========================================================================================
  std::cout << "Start running." << std::endl;



  u32 then = device->getTimer()->getTime();
  
  // This is the movemen speed in units per second.
  const f32 MOVEMENT_SPEED = 30.f;
  core::vector3df lastCamPosition = camera->getPosition();
  
  
  int lastFPS = -1;
  double downFrameCounter = 0;
  
  double iau_scale = 100.;
  
  char tmp[256];
  
  double absolutTime = 0.;

  // camera->bindTargetAndRotation(false);


  double angle = 0.;
  double angle_speed = 0.1;

  while(device->run())
    if (device->isWindowActive())
      {
	const u32 now = device->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	absolutTime += frameDeltaTime;
	then = now;

	/* Check if keys W, S, A or D are being held down, and move the
	   sphere node around respectively. */
	core::vector3df camPosition = camera->getPosition();
	//core::vector3df camRotation = camera->getRotation();
	//core::vector3df camRotation = camera->getUpVector();


	/*if(receiver->IsKeyDown(irr::KEY_KEY_W))
	  camRotation.Y += MOVEMENT_SPEED * frameDeltaTime;
	else if(receiver->IsKeyDown(irr::KEY_KEY_S))
	  camRotation.Y -= MOVEMENT_SPEED * frameDeltaTime;
	*/

	if(receiver->IsKeyDown(irr::KEY_KEY_A)) {
	  angle -= angle_speed * frameDeltaTime;
	} else if(receiver->IsKeyDown(irr::KEY_KEY_D)) {
	  angle += angle_speed * frameDeltaTime;
	}
	SetCameraTilt(angle);
	
	//camera->setPosition(camPosition);
	//camera->setRotation(camRotation);
	//camera->setUpVector(camRotation);

	//cout << ".";
	

	DataPacket d;
	MessageHeader outhead;
	outhead.ToPacket(d);
	d.Write("position");
	d.Write((double)camPosition.X);
	d.Write((double)camPosition.Y);
	d.Write((double)camPosition.Z);
	
	m_pTrans->Send(d);

	string msg, dir;
	while (m_pRec->Get(d)) {  
	  MessageHeader inhead;
	  inhead.FromPacket(d);
	  //std::cout << inhead.GetTimeStamp().GetReadable() << " -> " << inhead.GetHeader() << std::endl;
	  
	  if (inhead.GetHeader() == MSG_ANIMNODE_ADD) {
	    std::cout << "ADDING MODEL!!" << std::endl;
	    ProcessMessage(MSG_ANIMNODE_ADD, d);
	    continue;
	  } 
	  if (inhead.GetHeader() == MSG_ANIMNODE_UPDATE) {
	    //std::cout << "UPDATING MODEL!!" << std::endl;
	    ProcessMessage(MSG_ANIMNODE_UPDATE, d);
	    continue;
	  } 
	  if (inhead.GetHeader() == MSG_NODE_ADD) {
	    std::cout << "ADDING MODEL!!" << std::endl;
	    ProcessMessage(MSG_NODE_ADD, d);
	    continue;
	  } 
	  
	  d.Read(msg);
	  d.Read(dir);	 	  
	}
	//if (dir == "left") {
	  //camPosition.Z -= 5.;
	//}

	//camera->setPosition(camPosition);


	double dd = Dist(core::vector3df(xp, yp, zp), camPosition);

	

	//std::cout << "Distance: " << dd << std::endl;
	/*if (dd < 200 || bCaught) {
	  fairy->setPosition(camPosition); 
	  bCaught = true;
	  }*/
	
	
	driver->beginScene(true, true, 0 );
	
	smgr->drawAll();
	env->drawAll();
	
	driver->endScene();
	
	// display frames per second in window title
	int fps = driver->getFPS();
	if (lastFPS != fps)
	  {
	    core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
	    str += driver->getName();
	    str += "] FPS:";
	    str += fps;
	    // Also print terrain height of current camera position
	    // We can use camera position because terrain is located at coordinate origin
	    str += " Height: ";
	    str += terrain->getHeight(camera->getAbsolutePosition().X,
				      camera->getAbsolutePosition().Z);
	    
	    //std::cout << "X=" << camera->getAbsolutePosition().X << " Y=" << camera->getAbsolutePosition().Z << std::endl;
	    
	    device->setWindowCaption(str.c_str());
	    lastFPS = fps;
	  }
      }
  
  device->drop();
  
}


void IrrlichtServer::SetCameraPosition(const StreamCoordinates & c)
{
  core::vector3df cam;
  cam.X = c[0];
  cam.Y = c[1];
  cam.Z = c[2];
  camera->setPosition(cam);
}

void IrrlichtServer::SetCameraRotation(const StreamCoordinates & c)
{
  core::vector3df cam;
  cam.X = c[0];
  cam.Y = c[1];
  cam.Z = c[2];
  camera->setRotation(cam);
}

void IrrlichtServer::SetCameraTilt(double angle)
{
  core::vector3df camRotation = camera->getRotation();
  core::vector3df camUp = camera->getUpVector();
  //cout << "a " << camUp.X << " " << camUp.Y << " " << camUp.Z << endl;


  //cout << "Raw rot " << camRotation.Y << endl;
  double alpha = camRotation.Y/360.*2.*IPI;
  double y = cos(angle);
  double d = sin(angle);
  alpha += 0.1;
  double z = -d*sin(alpha);
  double x = d*cos(alpha);
  
	
  //cout << "angle " << angle << " alpha " << alpha << " morm " << x*x + y*y + z*z << endl;
  camUp.X = x;
  camUp.Y = y;
  camUp.Z = z;
  //cout << "b " << camUp.X << " " << camUp.Y << " " << camUp.Z << endl;

  camera->setUpVector(camUp);
  // camera->updateAbsolutePosition();

}

void IrrlichtServer::GetCameraPosition(StreamCoordinates & c)
{
  core::vector3df camPosition = camera->getPosition();
  c[0] = camPosition.X;
  c[1] = camPosition.Y;
  c[2] = camPosition.Z;
}

void IrrlichtServer::GetCameraRotation(StreamCoordinates & c)
{
  core::vector3df camRotation = camera->getRotation();
  c[0] = camRotation.X;
  c[1] = camRotation.Y;
  c[2] = camRotation.Z;
}

void IrrlichtServer::GetCameraTilt(StreamCoordinates & c)
{
  //core::vector3df camRotation = camera->getRotation();
  core::vector3df camUp = camera->getUpVector();

  //TODO: translate
  c[0] = camUp.X;
  c[1] = camUp.Y;
  c[2] = camUp.Z;
  
}

/*
The start of the main function starts like in most other example. We ask the
user for the desired renderer and start it up. This time with the advanced
parameter handling.
*/
int main()
{
 
  
  
  IrrlichtServer irr;

  irr.AddCamera(0, 0, 0);
  irr.AddTerrain();
  irr.AddFairy();
  //irr.AddLamp();
  //irr.AddSceneNodes();

  irr.Run();

  
  
  
  return 0;
}

