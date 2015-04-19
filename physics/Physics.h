#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include "base/SVector.h"
#include "physics/Coordinates.h"


class PhysMinimal
{
 public:
  PhysMinimal() {
    m_mass = 0.;
  }
  PhysMinimal(double mass) {
    m_mass = mass;
  }
  
  void SetMass(double m) {
    m_mass = m;
  }

  void SetPosition(const Coordinates & c) {
    m_x = c;
  }
  void SetVelocity(const Coordinates & v) {
    m_v = v;
  }

  double GetMass() const {return  m_mass;}
  const Coordinates & GetPosition() const {return m_x;}
  const Coordinates & GetVelocity() const {return m_v;}

  Coordinates & Position() {return m_x;}
  Coordinates & Velocity() {return m_v;}

  void Clear() {
    for (int i=0; i<m_x.isize(); i++) {
      m_v[i] = 0.;
      m_x[i] = 0.;
    }
    m_mass = 0.;
  }

  void operator += (const PhysMinimal & c) {
    m_mass += c.m_mass;
    m_x += c.m_x;
    m_v += c.m_v;
  }
  void Print() const {
    cout << "x: " << m_x[0] << "\t" << m_x[1] << "\t" << m_x[2] << endl;
    cout << "v: " << m_v[0] << "\t" << m_v[1] << "\t" << m_v[2] << endl;
  }
 private:
  double m_mass;
  Coordinates m_x;
  Coordinates m_v;
};

class PhysObject
{
 public:
  PhysObject() {}
  
  void Add(const PhysMinimal & m) {
    m_objects.push_back(m);
  }
  
  // Call this when setup is done.
  void Fixate();

  void Impulse(int i, const Coordinates & direction);
  
  // Time in seconds
  void Update(double deltatime, double gravity = 9.81);

  int isize() const {return m_objects.isize();}
  PhysMinimal & operator[] (int i) {return m_objects[i];}
  const PhysMinimal & operator[] (int i) const {return m_objects[i];}

  void Print() const;
 private:
  svec<PhysMinimal> m_objects;
  PhysMinimal m_center;
  Coordinates m_rotspeed;
 
};






#endif //PHYSICS_H


