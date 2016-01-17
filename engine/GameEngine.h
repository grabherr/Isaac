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

  void RegisterGlobal(IGlobal * p) {
    m_globals.push_back(p);
  }

  virtual void SetupMap(int n);
  virtual void Run();

  void AddLight(const MsgLightNode & l);

  void AddSceneNode(const MsgSceneNode & n, IManipulator * p = NULL);

  // WARNING: Will be obsolete!!!
  // Load a model from file
  void AddAnimatedModel(const AnimatedSceneNode & m, IManipulator * p = NULL);

  // Load a model from file
  void AddSceneNode(const SceneNode & m, IManipulator * p = NULL);

  // All the verices etc. have to be defined explicitely
  void AddMeshModel(const MeshModel & m, IManipulator * p = NULL);
  void AddMeshModelSilent(const MeshModel & m);

  void RegisterCompound(IManipulator * p);
  void DoTriangleCollision(bool b) {m_ctrl.DoTriangleCollision(b);}
  void DoObjectCollision(bool b ) {m_ctrl.DoObjectCollision(b);}
  void DoAllInteractions(bool b) {m_ctrl.DoAllInteractions(b);}

 protected:
  GameControl m_ctrl;
  GameConfig m_config;
  GUIEngineControl m_graphics;
  Entity m_template;

  void Push(IManipulator*, const string & name);
  IManipulator * Pop(const string & name);
  
  svec<IManipulator*> m_manipCache;
  svec<string> m_cacheName;

  Coordinates m_camPos;
  svec<IGlobal*> m_globals;
};




#endif //GAMEENGINE
