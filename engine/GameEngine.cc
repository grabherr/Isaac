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
  }
  for (i=0; i<c.GetAnimatedNodeCount(); i++) {
    const AnimatedSceneNode & anim = c.GetAnimatedNode(i);
    m_graphics.AddAnimatedNode(anim);
  }
}

void GameEngine::Run()
{
  while (true) {
    DataPacket d;
    while (m_graphics.GetDataPacket(d)) {  
      string msg;
      double x, y, z;
      MessageHeader tmp;
      tmp.FromPacket(d);
      d.Read(msg);
      d.Read(x);
      d.Read(y);
      d.Read(z);
      //cout << msg << " " << x << " " << y << " " << z << endl;
    }
    usleep(1000);
  }
}
