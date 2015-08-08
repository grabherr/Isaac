// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "engine/GameConfig.h"
#include "graphics/EngineComm.h"
#include "engine/EngineCtrl.h"





class GameEngine
{
 public:
  GameEngine();
  virtual ~GameEngine();

  void ReadConfig(const string & fileName);

  void SetScale(double s) {
    m_ctrl.SetScale(s);
  }
  virtual void SetupMap(int n);
  virtual void Run();

  // Load a model from file
  void AddAnimatedModel(const AnimatedSceneNode & m, IManipulator * p = NULL);

  // All the verices etc. have to be defined explicitely
  void AddMeshModel(const MeshModel & m, IManipulator * p = NULL);
  void RegisterCompound(IManipulator * p);

 protected:
  GameControl m_ctrl;
  GameConfig m_config;
  GUIEngineControl m_graphics;
  Entity m_template;
};




#endif //GAMEENGINE
