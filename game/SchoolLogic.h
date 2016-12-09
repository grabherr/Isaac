#ifndef SCHOOLLOGIC_H
#define SCHOOLLOGIC_H


#include "base/SVector.h"


class SchoolCharacter
{
 public:
  SchoolCharacter() {
    m_attract = 0.;
    m_strength = 0.;
    m_gender = 0.;
    m_target = -1;
    m_interact = 0.;
  }

  void Reset() {
    m_target = -1;
    m_interact = 0.;
  }
  
  double GetStrength() const {return m_strength;}
  double GetAttract()  const {return m_attract;}
  void SetStrength(double d)  {m_strength = d;}
  void SetAttract(double d)   {m_attract = d;}
  double GetGender()  const {return m_gender;}
  void SetGender(double d)  {m_gender = d;}
  
  double GetInteract()  const {return m_interact;}
  void SetInteract(double d)  {m_interact = d;}

  void AdjustStrength(double d)  {
    m_strength *= d;
  }
  void AdjustAttract(double d)  {
    m_attract *= d;
  }
  
  double AddStrength(double d)  {
    m_strength += d;
    double diff = 0.;
    if (m_strength > 1.) {
      diff = 1. - m_strength;
      m_strength = 1.;
    }
    if (m_strength < 0.1) {
      diff = 0.1 - m_strength;
      m_strength = 0.1;
    }
    return diff;
  }
  
  double AddAttract(double d)   {
    double diff = 0.;
    if (m_attract > 1.) {
      diff = 1. - m_attract;
      m_attract = 1.;
    }
    if (m_attract < 0.1) {
      diff = 0.1 - m_attract;
      m_attract = 0.1;
    }
    return diff;
  }
  
  const string & GetName() const {return m_name;}
  void SetName(const string & s) {m_name = s;}

  void SetTarget(int t) {
    m_target = t;
  }
  int GetTarget() const {return m_target;}
 private:
  double m_strength;
  double m_attract;
  double m_gender;
  string m_name;
  int m_target;
  double m_interact;
};





//=====================================================
class SchoolLogic
{
 public:
  SchoolLogic() {}

  void push_back(const SchoolCharacter & c) {
    m_char.push_back(c);
  }
  int isize() const {return m_char.isize();}
  SchoolCharacter & operator[] (int i) {return m_char[i];}
  const SchoolCharacter & operator[] (int i) const {return m_char[i];}


  void BeginRound();
  void EndRound();

  int Find(const string & name) const {
    for (int i=0; i<m_char.isize(); i++) {
      if (m_char[i].GetName() == name)
	return i;
    }
    return -1;
  }
  const SchoolCharacter & Get(const string & name) const {
    return m_char[Find(name)];  
  }
  SchoolCharacter & Get(const string & name) {
    return m_char[Find(name)];  
  }
  svec<SchoolCharacter> & Last() {return m_last;}
  const svec<SchoolCharacter> & Last() const {return m_last;}

  void Print() const;
  
 private:
  double Compete(double interact, double ext, double is);
  
  svec<SchoolCharacter> m_char;
  svec<SchoolCharacter> m_last;
};


#endif
