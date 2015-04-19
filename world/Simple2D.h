#ifndef SIMPLE2D_H
#define SIMPLE2D_H

#include "base/SVector.h"
#include "core/Matrix.h"
#include "world/IWorld.h"

#define FOOD 'f'
#define WATER '~'
#define TREE 'T'
#define CACTUS 'C'
#define EMPTY ' '

class WorldSimple2D : public IWorld
{
 public:
  WorldSimple2D(int x, int y) {
    m_area.SetSize(x, y, EMPTY);
    m_food = 0;
  }

  void Clear() {
    for (int i=0; i<SizeX(); i++) {
      for (int j=0; j<SizeY(); j++) {
	Set(i, j, EMPTY);
      }
    }
    m_food = 0;
  }

  void Set(int x, int y, char element) {
    m_area.Set(x, y, element);
    if (element == FOOD)
      m_food++;
  }
  char Get(int x, int y) const {
    return m_area.Get(x, y);
  }
  
  void Print();
  int SizeX() const {return m_area.SizeX();}
  int SizeY() const {return m_area.SizeY();}

  virtual bool Try(Reaction & outcome, Coordinates & pos, Entity & ent, int handle, const IAction & action);
  virtual void GetEntities(svec<Entity> & ent, const Coordinates & pos);
  virtual int Register(IAutonomosUnit * pUnit, const Coordinates & pos);

  void MakeEntity(Entity & ent, char c);

  int Food() const {return m_food;}
 private:
  Matrix2D<char> m_area;
  svec<IAutonomosUnit*> m_reg;
  svec<Coordinates> m_pos;

  int m_food;
};

class Simple2DAction : public IAction
{
 public:
  Simple2DAction() {
    m_move.resize(2, 0.);
  }
  virtual ~Simple2DAction() {}

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










#endif //SIMPLE2D_H
