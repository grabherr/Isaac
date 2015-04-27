#ifndef ENGINECOMM_H
#define ENGINECOMM_H

#include "util/StreamComm.h"
#include "graphics/Messages.h"




class GUIEngineControl
{
 public:
  GUIEngineControl();
  ~GUIEngineControl();

  void SetTerrain(const Terrain & terrain);
  

 private:
  StreamCommTransmitter * m_pTrans; 
  StreamCommReceiver * m_pRec;
};

#endif //ENGINECOMM_H

