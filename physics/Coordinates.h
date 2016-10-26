// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef COORDINATES_H
#define COORDINATES_H


#include <math.h>
#include "base/SVector.h"

#define DIMENSIONS 3
//#define PI 3.141592653589793
const double PI_P = 3.1415926535897932384626433832795;



inline double Sign(double x) 
{
  if (x < 0)
    return -1;
  return 1;
}

inline double Circle(double x) {
  return x;
  if (x >= 2*PI_P)
    x -= 2*PI_P;
  if (x <= -2*PI_P)
    x += 2*PI_P;
  return x;
}

inline double CircleAbs(double x) {
  if (x >= 2*PI_P)
    x -= 2*PI_P;
  if (x < 0.)
    x += 2*PI_P;
  return x;
}

inline double Radians(double degrees)
{
  return degrees/360.*2.*PI_P;
}

inline double Degrees(double radians)
{
  return radians*360./(2.*PI_P);
}

class Coordinates;


class SphereCoordinates
{
 public:
  SphereCoordinates() {
    m_phi = 0;
    m_theta = 0;
    m_r = 0;
    m_eta = 0;
  }
  SphereCoordinates(double phi, double theta, double r, double eta = 0) {
    m_phi = phi;
    m_theta = theta;
    m_r = r;
    m_eta = eta;
  }

  const double & phi() const {return m_phi;}
  const double & theta() const {return m_theta;}
  const double & eta() const {return m_eta;}
  const double & r() const {return m_r;}

  double & phi() {return m_phi;}
  double & theta() {return m_theta;}
  double & eta() {return m_eta;}
  double & r() {return m_r;}
 
  void SetPhi(double p) {m_phi = p;}
  void SetTheta(double p) {m_theta = p;}
  void SetEta(double p) {m_eta = p;}
  void SetR(double p) {m_r = p;}


  const double & Yaw() const {return m_phi;}
  const double & Pitch() const {return m_theta;}
  const double & Roll() const {return m_eta;}

  double & Yaw()  {return m_phi;}
  double & Pitch() {return m_theta;}
  double & Roll() {return m_eta;}
 
  void SetYaw(double p) {m_phi = p;}
  void SetPitch(double p) {m_theta = p;}
  void SetRoll(double p) {m_eta = p;}


  void Switch() {
    m_theta = -m_theta;
    if (m_phi >= PI_P)
      m_phi -= PI_P;
    else
      m_phi += PI_P;
  }
  void Switch(const SphereCoordinates & c) {
    double d1 = (m_phi - c.phi());
    SphereCoordinates tmp = *this;
    tmp.Switch();
    double d2 = (tmp.phi() - c.phi());
    if (d2*d2 < d1*d1)
      Switch();
  }
  
  void operator += (const SphereCoordinates &s) {    
    m_phi += s.phi();
    m_theta += s.theta();
  }
  void operator -= (const SphereCoordinates &s)  {
    m_phi -= s.phi();
    m_theta -= s.theta();
  }
  void operator *= (double d)  {
    m_r *= d;
  }
  void operator /= (double d)  {
    m_r /= d;
  }
  SphereCoordinates Mult(double r) const {
    SphereCoordinates m = *this;
    m.phi() *= r;
    m.theta() *= r;
    return m;
  }

  SphereCoordinates operator + (const SphereCoordinates & c) const {
    SphereCoordinates coords = *this;
    coords += c;
    return coords;
  }
  SphereCoordinates operator - (const SphereCoordinates & c) const {
    SphereCoordinates coords = *this;
    coords -= c;
    return coords;
  }
  SphereCoordinates operator * (double d) const {
    SphereCoordinates coords = *this;
    coords *= d;
    return coords;
  }
  SphereCoordinates operator / (double d) const {
    SphereCoordinates coords = *this;
    coords /= d;
    return coords;
  }


  int isize() const {return 3;}
  double & operator [] (int i) {
    if (i == 0)
      return m_phi;
    if (i == 1)
      return m_theta;    
    return m_r;
  }
  const double & operator [] (int i) const {
    if (i == 0)
      return m_phi;
    if (i == 1)
      return m_theta;    
    return m_r;
  }

  void Print() const {
    cout << "r=" << m_r << " phi=" << m_phi << " theta=" << m_theta << endl;
  }

  void NonNegative() {
    if (m_phi < 0.) {
      m_phi = -m_phi;
      m_theta = -m_theta;
    }
  }

  Coordinates RotateRoll(const Coordinates & c) const;
  SphereCoordinates RotateRoll(const SphereCoordinates & c) const;

  SphereCoordinates Rotate(const SphereCoordinates & c) const;
 
 private:
  double m_phi;
  double m_theta;
  double m_r;
  double m_eta;
};


//==============================================================
// Cartesian coordinates used throughout 
class Coordinates
{
 public:
  Coordinates() {
    m_data.resize(DIMENSIONS, 0.);
  }
  Coordinates(double x, double y, double z = 0.) {
    m_data.resize(DIMENSIONS, 0.);
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  // Basic access functions
  int isize() const {return m_data.isize();}
  const double & operator [] (int i) const {return m_data[i];}
  double & operator [] (int i) {return m_data[i];}

  // Access via x, y, and z
  double X() const {return m_data[0];}
  double Y() const {return m_data[1];}
  double Z() const {return m_data[2];}

  void ModX(double a) {m_data[0] += a;}
  void ModY(double a) {m_data[1] += a;}
  void ModZ(double a) {m_data[2] += a;}
 
  void SetX(double a) {m_data[0] = a;}
  void SetY(double a) {m_data[1] = a;}
  void SetZ(double a) {m_data[2] = a;}


  void Flip() {
    double tmp;
    tmp = m_data[2];
    m_data[2] = m_data[1];
    m_data[1] = tmp;
  }

  Coordinates operator + (const Coordinates & c) const {
    Coordinates out = *this;
    out += c;
    return out;
  }

  Coordinates operator - (const Coordinates & c) const {
    Coordinates out = *this;
    out -= c;
    return out;
  }

  Coordinates operator * (double v) const {
    Coordinates out = *this;
    out *= v;
    return out;
  }

  Coordinates operator / (double v) const {
    Coordinates out = *this;
    out /= v;
    return out;
  }

  void operator += (const Coordinates & c) {
    for (int i=0; i<isize(); i++)
      m_data[i] += c.m_data[i];
  }

  void operator -= (const Coordinates & c) {
    for (int i=0; i<isize(); i++)
      m_data[i] -= c.m_data[i];
  }

  void operator *= (double v) {
    for (int i=0; i<isize(); i++)
      m_data[i] *= v;
  }

  void operator /= (double v) {
    for (int i=0; i<isize(); i++)
      m_data[i] /= v;
  }

  bool operator == (const Coordinates & c) const {
    for (int i=0; i<isize(); i++) {
      if (m_data[i] != c.m_data[i])
	return false;
    }
    return true;
  }

  bool operator != (const Coordinates & c) const {
    return !(*this == c);
  }

  Coordinates Einheitsvector() const {
    Coordinates e;
    double s = Length();
    if (s == 0.)
      return e;
    
    for (int i=0; i<isize(); i++) {
      e[i] = m_data[i] / s;      
    }
    
    return e;
  }

  double Scalar(const Coordinates & c) const {
    double s = 0.;
    for (int i=0; i<isize(); i++) {
      s += m_data[i] * c.m_data[i];
 
    }
    return s;
  }

  double Length() const {
    double s = 0.;
    for (int i=0; i<isize(); i++) 
      s += m_data[i]*m_data[i];
    return sqrt(s);
  }

  double Length(const Coordinates & c) const {
    double s = 0.;
    for (int i=0; i<isize(); i++) 
      s += (m_data[i]-c.m_data[i])*(m_data[i]-c.m_data[i]);
    return sqrt(s);
  }

  SphereCoordinates AsSphere() const;
  void FromSphere(const SphereCoordinates & c);

  Coordinates AsRotation() const;
  void FromRotation(const Coordinates & c);




  // Rotates this into the coordinates of the argument
  Coordinates Rotate(const Coordinates & s) const;

  Coordinates RotateBack(const Coordinates & s) const;

  Coordinates CrossProduct(const Coordinates & s) const;

  bool operator < (const Coordinates & c) const {  
    SphereCoordinates me = AsSphere();
    SphereCoordinates other = c.AsSphere();
    return me.r() < other.r();
  }


  void Print() const {
    cout << m_data[0] << "\t" << m_data[1] << "\t" << m_data[2] << endl;
  }
private:
  svec<double> m_data;
};


inline double Angle(double x, double y)
{
  Coordinates c(x, y, 0);
  SphereCoordinates s = c.AsSphere();
  return s.phi();
}


inline double RelativeAngle(double x, double y, double xo, double yo)
{
  Coordinates c(xo-x, yo-y, 0.);
  SphereCoordinates s = c.AsSphere();
  double rel = s.phi();
  double ab = Angle(x, y);
  return (ab - rel);
}


class Plane
{
 public:
  Plane() {
    m_a = m_b = m_c = m_d = 0.;
  }

  void Setup(const Coordinates & a,
	     const Coordinates & b,
	     const Coordinates & c) {
    Coordinates one = a;
    Coordinates two = a;
    one -= b;
    two -= c;
    Coordinates cross = one.CrossProduct(two);
    m_a = cross[0];
    m_b = cross[1];
    m_c = cross[2];
    m_d = -(m_a*a[0]+m_b*a[1]+m_c*a[2]);
  }
	     

 private:
  double m_a;
  double m_b;
  double m_c;
  double m_d;
};






// ==========================================================
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




#endif //COORDINATES_H

