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
  SceneProp() {}

  bool NeedUpdate() const {return m_phys.isize() > 0;}

  const SceneNode & GetSceneNodeConst() const {return m_node;}
  SceneNode & GetSceneNode() {return m_node;}
  PhysObject & GetPhysObject() {return m_phys;}

  void Update(double deltatime, double gravity = 9.81);

 private:
  SceneNode m_node;
  PhysObject m_phys;
};

// Animated object
class AnimProp
{
 public:
  AnimProp() {}

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
};

class GameControl
{
 public:
  GameControl() {
    m_gravity = 9.81;
    Start();
  }

  void SetFrameRate(int n) {
    m_clock.SetFrameRate(n);
  }

  void AddProp(const SceneNode & n);
  void AddAnimated(const AnimatedSceneNode & a);

  int GetNodeCount() const {return m_props.isize();}
  int GetAnimCount() const {return m_anim.isize();}

  const SceneNode & GetProp(int i) const {return m_props[i].GetSceneNodeConst();}
  const AnimatedSceneNode & GetAnimated(int i) const {return m_anim[i].GetAnimNodeConst();}

  void Start();
  void Run();

 private:
  svec<AnimProp> m_anim;
  svec<SceneProp> m_props;
  double m_gravity;
  FrameClock m_clock;
  double m_lastTime;
  
};



#endif //ENGINECTRL_H
