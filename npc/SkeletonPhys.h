#ifndef SKELETONWITHPHYSICS_H
#define SKELETONWITHPHYSICS_H

#include "npc/Skeleton.h"
#include "physics/Physics.h"

class NPCSkeletonWithPhysics : public NPCSkeleton
{
 public:
  NPCSkeletonWithPhysics() {
    m_gravity = 9.81;
  }

  void SetupPhysics();

  void UpdateAndSync(double deltatime);


 protected:
  PhysObject m_phys;
  double m_gravity;
  Coordinates m_offset;
};


#endif 
