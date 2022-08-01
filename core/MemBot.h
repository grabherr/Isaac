// (c) Manfred G. Grabherr/Louis Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef MEMBOT_H
#define MEMBOT_H

#include "core/Cerebellum.h"
#include "core/Memory.h"
#include "core/SpatAssocMem.h"
#include "core/IAutUnit.h"
#include "creature/ActReact.h"

class MemBot : public IAutonomosUnit 
{
 public:
  MemBot() {
    m_moveX = 0.;
    m_moveY = 0.;
    m_isClose = false;
    m_assoc.Setup(4, 2);
  }
  
  virtual void Feed(const svec<Entity> & ent);
  virtual void GetAction(IAction & a);
  virtual void Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos);
 
 private:
  Coordinates m_target;
  Coordinates m_mypos;
  double m_moveX;
  double m_moveY;
  Memory m_mem;
  AssociativeMemory m_assoc;
  bool m_isClose;

};







#endif //MEMBOT_H
