// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html



#ifndef ENGINECTRL_H
#define ENGINECTRL_H

#include "physics/Physics.h"
#include "graphics/Messages.h"
#include "creature/Isaac.h"
#include "engine/FrameClock.h"
#include "engine/IManipulator.h"

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

class GameNPC
{
 public:
  GameNPC() {
    m_physIndex = -1;
  }

  void SetName(const string & s) {
    m_name = s;
  }
  const string & GetName() const {return m_name;}
  
  void Update(PhysObject & o, double deltatime);

 private:
  int m_physIndex;
  string m_name;
};

class GamePhysObject
{
 public:
  GamePhysObject() {
    m_npcIndex = -1;
    m_pManip = NULL;
  }

  void SetManipulator(IManipulator * p) {
    m_pManip = p;
  }
  IManipulator * GetManipulator() {return m_pManip;}
  void StartFeed() {
    if (m_pManip != NULL)
      m_pManip->StartFeed();
  }
  void Feed(GamePhysObject & other) {
    if (m_pManip != NULL)
      m_pManip->Feed(other);
  }
  void DoneFeed() {
    if (m_pManip != NULL)
      m_pManip->DoneFeed();
  }
  void Interact(GamePhysObject & other) {
    if (m_pManip != NULL)
      m_pManip->Interact(other);
  }
  void CamPos(const Coordinates & c) {
    if (m_pManip != NULL)
      m_pManip->CamPos(c);
  }

  void SetName(const string & s) {
    m_name = s;
  }
  const string & GetName() const {return m_name;}

  void Update(double deltatime, double gravity);
  void GetMeshModel(MeshModel & m);
  void SetMeshModel(const MeshModel & m);

  PhysObject & GetPhysObject() {return m_phys;}
  const PhysObject & GetPhysObjectConst() const {return m_phys;}

 private:
  IManipulator * m_pManip;
  PhysObject m_phys;
  int m_npcIndex;
  string m_name;
};





//--------------------------------------------------------------
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
  void AddMeshModel(const MeshModel & a, IManipulator * p = NULL);

  // Adds a compound
  void AddCompound(const svec<AnimatedSceneNode> & a);

  // For debugging only
  void AddTriangle(const SolidTriangle & t) {
    m_triangles.push_back(t);
  }
  
  void DoTriangleCollision(bool b) {m_bDoTriangleCollision = b;}
  void DoObjectCollision(bool b ) {m_bDoObjectCollision = b;}

  void RegisterCompound(IManipulator * p);

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
  void Run(const Coordinates & camPos);
  
  // For debugging only!!!
  void GetCubeModel(MeshModel & m);

  int GetObjectCount() const {return m_phys.isize();}
  void GetObjectModel(int i, MeshModel & m);

  void SetGravity(double g) {m_gravity = g;}
  
 private:
  int PhysIndex(const string & name) const {
    for (int i=0; i<m_phys.isize(); i++) {
      if (m_phys[i].GetName() == name)
	return i;
    }
    return -1;
  }

  int NPCIndex(const string & name) const {
    for (int i=0; i<m_npc.isize(); i++) {
      if (m_npc[i].GetName() == name)
	return i;
    }
    return -1;
  }

  bool CheckCollision(PhysObject & o);

  PhysObject m_testCube;

  svec<GamePhysObject> m_phys;
  svec<GameNPC> m_npc;

  svec<AnimProp> m_objects;
  svec<SceneProp> m_props;
  svec<Compound> m_compounds;
  svec<IManipulator*> m_custom; 

  int m_animInComp;
  double m_gravity;
  FrameClock m_clock;
  double m_lastTime;
  double m_scale;
  svec<SolidTriangle> m_triangles;
  bool m_bDoTriangleCollision;
  bool m_bDoObjectCollision;
};



#endif //ENGINECTRL_H
