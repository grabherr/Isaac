#ifndef ENGINECOMM_H
#define ENGINECOMM_H

#include "util/StreamComm.h"
#include "graphics/Messages.h"




class GUIEngineControl
{
 public:
  GUIEngineControl(const string & hostname = "localhost");
  ~GUIEngineControl();

  void SetTerrain(const Terrain & terrain);
  void AddNode(const SceneNode & n);
  void AddAnimatedNode(const AnimatedSceneNode & n);

  void UpdateNode(const SceneNode & n);
  void UpdateAnimatedNode(const AnimatedSceneNode & n);

  // Retrieves raw messages from the GUI
  bool GetDataPacket(DataPacket & d);

 private:
  StreamCommTransmitter * m_pTrans; 
  StreamCommReceiver * m_pRec;
};

#endif //ENGINECOMM_H

