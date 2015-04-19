#ifndef HMMBOT_H
#define HMMBOT_H

#include "core/IAutUnit.h"
#include "creature/ActReact.h"
#include "core/Cerebellum.h"

class HMMBot : public IAutonomosUnit 
{
 public:
  HMMBot() {
    m_bRand = false;
    m_moveX = 0.;
    m_moveY = 0.;
  }
  
  virtual void Feed(const svec<Entity> & ent);
  virtual void GetAction(IAction & a);
  virtual void Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos);

 private:
  Coordinates m_target;
  Coordinates m_mypos;
  bool m_bRand;
  double m_moveX;
  double m_moveY;
  //Cerebellum m_cere;
};







#endif //SIMPLEBOT_H
