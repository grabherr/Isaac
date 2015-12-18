#include "engine/GameEngine.h"
#include <unistd.h>


GameEngine::GameEngine()
{
 
}

GameEngine::~GameEngine()
{
}

void GameEngine::Push(IManipulator * p, const string & name)
{
  cout << "Engine: push manip for " << name << endl;
  m_manipCache.push_back(p);
  m_cacheName.push_back(name);
}

IManipulator * GameEngine::Pop(const string & name)
{
  int i;
  IManipulator * p = NULL;
  cout << "Engine: look for " << name << endl;
  for (i=0; i<m_manipCache.isize(); i++) {
    if (m_cacheName[i] == name) {
      p = m_manipCache[i];
      cout << "Engine: found manip for " << name << endl;
      m_manipCache[i] = m_manipCache[m_manipCache.isize()-1];
      m_cacheName[i] = m_cacheName[m_manipCache.isize()-1];
      m_manipCache.resize(m_manipCache.isize()-1);
      m_cacheName.resize(m_cacheName.isize()-1);
      break;
    }
  }
  return p;
}


void GameEngine::ReadConfig(const string & fileName)
{
  m_config.Read(fileName);
  m_graphics.SetGraphicsEngine(m_config.GetBasicConfig().GetGraphicsEngine());
  m_template = m_config.GetTemplate();
}

void GameEngine::RegisterCompound(IManipulator * p)
{
  throw;
  /*
  m_ctrl.RegisterCompound(p);
  int i;
  // NOTE: This adds all the models at the beginning!!!
  for (i=0; i<p->GetNewNodeCount(); i++) {
    const AnimatedSceneNode & a = p->GetNewAnimNode(i);
    m_graphics.AddAnimatedNode(a);
    }*/
}

void GameEngine::AddMeshModel(const MeshModel & m, IManipulator * p)
{
  m_graphics.AddMeshModel(m);
  cout << "Engine: add mesh model " << m.GetName() << " manip " << p << endl;
  m_ctrl.AddMeshModel(m, p);
}

void GameEngine::AddLight(const MsgLightNode & l)
{
  m_graphics.AddLightNode(l);
}
  
void GameEngine::AddSceneNode(const SceneNode & m, IManipulator * p)
{
  m_graphics.AddNode(m);
  if (p != NULL)
    Push(p, m.GetName());

}

//---------------------------------------------------------------
void GameEngine::AddSceneNode(const MsgSceneNode & m, IManipulator * p)
{
  m_graphics.AddSceneNode(m);
  if (p != NULL)
    Push(p, m.GetName());
}
//---------------------------------------------------------------

void GameEngine::AddAnimatedModel(const AnimatedSceneNode & m, IManipulator * p)
{
  m_graphics.AddPhysicsNode(m);
  if (p != NULL)
    Push(p, m.GetName());
}

void GameEngine::SetupMap(int n)
{
  if (n >= m_config.NumMaps()) {
    cout << "ERROR, map not found." << endl;
    return;
  }
  
  const MapConfig & c = m_config.GetMap(n);
  m_graphics.SetTerrain(c.GetTerrain());
  
  m_graphics.StartGraphics(m_config.GetBasicConfig().ResX(),
			   m_config.GetBasicConfig().ResY(),
			   m_config.GetBasicConfig().Full());

  cout << "Set gravity to " << c.GetGravity() << endl;
  m_ctrl.SetGravity(c.GetGravity());
  
  int i;
  for (i=0; i<c.GetNodeCount(); i++) {
    const SceneNode & node = c.GetNode(i);
    m_graphics.AddNode(node);
    m_ctrl.AddProp(node);
  }
  for (i=0; i<c.GetAnimatedNodeCount(); i++) {
    const AnimatedSceneNode & anim = c.GetAnimatedNode(i);
    m_graphics.AddAnimatedNode(anim);
    m_ctrl.AddObject(anim);
    cout << "Adding animated w/ name " << anim.GetName() << endl;
    // cout << "After pushback" 
  }
  for (i=0; i<c.GetPhysModelCount(); i++) {
    const AnimatedSceneNode & anim = c.GetPhysModelNode(i);
    m_graphics.AddPhysicsNode(anim);
    cout << "Adding physics w/ name " << anim.GetName() << endl;
    // cout << "After pushback" 
  }
}

void GameEngine::Run()
{
  m_ctrl.Start();
  int i;

  while (true) {
    DataPacket d;
    while (m_graphics.GetDataPacket(d)) {  
      string msg;
      double x, y, z;
      MessageHeader tmp;
      tmp.FromPacket(d);
      d.Read(msg);
      cout << "DEBUG: Got Message " << msg << endl;
      string message;
      //d.Read(message);
      cout << "From graphics: " << msg << endl;
      if (msg == MSG_MESH_ADD) {
	MeshModel mm;
	mm.FromPacket(d);
	cout << "ADD Mesh model " << mm.GetName() << endl;
      
	IManipulator * pManip = Pop(mm.GetName());
	if (pManip != NULL) 
	  cout << "Retrieved cached Manipulator!!" << endl;
      
	m_ctrl.AddMeshModel(mm, pManip);
	//cout << " do something " << endl;
      }

      //=======================================================
      if (msg == MSG_SCENENODE_ADD) {
	MsgSceneNode mm;
	mm.FromPacket(d);
	cout << "ADD Scene Node " << mm.GetName() << endl;
      
	IManipulator * pManip = Pop(mm.GetName());
	if (pManip != NULL) 
	  cout << "Retrieved cached Manipulator!!" << endl;
      
	m_ctrl.SceneNodeFromLoopBack(mm, pManip);       
      }
      //=======================================================
      

      if (msg == "position") {
	cout << "Got coordinates";
	d.Read(x);
	d.Read(y);
	d.Read(z);
	m_camPos = Coordinates(x, y, z);
	m_camPos.Print();
	cout << endl;
      }
      //cout << msg << " " << x << " " << y << " " << z << endl;
    }
     // Communicate w/ graphics engine
    for (i=0; i<m_ctrl.GetNodeCount(); i++) {
      const SceneNode & n = m_ctrl.GetProp(i);
      cout << "Update prop " << n.GetName() << endl;
      m_graphics.UpdateNode(n);      
    }
    
    for (i=0; i<m_ctrl.GetAnimCount(); i++) {
      const AnimatedSceneNode & a = m_ctrl.GetAnimated(i);
      cout << "Update animated " << a.GetName() << endl;
      m_graphics.UpdateAnimatedNode(a);      
    }
    

    for (i=0; i<m_ctrl.GetObjectCount(); i++) {
      MeshModel meshObj;
      MsgSceneNode sn;
      //===========================================================
      // Switch over to MsgSceneNode HERE!!
      //===========================================================
      if (m_ctrl.IsSceneNode(i)) {
	m_ctrl.GetObjectModel(i, sn);	
	cout << "Sending update for scene node " << sn.GetName() << endl;
	m_graphics.UpdateSceneNode(sn);
      } else {
	m_ctrl.GetObjectModel(i, meshObj);
	meshObj.RecomputeNormals();
	cout << "Sending update for mesh " << meshObj.GetName() << endl;
	m_graphics.UpdateMeshModel(meshObj);
      }

    }


    // DEBUG
    //MeshModel mesh;
    //m_ctrl.GetCubeModel(mesh);
    //m_graphics.UpdateMeshModel(mesh);
    // Allow for action
    int x;
    for (x=0; x<m_globals.isize(); x++)   
      m_globals[x]->StartFrame(0);

    m_ctrl.Run(m_camPos);

    for (x=0; x<m_globals.isize(); x++)   
     m_globals[x]->EndFrame(0);

  }
}
