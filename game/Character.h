#ifndef CHARACTER_H
#define CHARACTER_H

#include "base/SVector.h"
#include "physics/Coordinates.h"
#include "npc/NNet.h"
#include "npc/TopLevel.h"

class Character
{
 public:
  Character();
  virtual ~Character() {

  }

  // Dimensionality of vector
  void SetupPeople(int n, int index);
  void SetName(const string & s) {
    m_name = s;
  }

  void FeedNeutral(const svec<double> & prop, const Coordinates & c, int index);
  void FeedAction(const svec<double> & prop, double in);
  void FeedDone(const svec<double> & prop, double in);

  void SetScore(double s);
  
  void SetCoords(const Coordinates & s) {
    m_self = s;
  }
  const Coordinates & GetCoords() const {return m_self;}

  int GetDesire() const {return m_desire;}
  int GetAvoid() const {return m_avoid;}
  double GetAct() const {return m_act;}

  void Print() const;

  
 private:
  void Select();
  NeuralNetwork m_nn;
  TopLevel m_top;
  string m_name;
  Coordinates m_self;
  int m_size;
  int m_myIndex;
  int m_desire;
  int m_avoid;
  double m_act;
  double m_score;
  svec<int> m_map;
};





#endif 
