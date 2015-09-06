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

class SphereCoordinates
{
 public:
  SphereCoordinates() {
    m_phi = 0;
    m_theta = 0;
    m_r = 0;
  }
  SphereCoordinates(double phi, double theta, double r) {
    m_phi = phi;
    m_theta = theta;
    m_r = r;
  }

  double phi() const {return m_phi;}
  double theta() const {return m_theta;}
  double r() const {return m_r;}
 
  void SetPhi(double p) {m_phi = p;}
  void SetTheta(double p) {m_theta = p;}
  void SetR(double p) {m_r = p;}

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

 private:
  double m_phi;
  double m_theta;
  double m_r;
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









#endif //COORDINATES_H

