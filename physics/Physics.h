#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include "base/SVector.h"
#include "physics/Coordinates.h"


//==============================================
class PhysConnection
{
 public:
  PhysConnection() {
    m_one = -1;
    m_two = -1;
    m_elast = 1.;
    m_damp = 0.;
    m_len = 1.;
    m_maxstretch = 100.;
  }

  void Set(int i, int j) {
    m_one = i;
    m_two = j;
  }

  void SetDistance(double d) {m_len = d;}
  double GetDistance() const {return m_len;}

  int GetFirst() const {return m_one;}
  int GetSecond() const {return m_two;}
  int GetOther(int i) {
    if (i == m_one)
      return m_two;
    else
      return m_one;
  }

  void SetElast(double e) {m_elast = e;}
  double GetElast() const {return m_elast;}

  void SetDamp(double d) {m_damp = d;}
  double GetDamp() const {return m_damp;}

  void SetMaxStretch(double d) {m_maxstretch = d;}
  double GetMaxStretch() const {return m_maxstretch;}

 private:
  double m_len;
  double m_elast;
  double m_damp;
  double m_maxstretch;
  int m_one;
  int m_two;
};

//==============================================
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
  const Coordinates & GetAcceleration() const {return m_a;}

  Coordinates & Position() {return m_x;}
  Coordinates & Velocity() {return m_v;}
  Coordinates & Acceleration() {return m_a;}

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

  int GetConnectCount() const {return m_connect.isize();}
  int GetConnect(int i) const {return m_connect[i];}

  void AddConnect(int c) {m_connect.push_back(c);}

  void RemoveConnect(int c) {
    int i;
    for (i=0; i<m_connect.isize(); i++) {
      if (m_connect[i] == c) {
	m_connect[i] = m_connect[m_connect.isize()-1];
	m_connect.resize(m_connect.isize()-1);
	break;
      }
    }
  }

 private:
  double m_mass;
  Coordinates m_x;
  Coordinates m_v;
  Coordinates m_a;
  svec<int> m_connect;
};



//=============================================================
class PhysObject
{
 public:
  PhysObject() {}
  
  // Adds an object. Note that objects need to be connected
  // through PhysConnection objects
  int Add(const PhysMinimal & m) {
    m_objects.push_back(m);
    return m_objects.isize()-1;
  }
  
  // Connects two points connection
  void Connect(const PhysConnection & c);

  // Connects all with the same connection
  void ConnectAll(const PhysConnection & c);
  
  // Call this when setup is done.
  void Fixate();

  // Set the initial rotation
  void SetRotation(const Coordinates & c) {
    m_rot = c;
    m_rot += m_center.GetPosition();
  }

  void Impulse(int i, const Coordinates & velocity, double mass = -1);
  void Impulse(int index1, PhysObject & other, int index2);
 
  // Time in seconds
  void Update(double deltatime, double gravity = 9.81);

  int isize() const {return m_objects.isize();}
  PhysMinimal & operator[] (int i) {return m_objects[i];}
  const PhysMinimal & operator[] (int i) const {return m_objects[i];}

  const PhysMinimal & GetCenter() const {return m_center;}
  const Coordinates & GetRotationSpeed() const {return m_rotspeed;}

  void SetRotationSpeed(const Coordinates & c) {m_rotspeed = c;}
 
  void GetRotation(Coordinates & rot);

  void Print() const;
 private:
  svec<PhysMinimal> m_objects;
  svec<PhysConnection> m_connect;
  PhysMinimal m_center;
  Coordinates m_rot;
  Coordinates m_rotspeed;
 
};


//=========================================================
class SolidTriangle
{
 public:
  SolidTriangle() {
    m_elast = 1.;
    m_maxlen = 0.;
    m_area = 0.;
  }

  void SetElasticity(double elast) {
    m_elast = elast;
  }

  double GetElasticity() const {return m_elast;}

  ~SolidTriangle() {}

  void Set(const Coordinates & a, 
	   const Coordinates & b,
	   const Coordinates & c) {
    m_a = a;
    m_b = b;
    m_c = c;

    double aa = m_a.Length(m_b);
    double bb = m_b.Length(m_c);
    double cc = m_c.Length(m_a);

    m_maxlen = aa;
    if (bb > m_maxlen)
      m_maxlen = bb;
    if (cc > m_maxlen)
      m_maxlen = cc;

    // Allow some slack
    m_maxlen *= 1.1;

    double s = (aa + bb + cc)/2.;
    m_area = sqrt(s*(s-aa)*(s-bb)*(s-cc));

    //Coordinates one = m_a;
    //Coordinates two = m_a;
    //one -= m_b;
    //two -= m_c;
    //m_cross = one.CrossProduct(two);

  }
  double GetArea() const {return m_area;}

  // Would it go through the triangle?
  bool Collision(const Coordinates & before, const Coordinates & after) const;

  bool Collide(PhysObject & object) const;

 

 private:
  Coordinates m_a;
  Coordinates m_b;
  Coordinates m_c;
  //Coordinates m_cross;
  double m_elast;
  double m_maxlen;
  double m_area;
};




#endif //PHYSICS_H


