#include "physics/Plane.h"

bool FlatPlane::Collide(PhysObject & object, 
			double deltatime)
{
  svec<Coordinates> touch;
  return Collide(object, deltatime, touch);
}

bool FlatPlane::Collide(PhysObject & object, 
			double deltatime,
			svec<Coordinates> & touch)
{
  bool b = false;

  int i;

  Coordinates & rotImp = object.RotImpulse();
  Coordinates & latImp = object.LatImpulse();
  double totalMass = object.GetTotalMass();

  touch.resize(object.isize());

  bool bDirect = false;
  for (i=0; i<object.isize(); i++) {
    Coordinates vv = object[i].GetVelocity()+latImp/totalMass;
    if (CollideSingle(object[i], 
		      object.GetCenterDirect(),
		      latImp,
		      rotImp,
		      totalMass, 
		      deltatime)) {
      b = true;
      Coordinates cc1 = vv;
      cc1[1] = -cc1[1]+10*deltatime/**object.GetTotalMass()*/;
      cc1[0] = cc1[2] = 0.;
      touch[i] = cc1;
      //object.Impulse(i, cc1, -1);
      //Coordinates cc = Coordinates(0, 1, 0);
      cout << "Floor, velocity (before): ";
      object.GetCenterDirect().GetVelocity().Print();
      object.GetCenterDirect().SetVelocity(object[i].GetVelocity()*-1);

      cout << "Floor, velocity (after): ";
      object.GetCenterDirect().GetVelocity().Print();

      object.Bounce(i, cc1, 0.3);
      cout << "Bouncing " << i << endl;

      // ERROR: HARD CODED!!!
      //if (i == 1 || i== 9 || i == 10)
      //bDirect = true;
    } else {
      touch[i] = Coordinates(0, 0, 0);
    }
  }
  if (b) {
    //Coordinates & v = object.LatImpulse();
    // ERROR: Fix that here!!!!
    //if (bDirect)
    //v[1] = 0.;
    //cout << "Diminuish: ";
    //v.Print();
    //v *= 0.8;
  }

  return b;
}

bool FlatPlane::CollideSingle(PhysMinimal & p, 
			      PhysMinimal & center,
			      Coordinates & latImp,
			      Coordinates & rotImp,
			      double totalMass,
			      double deltatime)
{

  Coordinates & cent = center.Position();
  Coordinates & cent_v = center.Velocity();
  
  Coordinates & c = p.Position();
  Coordinates & v = p.Velocity();

  if (c[1]+cent[1] <= m_plane[1]) {
    double diff = m_plane[1] - (c[1]+cent[1]);
    double speed = v[1];
    speed = 1.;
    v[1] = 0;
    c[1] = m_plane[1]-cent[1];

    //cent[1] += diff;
    
    
    Coordinates toCenter = (cent - c).Einheitsvector();
    //double flat = sqrt(toCenter[0]*toCenter[0] + toCenter[2]*toCenter[2]);
    double up = toCenter[1];
    if (up < 0)
      up = -up;

    double lat = acos(up);
    latImp[1] += -latImp[1]*lat;
    latImp[1] *= 0.7;
    
    /*
    double x = speed*asin(toCenter[2]);
    double y = speed*asin(toCenter[0]);
    
    cout << "ROT IMPULSE: " << rotImp[0] << " + " << y << ", " << rotImp[2] << " + " << x << " diff: " << diff << " speed: " << speed << endl;
    rotImp[2] += x;    
    rotImp[0] += y;
    cout << "ROT IMPULSE (after): " << rotImp[0] << " + " << y << ", " << rotImp[2] << " + " << x << " diff: " << diff << " speed: " << speed << endl;  
    //rotImp[2] *= 0.8;    
    //rotImp[0] *= 0.8;
    */
    

    //cout << "Add to center: " << diff << " " << cent[1] << endl;
    //cent_v[1] = 0.;
    //v[0] *= 0.8;
    //v[2] *= 0.8;
    return true;
  }
  return false;
}
