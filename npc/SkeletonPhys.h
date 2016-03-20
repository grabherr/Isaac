// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef SKELETONWITHPHYSICS_H
#define SKELETONWITHPHYSICS_H

#include "npc/Skeleton.h"
#include "physics/Physics.h"

class NPCPhys
{
 public:
  NPCPhys() {
    m_childMass = 0.;
  }
  
  NPCBoneCoords & RotImp() {return m_p;}
  const NPCBoneCoords & RotImp() const {return m_p;}
  
  double GetMass() const {return m_p.Radius();}
  void SetMass(double m) {m_p.Radius() = m;}

  void Print() const {
    m_p.Print();
  }
  void AddToChildMass(double d) {
    m_childMass += d;
  }
  double ChildMass() const {return m_childMass;}
 private:
  // strore the impulse here
  NPCBoneCoords m_p;
  double m_childMass;
};


class NPCSkeletonWithPhysics : public NPCSkeleton
{
 public:
  NPCSkeletonWithPhysics() {
    m_gravity = 9.81/10.;
    m_floorY = -1000.;
  }

  void SetupPhysics();

  void UpdateAndSync(double deltatime);

  void SetAbsOffset(const Coordinates & c) {
    m_absoffset = c;
  }

  void SetFloorY(double d) {
    m_floorY = d;
  }
  void UpdateFromPhys();
  void UpdateToPhys();

 protected:
  void UpdatePhys();

  void ForceDiffOne(NPCBone & bone, int dim, double val); 
  void ForceDiffCoords(NPCBone & bone, const Coordinates & c); 

  double m_gravity;
  svec<NPCPhys> m_physics;
  PhysObject m_physObj;
  Coordinates m_absoffset;
  Coordinates m_rootSpeed;
  double m_floorY;
};


#endif 
