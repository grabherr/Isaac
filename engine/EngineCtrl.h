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

class Compound
{
 public:
  Compound() {
    m_scale = 1.;
  }
  void SetScale(double s) {m_scale = s;}

  int isize() const {return m_list.isize();}
  void push_back(const AnimProp & a) {
    m_list.push_back(a);
  }
  const AnimProp & operator[] (int i) const {return m_list[i];}
  AnimProp & operator[] (int i) {return m_list[i];}
  PhysObject & GetPhysObject() {return m_master;}

  void Update(double deltatime, double gravity = 9.81);

 private:
  svec<AnimProp> m_list;
  PhysObject m_master;
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

  // Adds a mesh model
  void AddMeshModel(const MeshModel & a);

  // Adds a compound
  void AddCompound(const svec<AnimatedSceneNode> & a);

  // For debugging only
  void AddTriangle(const SolidTriangle & t) {
    m_triangles.push_back(t);
  }

  int GetNodeCount() const {return m_props.isize();}
  int GetAnimCount() const {return m_objects.isize() + m_animInComp;}

  const SceneNode & GetProp(int i) const {return m_props[i].GetSceneNodeConst();}
  const AnimatedSceneNode & GetAnimated(int i) const {
    // STUPID Implementation
    if (i < m_objects.isize())
      return m_objects[i].GetAnimNodeConst();
    i -= m_objects.isize();
    int n = 0;
    for (int j=0; j<m_compounds.isize(); j++) {
      const Compound & cc = m_compounds[j];
      for (int k=0; k<cc.isize(); k++) {
	if (i == n) {
	  return cc[k].GetAnimNodeConst();
	}      
	n++;
      }
    }
    static AnimatedSceneNode dummy;
    return dummy;
  }

  void Start();
  void Run();
  
  // For debugging only!!!
  void GetCubeModel(MeshModel & m);
 
 private:
  bool CheckCollision(PhysObject & o);

  PhysObject m_testCube;

  svec<PhysObject> m_phys;

  svec<AnimProp> m_objects;
  svec<SceneProp> m_props;
  svec<Compound> m_compounds;
  
  int m_animInComp;
  double m_gravity;
  FrameClock m_clock;
  double m_lastTime;
  double m_scale;
  svec<SolidTriangle> m_triangles;
};



#endif //ENGINECTRL_H
