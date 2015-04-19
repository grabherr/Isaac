
#include "physics/Physics.h"
#include <math.h>

int main(int argc,char** argv)
{
  /* double d = PI-0.01;
  cout << cos(-0.01) << endl;
  cout << d << endl;
  cout << acos(cos(d)) << endl;
  
  return 0;
  */

  /*Coordinates aa(1, 1, 1);
  Coordinates bb(1, 1, 1);

  SphereCoordinates ss;// = aa.AsSphere();
  ss.SetPhi(PI/2);
  ss.SetR(1);
  Coordinates s;
  s.FromSphere(ss);
  cout << s[0] << " " << s[1] << " " << s[2] << endl;
 
  Coordinates r = aa.Rotate(bb);
  cout << r[0] << " " << r[1] << " " << r[2] << endl;
  */

  //==================================================
  Coordinates ele(0, 0, 1);
  Coordinates force(0, 1, 1);

  SphereCoordinates test = ele.AsSphere();
  Coordinates test2;
  test2.FromSphere(test);
  cout << "Rev " << test2[0] << " " << test2[1] << " " << test2[2] << endl;

  Coordinates r = force.Rotate(ele);
  cout << "Rot " << r[0] << " " << r[1] << " " << r[2] << endl;
  cout << "centrifugal: " << r[0] << endl;
  r[0] = 0.;

  Coordinates rr = r.RotateBack(ele);
  cout << "lateral: " << rr[0] << " " << rr[1] << " " << rr[2] << endl;
  

  //==================================================

  //return 0;

  int i;

  PhysObject o;

  PhysMinimal a, b, c;

  a.Position()[0] = -1.;
  a.Position()[1] = 4.;

  b.Position()[0] = 1.;
  b.Position()[1] = 4.;

  a.SetMass(1.);
  b.SetMass(1.);

  o.Add(a);
  o.Add(b);
  //o.Print();
  o.Fixate();
  //o.Print();
  Coordinates imp;
  imp[1] = 1.;
  imp[0] = .2;
  imp[2] = -.4;
  o.Impulse(1, imp);

  for (i=0; i<100; i++) {
    o.Print();
    o.Update(0.01, 0);
  }

  return 0;
}
