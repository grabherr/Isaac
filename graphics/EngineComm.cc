#include "graphics/EngineComm.h"
#include <unistd.h>

GUIEngineControl::GUIEngineControl(const string & hostname)
{
  m_pTrans = GetTransmitter(hostname, DEFPORT);
  m_pRec = GetReceiver(DEFPORT + 1);
 }

GUIEngineControl::~GUIEngineControl()
{
  delete m_pTrans;
  delete m_pRec;
}

void GUIEngineControl::SetTerrain(const Terrain & terrain)
{
  m_terrain = terrain;
}

void GUIEngineControl::AddNode(const SceneNode & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_NODE_ADD);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);}

void GUIEngineControl::AddAnimatedNode(const AnimatedSceneNode & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_ANIMNODE_ADD);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

void GUIEngineControl::AddPhysicsNode(const AnimatedSceneNode & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_PHYS_ADD);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);
}
void GUIEngineControl::AddLightNode(const LightNode & n)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_LIGHT_ADD);
  header.ToPacket(fairy);
  n.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

void GUIEngineControl::UpdateLightNode(const LightNode & n)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_LIGHT_UPDATE);
  header.ToPacket(fairy);
  n.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

void GUIEngineControl::UpdateNode(const SceneNode & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_NODE_UPDATE);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

void GUIEngineControl::UpdateAnimatedNode(const AnimatedSceneNode & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_ANIMNODE_UPDATE);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

void GUIEngineControl::UpdateMeshModel(const MeshModel & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_MESH_UPDATE);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

void GUIEngineControl::AddMeshModel(const MeshModel & m)
{
  DataPacket fairy;
  MessageHeader header;
  header.SetHeader(MSG_MESH_ADD);
  header.ToPacket(fairy);
  m.ToPacket(fairy);
  m_pTrans->Send(fairy);
}

bool GUIEngineControl::GetDataPacket(DataPacket & d)
{
  return m_pRec->Get(d);
}



void GUIEngineControl::StartGraphics(int resX, int resY, bool fullScreen)
{
  // First, start engine
  string cmmd = m_graphics;
  char tmp[256];

  sprintf(tmp, " %d %d ", resX, resY);
  cmmd += tmp;
  
  if (fullScreen)
    cmmd += " true ";
  else
    cmmd += " false ";
  cmmd += " > video_engine.log &";
  cout << "Starting " << cmmd << endl;
  int r = system(cmmd.c_str());

  DataPacket d;
  // Wait until we can send the terrain/the map.
  cout << "Waiting for engine (feed terrain)..." << endl;
  while (true) {
    while (!m_pRec->PeekLast(d)) {  
      usleep(1000);
    }
    string msg;
    MessageHeader tmp;
    tmp.FromPacket(d);
    d.Read(msg);
    //cout << "Wait Loop: Message: " << msg << endl;
    if (msg == "waiting_for_terrain") {
      break;
    } else {
      // Do nothing.
    }
  }
  cout << "## Sending terrain..." << endl;
 

  // Next, send terrain

  DataPacket dd;
  MessageHeader header;
  header.SetHeader(MSG_TERRAIN);
  header.ToPacket(dd);
  m_terrain.ToPacket(dd);
  m_pTrans->Send(dd);
  
  
 
  // Wait until the client is up.
  cout << "Waiting for engine..." << endl;
  while (true) {
    while (!m_pRec->PeekLast(d)) {  
      usleep(1000);
    }
    string msg;
    MessageHeader tmp;
    tmp.FromPacket(d);
    d.Read(msg);
    //cout << "Wait Loop: Message: " << msg << endl;
    if (msg == "engine_ready") {
      break;
    } else {
      // Do nothing.
    }
  }
  cout << "################## Engine is initialized #############" << endl;
}


