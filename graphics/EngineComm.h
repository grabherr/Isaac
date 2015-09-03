// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef ENGINECOMM_H
#define ENGINECOMM_H

#include "util/StreamComm.h"
#include "graphics/Messages.h"




class GUIEngineControl
{
 public:
  GUIEngineControl(const string & hostname = "localhost");
  ~GUIEngineControl();

  void SetGraphicsEngine(const string & executable) {
    m_graphics = executable;
  }
  // Call this before starting the graphics
  void SetTerrain(const Terrain & terrain);

  void StartGraphics(int resX, int resY, bool fullScreen);

  void AddNode(const SceneNode & n);
  void AddAnimatedNode(const AnimatedSceneNode & n);
  void AddPhysicsNode(const AnimatedSceneNode & n);
  void AddLightNode(const LightNode & n);

  void UpdateNode(const SceneNode & n);
  void UpdateAnimatedNode(const AnimatedSceneNode & n);
  void UpdateMeshModel(const MeshModel & n);
  void UpdateLightNode(const LightNode & n);
  void AddMeshModel(const MeshModel & n);

  // Retrieves raw messages from the GUI
  bool GetDataPacket(DataPacket & d);

 private:
  StreamCommTransmitter * m_pTrans; 
  StreamCommReceiver * m_pRec;
  string m_graphics;
  Terrain m_terrain;
};

#endif //ENGINECOMM_H
