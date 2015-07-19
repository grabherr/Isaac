
#include "physics/Physics.h"
#include <math.h>

int main(int argc,char** argv)
{
  int i, j;
  Coordinates rot;
  rot[2] = 0.1;

  Coordinates pos1;
  pos1[0] = 1.;

  pos1.Print();
  Coordinates result;
  SphereCoordinates sp = pos1.AsSphere();

  Coordinates toCenter = pos1;
  Coordinates toCenterE = toCenter.Einheitsvector();
  Coordinates ehB = rot - toCenterE * rot.Scalar(toCenterE);
  Coordinates y = ehB.CrossProduct(toCenter).Einheitsvector();
  double r = toCenter.Length();
  double fac = ehB.Length();
  double ab = fac;
  Coordinates yy = pos1 + y * ab;
  yy.Print();
  result += yy;
  cout << "Rel" << endl;
  SphereCoordinates s = yy.AsSphere();
  s.SetR(sp.r());
  s.SetPhi(Circle(s.phi()-sp.phi()));
  s.SetTheta(Circle(s.theta()-sp.theta()));

  //s.SetPhi(s.phi() * s.r());
  //s.SetTheta(s.theta() * s.r());
  s.Print();
  Coordinates moved;
  moved.FromSphere(s);
  moved.Print();

  cout << "Pos" << endl;
  SphereCoordinates rr = pos1.AsSphere();
  rr.Print();

  rr.SetPhi(Circle(s.phi() + rr.phi()));
  rr.SetTheta(Circle(s.theta() + rr.theta()));
  cout << "Added: " << endl;
  rr.Print();
  Coordinates cc;
  cc.FromSphere(rr);
  cc.Print();

  /*
  SphereCoordinates s1;
  s1.SetR(1.);
  s1.SetPhi(1.5708);
  s1.SetTheta(1.5708);
  Coordinates t;
  t.FromSphere(s1);
  t.Print();*/

  
  return 0;

  
  PhysObject o1;
  PhysObject o2;
  PhysMinimal a1, a2;
  PhysMinimal b1, b2;
  
  a1.SetPosition(Coordinates(0, -1, 0));
  b1.SetPosition(Coordinates(0, 1, 0));
  a2.SetPosition(Coordinates(0, 0, 0));
  b2.SetPosition(Coordinates(-1, 0, 10));
  

  a1.SetMass(.5);
  a2.SetMass(1);
  b1.SetMass(.5);
  b2.SetMass(1);

  a1.SetVelocity(Coordinates(0, 0, 0));
  a2.SetVelocity(Coordinates(1, 0, 0));

  b1.SetVelocity(Coordinates(0, 0, 0));
  b2.SetVelocity(Coordinates(1, 0, 0));

  o1.Add(a1);
  o1.Add(b1);

  o2.Add(a2);
 
  PhysConnection c1;
  c1.Set(0, 1);
  o1.Connect(c1);
  o1.Fixate();
  o2.Fixate();
  cout << "move" << endl;
  o1.MoveTo(Coordinates(0, 11, 0));

 
  cout << "Before " << endl;
  o1.Print();
  o2.Print();

  //o1.Impulse(0, o2, 0);
  
  cout << "After" << endl;
  o1.Print();
  o2.Print();

  cout << "Updating" << endl;
  
  double sec = 0.01;
  double accum = 0.;

  Coordinates up;
  up[1] = 1.;

  for (i=0; i<1000; i++) {
    cout << "Accum " << accum << endl;
    accum += sec; // 10 ms
    o1.Update(sec, 9.81);

    for (j=0; j<o1.isize(); j++) {
      PhysMinimal & min = o1[j];
      Coordinates hit;
      hit[1] = -min.GetVelocity()[1];
      if (min.GetPosition()[1] + o1.GetCenter().GetPosition()[1] < 0.) {
	cout << "Bounce!!" << endl;
	o1.Bounce(j, up);
	//o1.Impulse(j, hit);
      }

    }

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
