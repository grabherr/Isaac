#ifndef SKELETON_H
#define SKELETON_H

#include "physics/Coordinates.h"
#include <math.h>



// Pass in the current angle
inline bool NPCAngle(double & phi, double x, double y)
{
  if (x == 0.) {
    return false;
  }
  double a = atan2(y, x);
  double b = atan2(x, y);
  //cout << "x=" << x << " y=" << y << " a=" << a << " b=" << b << endl;
  //if (phi < 0. && a > 0.)
  // a -= 2*PI_P;
  phi += a;
  return true;
}

const Coordinates defaultbase = Coordinates(1., 0.0, 0.0).Einheitsvector();

// ==========================================================
//Rotation-based coordinate system
class NPCBoneCoords
{
public:
  NPCBoneCoords() {
  }

  
  NPCBoneCoords(double radius, double yaw, double pitch, double roll) {    
    Set(radius, yaw, pitch, roll);
  }

  const SphereCoordinates & SCoords() const {return m_coords;}
  SphereCoordinates & SCoords() {return m_coords;}


  void Set(double radius, double yaw, double pitch, double roll) {
    m_coords.r() = radius;
    m_coords.Yaw() = yaw;
    m_coords.Pitch() = pitch;
    m_coords.Roll() = roll;
  }
  

  void operator += (const NPCBoneCoords & c) {
    m_coords += c.m_coords;
  }
  void operator -= (const NPCBoneCoords & c) {
    m_coords -= c.m_coords;
  }
  void operator *= (double c) {
    m_coords *= c;
  }
  void operator /= (double c) {
    m_coords /= c;
  }

  NPCBoneCoords operator + (const NPCBoneCoords & c) const {
    NPCBoneCoords coords = *this;
    coords += c;
    return coords;
  }

  NPCBoneCoords operator - (const NPCBoneCoords & c) const {
    NPCBoneCoords coords = *this;
    coords -= c;
    return coords;
  }

  NPCBoneCoords operator * (double c) const {
    NPCBoneCoords coords = *this;
    coords.SCoords() = m_coords.Mult(c);
    //coords *= c;
    return coords;
  }

  NPCBoneCoords operator / (double c) const {
    NPCBoneCoords coords = *this;
    coords.SCoords() = m_coords.Mult(1/c);
    return coords;
  }




  const double & Radius() const {return m_coords.r();}  
  const double & Yaw() const    {return m_coords.Yaw();}
  const double & Pitch() const  {return m_coords.Pitch();}
  const double & Roll() const   {return m_coords.Roll();}
  //const double & RX() const     {return m_rx;}
  //const double & RY() const     {return m_ry;}
  //const double & RZ() const     {return m_rz;}

  double & Radius()  {return m_coords.r();}
  double & Yaw()     {return m_coords.Yaw();}
  double & Pitch()   {return m_coords.Pitch();}
  double & Roll()    {return m_coords.Roll();}
  //double & RX()      {return m_rx;}
  //double & RY()      {return m_ry;}
  //double & RZ()      {return m_rz;}

  Coordinates GetCoords() const {
    Coordinates cc;
    cc.FromSphere(m_coords);
    return cc;
    
    /*RotMatrix r1, r2, r3;

    r1.Set(0, 0, 1.);
    r1.Set(1, 1, cos(m_rx));
    r1.Set(1, 2, -sin(m_rx));
    r1.Set(2, 1, sin(m_rx));
    r1.Set(2, 2, cos(m_rx));
 
    r3.Set(0, 0, cos(m_ry));
    r3.Set(0, 2, sin(m_ry));
    r3.Set(1, 1, 1.);
    r3.Set(2, 0, -sin(m_ry));
    r3.Set(2, 2, cos(m_ry));

    r2.Set(0, 0, cos(m_rz));
    r2.Set(0, 1, -sin(m_rz));
    r2.Set(1, 0, sin(m_rz));
    r2.Set(1, 1, cos(m_rz));
    r2.Set(2, 2, 1.);

  
    RotMatrix a = r1.MultMat(r2);
    RotMatrix b = a.MultMat(r3);
    Coordinates out = b.MultVec(m_base);
    
   
    out *= m_d;
    return out;*/
    
  }

 

  void Limit(const NPCBoneCoords & lo, const NPCBoneCoords & hi) {
    return;
 
   }
  

  void Print() const {
    m_coords.Print();
  }
private:
  SphereCoordinates m_coords;
};


class NPCSkeleton;

// ==========================================================
// Bone, connected to a joint
class NPCBone
{
public:
  
  NPCBone() {
    m_width = 1.;
    m_myID = -1;
    m_physID = -1;
    m_parent = -1;
    m_haveLimit = false;
    m_bOverride = false;

  }

  bool HaveLimit() const {return m_haveLimit;}
  void SetHaveLimit(bool b) {m_haveLimit = b;}

  void SetLimits(const NPCBoneCoords & lo, const NPCBoneCoords & hi) {
    m_lower = m_rel + lo;
    m_upper = m_rel + hi;
    m_haveLimit = true;
  }

  double GetWidth() const {return m_width;}
  void SetWidth(double d) {m_width = d;}

  void SetRelCoords(const NPCBoneCoords & rel) {
    m_rel = rel;
  }

  void AddToRelCoordsSimple(const NPCBoneCoords & v) {
    m_rel += v;
    /*
    if (m_rel.SCoords().phi() > 2*PI_P)
      m_rel.SCoords().phi() -= 2*PI_P;
    if (m_rel.SCoords().phi() < -2*PI_P)
      m_rel.SCoords().phi() += 2*PI_P;
    if (m_rel.SCoords().theta() > 2*PI_P)
      m_rel.SCoords().theta() -= 2*PI_P;
    if (m_rel.SCoords().theta() < -2*PI_P)
      m_rel.SCoords().theta() += 2*PI_P;
    */
 }
  
  void AddToRelCoords(const NPCBoneCoords & v, const SphereCoordinates & base) {
    m_rel += v;
  }


  void UpdateChildren(NPCSkeleton & s, const Coordinates & tip, const Coordinates & root);

  void SetBaseCoords(const Coordinates & abs) {
    m_root = abs;
  }
  Coordinates GetBaseCoords() const {return m_root;}
  
  void AddToBaseCoords(const Coordinates & abs) {
    m_root += abs;
  }
  const Coordinates & Root() const {return m_root;}
  Coordinates & Root() {return m_root;}

  void SetOverride(const Coordinates & c) {
    m_override = c;
    m_bOverride = true;
  }
  void SetOverride(bool b) {
    m_bOverride = b;
  }

  void SetCoords(const Coordinates & c);
  
  Coordinates & GetCoords() {
    return m_real;
  }
  const Coordinates & GetCoords() const {
    return m_real;
    //Coordinates c = m_root;    
    //Coordinates plus = m_abs.GetCoords();
    //c += plus;
    //return c;
  }

  /*
  Coordinates GetCoordsRaw() const {
    Coordinates c = m_root;
    //Coordinates plus = m_rel.GetCoords();
    NPCBoneCoords both = m_rel + m_abs;
    Coordinates plus = both.GetCoords();
    c += plus;
    return c;
    }*/


  NPCBoneCoords & Rel() {return m_rel;}
  const NPCBoneCoords & Rel() const {return m_rel;}
  /*NPCBoneCoords Both() const {
    NPCBoneCoords out = m_abs + m_rel;
    return out;
    }*/


  int MyID() const {return m_myID;}
  int PhysID() const {return m_physID;}

  void SetMyID(int i) {m_myID = i;}
  void SetPhysID(int i) {m_physID = i;}
  void AddChild(int i) {
    m_children.push_back(i);
  }
  int GetChildCount() const {return m_children.isize();}
  int GetChild(int i) const {return m_children[i];}

  void SetOffset(int n) {
    for (int i=0; i<m_children.isize(); i++)
      m_children[i] += n;
    if (m_parent >= 0) {
      m_parent += n;
    } 

  }

  int GetParent() const {return m_parent;}
  void SetParent(int i) {m_parent = i;}

  const string GetName() const {return m_name;}
  void SetName(const string & n) {m_name = n;}

  void Print() const {
    cout << "Relative rotation: " << endl;
    m_rel.Print();
    //cout << "Absolute rotation: " << endl;
    //m_abs.Print();
    //cout << "Combined rotation: " << endl;
    //(m_abs+m_rel).Print();
    cout << "Base coordinates: " << endl;
    m_root.Print();
    cout << "Absolute coordinates, length: " <<  GetCoords().Length() << endl;
    GetCoords().Print();
  }

  void Mirror() {
    m_root[2] *= -1.;
    //m_abs.Yaw() *= -1.;
    m_rel.Yaw() *= -1.;
  }
  void Scale(double d) {
    m_root *= d;
    //m_abs.Radius() *= d;
    m_rel.Radius() *= d;    
  }

  void Move(const Coordinates & target)
  {
    throw;
  }

  const Coordinates & GetTouch() const {return m_touch;}
  Coordinates & Touch() {return m_touch;}


  //  NPCBoneCoords & Rel() {return m_rel;}
  //NPCBoneCoords & Abs() {return m_abs;}
  NPCBoneCoords & Correction() {return m_corr;}
  const NPCBoneCoords & Correction() const {return m_corr;}


  const Coordinates & BaseDelta() const {return m_basedelta;}
  const Coordinates & TipDelta() const {return m_tipdelta;}

  Coordinates & BaseDelta() {return m_basedelta;}
  Coordinates & TipDelta() {return m_tipdelta;}

  void SetBaseDelta(const Coordinates & c) {
    m_basedelta = c - m_root;
  }
  void SetTipDelta(const Coordinates & c) {
    m_tipdelta = c - m_real;
  }

  const Coordinates GetRootPlusDelta() const {return m_root + m_basedelta;}
  const Coordinates GetBasePlusDelta() const {return m_root + m_basedelta;}
  const Coordinates GetCoordsPlusDelta() const {return m_real + m_tipdelta;}

  
protected:
  SphereCoordinates m_last;
  NPCBoneCoords m_rel;
  NPCBoneCoords m_corr;
  NPCBoneCoords m_upper;
  NPCBoneCoords m_lower;
  double m_width;
  svec<int> m_children;
  int m_myID;
  int m_parent;
  int m_physID;
  Coordinates m_root;
  string m_name; 
  bool m_haveLimit;
  Coordinates m_override;
  bool m_bOverride;
  Coordinates m_touch;
  Coordinates m_real;

  Coordinates m_basedelta;
  Coordinates m_tipdelta;

 };


class CollState
{
 public:
  CollState() {}

  bool Constraint(Coordinates &c) {
    bool b = false;
    if (c[1] <= 0.) {     
      c[1] = 0.;
      b = true;
    }
    return b;
  }
};


//=========================================================
//=========================================================
class NPCNerve
{
 public:
  NPCNerve() {
    m_max = 3.14;
    m_min = -3.14;
    m_move = 0;
  }

  void SetMinMax(double min, double max) {
    m_min = min;
    m_max = max;
  }
  
  bool Move(NPCSkeleton & bones, double speed);

  void AddBone(int index, const NPCBoneCoords & c) {
    m_index.push_back(index);
    m_relMove.push_back(c);
  }

  void SetName(const string & name) {
    m_name = name;
  }
  const string & GetName() const {return m_name;}

  double GetMove() const {return m_move;}
  
  void Clear() {
    m_index.clear();
    m_relMove.clear();
    m_max = 3.14;
    m_min = -3.14;
    m_move = 0;
    m_name = "";
  }
 private:
  svec<int> m_index;
  svec<NPCBoneCoords> m_relMove;
  double m_max;
  double m_min;
  double m_move;
  string m_name;
};

class NPCNerveCostume
{
 public:
  NPCNerveCostume() {}

  void AddNerve(const NPCNerve & n) {
    m_nerves.push_back(n);
  }

  bool Move(NPCSkeleton & s, const string & name, double speed) {
    for (int i=0; i<m_nerves.isize(); i++) {
      if (m_nerves[i].GetName() == name) {
	m_nerves[i].Move(s, speed);
      }
    }
    return true;
  }
  bool Move(NPCSkeleton & s, int i, double speed) {
    return m_nerves[i].Move(s, speed);
  }

  int isize() const {return m_nerves.isize();}
  NPCNerve & operator [] (int i) {return m_nerves[i];}
  const  NPCNerve & operator [] (int i) const {return m_nerves[i];}
    
  private:
  svec<NPCNerve> m_nerves;
};



//=========================================================
//=========================================================
//=========================================================
//=========================================================
//=========================================================
//=========================================================
class NPCSkeleton
{
 public:
  NPCSkeleton() {
    m_gravity = 9.81;
    m_bFirst = true;
    //m_gravity = 0;
    //m_base = Coordinates(0, 29, 0);
    m_xrot = 0.;
    m_zrot = 0.;
    m_rotSpeed = 0.;
    m_onFloor = 0;
    m_angle = 0.;
  }
  void SetBaseCoords(const Coordinates & c) {
    m_base = c;
  }
  
  void Read(const string & fileName);
  void Write(const string & fileName);  
  

  int isize() const {return m_bones.isize();}
  const NPCBone & operator[] (int i) const {return m_bones[i];}
  NPCBone & operator[] (int i) {return m_bones[i];}

  void Attach(const NPCSkeleton & s, int where) {
    int i;
    int offset = m_bones.isize();
    Coordinates root = m_bones[where].GetCoords();
    for (i=0; i<s.isize(); i++) {
      m_bones.push_back(s[i]);
      m_bones[m_bones.isize()-1].SetOffset(offset);
      //m_bones[m_bones.isize()-1].AddToAbsCoords(m_bones[where].Both());
      m_bones[m_bones.isize()-1].AddToBaseCoords(root);
      if (m_bones[m_bones.isize()-1].GetParent() < 0) {
	m_bones[m_bones.isize()-1].SetParent(where);
	m_bones[where].AddChild(offset+i);
      }
    }
  }

  void Mirror() {
    for (int i=0; i<m_bones.isize(); i++) {
      m_bones[i].Mirror();
    }
  }


  int AddBone(const NPCBone & bone) {
    int n = m_bones.isize();
    m_bones.push_back(bone);
    m_bones[n].SetMyID(n);
    //cout << "AddBone (1)" << endl;
    //bone.GetCoords().Print();
    //m_bones[n].GetCoords().Print();
    return n;
  }

  int AddBoneTo(const NPCBone & bone, int parent) {
    int n = m_bones.isize();
    m_bones.push_back(bone);
    m_bones[n].SetMyID(n);
    SetParentChild(parent, n);
    return n;
  }

  void SetParentChild(int parent, int child) {
    NPCBone & a = m_bones[parent];
    NPCBone & b = m_bones[child];
    Coordinates base = a.GetCoords();
    b.SetBaseCoords(base);
    //cout << "SetParent (1)" << endl;
    //b.GetCoords().Print();
    a.AddChild(child);
    b.SetParent(parent);
    //cout << "SetParent (2)" << endl;
    //b.GetCoords().Print();
  }

  void Print() const {
    cout << "********** Printing sekeleton:" << endl;
    for (int i=0; i<m_bones.isize(); i++) {
      cout << "Bone " << i << endl;
      m_bones[i].Print();
    }
  }

  void AddToBase(const Coordinates & c) {    
    for (int i=0; i<m_bones.isize(); i++) {
      m_bones[i].Root() += c;
    }
  } 

  void RotateAll(const Coordinates & axis, double angle);
  
  void AddToBoneRot(int index, const NPCBoneCoords & rel) {
    // Add
    //m_bones[index].AddToAbsCoords(rel);
    m_bones[index].AddToRelCoordsSimple(rel);
    if (index == 0) {
      //m_bones[0].Abs() = m_bones[0].Rel();
      m_bones[0].GetCoords().FromSphere(m_bones[0].Rel().SCoords());
      //cout << "From SCoords (base) ";
      //m_bones[0].GetCoords().Print();
      //m_real += m_root;
    }
    //cout << "AddToBoneRot ";
    //rel.Print();
    m_bones[0].UpdateChildren(*this, m_bones[0].GetCoords(), m_bones[0].GetBaseCoords());    
  }

  /*
  void SetBoneRot(int index, const NPCBoneCoords & rel) {
    NPCBoneCoords old = m_bones[index].Rel();
    NPCBoneCoords update = rel - old;
    AddToBoneRot(index, update);
    }*/

  void Scale(double d) {
    for (int i=0; i<m_bones.isize(); i++) {
      m_bones[i].Scale(d);
    }
  }

  void Update(double deltatime);

  void SyncTo(double deltatime);

  double PhysDist();

  void MoveBones();

  void MoveOneBone(int i);

  void SetBaseline() {m_baseline = m_bones;}

  NPCNerveCostume & Nerves() {return m_nerves;}
  const NPCNerveCostume & GetNerves() const {return m_nerves;}

  bool Move(const string & name, double speed) {
    return m_nerves.Move(*this, name, speed);
  }
  bool Move(int i, double speed) {
    return m_nerves.Move(*this, i, speed);
  }

  const Coordinates RelRot() const {return m_relRot;}
  const Coordinates RelPos() const {return m_relPos;}
  const Coordinates AbsPos() const {return m_absPos;}
  
 protected:
  svec<NPCBone> m_bones;
  svec<NPCBone> m_baseline;
  svec<Coordinates> m_v;
  svec<Coordinates> m_x;
  double m_gravity;
  CollState m_coll;
  NPCNerveCostume m_nerves;
  Coordinates m_base;
  Coordinates m_relRot;
  Coordinates m_relPos;
  Coordinates m_absPos;
  SphereCoordinates m_rotImp;
  Coordinates m_imp;
  bool m_bFirst;
  Coordinates m_lastAbsPos;
  Coordinates m_lastRotImp;
  double m_xrot;
  double m_zrot;
  double m_angle;
  double m_rotSpeed;
  Coordinates m_axis;
  int m_onFloor;
};


#endif

