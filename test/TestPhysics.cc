
#include "physics/Physics.h"
#include <math.h>

int main(int argc,char** argv)
{

  
  PhysObject o1;
  PhysObject o2;
  PhysMinimal a1, a2;
  PhysMinimal b1, b2;
  
  a1.SetPosition(Coordinates(0, 0, 1));
  b1.SetPosition(Coordinates(1, 0, 1));
  a2.SetPosition(Coordinates(0, 0, 1));
  b2.SetPosition(Coordinates(-1, 0, 1));
  
  a1.SetMass(.5);
  a2.SetMass(1);
  b1.SetMass(.5);
  b2.SetMass(1);

  a1.SetVelocity(Coordinates(0, 0, 1));
  a2.SetVelocity(Coordinates(1, 0, 1));

  b1.SetVelocity(Coordinates(0, 0, 1));
  b2.SetVelocity(Coordinates(1, 0, 1));

  o1.Add(a1);
  o1.Add(b1);
  o2.Add(a2);
  PhysConnection c1;
  c1.Set(0, 1);
  o1.Connect(c1);

  o1.Fixate();
  o2.Fixate();

  cout << "Before " << endl;
  o1.Print();
  o2.Print();

  o1.Impulse(0, o2, 0);
  
  cout << "After" << endl;
  o1.Print();
  o2.Print();

  cout << "Updating" << endl;
  
  int i;
  double sec = 0.01;
  double accum = 0.;
  for (i=0; i<1000; i++) {
    cout << "Accum " << accum << endl;
    accum += sec; // 10 ms
    o1.Update(sec, 0.);
    o1.Print();
  }
  






  return 0;

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
  /*  Coordinates ele(0, 0, 1);
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

  a.Position()[0] = 1.;
  a.Position()[1] = 1.;
  a.Position()[2] = 20.;

  b.Position()[0] = 1.;
  b.Position()[1] = 1.;
  b.Position()[2] = 22;

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
  //o.Impulse(1, imp);

  SolidTriangle t;
  t.SetElasticity(0.8);
  t.Set(Coordinates(-10, -10, 0),
	Coordinates(-10, 20, 0),
	Coordinates(20, -10, 0));

  for (i=0; i<100; i++) {
    bool b = t.Collide(o);
    if (b)
      cout << "COLLISION" << endl;
    o.Print();
    o.Update(0.05);
  }
  */
  return 0;
}
