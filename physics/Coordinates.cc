#include "physics/Coordinates.h"

SphereCoordinates SphereCoordinates::Rotate(const SphereCoordinates & c) const
{
  SphereCoordinates cc = *this; 
  Coordinates a;

  a.FromSphere(cc);
  /*
  Coordinates bottom;
  bottom.FromSphere(c);
  Coordinates top;
  top.FromSphere(cc);
  // top += bottom;
  
  Coordinates rot = top.Rotate(bottom);
  
  SphereCoordinates rotc = rot.AsSphere();
  
  cc.theta() = rotc.theta();
  cc.phi() = rotc.phi();  

  return cc;
  */
  
  //Coordinates up(0, 1, 0);
  //Coordinates axis = abs_c.CrossProduct(up);
  SphereCoordinates ks;
   
  ks.theta() = PI_P/2;
  ks.phi() = PI_P/2.;
  ks.r() = 1.;

  double beta = -c.theta();
  
  Coordinates k;
  k.FromSphere(ks);

  Coordinates b;
  b = a*cos(beta);

  Coordinates b2 = k.CrossProduct(a)*sin(beta);
  
  Coordinates b3 = k*k.Scalar(a)*(1.-cos(beta));

  b += b2;
  b += b3;

  SphereCoordinates bb;
  bb = b.AsSphere();



  //cout << "DEBUG Before rot: ";
  //cc.Print();
  
  cc.theta() = bb.theta();
  cc.phi() = bb.phi();  
  //cc.phi() += c.phi();


  
  cout << "DEBUG After rot, c.phi=" << c.phi() << " bb.phi=" << bb.phi() <<": ";
  cc.Print();
  cout << "DEBUG Rotate around: ";
  c.Print();
  cout << "DEBUG Cart: ";
  Coordinates test;
  test.FromSphere(cc);
  test.Print();

   
  return cc;
  
}

SphereCoordinates SphereCoordinates::RotateRoll(const SphereCoordinates & c) const {
  Coordinates cc;
  cc.FromSphere(c);
  Coordinates tmp = RotateRoll(cc);
  SphereCoordinates out;
  out = tmp.AsSphere();
  return out;
}

Coordinates SphereCoordinates::RotateRoll(const Coordinates & c) const
{
  Coordinates me;
  me.FromSphere(*this);
  Coordinates mee = me.Einheitsvector();
  //mee.Flip();
  double x = mee[0];
  double y = mee[1];
  double z = mee[2];
  
  RotMatrix r;
  r.Set(0, 0, cos(m_eta)+x*x*(1-cos(m_eta)));
  r.Set(0, 1, x*y*(1-cos(m_eta))-z*sin(m_eta));
  r.Set(0, 2, x*z*(1-cos(m_eta))+y*sin(m_eta));

  r.Set(1, 0, y*x*(1-cos(m_eta))+z*sin(m_eta));
  r.Set(1, 1, cos(m_eta)+y*y*(1-cos(m_eta)));
  r.Set(1, 2, y*z*(1-cos(m_eta))-x*x*sin(m_eta));

  r.Set(2, 0, z*x*(1-cos(m_eta))-y*sin(m_eta));
  r.Set(2, 1, z*y*(1-cos(m_eta))+x*sin(m_eta));
  r.Set(2, 2, cos(m_eta)+z*z*(1-cos(m_eta)));

  Coordinates out = r.MultVec(c);
  //out.Flip();
  return out;
}


//==========================================================
SphereCoordinates Coordinates::AsSphere() const 
{
  int x=0;
  int y=2;
  int z=1;
  double r = sqrt(m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);
  double phi = 0;
  double theta = 0;

  /*
 
  theta = acos(m_data[z]/r);
  
  if (r == 0)
    theta = 0.;
  

  //===================================
  
  if (m_data[0] > 0.) {
    phi = atan(m_data[y]/m_data[0]);
  }
  if (m_data[0] == 0) {
    if (m_data[y] > 0) {
      phi = PI_P / 2.;
    } else {
      phi = -PI_P / 2.;
    }
  } 
  if (m_data[0] < 0) {
    if (m_data[y] >= 0) {
      phi = atan(m_data[y]/m_data[0]) + PI_P;
    } else {
      phi = atan(m_data[y]/m_data[0]) - PI_P;
    }
    }*/

  
  double phi2 = atan2(m_data[2], m_data[0]); 
  double theta2 = atan2(hypot(m_data[2], m_data[0]), m_data[1]);

    
  
  phi = phi2;
  theta = theta2;

   
  SphereCoordinates out(phi, theta, r);
  return out;
}

void Coordinates::FromSphere(const SphereCoordinates & c) {
  //cout << "FROM SPHERE " << c.phi() << " " << c.theta() << endl;
  m_data[0] = c.r() * sin(c.theta()) * cos(c.phi());
  m_data[2] = c.r() * sin(c.theta()) * sin(c.phi());
  m_data[1] = c.r() * cos(c.theta()); 
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

Coordinates Coordinates::AsRotation() const
{
  Coordinates tmp = *this;
  Coordinates ret;
 
  tmp[0] = (*this)[0];
  tmp[1] = (*this)[2];
  tmp[2] = (*this)[1];

  SphereCoordinates s = tmp.AsSphere();
  
  ret[1] = CircleAbs(s.phi()-PI_P/2.);
  double theta = CircleAbs(s.theta()-PI_P/2.);
  cout << "Theta: " << theta << endl;
  ret[0] = CircleAbs(theta*sin(s.phi())+PI_P/2.);
  ret[2] = CircleAbs(-theta*cos(s.phi()));
  
  /* Coordinates theta;
  theta[0] = s.theta();
  ret[2] = CircleAbs(s.phi());
  tmp[0] = (*this)[1];
  tmp[1] = (*this)[2];
  tmp[2] = (*this)[0];
  
  s = tmp.AsSphere();
  theta[1] =  s.theta();
  ret[0] = CircleAbs(s.phi());

  tmp[0] = (*this)[2];
  tmp[1] = (*this)[0];
  tmp[2] = (*this)[1];
  
  s = tmp.AsSphere();
  theta[2] = s.theta();
  ret[1] = CircleAbs(s.phi());

  ret.Print();
  theta.Print();
  cout << "-------------" << endl;
  */

  return ret;
}


void Coordinates::FromRotation(const Coordinates & c)
{
}
