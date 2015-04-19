#include "physics/Coordinates.h"


SphereCoordinates Coordinates::AsSphere() const 
{
  double r = sqrt(m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);
  double theta = acos(m_data[2]/r);
  if (r == 0)
    theta = 0.;
  double phi = 0;
  if (m_data[0] > 0.) {
    phi = atan(m_data[1]/m_data[0]);
  }
  if (m_data[0] == 0) {
    if (m_data[1] > 0)
      phi = PI_P / 2.;
    else
      phi = -PI_P / 2.;	
  } 
  if (m_data[0] < 0) {
    if (m_data[1] >= 0) 
      phi = atan(m_data[1]/m_data[0]) + PI_P;
    else
      phi = atan(m_data[1]/m_data[0]) - PI_P;
  }
  SphereCoordinates out(phi, theta, r);
  return out;
}

void Coordinates::FromSphere(const SphereCoordinates & c) {
  //cout << "FROM SPHERE " << c.phi() << " " << c.theta() << endl;
  m_data[0] = c.r() * sin(c.theta()) * cos(c.phi());
  m_data[1] = c.r() * sin(c.theta()) * sin(c.phi());
  m_data[2] = c.r() * cos(c.theta()); 
  //cout << " -> " << m_data[0] << " " << m_data[1] << " " << m_data[2] << endl;
}

// Rotates this into the coordinates of the argument
Coordinates Coordinates::Rotate(const Coordinates & s) const {
  Coordinates out;
  SphereCoordinates mySphere = AsSphere();
  SphereCoordinates sSphere = s.AsSphere();
  
  double dphi = mySphere.phi() - sSphere.phi();
  double dtheta = mySphere.theta() - sSphere.theta() + PI_P/2.;
  /*if (dtheta < 0.)
    dtheta += 2*PI_P;
    if (dtheta > 2*PI_P)
    dtheta -= 2*PI_P;
    if (dphi < 0.)
    dphi += 2*PI_P;
    if (dphi > 2*PI_P)
    dphi -= 2*PI_P;*/
  
  //cout << "phi: " << dphi << endl;
  //cout << "the: " << dtheta << endl;
  mySphere.SetPhi(dphi);
  mySphere.SetTheta(dtheta);
  
  out.FromSphere(mySphere);
  
  return out;
}

Coordinates Coordinates::RotateBack(const Coordinates & s) const 
{
  Coordinates out;
  SphereCoordinates mySphere = AsSphere();
  SphereCoordinates sSphere = s.AsSphere();
  
  double dphi = mySphere.phi() + sSphere.phi();
  double dtheta = mySphere.theta() + sSphere.theta() - PI_P/2.;
  
  //cout << "phi: " << dphi << endl;
  //cout << "the: " << dtheta << endl;
  mySphere.SetPhi(dphi);
  mySphere.SetTheta(dtheta);
  
  out.FromSphere(mySphere);
  
  return out;
}

Coordinates Coordinates::CrossProduct(const Coordinates & s) const {
  Coordinates out;
  out[0] = m_data[1]*s[2] - m_data[2]*s[1];
  out[1] = m_data[2]*s[0] - m_data[0]*s[2];
  out[2] = m_data[0]*s[1] - m_data[1]*s[0];
  return out;
}
