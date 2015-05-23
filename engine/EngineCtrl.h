#ifndef ENGINECTRL_H
#define ENGINECTRL_H

#include "physics/Physics.h"
#include "graphics/Messages.h"
#include "creature/Isaac.h"
#include "engine/FrameClock.h"

// Simple object
class SceneProp
{
 public:
  SceneProp() {
    m_scale = 1.;
  }

  void SetScale(double s) {m_scale = s;}

  bool NeedUpdate() const {return m_phys.isize() > 0;}

  const SceneNode & GetSceneNodeConst() const {return m_node;}
  SceneNode & GetSceneNode() {return m_node;}
  PhysObject & GetPhysObject() {return m_phys;}

  void Update(double deltatime, double gravity = 9.81);

 private:
  SceneNode m_node;
  PhysObject m_phys;
  double m_scale;
};

// Animated object
class AnimProp
{
 public:
  AnimProp() {
    m_scale = 1.;
  }
  
  void SetScale(double s) {m_scale = s;}

  // void Feed(...); feed the game info here
  void Update(double deltatime, double gravity = 9.81);

  const AnimatedSceneNode & GetAnimNodeConst() const {return m_anim;}
  AnimatedSceneNode & GetAnimNode() {return m_anim;}
  Isaac & GetIsaac() {return m_isaac;}
  PhysObject & GetPhysObject() {return m_phys;}

 private:
  AnimatedSceneNode m_anim;
  Isaac m_isaac;
  PhysObject m_phys;
  double m_scale;
};

class GameControl
{
 public:
  GameControl();

  void SetScale(double s) {m_scale = s;}

  void SetFrameRate(int n) {
    m_clock.SetFrameRate(n);
  }

  // Uses simple physics, no NPC
  void AddProp(const SceneNode & n);

  // Adds an object with physics and optinal NPC control
  void AddObject(const AnimatedSceneNode & a);

  // For debugging only
  void AddTriangle(const SolidTriangle & t) {
    m_triangles.push_back(t);
  }

  int GetNodeCount() const {return m_props.isize();}
  int GetAnimCount() const {return m_objects.isize();}

  const SceneNode & GetProp(int i) const {return m_props[i].GetSceneNodeConst();}
  const AnimatedSceneNode & GetAnimated(int i) const {return m_objects[i].GetAnimNodeConst();}

  void Start();
  void Run();

 private:
  bool CheckCollision(PhysObject & o);

  svec<AnimProp> m_objects;
  svec<SceneProp> m_props;
  double m_gravity;
  FrameClock m_clock;
  double m_lastTime;
  double m_scale;
  svec<SolidTriangle> m_triangles;
};



#endif //ENGINECTRL_H
