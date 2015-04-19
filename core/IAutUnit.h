#ifndef IAUTUNIT_H
#define IAUTUNIT_H

#include "base/SVector.h"
#include "physics/Interaction.h"
#include "creature/ActReact.h"

// IAU abstract interface
class IAutonomosUnit
{
 public:
  IAutonomosUnit() {
    m_handle = -1;
  }
  virtual ~IAutonomosUnit() {}
   
  
  virtual void Feed(const svec<Entity> & ent) = 0;
  virtual void GetAction(IAction & a) = 0;
  virtual void Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos) = 0;
  
  void SetHandle(int h) {
    m_handle = h;
  }
  int GetHandle() const {return m_handle;}
  

  virtual double GetResource(const string & name) const {
    string n = name;
    return 0.;
  }
  virtual double GetRelResource(const string & name) const {
    string n = name;
    return 0.;
  }
  virtual double AddToResource(const string & name, double d=1.) {
    string n = name;
    return 0.*d;
  }
  virtual double RemoveFromResource(const string & name, double d=1.) {
    string n = name;
    return 0.*d;
  }

  
 private:
  int m_handle;
};

#endif //IAUTUNIT_H
