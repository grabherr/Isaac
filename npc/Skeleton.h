#ifndef SKELETON_H
#define SKELETON_H

#include "physics/Coordinates.h"
#include <math.h>


class RotMatrix
{
 public:
  RotMatrix() {
    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
	m_data[i][j] = 0.;
      }
    }      
  }

  void Set(int i, int j, double d) {
    m_data[i][j] = d;
  }
  double Get(int i, int j) const {
    return m_data[i][j];
  }

  RotMatrix MultMat(const RotMatrix & m) const {
    
    RotMatrix r;
    for (int j=0; j<3; j++) {
      for (int i=0; i<3; i++) {
	double v = 0;
	for (int k=0; k<3; k++) {
	  v += Get(i, k)*m.Get(k, j);
	} 
	r.Set(i, j, v);
      }
    }
    return r;
  }

  Coordinates MultVec(const Coordinates & c) const {     
    Coordinates r;
    for (int j=0; j<3; j++) {
      double v = 0;
      for (int i=0; i<3; i++) {
	//cout << "Mult " << j << " " << i << " -> " << Get(j, i) << " " << c[i] << endl; 
	v += Get(j, i)*c[i];   
      }
      //cout << "v=" << v << " j=" << j << endl;
      r[j] = v;
    }
    //r.Print();
    return r;
  }

  void Print() const {
    int i, j;
    for (i=0; i<3; i++) {
      for (j=0; j<3; j++) {
	cout << m_data[i][j] << " ";
      }
      cout << endl;
    }
  }

 private:
  double m_data[3][3];  

};

//const Coordinates defaultbase = Coordinates(1., 1., 1.).Einheitsvector();
const Coordinates defaultbase = Coordinates(1., 0, 0).Einheitsvector();

//Rotation-based coordinate system
class NPCBoneCoords
{
public:
  NPCBoneCoords() {
    m_d = 1.;
    m_rx = 0.;
    m_ry = 0.;
    m_rz = 0.;
    m_base = defaultbase;
  }

  NPCBoneCoords(double radius, double rx, double ry, double rz) {
    m_base = defaultbase;
    Set(radius, rx, ry, rz);
  }

  void Set(double radius, double rx, double ry, double rz) {
    m_d = radius;
    m_rx = rx;
    m_ry = ry;
    m_rz = rz;    
  }
  
  void FromEuclidean(const Coordinates & c) {}

  const Coordinates & Base() const {return m_base;}
  void SetBase(const Coordinates & c) {
    m_base = c.Einheitsvector();    
  }

  /*
  void Set(const Coordinates & c) {
    m_d = c.Length();
    m_rz = acos(c[0]/m_d);
    m_ry = acos(c[1]/m_d);
    m_rx = acos(c[2]/m_d);
    }*/

  void operator += (const NPCBoneCoords & c) {
    
    m_rx += c.m_rx;
    m_ry += c.m_ry;
    m_rz += c.m_rz;
  }
  void operator -= (const NPCBoneCoords & c) {
    m_rx -= c.m_rx;
    m_ry -= c.m_ry;
    m_rz -= c.m_rz;
  }
  void operator *= (double c) {
    m_rx *= c;
    m_ry *= c;
    m_rz *= c;
  }
  void operator /= (double c) {
    m_rx /= c;
    m_ry /= c;
    m_rz /= c;
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
    coords *= c;
    return coords;
  }

  NPCBoneCoords operator / (double c) const {
    NPCBoneCoords coords = *this;
    coords /= c;
    return coords;
  }



  const double & operator[] (int i) const {
    switch(i) {
    case 1:
      return m_ry;
    case 2:
      return m_ry;
    default:
      return m_rx;
    }
  }
  double & operator[] (int i) {
    switch(i) {
    case 1:
      return m_ry;
    case 2:
      return m_ry;
    default:
      return m_rx;
    }
  }



  const double & Radius() const {return m_d;}
  const double & RX() const     {return m_rx;}
  const double & RY() const     {return m_ry;}
  const double & RZ() const     {return m_rz;}
 
  double & Radius()  {return m_d;}
  double & RX()      {return m_rx;}
  double & RY()      {return m_ry;}
  double & RZ()      {return m_rz;}

  Coordinates GetCoords() const {
    //double x, y, z;
    //Coordinates ax, ay, az;
    //cout << "Rotating, based on ";
    //m_base.Print();
    RotMatrix r1, r2, r3;

    r1.Set(0, 0, 1.);
    r1.Set(1, 1, cos(m_rx));
    r1.Set(1, 2, -sin(m_rx));
    r1.Set(2, 1, sin(m_rx));
    r1.Set(2, 2, cos(m_rx));
    //cout << "r1" << endl;
    //r1.Print();

    r2.Set(0, 0, cos(m_ry));
    r2.Set(0, 2, sin(m_ry));
    r2.Set(1, 1, 1.);
    r2.Set(2, 0, -sin(m_ry));
    r2.Set(2, 2, cos(m_ry));
    //cout << "r2" << endl;
    //r2.Print();

    r3.Set(0, 0, cos(m_rz));
    r3.Set(0, 1, -sin(m_rz));
    r3.Set(1, 0, sin(m_rz));
    r3.Set(1, 1, cos(m_rz));
    r3.Set(2, 2, 1.);

    //cout << "r3" << endl;
    //r3.Print();

    RotMatrix a = r1.MultMat(r2);
    RotMatrix b = a.MultMat(r3);
    //RotMatrix a = r3.MultMat(r2);
    //RotMatrix b = a.MultMat(r1);

    //cout << "After mult: " << endl;
    //b.Print();

    Coordinates out = b.MultVec(m_base);
    out *= m_d;
    //cout << "After vec mul" << endl;
    //out.Print();
    //cout << "DONE HERE" << endl;
    return out;
    /*
    ax[0] = m_base[0];
    ax[1] = m_base[1]*cos(m_rx) - m_base[2]*sin(m_rx);
    ax[2] = m_base[1]*sin(m_rx) + m_base[2]*cos(m_rx);
    cout << "around x ";
    ax.Print();

    ay[0] = ax[0]*cos(m_ry) + ax[2]*sin(m_ry);
    ay[1] = ax[1];
    ay[2] = -ax[0]*sin(m_ry) + ax[2]*cos(m_ry);
    cout << "around y ";
    ay.Print();

    az[0] = ay[0]*cos(m_rz) - ay[1]*sin(m_rz);
    az[1] = ay[0]*sin(m_rz) + ay[1]*cos(m_rz);
    az[2] = ay[2];
    //cout << m_rz << endl;
    cout << "around z ";
    az.Print();
   
    az *= m_d;
    return az;*/
  }

  /*
  double X() const {
    //cout << "**** X: " << m_d*cos(m_ry)*cos(m_rz) << " " << m_d << endl;
    Coordinates c(0., 0., 
    return m_d*cos(m_ry)*cos(m_rz);
  }

  double Y() const {
    //cout << "**** Y: " << m_d*sin(m_rz)*sin(m_rx) << " " << m_d  << endl;
    return m_d*sin(m_rz)*cos(m_rx);
  }

  double Z() const {
    //cout << "**** Z: " << m_d*cos(m_rx)*sin(m_ry) << " " << m_d << endl;
    return m_d*cos(m_rx)*sin(m_ry);
    }*/
 

  void Limit(const NPCBoneCoords & lo, const NPCBoneCoords & hi) {
    //cout << "ERROR: No limit!!" << endl;
    //return;
    //cout << "Limit" << endl;
    //Print();
    //lo.Print();
    //hi.Print();

    if (m_rx < lo.RX())
      m_rx = lo.RX();
    if (m_ry < lo.RY())
      m_ry = lo.RY();
    if (m_rz < lo.RZ())
      m_rz = lo.RZ();

    if (m_rx > hi.RX())
      m_rx = hi.RX();
    if (m_ry > hi.RY())
      m_ry = hi.RY();
    if (m_rz > hi.RZ())
      m_rz = hi.RZ();
    //Print();
  }
  

  void Print() const {
    cout << "d=" << m_d << " rx=" << m_rx << " ry=" << m_ry << " rz=" << m_rz << endl;
  }
private:
  double m_d;
  double m_rx;
  double m_ry;
  double m_rz;
  Coordinates m_base;
};


class NPCSkeleton;

// Bone, connected to a joint
class NPCBone
{
public:
  
  NPCBone() {
    m_width = 1.;
    m_myID = -1;
    m_physID = -1;
    m_parent = -1;
    m_lower.RX() = -PI_P;
    m_upper.RX() = PI_P;
    m_lower.RY() = -PI_P;
    m_upper.RY() = PI_P;
    m_lower.RZ() = -PI_P;
    m_upper.RZ() = PI_P;
    
  }

  double GetWidth() const {return m_width;}
  void SetWidth(double d) {m_width = d;}

  void SetRelCoords(const NPCBoneCoords & rel) {
    m_rel = rel;
  }
  void AddToRelCoords(const NPCBoneCoords & v) {
    //cout << "AddToRelCoords" << endl;
    //m_rel.Print();
    m_rel += v;
    //m_rel.Print();
    //m_lower.Print();
    //m_upper.Print();
    m_rel.Limit(m_lower, m_upper);   
    //m_rel.Print();
  }

  void AddToAbsCoords(const NPCBoneCoords & v) {
    m_abs += v;
    //cout << "New abs coords: " << endl;
    //m_abs.Print();
  }

  void UpdateChildren(NPCSkeleton & s, const NPCBoneCoords & delta);

  void SetBaseCoords(const Coordinates & abs) {
    m_root = abs;
  }
  void AddToBaseCoords(const Coordinates & abs) {
    m_root += abs;
  }
  const Coordinates & Root() const {return m_root;}
  Coordinates & Root() {return m_root;}

  Coordinates GetCoords() const {
    Coordinates c = m_root;
    //Coordinates plus = m_rel.GetCoords();
    NPCBoneCoords both = m_rel + m_abs;
    Coordinates plus = both.GetCoords();
    c += plus;
    return c;
  }

  /*
  void Propagate(const NPCBoneCoords & diff_coords) {
    m_abs += diff_coords;
  }
  */

  NPCBoneCoords & Rel() {return m_rel;}
  const NPCBoneCoords & Rel() const {return m_rel;}
  NPCBoneCoords Both() const {
    NPCBoneCoords out = m_abs + m_rel;
    return out;
  }


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
    cout << "Absolute rotation: " << endl;
    m_abs.Print();
    cout << "Combined rotation: " << endl;
    (m_abs+m_rel).Print();
    cout << "Base coordinates: " << endl;
    m_root.Print();
    cout << "Absolute coordinates, length: " <<  GetCoords().Length() << endl;
    GetCoords().Print();
  }

  void Mirror() {
    m_root[2] *= -1.;
    m_abs[1] *= -1.;
    m_rel[1] *= -1.;
    int tmp = m_upper[1];
    m_upper[1] = -m_lower[1];
    m_lower[1] = -tmp;
    //m_upper[1] *= -1.;
    //m_lower[1] *= -1.;
 }
 void Scale(double d) {
    m_root *= d;
    m_abs.Radius() *= d;
    m_rel.Radius() *= d;    
  }

protected:
  NPCBoneCoords m_rel;
  NPCBoneCoords m_abs;
  NPCBoneCoords m_upper;
  NPCBoneCoords m_lower;
  double m_width;
  svec<int> m_children;
  int m_myID;
  int m_parent;
  int m_physID;
  Coordinates m_root;
  string m_name;
  //Coordinates m_base;


};

//=========================================================
class NPCSkeleton
{
 public:
  NPCSkeleton() {}
  

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
  
  void AddToBoneRot(int index, const NPCBoneCoords & rel) {
    // Add
    m_bones[index].AddToRelCoords(rel);
    //cout << "ADD RELATIVE: " << endl;
    //rel.Print();
    //cout << "Update bone " << index << endl;
    m_bones[index].UpdateChildren(*this, rel);    
  }

  void SetBoneRot(int index, const NPCBoneCoords & rel) {
    NPCBoneCoords old = m_bones[index].Rel();
    NPCBoneCoords update = rel - old;
    AddToBoneRot(index, update);
  }

  void Scale(double d) {
    for (int i=0; i<m_bones.isize(); i++) {
      m_bones[i].Scale(d);
    }
  }


 protected:
  svec<NPCBone> m_bones;
  
};


#endif

