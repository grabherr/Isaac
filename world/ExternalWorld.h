#ifndef EXTERNALWORLD_H
#define EXTERNALWORLD_H

#include "base/SVector.h"
#include "core/Matrix.h"
#include "world/IWorld.h"


class ExternalWorld : public IWorld
{
 public:
  ExternalWorld() {
    m_pain = 0;
    m_reward = 0;
    m_collision = 0;
  }

  void Clear() {
    m_ent.clear();
    m_pain = 0;
    m_reward = 0;
    m_collision = 0;
  }

  void AddEntity(const Entity & e) {
    m_ent.push_back(e);
  }
  void SetCoordinates(const Coordinates & pos) {
    m_pos = pos;
  }

  void SetStatus(double pain, double reward, double coll) {
    m_pain = pain;
    m_reward = reward;
    m_collision = coll;
  }

  void Print();

  virtual bool Try(Reaction & outcome, Coordinates & pos, Entity & ent, int handle, const IAction & action);
  virtual void GetEntities(svec<Entity> & ent, const Coordinates & pos);
  virtual int Register(IAutonomosUnit * pUnit, const Coordinates & pos);

 private:
  svec<IAutonomosUnit*> m_reg;
  svec<Entity> m_ent;
  Coordinates m_pos;
  double m_pain;
  double m_reward;
  double m_collision;
 
};

class ExternalAction : public IAction
{
 public:
  ExternalAction() {
    m_move.resize(3, 0.);
  }
  virtual ~ExternalAction() {}

  int isize() const {return m_move.isize();}
  double Get(int i) const {return m_move[i];}
  double Set(int i, double v) {
    if (v < -1.)
      v = -1.;
    if (v > 1.)
      v = 1.;
    m_move[i] = v;
    return v;
  }

  double Limit(int i) const {return 1.;}

 private:
  svec<double> m_move;
};










#endif //EXTERNALWORLD_H
