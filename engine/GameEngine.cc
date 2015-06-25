#include "engine/GameEngine.h"
#include <unistd.h>

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReadConfig(const string & fileName)
{
  m_config.Read(fileName);
  m_graphics.SetGraphicsEngine(m_config.GetBasicConfig().GetGraphicsEngine());
  m_template = m_config.GetTemplate();
}
 
void GameEngine::SetupMap(int n)
{
  if (n >= m_config.NumMaps()) {
    cout << "ERROR, map not found." << endl;
    return;
  }
  
  const MapConfig & c = m_config.GetMap(n);
  m_graphics.SetTerrain(c.GetTerrain());
  
  m_graphics.StartGraphics();
  
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
      string message;
      //d.Read(message);
      cout << "From graphics: " << msg << endl;
      if (msg == MSG_MESH_ADD) {
	MeshModel mm;
	mm.FromPacket(d);
	m_ctrl.AddMeshModel(mm);
	//cout << " do something " << endl;
      }

      if (msg == "position") {
	cout << "Got coordinates" << endl;
	d.Read(x);
	d.Read(y);
	d.Read(z);
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
    
    // DEBUG
    MeshModel mesh;
    m_ctrl.GetCubeModel(mesh);
    m_graphics.UpdateMeshModel(mesh);
    // Allow for action
    m_ctrl.Run();
  }
}
