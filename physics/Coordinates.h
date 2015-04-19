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



  void operator += (const Coordinates & c) {
    for (int i=0; i<isize(); i++)
      m_data[i] += c.m_data[i];
  }

  void operator -= (const Coordinates & c) {
    for (int i=0; i<isize(); i++)
      m_data[i] -= c.m_data[i];
  }

  bool operator == (const Coordinates & c) const {
    for (int i=0; i<isize(); i++) {
      if (m_data[i] != c.m_data[i])
	return false;
    }
    return true;
  }

  double Scalar(const Coordinates & c) const {
    double s = 0.;
    for (int i=0; i<isize(); i++) {
      s += m_data[i] * c.m_data[i];
 
    }
    return s;
  }

  SphereCoordinates AsSphere() const;

  void FromSphere(const SphereCoordinates & c);

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















#endif //COORDINATES_H

