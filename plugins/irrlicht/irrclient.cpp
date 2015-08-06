#include "irrclient.h"


double Dist(const core::vector3df & a, const core::vector3df & b) {
  double d = 0.;
  d += (a.X - b.X) * (a.X - b.X);
  d += (a.Y - b.Y) * (a.Y - b.Y);
  d += (a.Z - b.Z) * (a.Z - b.Z);
  //std::cout << a.X << " " << b.X << " " << a.Y << " " << b.Y << " " << a.Z << " " << b.Z << std::endl;
  return sqrt(d);
}



IrrlichtServer::IrrlichtServer(int resX, int resY, bool fullScreen, const string & host, MyEventReceiver * pRec)
{

  m_pTrans = GetTransmitter(host, DEFPORT+1);
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
  
  params.Fullscreen=fullScreen;
  
  params.WindowSize=core::dimension2d<u32>(resX, resY);
  
  device = createDeviceEx(params);
  
  
  if (device == 0)
    exit(-1); // could not create selected driver.
  
  driver = device->getVideoDriver();
  smgr = device->getSceneManager();
  env = device->getGUIEnvironment();


  // set event receiver, if supplied
  if (pRec != NULL) {
    receiver = pRec;
    device->setEventReceiver(receiver);
  }

 
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


//====================================================================
void IrrlichtServer::AddTerrain(const Terrain & t)
{

  // add terrain scene node
  terrain = smgr->addTerrainSceneNode(
				      t.GetTerrain().c_str(),				     
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
  
  terrain->setMaterialTexture(0,
			      driver->getTexture(t.GetTexture1().c_str()));
  terrain->setMaterialTexture(1,
			      driver->getTexture(t.GetTexture2().c_str()));
  
  terrain->setMaterialType(video::EMT_DETAIL_MAP);
  
  terrain->scaleTexture(1.0f, 20.0f);
  
  
  
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
  
    
  // create skybox and skydome
  driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
  
  scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(						     
						     driver->getTexture(t.GetSkyUP().c_str()),
						     driver->getTexture(t.GetSkyDN().c_str()),
						     driver->getTexture(t.GetSkyLF().c_str()),
						     driver->getTexture(t.GetSkyRT().c_str()),
						     driver->getTexture(t.GetSkyFT().c_str()),
						     driver->getTexture(t.GetSkyBK().c_str()));
  
  
  scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture(t.GetSkydome().c_str()),16,8,0.95f,2.0f);
  
  driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


  if (receiver == NULL) {
    receiver = new MyEventReceiver(terrain, skybox, skydome);
    device->setEventReceiver(receiver);
  }

  
  std::cout << "AddTerrain done " << std:: endl;
  DataPacket d;
  MessageHeader outhead;
  outhead.ToPacket(d);
  d.Write("terrain_added");
  m_pTrans->Send(d);

}

void IrrlichtServer::WaitLoadTerrain()
{

  DataPacket t;
  MessageHeader terrhead;
  terrhead.ToPacket(t);
  t.Write("waiting_for_terrain");
  std::cout << "Waiting for terrain" << std::endl;
  m_pTrans->Send(t);



  Terrain terr;
  // Get the terrain from the server
  string msg;
  while (true) {
    DataPacket d;
    if (m_pRec->Get(d)) {  
      MessageHeader inhead;
      inhead.FromPacket(d);
   	  
      if (inhead.GetHeader() == MSG_TERRAIN) {
	cout << "Got terrain." << endl;
	terr.FromPacket(d);
	break;	
      }
    }
    usleep(1000);
  }

  AddTerrain(terr);
}

bool IrrlichtServer::SendMeshModel(scene::IMesh * pMesh, const string & name, core::vector3df posA,
				   const Coordinates & rot, int phys)
{
  MeshModel mesh;
  mesh.SetName(name);

  int i, j;

  // TODO: Send multiple mesh buffers
  std::cout << "Meshes: " << pMesh->getMeshBufferCount() << std::endl;

  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    cout << "Sending mesh " << i << endl;
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    video::E_VERTEX_TYPE type = pBuf->getVertexType();
    int ni = pBuf->getIndexCount();
    video::E_INDEX_TYPE itype = pBuf->getIndexType();
    u16 * indices = pBuf->getIndices();
    
    for (j=0; j<ni; j++) {
      mesh.AddIndexTotal(indices[j]);
    }

 
    int n = pBuf->getVertexCount();
    std::cout << "Buffer " << i << " vertices " << n << " indices " << ni << std::endl;
    


    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      core::vector3df & norm = pBuf->getNormal(j); // TODO: Send normal
      StreamCoordinates cc;
      cc[0] = pos.X;
      cc[1] = pos.Y;
      cc[2] = pos.Z;

      mesh.AddVertex(cc);
    }
  }

  mesh.SetPhysMode(phys);
  mesh.SetRotImp(rot);
  
  StreamCoordinates & a = mesh.AbsCoords();
  a[0] = posA.X;
  a[1] = posA.Y;
  a[2] = posA.Z;
 
  DataPacket data;
  int data_size = mesh.SizeInBytes();
  std::cout << "Actual size: " << data_size << " buffer size " << data.size() << std::endl;
  MessageHeader head;
  head.ToPacket(data);
  data.Write(MSG_MESH_ADD);
  std::cout << "WRITING TO PACKET!" << std::endl;
  mesh.ToPacket(data);
  std::cout << "Sending mesh..." << std::endl;
  m_pTrans->Send(data);

}

void IrrlichtServer::UpdateMeshModel(MeshModel & mesh)
{
  int i, j;

 
  std::cout << "Updating mesh model " << mesh.GetName() << std::endl;

  int index = -1;
  for (i=0; i<m_meshes.isize(); i++) {
    if (m_meshes[i].Name() == mesh.GetName()) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    std::cout << "ERROR: Mesh not found " << mesh.GetName() << std::endl;
    return;
  }
  StreamCoordinates & a = mesh.AbsCoords();

  std::cout << "Found " << index << ", updating position to " << a[0] << " " << a[1] << " " << a[2] << std:: endl;
  m_meshes[index].SetPosition(core::vector3df(a[0], a[1], a[2])); 
  std::cout << "CUBE absolute position: " << a[0] << " " << a[1] << " " << a[2] << std::endl;

   std::cout << "Doing it. " << std::endl;

  scene::IMesh * pMesh = m_meshes[index].Mesh();
  std::cout << "Mesh ptr " << pMesh << std::endl;
  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    video::E_VERTEX_TYPE type = pBuf->getVertexType();
    int ni = pBuf->getIndexCount();
    video::E_INDEX_TYPE itype = pBuf->getIndexType();
    u16 * indices = pBuf->getIndices();
    
    // DEBUG
    // Don't update indices for now!
    //for (j=0; j<ni; j++) {
    //indices[j] = mesh.GetIndexTotal(j);
    //}

    int n = pBuf->getVertexCount();
    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      core::vector3df & norm = pBuf->getNormal(j); // TODO: Send normal
      const StreamCoordinates & cc = mesh.GetVertexConst(j);
      pos.X = cc[0];
      pos.Y = cc[1];
      pos.Z = cc[2];
      std::cout << "CUBE vertex position " << j << " " << cc[0] << " " << cc[1] << " " << cc[2] << std::endl;
    }
    pBuf->recalculateBoundingBox();
  }

  std::cout << "Done updating mesh " << mesh.GetName() << endl;
 
}

// For debugging only
void IrrlichtServer::AddCube()
{

  m_pCube = smgr->addCubeSceneNode(1.0f);
  m_pCube->setScale(core::vector3df(130.6f)); // Make it appear realistically scaled
  m_pCube->setMaterialFlag(video::EMF_LIGHTING, 0);
  m_pCube->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
  m_pCube->setMaterialTexture(0, driver->getTexture("data/Models/cube.jpg"));
  m_pCube->setPosition(core::vector3df(xp, 300, zp)); 
  
  scene::IMesh * pMesh = m_pCube->getMesh();
  int i, j;

  std::cout << "MESH BUFFER" << std::endl;
  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    video::E_VERTEX_TYPE type = pBuf->getVertexType();
    int ni = pBuf->getIndexCount();
    video::E_INDEX_TYPE itype = pBuf->getIndexType();
    u16 * indices = pBuf->getIndices();
    for (j=0; j<ni; j+=3) {
      std::cout << "j=" << j/3 << " ";
      for (int x=j; x<j+3; x++)
	std::cout << indices[x] << " ";
      cout << std::endl;
    }

    int n = pBuf->getVertexCount();
    video::S3DVertex * pVertBuf = (video::S3DVertex*)pBuf->getVertices();
    pVertBuf[0].TCoords = core::vector2d<f32>(0.5, 0.5);
    pVertBuf[1].TCoords = core::vector2d<f32>(.75, 0.5);
    pVertBuf[2].TCoords = core::vector2d<f32>(.75, 0.75);
    pVertBuf[3].TCoords = core::vector2d<f32>(0.5, 0.75);

    pVertBuf[4].TCoords = core::vector2d<f32>(0.75, 0.25);
    pVertBuf[5].TCoords = core::vector2d<f32>(0.75, 0);
    pVertBuf[6].TCoords = core::vector2d<f32>(0.5, 0);
    pVertBuf[7].TCoords = core::vector2d<f32>(0.5, 0.25);

  
    pVertBuf[8].TCoords = pVertBuf[6].TCoords;
    pVertBuf[9].TCoords = pVertBuf[3].TCoords;
    pVertBuf[10].TCoords = pVertBuf[4].TCoords;
    pVertBuf[11].TCoords = pVertBuf[1].TCoords; //core::vector2d<f32>(0, 0);

    std::cout << "Buffer " << i << std::endl;
    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      //TCoords

      std::cout << j << std::endl << "pos:  " << pos.X << " " << pos.Y << " " << pos.Z << std::endl;
      pos.Y += 10;
      if (j == 0) {
	//pos.X = pos.Y = pos.Z = -2.;
	
      }
      core::vector3df & norm = pBuf->getNormal(j);
      std::cout << "norm: " << norm.X << " " << norm.Y << " " << norm.Z << std::endl;
    }
    pBuf->recalculateBoundingBox();

    //void * pRaw = pBuf->getVertices();
 
  }
  string name = "cube";
 
  m_meshes.push_back(MeshNode(name, m_pCube));

  SendMeshModel(pMesh, "cube", m_pCube->getPosition());

  //pMesh->recalculateBoundingBox();
 
  std::cout << "AddCube done " << std:: endl;
}
  



bool IrrlichtServer::ProcessMessage(const string & type, DataPacket & d)
{
  //============================================================
  if (type == MSG_MESH_ADD) {
    
    MeshModel m;
    m.FromPacket(d);
    cout << "Add mesh NOT IMPLEMENTED!!!" << endl;
    throw;
    /*
    const StreamCoordinates & coords = m.GetCoordinates();
    //const StreamCoordinates & dir = m.GetDirection();

    scene::IAnimatedMeshSceneNode * pMM;

    pMM = smgr->addAnimatedMeshSceneNode(smgr->getMesh(m.GetModel().c_str()),
					 0, IDFlag_IsPickable | IDFlag_IsHighlightable);


    //pMM->setScale(core::vector3df(10.6f));
    pMM->setScale(core::vector3df(m.GetScale())); 
    pMM->setMaterialFlag(video::EMF_LIGHTING, 0);
    pMM->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    pMM->setMaterialTexture(0, driver->getTexture(m.GetTexture().c_str()));
    pMM->setPosition(core::vector3df(coords[0], coords[1], coords[2])); 
    pMM->setMD2Animation("");
    pMM->setAnimationSpeed(0.);      
   

    scene::IMesh * pMesh = pMM->getMesh();

    // Remove it from the cache.
    smgr->getMeshCache()->removeMesh(pMesh);

    
    if (pMesh == NULL) {
      std::cout << "ERROR, Mesh ptr == NULL" << std::endl;
    }

    m_meshes.push_back(MeshNode(m.GetName(), pMM));
    */
 
  }
  //============================================================

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

    anim.m_pModel->setScale(core::vector3df(m.GetScale())); // Make it appear realistically scaled
    cout << "Animation: " << endl;
    if (m.GetAnimation() != "") {      
      anim.SetAnimation(m.GetAnimation(), m.GetAnimationSpeed());
    }
    material.setTexture(0, driver->getTexture(m.GetTexture().c_str()));
    material.Lighting = false;
    material.NormalizeNormals = true;
    anim.m_pModel->getMaterial(0) = material;
    anim.SetName(m.GetName());
    m_anim.push_back(anim);
    return true;
  }

  //=====================================================================
  if (type == MSG_PHYS_ADD) {
    AnimatedSceneNode m;
    m.FromPacket(d);
    const StreamCoordinates & coords = m.GetCoordinates();
    const StreamCoordinates & dir = m.GetDirection();

    scene::IAnimatedMeshSceneNode * pMM;

    pMM = smgr->addAnimatedMeshSceneNode(smgr->getMesh(m.GetModel().c_str()),
					 0, IDFlag_IsPickable | IDFlag_IsHighlightable);

    std::cout << "Physics request node " << pMM << " mesh " << pMM->getMesh() << endl;

    //pMM->setScale(core::vector3df(10.6f));
    pMM->setScale(core::vector3df(m.GetScale())); 
    pMM->setMaterialFlag(video::EMF_LIGHTING, 0);
    pMM->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    pMM->setMaterialTexture(0, driver->getTexture(m.GetTexture().c_str()));
    pMM->setPosition(core::vector3df(coords[0], coords[1], coords[2])); 
    pMM->setMD2Animation("");
    pMM->setAnimationSpeed(0.);      
   

    scene::IMesh * pMesh = pMM->getMesh();

    // Remove it from the cache.
    smgr->getMeshCache()->removeMesh(pMesh);

    
    if (pMesh == NULL) {
      std::cout << "ERROR, Mesh ptr == NULL" << std::endl;
    }

    m_meshes.push_back(MeshNode(m.GetName(), pMM));
    
    std::cout << "Joints: " << pMM->getJointCount() << std::endl;
    for (int i=0; i<pMM->getJointCount(); i++) {
      scene::IBoneSceneNode * pJoint = pMM->getJointNode(i);
      std::cout << "   " << pJoint->getName() << std::endl;
    }
    std::cout << "Sending rot impulse for " << m.GetName() << ": ";
    m.GetRotImp().Print();
      
    SendMeshModel(pMesh, m.GetName(), pMM->getPosition(), m.GetRotImp(), m.PhysMode());

 
    return true;

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
    elm1->setScale(core::vector3df(sn.GetScale(), sn.GetScale(), sn.GetScale())); // Make it appear realistically scaled
    
    material.setTexture(0, driver->getTexture(sn.GetTexture1().c_str()));
    if (sn.GetTexture2() != "")
      material.setTexture(1, driver->getTexture(sn.GetTexture2().c_str()));
    material.Lighting = false;
    material.NormalizeNormals = true;
    elm1->getMaterial(0) = material;
    cout << "Added NODE." << endl;
    return true;

  }
  if (type == MSG_MESH_UPDATE) {
    MeshModel model;
    std::cout << "Packet to mesh" << std::endl;
    model.FromPacket(d);
    std::cout << "Done, calling update" << std::endl;
    UpdateMeshModel(model);
    return true;
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
    return true;
    
  }
  if (type ==  MSG_NODE_UPDATE) {
    return true;

  }
  return false;
}

void IrrlichtServer::Run()
{
  
  //=========================================================================================
  std::cout << "Start running." << std::endl;



  u32 then = device->getTimer()->getTime();
  
  // This is the movement speed in units per second.
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

  bool bFirst = true;
  while(device->run())
    if (device->isWindowActive())
      {
	
	if (bFirst) {
	  // Signal that we are ready
	  DataPacket ready;
	  MessageHeader readyhead;
	  readyhead.ToPacket(ready);
	  ready.Write("engine_ready");
	  std::cout << "Engine is ready!!" << std::endl;
	  m_pTrans->Send(ready);
	  bFirst = false;
	}


	const u32 now = device->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	absolutTime += frameDeltaTime;
	then = now;

	/* Check if keys W, S, A or D are being held down, and move the
	   sphere node around respectively. */
	core::vector3df camPosition = camera->getPosition();
	//core::vector3df camRotation = camera->getRotation();
	//core::vector3df camRotation = camera->getUpVector();



	if(receiver->IsKeyDown(irr::KEY_KEY_A)) {
	  angle -= angle_speed * frameDeltaTime;
	} else if(receiver->IsKeyDown(irr::KEY_KEY_D)) {
	  angle += angle_speed * frameDeltaTime;
	}
	SetCameraTilt(angle);
	
	

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
	  std::cout << "RECEIVED " << inhead.GetTimeStamp().GetReadable() << " -> " << inhead.GetHeader() << std::endl;
	  bool bHandled = ProcessMessage(inhead.GetHeader(), d);
	  if (bHandled) 
	    continue;
	  
	  cout << "WARNING: Unhandled message " << inhead.GetHeader() << ", removing from stack." << endl;
	  d.Read(msg);
	  d.Read(dir);	 	  
	}


	double dd = Dist(core::vector3df(xp, yp, zp), camPosition);     	
	
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
