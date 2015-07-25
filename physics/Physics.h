#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include "base/SVector.h"
#include "physics/Coordinates.h"


//==============================================
class PhysConnection
{
 public:
  PhysConnection(int i = -1, int j = -1) {
    m_one = i;
    m_two = j;
    m_elast = 0.01;
    m_damp = 0.1;
    //m_damp = 0.0;
    m_len = 1.;
    m_maxstretch = 100.;
  }

  void Set(int i, int j) {
    if (m_one < m_two) {
      m_one = i;
      m_two = j;
    } else {
      m_two = i;
      m_one = j;
    }
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

  bool operator == (const PhysConnection & p) const {
    if (m_one == p.m_one && m_two == p.m_two)
      return true;
    if (m_one == p.m_two && m_two == p.m_one)
      return true;
    return false;
  }

  void Print() const {
    cout << "Connecting " << m_one << " <-> " << m_two << endl;
    cout << "Len: " << m_len << " elast: " << m_elast << " damp " << m_damp << endl;
  }
  
  double Energy(double len) const {
    double diff = len - m_len;
    return diff * diff / m_elast;
  }

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
    m_bFixed = false;
    m_centerDist = 0.;
  }
  PhysMinimal(double mass) {
    m_mass = mass;
  }
  
  void SetFixed(bool b) {
    m_bFixed = b;
  }

  bool IsFixed() const {
    return m_bFixed;
  }

  void SetMass(double m) {
    m_mass = m;
  }

  double GetCenterDist() const {return m_centerDist;}
  void SetCenterDist(double d) {
    m_centerDist = d;
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
  const Coordinates GetImpulse() const {
    return m_v * m_mass;
  }
  const Coordinates & GetAcceleration() const {return m_a;}

  Coordinates & Position() {return m_x;}
  Coordinates & Velocity() {return m_v;}
  Coordinates & Acceleration() {return m_a;}

  double GetEnergy() const {
    double e = m_mass * m_v.Length()*m_v.Length()/2.;
    return e;
  }

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

  bool operator == (const PhysMinimal & m) const {
    int i;
    for (i=0; i<m_x.isize(); i++) {
      if (m_x[i] != m.m_x[i])
	return false;
    }
    return true;
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
  bool m_bFixed;
  double m_centerDist;
};



//=============================================================
class PhysObject
{
 public:
  PhysObject() {
    m_scale = 1.;
    m_bImpulse = false;
    m_energy = 0.;
    m_bElast = false;
    m_lastBounce = -1;
    m_farthest = 0.;
  }
  
  void SetElast(bool b) {
    m_bElast = b;
  }


  // Adds an object. Note that objects need to be connected
  // through PhysConnection objects
  // All coordinates are relative to the center!!
  int Add(const PhysMinimal & m) {
    m_objects.push_back(m);
    return m_objects.isize()-1;
  }
  
  // Connects two points connection
  void Connect(const PhysConnection & c);

  // Connects all with the same connection
  void ConnectAll(const PhysConnection & c);

  void ConnectToCenter(const PhysConnection & c);

  // Call this when setup is done.
  void Fixate();

  void Center();

  // Set the initial rotation (OBSOLETE!!!)
  void SetRotation(const Coordinates & c) {
    m_rot = c;
    m_rot += m_center.GetPosition();
  }

  void MoveTo(const Coordinates & c);

  void Bounce(int i, const Coordinates & direction, double elast);

  void Impulse(int i, const Coordinates & velocity, double mass = -1);
  void Impulse(int index1, PhysObject & other, int index2);
 
  // Time in seconds
  void Update(double deltatime, double gravity = 9.81);

  int isize() const {return m_objects.isize();}
  PhysMinimal & operator[] (int i) {return m_objects[i];}
  const PhysMinimal & operator[] (int i) const {return m_objects[i];}

  // Mapped access
  int MappedSize() const {return m_map.isize();}
  PhysMinimal & GetMapped(int i) {return m_objects[m_map[i]];}
  const PhysMinimal & GetMapped(int i) const {return m_objects[m_map[i]];}
  int AddMapped(const PhysMinimal & m) {
    int i;
    for (i=0; i<m_objects.isize(); i++) {
      if (m_objects[i] == m) {
	m_map.push_back(i);
	return i;
      }
    }
    m_map.push_back(m_objects.isize());
    m_objects.push_back(m);
    return m_objects.isize()-1;
  }

  void ConnectMapped(const PhysConnection & c)
  {
    PhysConnection b = c;
    b.Set(m_map[c.GetFirst()], m_map[c.GetSecond()]);
    Connect(b);
  }

  void AddTriangleMapped(int a, int b, int c) {
    m_connectTriangles.push_back(m_map[a]);
    m_connectTriangles.push_back(m_map[b]);
    m_connectTriangles.push_back(m_map[c]);
  }

  bool DoesCollide(PhysObject & o);


  const PhysMinimal & GetCenter() const {return m_center;}
  const Coordinates & GetRotationSpeed() const {return m_rotspeed;}

  void SetRotationSpeed(const Coordinates & c) {m_rotspeed = c;}
 
  void GetRotation(Coordinates & rot);

  void Print() const;

  double UpdateImpulseEnergy();

  void SetLatImpulse(const Coordinates &c) {m_latImp = c;}
  void SetRotImpulse(const Coordinates &c) {m_rotImp = c;}

  const Coordinates & GetLatImpulse() {return m_latImp;}
  const Coordinates & GetRotImpulse() {return m_rotImp;}

  double GetTotalMass() const {return m_center.GetMass();}
  double GetFarthest() const {return m_farthest;}

 private:
  void UpdateElast(double deltatime, double gravity = 9.81);
  void UpdateFixed(double deltatime, double gravity = 9.81);
  void ApplyGravity(double deltatime, double gravity = 9.81);

  Coordinates GetCenterPos();
  Coordinates GetTotalImpulse(double & totalMass);
  void AdjustImpulseEnergy();
  void AdjustCoordinates();


  double Energy() const;

  svec<PhysMinimal> m_objects;
  svec<PhysConnection> m_connect;
  svec<int> m_map;
  svec<int> m_connectTriangles;
  PhysMinimal m_center;
  Coordinates m_rot;

  Coordinates m_latImp;
  Coordinates m_rotImp;

  Coordinates m_rotspeed;
  double m_scale;

  bool m_bImpulse;
  double m_energy;
  bool m_bElast;
  int m_lastBounce;
  double m_farthest;
};


//=========================================================
//=========================================================
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

    Coordinates one = m_a;
    Coordinates two = m_a;
    one -= m_b;
    two -= m_c;
    m_cross = one.CrossProduct(two);

  }

  // Inverts the cross-product
  void InvertDirection() {
    m_cross = m_cross * -1.;
  }

  double GetArea() const {return m_area;}

  // Would it go through the triangle?
  bool Collision(const Coordinates & before, const Coordinates & after) const;

  bool Collide(PhysObject & object) const;

  const Coordinates & Cross() const {return m_cross;}

 private:
  Coordinates m_a;
  Coordinates m_b;
  Coordinates m_c;
  Coordinates m_cross;
  double m_elast;
  double m_maxlen;
  double m_area;
};




#endif //PHYSICS_H


