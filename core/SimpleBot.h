// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef SIMPLEBOT_H
#define SIMPLEBOT_H

#include "core/IAutUnit.h"
#include "creature/ActReact.h"


class SimpleBot : public IAutonomosUnit 
{
 public:
  SimpleBot() {
    m_bRand = false;
  }
  
  virtual void Feed(const svec<Entity> & ent);
  virtual void GetAction(IAction & a);
  virtual void Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos);

 private:
  Coordinates m_target;
  Coordinates m_mypos;
  bool m_bRand;

};







#endif //SIMPLEBOT_H
