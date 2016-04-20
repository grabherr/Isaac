// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include "base/SVector.h"
#include "physics/Coordinates.h"
#include "physics/Sound.h"

//============================================================
//============================================================
//============================================================
class PhysConnection
{
 public:
  PhysConnection(int i = -1, int j = -1, double elast = 0.01) {
    m_one = i;
    m_two = j;
    m_elast = elast;
    m_damp = 0.1;
    //m_damp = 0.0;
    m_len = 1.;
    m_maxstretch = 100.;
    m_relax = false;
    m_currDist = 1.;
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

  void SetCurrDistance(double d) {m_currDist = d;}
  double GetCurrDistance() const {return m_currDist;}

  bool IsRelaxed() const {return m_relax;}
  void SetRelax(bool b) {m_relax = b;}

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
  bool m_relax;
  double m_currDist;
};

//============================================================
//============================================================
//============================================================
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


//============================================================
//============================================================
//============================================================
      
class PhysAttractor
{
 public:
  PhysAttractor() {
    m_force = 1.;
    m_back = 1.;
    m_index = -1;
    m_active = true;
  }
  
  const Coordinates & GetPosition() const {return m_coords;}
  Coordinates & Position() {return m_coords;}
  
  double GetForce() const {return m_force;}
  void SetForce(double d) {m_force = d;}
  
  double GetBack() const {return m_back;}
  void SetBack(double d) {m_back = d;}
  
  double GetPull(const Coordinates & c) const {
    return m_force;
  }
  double GetPush(const Coordinates & c) const {
    return m_back;
  }


  void SetIndex(int i) {m_index = i;}
  int GetIndex() const {return m_index;}
  

  void SetActive(bool b) {
    m_active = b;
  }
  bool IsActive() const {return m_active;}
  
  int GetAttachCount() const {return m_attach.isize();}
  int GetAttach(int i) const {return m_attach[i];}
  double GetAttachDist(int i) {return m_dist[i];}

  void AddAttach(int i) {
    m_attach.push_back(i);
    m_dist.push_back(0.);
  }

  void SetDist(int i, double d) {
    m_dist[i] = d;
  }

 private:
  Coordinates m_coords;
  double m_force;
  double m_back;
  int m_index;
  bool m_active;
  svec<int> m_attach;
  svec<double> m_dist;
};



//============================================================
//============================================================
//============================================================
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
    m_meshScale = 1.;
    m_damp = 0.02;
    m_stop = 0.01;
    m_bIsStopped = false;
    m_physMode = 0;
    m_haveEngRot = 0;
    m_bIsInvisible = false;
    m_bSkipUpdate = false;

  }
  
  void SetSkipUpdate(bool b) {m_bSkipUpdate = b;}

  double GetMeshScale() const {return m_meshScale;}
  void SetMeshScale(double m) {m_meshScale = m;}

  void SetElast(bool b) {
    m_bElast = b;
  }

  void SetPhysMode(int n) {
    m_physMode = n;
    if (n == 0)
      m_bElast = true;
  }
  int GetPhysMode() const {
    return m_physMode;
  }


  bool IsMoving() const {
    if (m_bImpulse) {      
      return true;
    }
    return m_bIsStopped; 
  }

  void Stop();

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

  void ConnectWithin(const PhysConnection & c, double dist);

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
  void MoveRelative(const Coordinates & c);

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

  const Coordinates & GetPosition() const {return m_center.GetPosition();}
  Coordinates & Position() {return m_center.Position();}

  const PhysMinimal & GetCenter() const {return m_center;}
  PhysMinimal & GetCenterDirect() {return m_center;}
  const Coordinates & GetRotationSpeed() const {return m_rotspeed;}

  void SetRotationSpeed(const Coordinates & c) {m_rotspeed = c;}
 
  const string & GetAnimation() const {return m_animation;}
  void SetAnimation(const string & s) {m_animation = s;}
  const string & GetTexture() const {return m_texture;}
  void SetTexture(const string & s) {m_texture = s;}

  void GetRotation(Coordinates & rot);

  void Print() const;

  double UpdateImpulseEnergy();

  void SetLatImpulse(const Coordinates &c) {m_latImp = c;}
  void SetRotImpulse(const Coordinates &c) {m_rotImp = c;}

  const Coordinates & GetLatImpulse() const {return m_latImp;}
  const Coordinates & GetRotImpulse() const {return m_rotImp;}
  Coordinates & LatImpulse()  {return m_latImp;}
  Coordinates & RotImpulse()  {return m_rotImp;}

  double GetTotalMass() const {return m_center.GetMass();}
  double GetFarthest() const {return m_farthest;}

  const Coordinates & GetDirection() const {return m_direction;}
  void SetDirection(const Coordinates & c) {m_direction = c;}

  const Coordinates & GetEngRotation() const {return m_engRot;}
  void SetEngRotation(const Coordinates & c) {
    m_engRot = c;
    m_haveEngRot = 1;
  }
  bool HasEngRot() const {return m_haveEngRot == 1;}

  const Coordinates & GetInvisible() const {return m_invisible;}
  void SetInvisible(const Coordinates & c) {
    m_invisible = c;
    m_bIsInvisible = true;
  }
  bool IsInvisible() const {return m_bIsInvisible;}
  void UnInvisible() {m_bIsInvisible = false;}
  
  Sound & GetSound() {return m_sound;}
  const Sound & GetSound() const {return m_sound;}

  int GetConnectCount() const {return m_connect.isize();}
  const PhysConnection & GetConnection(int i) const {return m_connect[i];}
  PhysConnection & Connection(int i) {return m_connect[i];}

  
  int AddAttractor(const PhysAttractor & a) {
    m_attract.push_back(a);
    int i;
    for (i=0; i<a.GetAttachCount(); i++) {
      int index = a.GetAttach(i);
      double d = (m_objects[index].GetPosition() - a.GetPosition()).Length();
      m_attract[m_attract.isize()-1].SetDist(i, d);
    }

    return m_attract.isize()-1;
  }
  PhysAttractor & Attractor(int i) {return m_attract[i];}
  const PhysAttractor & Attractor(int i) const {return m_attract[i];}
  


 private:
  void UpdateElast(double deltatime, double gravity = 9.81);
  void UpdateFixed(double deltatime, double gravity = 9.81);
  void UpdateSimple(double deltatime, double gravity = 9.81);
  void ApplyGravity(double deltatime, double gravity = 9.81, bool bMoveAll=true);
  void ApplyAttractors(double deltatime);

  void AdjustAttractPos(PhysAttractor & a);
  double AttractPosDist(PhysAttractor & a);

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
  PhysMinimal m_lastcenter;
  Coordinates m_rot;
  Coordinates m_engRot;
  Coordinates m_direction;

  svec<PhysAttractor> m_attract;

  Coordinates m_latImp;
  Coordinates m_rotImp;
  Coordinates m_rotspeed;
  double m_scale;
  double m_meshScale;
  
  bool m_bImpulse;
  double m_energy;
  bool m_bElast;
  int m_lastBounce;
  double m_farthest;
  double m_damp;
  double m_stop;
  bool m_bIsStopped;
  string m_animation;
  string m_texture;
  Coordinates m_invisible;
  bool m_bIsInvisible;
  int m_haveEngRot;
  bool m_bSkipUpdate;
  int m_physMode;
  Sound m_sound;
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


