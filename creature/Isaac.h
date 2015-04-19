#ifndef ISAAC_H
#define ISAAC_H

#include "creature/ActReact.h"
#include "core/Cerebellum.h"
#include "core/Memory.h"
#include "core/SpatAssocMem.h"
#include "core/Resources.h"

class Isaac 
{
 public:
  Isaac() {
    m_moveX = 0.;
    m_moveY = 0.;
    m_handle = 0;
  }

  void SetTemplate(const Entity & temp) {
    m_mem.SetNumNeurons(100, temp.isize());
  }

  void SetHandle(int h) {
    m_handle = h;
  }
  int GetHandle() const {return m_handle;}

  virtual void Feed(const svec<Entity> & ent);
  virtual void GetAction(IAction & a);
  virtual void Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos);

  // Resource management
  void AddResource(IResource * p);

  double GetResource(const string & name) const;
  double GetRelResource(const string & name) const;
  double AddToResource(const string & name, double d=1.);
  double RemoveFromResource(const string & name, double d=1.);


 private:
  Coordinates m_target;
  Coordinates m_mypos;
  double m_moveX;
  double m_moveY;
  Memory m_mem;
  AssociativeMemory m_assoc;
  ResourcePool m_res;
 
  int m_handle;
};







#endif //ISAAC_H
