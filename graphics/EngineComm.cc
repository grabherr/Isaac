#include "graphics/EngineComm.h"

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

bool GUIEngineControl::GetDataPacket(DataPacket & d)
{
  return m_pRec->Get(d);
}
