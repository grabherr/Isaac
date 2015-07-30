// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef ACTREACT_H
#define ACTREACT_H

#include "base/SVector.h"
#include "physics/Coordinates.h"

class IAction
{
 public:
  IAction() {}
  virtual ~IAction() {}

  virtual int isize() const = 0;
  virtual double Get(int i) const = 0;
  virtual double Set(int i, double v) = 0;
  virtual double Limit(int i) const = 0;
 
};

class Reaction
{
 public:
  Reaction() {
    Reset();
  }

  void Reset() {
    m_reward = 0.;
    m_pain = 0.;
    m_un = 0.;
  }
  
  double Reward() const {return m_reward;}
  double Pain() const {return m_pain;}
  double Unsuccess() const {return m_un;}

  void SetReward(double s) {m_reward = s;}
  void SetPain(double s) {m_pain = s;}
  void SetUnsuccess(double s) {m_un = s;}

  void Print() const {
    cout << "  pain:      " << m_pain << endl;
    cout << "  reward:    " << m_reward << endl;
    cout << "  unsuccess: " << m_un << endl;
  }

  int isize() const {return 3;}
  double & operator[] (int i) {
    if (i == 0)
      return m_reward;
    if (i == 1)
      return m_pain;
    return m_un;
  }
  const double & operator[] (int i) const {
    if (i == 0)
      return m_reward;
    if (i == 1)
      return m_pain;
    return m_un;
  }

 private:
  double m_reward;
  double m_pain;
  double m_un;
};

//typedef Coordinates Position;





#endif //ACTREACT_H

