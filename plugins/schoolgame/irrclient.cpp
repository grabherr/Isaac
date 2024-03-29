// (c) 2015 Manfred G. Grabherr. All rights reserved.

//#define FORCE_DEBUG

#include "irrclient.h"
#include "IGUIFont.h"
#include "IGUIEnvironment.h"
#include "irrString.h"

int WALK_HEIGHT=580;

double Dist(const core::vector3df & a, const core::vector3df & b) {
  double d = 0.;
  d += (a.X - b.X) * (a.X - b.X);
  d += (a.Y - b.Y) * (a.Y - b.Y);
  d += (a.Z - b.Z) * (a.Z - b.Z);
  //std::cout << a.X << " " << b.X << " " << a.Y << " " << b.Y << " " << a.Z << " " << b.Z << std::endl;
  return sqrt(d);
}



IrrlichtServer::IrrlichtServer(int resX, int resY, bool fullScreen, const string & host, MyEventReceiver * pRec, bool bTCP)
{
  if (!bTCP) {
    m_pTrans = GetTransmitter(host, DEFPORT+1);
    m_pRec = GetReceiver(DEFPORT);
  } else {
    cout << "Use TCP connection!" << endl;
    m_pRec = GetReceiver(host, DEFPORT);
    m_pTrans = GetTransmitter(host, DEFPORT+1);
  }

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
  //std::cout << "Using driver type " << driverType << std::endl;
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

 
  //std::cout << "Constructor done " << std:: endl;

}

void IrrlichtServer::AddCamera(double x, double y, double z)
{
  //std::cout << "Start AddCamera " << std:: endl;
  xp = 2700*2;
  yp = 255*2;
  zp = 2600*2;
  
  // add camera
  //std::cout << "1" << std:: endl;
  camera = smgr->addCameraSceneNodeFPS(0,50.0f,0.2f);
  //std::cout << "2" << std:: endl;
  camera->setPosition(core::vector3df(2700*2-2000,255*2+1300,2600*2-900));
  //std::cout << "3" << std:: endl;
  camera->setRotation(core::vector3df(0., 270, 0.));
  
  
  //std::cout << "4" << std:: endl;
  camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
  //std::cout << "5" << std:: endl;
  camera->setFarValue(42000.0f);
    
  // disable mouse cursor
  //std::cout << "6" << std:: endl;
  device->getCursorControl()->setVisible(false);
  //std::cout << "AddCamera done " << std:: endl;

  
 
 
}


//====================================================================
void IrrlichtServer::AddTerrain(const MsgTerrain & t)
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



  MsgTerrain terr;
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
  //std::cout << "Meshes: " << pMesh->getMeshBufferCount() << std::endl;

  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    //cout << "Sending mesh " << i << endl;
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    video::E_VERTEX_TYPE type = pBuf->getVertexType();
    int ni = pBuf->getIndexCount();
    video::E_INDEX_TYPE itype = pBuf->getIndexType();
    u16 * indices = pBuf->getIndices();

    //if (phys == 2)
    //ni = 0;
    
    for (j=0; j<ni; j++) {
      mesh.AddIndexTotal(indices[j]);
    }
    
    
    int n = pBuf->getVertexCount();
    //std::cout << "Buffer " << i << " vertices " << n << " indices " << ni << std::endl;
    
    //if (phys == 2)
    //n = 1;
     
    
    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      core::vector3df & norm = pBuf->getNormal(j); // TODO: Send normal
      StreamCoordinates cc;
      cc[0] = pos.X;
      cc[1] = pos.Y;
      cc[2] = pos.Z;
      //cout << "Vertex " << j << ": " << cc[0] << " " << cc[1] << " " << cc[2] << endl;
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
  //std::cout << "Actual size: " << data_size << " buffer size " << data.size() << std::endl;
  MessageHeader head;
  head.ToPacket(data);
  data.Write(MSG_MESH_ADD);
  //std::cout << "WRITING TO PACKET!" << std::endl;
  bool bTrunc = false;
  if (data_size >= data.size()) {
    cout << "Model " << name << " too big, not sending." << endl;
    //data.resize_add(data_size + 1024); 
    //mesh.ToPacket(data);
    //data.WriteToFile("tmpmessage_mesh.dat");
    bTrunc = true;
    return false;
  }
  mesh.ToPacket(data, bTrunc);
  std::cout << "Sending mesh for " << name << std::endl;
  m_pTrans->Send(data);
  //std::cout << "Done." << endl;
  return true;
}

void IrrlichtServer::UpdateMeshModel(MeshModel & mesh)
{
  int i, j;

 
  std::cout << "Updating mesh model " << mesh.GetName() << std::endl;

  int index = FindMeshIndex(mesh.GetName());
  /*
  int index = -1;
  for (i=0; i<m_meshes.isize(); i++) {
    if (m_meshes[i].Name() == mesh.GetName()) {
      index = i;
      break;
    }
    }*/

  if (index == -1) {
    std::cout << "ERROR: Mesh not found " << mesh.GetName() << std::endl;
    return;
  }
  StreamCoordinates & a = mesh.AbsCoords();
  //  StreamCoordinates & dir = mesh.GetDirection();

  scene::ISceneNode * pSceneNode = m_meshes[index].SceneNode();
  core::vector3df currPos =  m_meshes[index].GetPosition();
  
  //=====================================================
  const MsgSound & sound = mesh.GetSound();
  //cout << "CHECKING sound: " << sound.GetName() << " " << sound.GetWavFile() << " for " << mesh.GetName() << endl;
  if (sound.GetName() != "") {
    SourceData sound_1;
    sound_1.SetWavFile(sound.GetWavFile());
    sound_1.SetName(sound.GetName());
    sound_1.SetCoords(sound.GetPosition());
    //cout << "UPDATE sound to ";
    //sound.GetPosition().Print();
    audioDat.AddSource(sound_1);
    //cout << "Adding sound (mesh update): " << sound.GetName() << endl;
  }
  //=====================================================






  //std::cout << "Found " << index << ", updating position to " << a[0] << " " << a[1] << " " << a[2] << std:: endl;
  m_meshes[index].SetPosition(core::vector3df(a[0], a[1], a[2])); 
  //cout << "Phys mode: " << mesh.PhysMode() << endl;

  if (m_meshes[index].NeedsTexture(mesh.GetTexture())) {
    const StreamCoordinates & invis = mesh.GetInvisible();
    if (invis[0] > 0.) {
      video::ITexture* myImage = driver->getTexture(mesh.GetTexture().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2])); 
      m_meshes[index].SetMaterialFlag(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
      //cout << "RESET Invisible!" << endl;
    }
    //cout << "CALL SetTexture, invis " << invis[0] << endl;
    m_meshes[index].SetTexture(driver->getTexture(mesh.GetTexture().c_str()));
  }

  if (mesh.PhysMode() > 0) {
    //Coordinates oldPos(currPos.X, currPos.Y, currPos.Z);
    //Coordinates newPos(a[0], a[1], a[2]);
    
    //SphereCoordinates sp = (newPos - oldPos).AsSphere();
    SphereCoordinates sp = mesh.GetDirection().AsSphere();
    StreamCoordinates ss = mesh.GetDirection();

    if (mesh.HasRotation()) {
      StreamCoordinates rot = mesh.GetRotation();
      currPos.X = 360*rot[0]/3.1415/2;
      currPos.Y = 360*rot[1]/3.1415/2;
      currPos.Z = 360*rot[2]/3.1415/2;
      //cout << "Have rot: " << currPos.X << " " << currPos.Y << " " << currPos.Z << endl;
    } else {
      currPos.Z = 0;
      currPos.X = 0;
      double phi = 0.;
      if (ss[2] != 0.) {
	phi = atan(ss[0]/ss[2]);
	if (ss[2] < 0.)
	  phi += 3.1415;
	if (phi < 0.)
	  phi += 2*3.1415;
      }
      currPos.Y = 360*(phi)/3.1415/2;
      
      phi = 0.;
      //====================================================
      if (ss[1] != 0.) {
	phi = atan(ss[2]/ss[1]);
	if (ss[1] < 0.)
	  phi += 3.1415;
	if (phi < 0.)
	  phi += 2*3.1415;
      }
      //currPos.X = 360*(phi)/3.1415/2;
      phi = 0.;
      
      if (ss[0] != 0.) {
	phi = atan(ss[1]/ss[0]);
	if (ss[0] < 0.)
	  phi += 3.1415;
	if (phi < 0.)
	  phi += 2*3.1415;
      }
      //currPos.Z = 360*(phi)/3.1415/2;
      phi = 0.;
    }
    //====================================================
    // cout << "Set rotation for " << index << " " << currPos.Y << " from " << phi  << " ";
    //cout << "Rotation " << currPos.X << " " << currPos.Y << " " << currPos.Z << endl;

    mesh.GetDirection().Print();
    //endl;
    pSceneNode->setRotation(currPos);
  }
  //cout << "SET Animation to " << mesh.GetAnimation() << endl;
  m_meshes[index].SetAnimation(mesh.GetAnimation());
 
  
  //std::cout << "CUBE absolute position: " << a[0] << " " << a[1] << " " << a[2] << std::endl;

  //std::cout << "Doing it. " << std::endl;

  scene::IMesh * pMesh = m_meshes[index].Mesh();
  //std::cout << "Mesh ptr " << pMesh << std::endl;
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

    // Do not update if no info
    n = mesh.VertexCount();
    //cout << "Vertices: " << n << endl;
    //n = 0;

    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      core::vector3df & norm = pBuf->getNormal(j); // TODO: Send normal

      const StreamCoordinates & cc = mesh.GetVertexConst(j);
      const StreamCoordinates & nn = mesh.GetNormalConst(j);
      pos.X = cc[0];
      pos.Y = cc[1];
      pos.Z = cc[2];
      norm.X = nn[0];
      norm.Y = nn[1];
      norm.Z = nn[2];
      //std::cout << "CUBE vertex position " << j << " " << cc[0] << " " << cc[1] << " " << cc[2] << std::endl;
    }
    pBuf->recalculateBoundingBox();
  }
  scene::IMeshManipulator * pMani = driver->getMeshManipulator();
  pMani->recalculateNormals(pMesh);

  // m_meshes[index].SceneNode()->render();

  //std::cout << "Done updating mesh " << mesh.GetName() << endl;
 
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

  //std::cout << "MESH BUFFER" << std::endl;
  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    video::E_VERTEX_TYPE type = pBuf->getVertexType();
    int ni = pBuf->getIndexCount();
    video::E_INDEX_TYPE itype = pBuf->getIndexType();
    u16 * indices = pBuf->getIndices();
    //for (j=0; j<ni; j+=3) {
      //std::cout << "j=" << j/3 << " ";
      //for (int x=j; x<j+3; x++)
	//std::cout << indices[x] << " ";
	//cout << std::endl;
    //}

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

    //std::cout << "Buffer " << i << std::endl;
    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      //TCoords

      //std::cout << j << std::endl << "pos:  " << pos.X << " " << pos.Y << " " << pos.Z << std::endl;
      pos.Y += 10;
      if (j == 0) {
	//pos.X = pos.Y = pos.Z = -2.;
	
      }
      core::vector3df & norm = pBuf->getNormal(j);
      //std::cout << "norm: " << norm.X << " " << norm.Y << " " << norm.Z << std::endl;
    }
    pBuf->recalculateBoundingBox();

    //void * pRaw = pBuf->getVertices();
 
  }
  string name = "cube";
 
  m_meshes.push_back(MeshNode(name, m_pCube));

  SendMeshModel(pMesh, "cube", m_pCube->getPosition());

  //pMesh->recalculateBoundingBox();
 
  //std::cout << "AddCube done " << std:: endl;
}
  
void IrrlichtServer::AddSceneNode(const MsgSceneNode & m)
{
  m_bNeedSort = true;

  cout << "MSG AddSceneNode" << endl;
  const StreamCoordinates & coords = m.GetPosition();
  const StreamCoordinates & rot = m.GetRotation();
  
  int i;

  scene::ISceneNode * pTop = NULL;
  scene::IMesh * pMesh = NULL;

  if (m.GetModel() != "") {
    scene::IAnimatedMeshSceneNode * pMM = NULL;
    cout << "Adding " << m.GetModel().c_str() << endl;

    pMM = smgr->addAnimatedMeshSceneNode(smgr->getMesh(m.GetModel().c_str()),
					 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    pTop = pMM;
    const SceneNodeAnimation & anim = m.GetAnimation();
    cout << "AddSceneNode animation " << anim.GetAnimation() << endl;
    if (anim.GetAnimation() != "") {
      pMM->setMD2Animation(anim.GetAnimation().c_str());
      pMM->setAnimationSpeed(anim.GetSpeed());
    } else {
      pMM->setAnimationSpeed(0.);
    }
    pMesh = pMM->getMesh();

    //smgr->getMeshCache()->removeMesh(pMesh);

    
    MeshNode meshnode(m.GetName(), pMM);
    meshnode.NeedsModel(m.GetModel()); // Set model
    m_meshes.push_back(meshnode);

    std::cout << "Joints: " << pMM->getJointCount() << std::endl;
    for (int i=0; i<pMM->getJointCount(); i++) {
      scene::IBoneSceneNode * pJoint = pMM->getJointNode(i);
      std::cout << "   " << pJoint->getName() << std::endl;
    }



  } else {
    // Add mesh from coordinates
    //std::cout << "Physics request node " << endl;
    scene::SMeshBuffer* buffer = new scene::SMeshBuffer();

    //===============================================
    // ONE MESH ONLY
    const SceneNodeMeshPhysics & p = m.GetMesh(0);
    //===============================================

    //cout << "Adding mesh, indices: " << p.IndexCountTotal() << " vertices: " << p.VertexCount() << endl;

    buffer->Indices.set_used(p.IndexCountTotal());

    for (i=0; i<p.IndexCountTotal(); i++) {
      buffer->Indices[i] = p.GetIndexTotal(i);
    }

    video::SColor clr(255,255,255,255);

    buffer->Vertices.reallocate(p.VertexCount());
  
    for (i=0; i<p.VertexCount(); i++) {
      // Coords, normals, color, texCoords
      const StreamCoordinates & s = p.GetVertexConst(i);
      const StreamCoordinates & n = p.GetNormalConst(i);
      const StreamCoordinates & t = p.GetTextCoordConst(i);
      
      buffer->Vertices.push_back(video::S3DVertex(s[0],s[1],s[2], n[0],n[1],n[2], clr, t[0], t[1]));
    }

    //----------------------------------------
    buffer->BoundingBox.reset(0,0,0);
    //----------------------------------------
  
    scene::SMesh * mesh = new scene::SMesh;
    mesh->addMeshBuffer(buffer);
    buffer->drop();

    mesh->recalculateBoundingBox();
    
    scene::IMeshSceneNode * pMSN;
    pMSN = smgr->addMeshSceneNode(mesh, 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    pTop = pMSN;
    pMesh = pMSN->getMesh();
    smgr->getMeshCache()->removeMesh(pMesh);

    m_meshes.push_back(MeshNode(m.GetName(), pMSN));

  }
  pTop->setPosition(core::vector3df(coords[0], coords[1], coords[2])); 
  pTop->setScale(core::vector3df(m.GetScale())); // Make it appear realistically scaled



  // Add textures & materials
  
  // const StreamCoordinates & glob_invis = m.GetInvisible();
  //bool bGlobLighting = m.GetLighting();

  for (i=0; i<m.MaterialCount(); i++) { 
    const SceneNodeMaterial & mat = m.GetMaterial(i);
    
    video::SMaterial material;
 
    const StreamCoordinates & invis = mat.GetInvisibleCoords();
    //if (!mat.IsInvisible() && glob_invis[0] != 0)
    //invis = glob_invis;
    if (mat.IsInvisible()) {
      video::ITexture* myImage = driver->getTexture(mat.GetTexture().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2])); 
      material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    }
  
    material.setTexture(0, driver->getTexture(mat.GetTexture().c_str()));
    material.Lighting = mat.GetLighting();
    cout << "AddSceneNode, lighting " << material.Lighting << endl;

    

    material.Shininess = mat.GetShinyness();
    pTop->getMaterial(i) = material;  

    if (mat.GetTransparent() >= 0.) {
      cout << "Set transparent: " << i << " " << mat.GetTransparent() << endl;
      pTop->setMaterialType(video::EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
      scene::IMeshManipulator *manipulator = smgr->getMeshManipulator();    
      manipulator->setVertexColorAlpha(pMesh, mat.GetTransparent());
    }  
 
  } 
  
  if (m.GetPhysMode() != 0) {
    StreamCoordinates rot = m.GetRotation();
    core::vector3df currPos;
    currPos.X = 360*rot[0]/3.1415/2;
    currPos.Y = 360*rot[1]/3.1415/2;
    currPos.Z = 360*rot[2]/3.1415/2;
    //cout << "Phys mode 2, Have rot: " << currPos.X << " " << currPos.Y << " " << currPos.Z << endl;
    //mesh.GetDirection().Print();
    pTop->setRotation(currPos);
  }
  
  
  if (m.RequestLoopBack()) {
    LoopBackSceneNode(m, pMesh, m.GetName(), pTop->getPosition(), rot, m.GetPhysMode(), m.RequestMesh());
  }

  cout << "Added mesh model, all done." << endl;

}

void IrrlichtServer::UpdateSceneNode(const MsgSceneNode & m)
{
  int i, j;
 
  std::cout << "Updating scene node " << m.GetName() << std::endl;


  int index = FindMeshIndex(m.GetName());
  /*
  for (i=0; i<m_meshes.isize(); i++) {
    if (m_meshes[i].Name() == m.GetName()) {
      index = i;
      break;
    }
    }*/

  if (index == -1) {
    std::cout << "ERROR: Mesh not found " << m.GetName() << std::endl;
    return;
  }

  const StreamCoordinates & a = m.GetPosition();

  scene::ISceneNode * pSceneNode = m_meshes[index].SceneNode();
  core::vector3df currPos =  m_meshes[index].GetPosition();
  
  //=====================================================
  const MsgSound & sound = m.GetSound();
  //cout << "CHECKING sound: " << sound.GetName() << " " << sound.GetWavFile() << " for " << m.GetName() << endl;
  if (sound.GetName() != "") {
    SourceData sound_1;
    sound_1.SetWavFile(sound.GetWavFile());
    sound_1.SetName(sound.GetName());
    sound_1.SetCoords(sound.GetPosition());
    //cout << "UPDATE sound to ";
    //sound.GetPosition().Print();
    audioDat.AddSource(sound_1);
    cout << "Adding sound (mesh update): " << sound.GetName() << endl;
  }
  //=====================================================



  //std::cout << "Found " << index << ", updating position to " << a[0] << " " << a[1] << " " << a[2] << std:: endl;
  m_meshes[index].SetPosition(core::vector3df(a[0], a[1], a[2])); 
  //cout << "Phys mode: " << m.GetPhysMode() << endl;

  
  // Update the model?

  if (m_meshes[index].NeedsModel(m.GetModel())) {
    scene::IAnimatedMeshSceneNode * pAnim = m_meshes[index].Anim();
    scene::IAnimatedMesh * pCurr = pAnim->getMesh();
    //cout << "Changing mesh to " << m.GetModel() << endl;
    //pCurr->drop();
    pAnim->setMesh(smgr->getMesh(m.GetModel().c_str()));

  }

  // Dynamic texture update
  if (m_meshes[index].NeedsTexture(m.GetMaterial(0).GetTexture())) {
    const StreamCoordinates & invis = m.GetMaterial(0).GetInvisibleCoords();
    if (m.GetMaterial(0).IsInvisible()) {
      video::ITexture* myImage = driver->getTexture(m.GetMaterial(0).GetTexture().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2])); 
      m_meshes[index].SetMaterialFlag(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
      //cout << "RESET Invisible!" << endl;
    }
    //cout << "CALL SetTexture " << m.GetMaterial(0).GetTexture() << endl;
    m_meshes[index].SetTexture(driver->getTexture(m.GetMaterial(0).GetTexture().c_str()));
  }

  if (m.GetPhysMode() != 0) {
    StreamCoordinates rot = m.GetRotation();
    currPos.X = 360*rot[0]/3.1415/2;
    currPos.Y = 360*rot[1]/3.1415/2;
    currPos.Z = 360*rot[2]/3.1415/2;
    //cout << "Have rot: " << currPos.X << " " << currPos.Y << " " << currPos.Z << endl;

    //mesh.GetDirection().Print();
    pSceneNode->setRotation(currPos);
  }

  
  //cout << "UpdateSceneNode animation " << m.GetAnimation().GetAnimation() << endl;
  
  if (m_meshes[index].NeedsAnimation(m.GetAnimation().GetAnimation())) {
    m_meshes[index].SetAnimation(m.GetAnimation().GetAnimation());
  }
  if (m_meshes[index].Anim() != NULL) {
    if (m.GetAnimation().GetFrame() != -1)
      m_meshes[index].Anim()->setCurrentFrame(m.GetAnimation().GetFrame());
    
    if (m.GetAnimation().GetLoopFrom() != -1 &&
	m.GetAnimation().GetLoopTo() != -1)
      m_meshes[index].Anim()->setFrameLoop(m.GetAnimation().GetLoopFrom(),
					   m.GetAnimation().GetLoopTo()); 
  }

  
  if (m.GetPhysMode() != 2 && m.MeshCount() > 0) {
    const SceneNodeMeshPhysics & mesh = m.GetMesh(0);
 
    scene::IMesh * pMesh = m_meshes[index].Mesh();
    //std::cout << "Mesh ptr " << pMesh << std::endl;
    int k = 0;
    
    
    for (i=0; i<pMesh->getMeshBufferCount(); i++) {
      scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
      video::E_VERTEX_TYPE type = pBuf->getVertexType();
      int ni = pBuf->getIndexCount();
      video::E_INDEX_TYPE itype = pBuf->getIndexType();
      u16 * indices = pBuf->getIndices();
      
      
      int n = pBuf->getVertexCount();
      //cout << "Real vertices " << n << endl;
      n = mesh.VertexCount();
      //cout << "Sent vertices " << n << endl;
      
      for (j=0; j<n; j++) {
	core::vector3df & pos = pBuf->getPosition(j);
	core::vector3df & norm = pBuf->getNormal(j); // TODO: Send normal

	core::vector2df & tc = pBuf->getTCoords(j);

	//cout << "Get " << j << endl;
	const StreamCoordinates & texcoords = mesh.GetTextCoordConst(k);
	const StreamCoordinates & cc = mesh.GetVertexConst(k);
	//cout << "Update vertex " << cc[0] << " " << cc[1] << " " << cc[2] << endl;
	//const StreamCoordinates & nn = mesh.GetNormalConst(k);
	k++;
	pos.X = cc[0];
	pos.Y = cc[1];
	pos.Z = cc[2];

	//cout << "TEXTURE Coords: " << tc.X << " " << tc.Y << " -> ";
	tc.X = texcoords[0];
	tc.Y = texcoords[1];
	//cout << tc.X << " " << tc.Y << endl;
	//norm.X = nn[0];
	//norm.Y = nn[1];
	//norm.Z = nn[2];
	//cout << "Set!" << endl;
      }
      pBuf->recalculateBoundingBox();
    }
    scene::IMeshManipulator * pMani = driver->getMeshManipulator();
    pMani->recalculateNormals(pMesh);
  }

  
  //std::cout << "Done updating mesh " << m.GetName() << endl;
 
}

void IrrlichtServer::LoopBackSceneNode(const MsgSceneNode & m_orig, scene::IMesh * pMesh, const string & name, core::vector3df posA,
				       const Coordinates & rot, int phys, bool sendMesh)
{
  MsgSceneNode m;

  m.SetRotation(m_orig.GetRotation());
  m.SetMass(m_orig.GetMass());
  m.SetScale(m_orig.GetScale());
  m.SetName(name);

  int i, j;

  // TODO: Send multiple mesh buffers
  std::cout << "Meshes: " << pMesh->getMeshBufferCount() << std::endl;
  int data_size = 0;
  m.SetMeshCount(pMesh->getMeshBufferCount());

  int k = 0;
  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    SceneNodeMeshPhysics & mesh = m.Mesh(i);
    cout << "Sending mesh " << i << endl;
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    video::E_VERTEX_TYPE type = pBuf->getVertexType();
    int ni = pBuf->getIndexCount();
    video::E_INDEX_TYPE itype = pBuf->getIndexType();
    u16 * indices = pBuf->getIndices();
    
    if (!sendMesh)
      ni = 0;
    for (j=0; j<ni; j++) {
      mesh.AddIndexTotal(indices[j]);
    }

 
    int n = pBuf->getVertexCount();
    //std::cout << "Buffer " << i << " vertices " << n << " indices " << ni << std::endl;
    

    if (!sendMesh)
      n = 1;

    for (j=0; j<n; j++) {
      core::vector3df & pos = pBuf->getPosition(j);
      core::vector3df & norm = pBuf->getNormal(j); // TODO: Send normal
      core::vector2df & tc = pBuf->getTCoords(j);

      StreamCoordinates cc;
      cc[0] = pos.X;
      cc[1] = pos.Y;
      cc[2] = pos.Z;
      //cout << "Vertex " << j << ": " << cc[0] << " " << cc[1] << " " << cc[2] << endl;
      mesh.AddVertex(cc);

      StreamCoordinates tt;
      tt[0] = tc.X;
      tt[1] = tc.Y;
      mesh.AddTexCoord(tt);
    }
    mesh.SetPhysMode(phys);
    m.SetPhysMode(phys);
    mesh.SetRotation(rot);
    
    StreamCoordinates & a = mesh.AbsCoords();
    a[0] = posA.X;
    a[1] = posA.Y;
    a[2] = posA.Z;
    
    m.SetPosition(a);
    cout << "Add size " << mesh.SizeInBytes() << " to mesh " << i << endl;
    data_size += mesh.SizeInBytes();
  }


  DataPacket data;

  std::cout << "Actual size: " << data_size << " buffer size " << data.size() << std::endl;
  MessageHeader head;
  head.ToPacket(data);
  data.Write(MSG_SCENENODE_ADD);
  //std::cout << "WRITING TO PACKET!" << std::endl;
  bool bTrunc = false;
  if (data_size >= data.size()) {
    cout << "Model " << name << " too big, not sending. Writing to file instead." << endl;
    data.resize_add(data_size + 1024); 
    m.ToPacket(data);
    data.WriteToFile("tmpmessage.dat");
    return;
  }
  m.ToPacket(data);
  //std::cout << "Sending mesh..." << std::endl;
  m_pTrans->Send(data);
  //std::cout << "Done." << endl;
}

void IrrlichtServer::AddMeshModel(MeshModel m)
{
  m_bNeedSort = true;

  cout << "ADDING Mesh Model." << endl;
  int i;
  scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
  buffer->Indices.set_used(m.IndexCountTotal());

  for (i=0; i<m.IndexCountTotal(); i++) {
    buffer->Indices[i] = m.GetIndexTotal(i);
  }

  video::SColor clr(255,255,255,255);

  buffer->Vertices.reallocate(m.VertexCount());
  
  for (i=0; i<m.VertexCount(); i++) {
    // Coords, normals, color, texCoords
    const StreamCoordinates & s = m.GetVertexConst(i);
    const StreamCoordinates & n = m.GetNormalConst(i);
    const StreamCoordinates & t = m.GetTextCoordConst(i);
    
    buffer->Vertices.push_back(video::S3DVertex(s[0],s[1],s[2], n[0],n[1],n[2], clr, t[0], t[1]));
  }

  //----------------------------------------
  buffer->BoundingBox.reset(0,0,0);
  //----------------------------------------


  for (u32 i=0; i<12; ++i) {
    buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
  }
  
  scene::SMesh * mesh = new scene::SMesh;
  mesh->addMeshBuffer(buffer);
  buffer->drop();

  mesh->recalculateBoundingBox();

  scene::IMeshSceneNode * pMM;
  //scene::IAnimatedMeshSceneNode * pMM;
  pMM = smgr->addMeshSceneNode(mesh, 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
  //pMM = smgr->addAnimatedMeshSceneNode(mesh,
  //				       0, IDFlag_IsPickable | IDFlag_IsHighlightable);

 
  pMM->setScale(core::vector3df(m.GetScale())); // Make it appear realistically scaled
  pMM->setMaterialFlag(video::EMF_LIGHTING, 0);
  pMM->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
  pMM->setMaterialTexture(0, driver->getTexture(m.GetTexture().c_str()));
  pMM->getMaterial(0).Lighting = m.GetLighting();

  const StreamCoordinates & a = m.GetAbsCoords();
  pMM->setPosition(core::vector3df(a[0], a[1], a[2])); 
  
  scene::IMesh * pMesh = pMM->getMesh();

  for (i=0; i<pMesh->getMeshBufferCount(); i++) {
    scene::IMeshBuffer * pBuf = pMesh->getMeshBuffer(i);
    pBuf->recalculateBoundingBox();
  }

  string name = m.GetName();
 
  m_meshes.push_back(MeshNode(name, pMM));
  cout << "Mesh add DONE!" << endl;
}
  
/*wchar ToUnicode(const string & s) {

  }*/


bool IrrlichtServer::ProcessMessage(const string & type, DataPacket & d)
{

  cout << "Process " << type << endl;
  if (type == MSG_LIGHT_ADD) {
    MsgLightNode m;
    m.FromPacket(d);
    const StreamCoordinates & c = m.Position();
    scene::ILightSceneNode* light1 =
      smgr->addLightSceneNode(0, core::vector3df(c[0],c[1],c[2]),
			      video::SColorf(m.R(), m.G(), m.B(), 0.0f), m.Radius());
   
    light1->enableCastShadow(true);

    if (m.Texture() != "") {
      scene::IBillboardSceneNode* bill =
	smgr->addBillboardSceneNode(light1, core::dimension2d<f32>(60, 60));
      
      bill->setMaterialFlag(video::EMF_LIGHTING, false);
      bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
      bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
      bill->setMaterialTexture(0, driver->getTexture(m.Texture().c_str()));
    }
    // WARNING: Store it in the mesh list for now
    //m_meshes.push_back(MeshNode(m.GetName(), light1));
    return true;
  }
  if (type == MSG_LIGHT_UPDATE) {
    /*LightNode m;
    m.FromPacket(d);
    const StreamCoordinates & c = m.Position();
    int i;
    for (i=0; i<m_meshes.isize(); i++) {
      if (m_meshes[i].GetName() == m.GetName()) {
	m_meshes[i].SceneNode()->setPosition(core::vector3df(c[0],c[1],c[2]);
      }
      }*/
  }

  if (type == MSG_SCENENODE_ADD) {    
    MsgSceneNode m;
    m.FromPacket(d);
    AddSceneNode(m);
    return true;
  }
  if (type == MSG_SCENENODE_UPDATE) {    
    MsgSceneNode m;
    m.FromPacket(d);
    UpdateSceneNode(m);
    return true;
  }

  //============================================================
  if (type == MSG_MESH_ADD) {
    
    MeshModel m;
    m.FromPacket(d);
    AddMeshModel(m);
    return true;
  }
  //============================================================

  if (type == MSG_ANIMNODE_ADD) {
    AnimatedSceneNode m;
    video::SMaterial material;
    //cout << "Read from packet." << endl;
    m.FromPacket(d);
    const StreamCoordinates & coords = m.GetCoordinates();
    const StreamCoordinates & dir = m.GetDirection();

    AnimModel anim;
    cout << "Add" << endl;
    anim.m_pModel = smgr->addAnimatedMeshSceneNode(smgr->getMesh(m.GetModel().c_str()),
					   0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    anim.SetCoords(coords);
    anim.SetDirection(dir);

    anim.m_pModel->setScale(core::vector3df(m.GetScale())); // Make it appear realistically scaled
    cout << "Animation: " << endl;
    if (m.GetAnimation() != "") {      
      anim.SetAnimation(m.GetAnimation(), m.GetAnimationSpeed());
    }

    const StreamCoordinates & invis = m.GetInvisible();
    if (invis[0] > 0.) {
      video::ITexture* myImage = driver->getTexture(m.GetTexture().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2])); 
      material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    }

    material.setTexture(0, driver->getTexture(m.GetTexture().c_str()));
    material.Lighting = m.GetLighting();
    material.Shininess = m.GetShinyness();


    material.NormalizeNormals = true;
    anim.m_pModel->getMaterial(0) = material;
    anim.SetName(m.GetName());
    m_anim.push_back(anim);
    return true;
  }

  //=====================================================================
  if (type == MSG_PHYS_ADD) {
    AnimatedSceneNode m;
    cout << "Reading from package." << endl;
    m.FromPacket(d);
    const StreamCoordinates & coords = m.GetCoordinates();
    const StreamCoordinates & dir = m.GetDirection();

    scene::IAnimatedMeshSceneNode * pMM;

    cout << "Adding " << m.GetModel().c_str() << endl;

 

    pMM = smgr->addAnimatedMeshSceneNode(smgr->getMesh(m.GetModel().c_str()),
					 0, IDFlag_IsPickable | IDFlag_IsHighlightable);

    //std::cout << "Physics request node " << pMM << " mesh " << pMM->getMesh() << endl;

    
    //==================================================================
    video::SMaterial material;
    pMM->setPosition(core::vector3df(coords[0], coords[1], coords[2])); 
    pMM->setScale(core::vector3df(m.GetScale())); // Make it appear realistically scaled
    //cout << "Animation (PHYSICS): " << endl;
    //if (m.GetAnimation() != "") {      
    //anim.SetAnimation(m.GetAnimation(), m.GetAnimationSpeed());
    //}

    if (m.GetAnimation() != "") {
      pMM->setMD2Animation(m.GetAnimation().c_str());
      pMM->setAnimationSpeed(m.GetAnimationSpeed());
    } else {
      pMM->setAnimationSpeed(0.);
    }

    const StreamCoordinates & invis = m.GetInvisible();
    if (invis[0] > 0.) {
      video::ITexture* myImage = driver->getTexture(m.GetTexture().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2])); 
      material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    }

    material.setTexture(0, driver->getTexture(m.GetTexture().c_str()));
    material.Lighting = m.GetLighting();
    material.Shininess = m.GetShinyness();

    pMM->getMaterial(0) = material;

    material.NormalizeNormals = true;
    for (int i=1; i<m.GetTextureCount(); i++) {
      if (m.GetTexture(i) != "") {
	material.setTexture(0, driver->getTexture(m.GetTexture(i).c_str()));
	pMM->getMaterial(i) = material;
      }
    }
   
    //==================================================================
    /*
    video::SMaterial material;
    pMM->getMaterial(0) = material;
    
    const StreamCoordinates & invis = m.GetInvisible();
    if (invis[0] > 0.) {
      video::ITexture* myImage = driver->getTexture(m.GetTexture().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2]));      
      pMM->getMaterial(0).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    }

 

    //pMM->setScale(core::vector3df(10.6f));
    pMM->setScale(core::vector3df(m.GetScale())); 
    pMM->setMaterialFlag(video::EMF_LIGHTING, true);
    pMM->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    pMM->setMaterialTexture(0, driver->getTexture(m.GetTexture().c_str()));
    pMM->setPosition(core::vector3df(coords[0], coords[1], coords[2])); 
    if (m.GetAnimation() != "") {
      pMM->setMD2Animation(m.GetAnimation().c_str());
      pMM->setAnimationSpeed(m.GetAnimationSpeed());
    } else {
      pMM->setAnimationSpeed(0.);
    }
    pMM->getMaterial(0).Lighting = m.GetLighting();
    pMM->getMaterial(0).Shininess = m.GetShinyness();    
    */
 
    
    scene::IMesh * pMesh = pMM->getMesh();

    
   //==================================================================
    if (m.GetTransparent() >= 0.) {
      pMM->setMaterialType(video::EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA);
      scene::IMeshManipulator *manipulator = smgr->getMeshManipulator();
      
      manipulator->setVertexColorAlpha(pMesh, m.GetTransparent());
    }
   //==================================================================
   


    // Remove it from the cache.
    smgr->getMeshCache()->removeMesh(pMesh);

    
    if (pMesh == NULL) {
      std::cout << "ERROR, Mesh ptr == NULL" << std::endl;
    }

    m_meshes.push_back(MeshNode(m.GetName(), pMM));
    cout << "Phys SetDirection" << endl;
    m_meshes[m_meshes.isize()-1].SetDirection(dir);

    //std::cout << "Joints: " << pMM->getJointCount() << std::endl;
    for (int i=0; i<pMM->getJointCount(); i++) {
      scene::IBoneSceneNode * pJoint = pMM->getJointNode(i);
      //std::cout << "   " << pJoint->getName() << std::endl;
    }
    //std::cout << "Sending rot impulse for " << m.GetName() << ": ";
    m.GetRotImp().Print();
      
    SendMeshModel(pMesh, m.GetName(), pMM->getPosition(), m.GetRotImp(), m.PhysMode());

    cout << "Added mesh model, all done." << endl;
    
    return true;

  }




  if (type ==  MSG_NODE_ADD) {
    SceneNode sn;
    sn.FromPacket(d);
    const StreamCoordinates & coords = sn.GetCoordinates();
    const StreamCoordinates & dir = sn.GetDirection();

    scene::IMeshSceneNode* elm1 = 0;
  
    // Add an MD2 node, which uses vertex-based animation.
    elm1 = smgr->addMeshSceneNode(smgr->getMesh(sn.GetMesh().c_str()),
				  0, IDFlag_IsPickable | IDFlag_IsHighlightable);
    elm1->setPosition(core::vector3df(coords[0], coords[1], coords[2])); // Put its feet on the floor.
    elm1->setScale(core::vector3df(sn.GetScale(), sn.GetScale(), sn.GetScale())); // Make it appear realistically scaled

    video::SMaterial mat;
    // Make invisible
    const StreamCoordinates & invis = sn.GetInvisible();
    if (invis[0] > 0.) {
      video::ITexture* myImage = driver->getTexture(sn.GetTexture1().c_str());
      driver->makeColorKeyTexture(myImage, core::position2d<s32>(invis[1], invis[2])); 
      mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    }

    if (sn.GetTexture1() != "")
      mat.setTexture(0, driver->getTexture(sn.GetTexture1().c_str()));
    //mat.setTexture(0, driver->getTexture(sn.GetTexture1().c_str())  );
    if (sn.GetTexture2() != "")
      mat.setTexture(1, driver->getTexture(sn.GetTexture2().c_str()));
    //mat.Lighting = true;
    mat.Lighting = sn.GetLighting();
    mat.NormalizeNormals = true;
    elm1->getMaterial(0) = mat;

    
    if (sn.GetTexture2() != "") {
      //mat.setTexture(0, driver->getTexture(sn.GetTexture2().c_str()));
      //cout << "SET MATERIAL 1 w/ " << sn.GetTexture2() << endl;
      //elm1->getMaterial(1) = mat;
      //elm1->setMaterialTexture(1, driver->getTexture(sn.GetTexture2().c_str()));
      //elm1->setMaterialTexture(2, driver->getTexture(sn.GetTexture2().c_str()));
      //elm1->getMaterial(1).TextureLayer[0].Texture = driver->getTexture(sn.GetTexture2().c_str());
    }


    core::vector3df idir;
    SphereCoordinates sp = dir.AsSphere();
    idir.Y = 360.*sp.phi()/2./IPI;
    idir.X = idir.Z = 0.;
    cout << "Direction (node): " << sp.phi() << " -> " << idir.Y << endl;
    
    elm1->setRotation(idir);


    cout << "Added NODE." << endl;
    return true;

  }
  if (type == MSG_MESH_UPDATE) {
    MeshModel model;
    //std::cout << "Packet to mesh" << std::endl;
    model.FromPacket(d);
    //std::cout << "Done, calling update" << std::endl;
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
      //std::cout << "ERROR, Model not found. " << std::endl;
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
  std::cout << "Start threads." << std::endl;

  ThreadHandler th;

  //SourceData sound_1;
  //sound_1.SetWavFile("data/Sounds/happy.wav");

  core::vector3df lastCamPosition = camera->getPosition();

  //sound_1.SetCoords(StreamCoordinates(lastCamPosition.X+5, lastCamPosition.Y, lastCamPosition.Z+5));
  //audioDat.AddSource(sound_1);


  AudioThread * pAudio = new AudioThread(&audioDat, 0);
  string init = "do";
  th.AddThread(pAudio, init);    
  th.Feed(0, init);

  std::cout << "Start running." << std::endl;



  u32 then = device->getTimer()->getTime();
  
  // This is the movement speed in units per second.
  const f32 MOVEMENT_SPEED = 3.f;
  //core::vector3df lastCamPosition = camera->getPosition();
  
  
  int lastFPS = -1;
  double downFrameCounter = 0;
  
  double iau_scale = 100.;
  
  char tmp[256];
  
  double absolutTime = 0.;

  // camera->bindTargetAndRotation(false);


  double angle = 0.;
  double angle_speed = 0.1;

  bool bFirst = true;

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  // Message Box 
  //device->getGUIEnvironment()->addMessageBox(L"Test", L"Let's show some text here.");

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  // Font
  gui::IGUIEnvironment* env = device->getGUIEnvironment();
  gui::IGUIFont *fnt = env->getFont("irrlicht-code/media/fonthaettenschweiler.bmp");
  if (fnt == NULL) {
    cout << "FATAL ERROR: font not found!!!" << endl;
  }

  string globaltext;

  core::vector3df initPosition;
  initPosition.X = 2300;
  initPosition.Y = WALK_HEIGHT;
  initPosition.Z = 2900;
  camera->setPosition(initPosition);

  
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
	core::vector3df camRotation = camera->getRotation();
	core::vector3df camUp = camera->getUpVector();

	audioDat.SetSetCamPos(StreamCoordinates(camPosition.X, camPosition.Y, camPosition.Z));
	audioDat.SetCamRot(StreamCoordinates(camRotation.X*2.*IPI/360., camRotation.Y*2.*IPI/360., camRotation.Z*2.*IPI/360.));

	//-----------------------------------------------
	//camPosition.Z += 2;
	if (camPosition.Y != WALK_HEIGHT) {
	  camPosition.Y = WALK_HEIGHT;
	  camera->setPosition(camPosition);
	}
	if (camPosition.X < 120) {
	  camPosition.X = 120;
	  camera->setPosition(camPosition);
	}
	if (camPosition.X > 4350) {
	  camPosition.X = 4350;
	  camera->setPosition(camPosition);
	}
	
	if (camPosition.Z < 2220) {
	  camPosition.Z = 2220;
	  camera->setPosition(camPosition);
	}
	if (camPosition.Z > 4880) {
	  camPosition.Z = 4880;
	  camera->setPosition(camPosition);
	}


	
	if(receiver->IsKeyDown(irr::KEY_KEY_A)) {
	  angle -= angle_speed * frameDeltaTime;
	} else if(receiver->IsKeyDown(irr::KEY_KEY_D)) {
	  angle += angle_speed * frameDeltaTime;
	}
	SetCameraTilt(angle);

	string keypressed;
	if(receiver->IsKeyDown(irr::KEY_KEY_B)) {
	  keypressed += "B";
	}
	if(receiver->IsKeyDown(irr::KEY_KEY_N)) {
	  keypressed += "N";
	}
	if(receiver->IsKeyDown(irr::KEY_KEY_M)) {
	  keypressed += "M";
	}
	if(receiver->IsKeyDown(irr::KEY_KEY_G)) {
	  keypressed += "G";
	}
	if(receiver->IsKeyDown(irr::KEY_KEY_H)) {
	  keypressed += "H";
	}
	if(receiver->IsKeyDown(irr::KEY_KEY_J)) {
	  keypressed += "J";
	}
	if(receiver->IsKeyDown(irr::KEY_BACK)) {
	  keypressed += "BACK";
	}
	if(receiver->IsKeyDown(irr::KEY_TAB)) {
	  keypressed += "TAB";
	}

	if (keypressed != "") {
	  //Send key strokes
	  DataPacket key;
	  MessageHeader keyhead;
	  keyhead.ToPacket(key);
	  key.Write("keypressed");	 
	  key.Write(keypressed);	 
	  m_pTrans->Send(key);
	}

       	

	DataPacket d;
	MessageHeader outhead;
	outhead.ToPacket(d);
	d.Write("position");
	d.Write((double)camPosition.X);
	d.Write((double)camPosition.Y);
	d.Write((double)camPosition.Z);
	
	cout << "Send position to server." << endl;
	m_pTrans->Send(d);

	string msg, dir;

	svec<MsgText> allText;
	//MsgText globalText;
	
	while (m_pRec->Get(d)) {  
	  MessageHeader inhead;
	  inhead.FromPacket(d);
	  //std::cout << "RECEIVED " << inhead.GetTimeStamp().GetReadable() << " -> " << inhead.GetHeader() << std::endl;
	  bool bHandled = ProcessMessage(inhead.GetHeader(), d);
	  if (bHandled) 
	    continue;

	  if (inhead.GetHeader() == "MSG_TEXT") {
	    MsgText text;
	    text.FromPacket(d);
	    allText.push_back(text);
	    continue;
	  }
	    
	  cout << "WARNING: Unhandled message " << inhead.GetHeader() << ", removing from stack." << endl;
	  d.Read(msg);
	  d.Read(dir);	 	  
	}


	double dd = Dist(core::vector3df(xp, yp, zp), camPosition);     	
	
	//driver->beginScene(true, true, 0 );
	driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, video::SColor(0));
	
	smgr->drawAll();
	env->drawAll();


	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	
	for (int x=0; x<allText.isize(); x++) {
	  const MsgText & mText = allText[x];
	  const string & wide = mText.Text();
	  wchar_t * pWide = new wchar_t[wide.length()+1];
	  core::utf8ToWchar(wide.c_str(), pWide, 2*(wide.length()+1));
	  
	  core::dimension2d<u32> size = fnt->getDimension(pWide);
	  fnt->draw(pWide,core::rect<s32>(mText.X(),mText.Y(), (mText.X()+size.Width),(mText.Y()+size.Height)), video::SColor(mText.R(),mText.G(),mText.B(),255));
	  delete [] pWide;
	}

	
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
