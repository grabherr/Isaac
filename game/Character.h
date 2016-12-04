#ifndef CHARACTER_H
#define CHARACTER_H

#include "base/SVector.h"
#include "physics/Coordinates.h"





class Character
{
 public:
  Character();
  virtual ~Character() {}


  const Coordinates & Coords() const {return m_coords;}
  Coordinates & Coords() {return m_coords;}
  const Coordinates & Desired() const {return m_desired;}
  Coordinates & Desired() {return m_desired;}


  double GetStrength() const {return m_strength;}
  double GetAttraction() const {return m_attraction;}
  int GetIndex() const {return m_index;}
  void SetIndex(int i) {m_index = i;}
  double Gender() const {return m_gender;}
  void SetGender(double d) {m_gender = d;}
  int GetTarget() const {return m_index;}
  double GetAction() const {return m_action;}
  
  virtual void Act(svec<Character> & character);
  void Suffer(double action, const Character & c);

 private:
  Coordinates m_coords;
  Coordinates m_desired;
  double m_attraction;
  double m_strength;
  double m_attraction_ext;
  double m_strength_ext;
  double m_action;
  int m_index;
  int m_gender;
  int m_target;
};





#endif 
