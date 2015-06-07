#define FORCE_DEBUG

#include "physics/Physics.h"


void PhysObject::MoveTo(const Coordinates & c)
{
  m_center.SetPosition(c);
  
  /*
  Coordinates ccc = m_center.GetPosition();

  for (int i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    Coordinates cc = p.GetPosition();
    cc += c - ccc;
    p.SetPosition(cc);
    }*/

  Fixate();
}


void PhysObject::Fixate()
{
  //m_center.Clear();
  int i, j;
  double weight = 0;
  for (i=0; i<m_objects.isize(); i++) {
    weight += m_objects[i].GetMass();
  }
  m_center.SetMass(weight);
  Coordinates newCenter;
  Coordinates newVel = m_center.GetVelocity();
  for (i=0; i<m_objects.isize(); i++) {
    const PhysMinimal & p = m_objects[i];
    Coordinates c;
    c = p.GetPosition();
    newVel += p.GetVelocity();
    newCenter += c * p.GetMass() / weight;   
    cout << "Add " << newCenter[1] << " " << (m_center.GetPosition())[1] << " vel " << newVel[1] << " delta " << p.GetVelocity()[1]  << endl;
  }
  cout << "Center pos " << newCenter[1] << endl;

  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    Coordinates & c = p.Position();
    c -= newCenter;
  }


  newCenter += m_center.GetPosition();
  m_center.SetPosition(newCenter);
  cout << "New center vel " << newCenter[1] << endl;
  m_center.SetVelocity(newVel);


 
  // Give it some default direction
  m_rot = m_objects[0].GetPosition();
}

void PhysObject::Connect(const PhysConnection & c)
{
  int k;
  for (k=0; k<m_connect.isize(); k++) {
    if (m_connect[k] == c) {
      cout << "Duplicate connection, omitting." << endl;
      return;
    }
  }

  int n = m_connect.isize();
  int i = c.GetFirst();
  int j = c.GetSecond();

  double dist = m_objects[i].Position().Length(m_objects[j].Position());
  cout << "DIST " << dist << endl;
  
  m_connect.push_back(c);
  m_connect[m_connect.isize()-1].SetDistance(dist);

  cout << "Connecting " << i << " <-> " << j << endl;
  m_objects[i].AddConnect(n);
  m_objects[j].AddConnect(n);
  
}

 
void PhysObject::ConnectAll(const PhysConnection & c)
{
  int i, j;
  PhysConnection tmp = c;
  for (i=0; i<m_objects.isize(); i++) {
    for (j=i+1; j<m_objects.isize(); j++) {      
      tmp.Set(i, j);
      Connect(tmp);
    }
  }
}

void PhysObject::Impulse(int index1, PhysObject & other, int index2)
{
  int i, j;

  cout << "Call Impulse" << endl;
  PhysMinimal & min1 = other[index2];
  PhysMinimal & min2 = m_objects[index1];
  
  double m1 = min1.GetMass();
  double m2 = min2.GetMass();

  

  Coordinates & x2 = min2.Position();
  Coordinates & velocity2 = min2.Velocity();
  Coordinates & velocity1 = min1.Velocity();
  Coordinates ee = velocity2.Einheitsvector();
  Coordinates ee1 = velocity1.Einheitsvector();



  Coordinates relVel = velocity1;
  relVel -= velocity2;
  Coordinates einheit = relVel.Einheitsvector();
  double v = relVel.Length();
  cout << relVel[0] << " " << relVel[1] << " " << relVel[2] << endl;

  Coordinates & toCenter = m_center.Position();
  toCenter -= min2.Position();
  Coordinates toCenterEE = toCenter.Einheitsvector();
  double rest = toCenterEE.Scalar(einheit);
  cout << "toCenter " << toCenter[0] << " " << toCenter[1] << " " << toCenter[2] << endl;
  cout << "einheit  " << einheit[0] << " " << einheit[1] << " " << einheit[2] << endl;
  double centerMass = rest*(m_center.GetMass()-m2);
  double m2_orig = m2;
  m2 += centerMass;
  cout << "centerMass " << centerMass << " rest " << rest << " " << m_center.GetMass() << endl;



  //double v2a = 2*(m1*v)/(m1+m2);
  double v2a = 2*(m1*v)/(m1+m2_orig);
  double v1a = 2*(m1*v)/(m1+m2)-v;

  v2a = (m1*v-m1*v1a)/m2_orig;
  cout << m1 << " " << v << " " << m2_orig << " " << v1a << endl;
  cout << "v1a=" << v1a << " v2a=" << v2a << endl;
  
  for (i=0; i<velocity2.isize(); i++) {
    cout << velocity2[i] << " " << v2a*einheit[i] << " " << relVel[i] << endl;
    velocity2[i] += v2a*einheit[i]; 
    cout << velocity2[i] << endl;
  }

  for (i=0; i<velocity1.isize(); i++) {
    cout << velocity1[i] << " " << v1a*einheit[i] << " " << relVel[i] << endl;
    velocity1[i] += v1a*einheit[i]-relVel[i]; 
  }

}

void PhysObject::Bounce(int index, const Coordinates & direction)
{
  PhysMinimal & min = m_objects[index];
  //Coordinates e = min.GetVelocity().Einheitsvector();
  Coordinates vel = min.GetVelocity() + m_center.GetVelocity();
  double len = vel.Length();
  Coordinates e2 = direction.Einheitsvector();
  
  double force = -vel.Scalar(e2);
  //if (force < 0)
  //force = -force;
  cout << "Force " << force << endl;

  Coordinates & plus = min.Velocity();
  for (int i=0; i<plus.isize(); i++) {
    cout << "Bounce " << i << " " << plus[i] << " + " << 2*e2[i]*force << endl;
    
    plus[i] += 2*e2[i]*force;
  }

  /*
  const Coordinates & x = min.GetPosition();
  Coordinates & v = min.Velocity();
  Coordinates toCenter = x.Einheitsvector();
        
  double lateral = -v.Scalar(toCenter);
  double m1 = min.GetMass();
  double m2 = m_center.GetMass() - .GetMass();
  double fact = lateral * o.GetMass() / (m_center.GetMass() - o.GetMass());
  m_center.Velocity() += toCenter * fact;
  v -= toCenter * (1. - fact);
  
  //double v2a = 2*(m1*lateral)/(m1+m2);
  //double v1a = 2*(m1*lateral)/(m1+m2)-lateral;
  //m_center.Velocity() += toCenter * v2a;
  //v += toCenter * (v1a - lateral);
  //cout << "Lat " << lateral << " " << v2a << " " << v1a << endl;
  
  cout << i << " After " << m_center.Velocity()[1] << " " << v[1] << " " << toCenter[1] << " lat " << lateral << " " <<  o.GetMass() / (m_center.GetMass() - o.GetMass()) <<  endl;
  cout << diff[0] << " " << diff[1] << " " << diff[2] << endl;
  cout << toCenter[0] << " " << toCenter[1] << " " << toCenter[2] << endl; */
    
}

void PhysObject::Impulse(int index, const Coordinates & velocity, double mass)
{
  if (mass < 0)
    mass = m_objects[index].GetMass();

  PhysObject tmp;
  PhysMinimal m;
  m.SetMass(mass);
  m.SetVelocity(velocity);
  tmp.Add(m);
  Impulse(index, tmp, 0);

 
}

void PhysObject::Print() const
{
  int i;
  cout << "Center" << endl;
  m_center.Print();
  cout << "Objects" << endl;
  for (i=0; i<m_objects.isize(); i++)
    m_objects[i].Print();
  cout << endl;
}

void PhysObject::GetRotation(Coordinates & rot)
{
  if (m_objects.isize() == 0) {
    rot[1] = rot[2] = 0.;
    rot[0] = 1.;
    return;
  }

  rot = m_rot;
  rot -= m_center.GetPosition();
  double n = sqrt(rot[0]*rot[0]+rot[1]*rot[1]+rot[2]*rot[2]);
  rot[0] /= n;
  rot[1] /= n;
  rot[2] /= n;
}

void PhysObject::Update(double deltatime, double gravity, int iterations)
{
  int i;
  for (i=0; i<iterations; i++) {
    UpdateReal(deltatime/(double)iterations, gravity);
  }
}

void PhysObject::UpdateReal(double deltatime, double gravity)
{
  int i, j;
  
  Fixate();

  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    Coordinates & x = o.Position();
    Coordinates old = x;
    Coordinates & v = o.Velocity();

    x += v * deltatime;
  }

  svec<PhysMinimal> tmp = m_objects;
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o1 = m_objects[i];
    const Coordinates & x1 = o1.GetPosition();
    const Coordinates & v1 = o1.GetVelocity();
  
    PhysMinimal & no1 = tmp[i];
    Coordinates & nx1 = no1.Position();
    Coordinates & nv1 = no1.Velocity();

    double m1 = o1.GetMass();
    //cout << "Update " << i << endl;
    for (j=0; j<o1.GetConnectCount(); j++) {
      PhysConnection & pc = m_connect[o1.GetConnect(j)];
      int c = pc.GetOther(i);
      PhysMinimal & o2 = m_objects[c];
      const Coordinates & x2 = o2.GetPosition();
      const Coordinates & v2 = o2.GetVelocity();
 
      PhysMinimal & no2 = tmp[c];
      Coordinates & nx2 = no2.Position();
      Coordinates & nv2 = no2.Velocity();

      double m2 = o2.GetMass();

      double len = pc.GetDistance();
      double elast = pc.GetElast();
      double damp = pc.GetDamp();

      Coordinates rel = x1 - x2;
      Coordinates eh = rel.Einheitsvector();
      
      double currDist = rel.Length();
      double f = (currDist - len)*elast * 20;
           
      nv2 = eh * deltatime * f / m2;
      //nv1 -= eh * deltatime * f / m1;

      cout << "debug " << i << " " << -eh[1] * deltatime * f / m2 << " " << f << endl;

    }
  }
  m_objects = tmp;

  m_center.Position() += m_center.Velocity() * deltatime;

  // Gravity
  (m_center.Velocity())[1] -= gravity*deltatime;
 
  Fixate();
 
  cout << "Printing objects: " << endl;
  cout << "Center: " << endl;
  m_center.Print();
  cout << "Objects: " << endl;
  
  for (i=0; i<m_objects.isize(); i++) {
    m_objects[i].Print();
  }

  /*  
  // Move objects
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    Coordinates & x = o.Position();
    Coordinates & v = o.Velocity();
    
    for (j=0; j<x.isize(); j++) 
      x[j] += v[j]*deltatime;
  }

  // 
  svec<PhysMinimal> tmp = m_objects;
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    const Coordinates & x = o.GetPosition();
    const Coordinates & v = o.GetVelocity();
    double m = o.GetMass();
    cout << "Update " << i << endl;
     for (j=0; j<o.GetConnectCount(); j++) {
       PhysConnection & pc = m_connect[o.GetConnect(j)];
       int c = pc.GetOther(i);
       PhysMinimal & o2 = m_objects[c];
       const Coordinates & x2 = o2.GetPosition();
       const Coordinates & v2 = o2.GetVelocity();
       double m2 = o2.GetMass();
       Coordinates rel_v = v;
       rel_v -= v2;
       Coordinates rel_x = x;
       rel_x -= x2;
       
       Coordinates rel_x_e = rel_x.Einheitsvector();
       Coordinates rel_v_e = rel_v.Einheitsvector();

       double len = rel_x.Length();
       double f = len - pc.GetDistance();
	 
       Coordinates pull = rel_x_e;
       double fact = f*m*pc.GetElast()*deltatime*pc.GetDistance();
       double scalar = rel_v_e.Scalar(rel_x_e);
       if (scalar < 0)
	 scalar = -scalar;
       cout << "Scalar: " << scalar << endl;
       fact *= scalar;

       double div = (double)o.GetConnectCount();
       //fact /= div;

  
       for (int x=0; x<pull.isize(); x++)
	 pull[x] *= fact;
       
       // Damp
       double damp = sqrt(rel_v.Length());
       for (int x=0; x<pull.isize(); x++)
	 pull[x] *= damp;
       
       cout << "Pull: " << pull[0] << " " << pull[1] << " " << pull[2] << endl;

       PhysMinimal & u = tmp[c];
       Coordinates & x3 = u.Position();
       Coordinates & v3 = u.Velocity();
       //cout << "Changing v for " << c << " from " << v3[0];
       for (int x=0; x<pull.isize(); x++) {
	 v3[x] += pull[x];       
       }
       //cout << " to " << v3[0] << endl;
    }
  }
  //cout << "TMP" << endl;
  //tmp[0].Print();
  m_objects = tmp;
  //cout << "OBJECTS" << endl;
  //m_objects[0].Print();
  Fixate();

  for (i=0; i<m_connect.isize(); i++)
    m_connect[i].Print();

  // Gravity (should be last)
  
  Coordinates xp = m_center.GetPosition();

  double before = (m_center.Position())[2];
  for (j=0; j<xp.isize(); j++) {
    (m_center.Position())[j] += deltatime*(m_center.Velocity())[j];
  }
  // Gravity
  (m_center.Velocity())[1] -= gravity*deltatime;

  cout << "Printing objects." << endl;
  // Move all objects according to the center
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    // PRINTING
    p.Print();
  }



  Fixate();

  */
  

}


//==========================================================

bool SolidTriangle::Collision(const Coordinates & before, const Coordinates & after) const
{
  // Too far away...
  if (before.Length(m_a) > m_maxlen && after.Length(m_a) > m_maxlen)
    return 0.;

  //Note: this should be a binary search!
  SolidTriangle test;
  double area = 0.;
  test.Set(m_a, m_b, before);
  area += test.GetArea();
  test.Set(m_b, m_c, before);
  area += test.GetArea();
  test.Set(m_c, m_a, before);
  area += test.GetArea();
  
  cout << "Area(1) " << area << " of " << GetArea() << " before " << before[2] << " " << m_a[2] << endl;
  double slack = 1.001;
  if (area/slack <= GetArea())
    return true;

  area = 0.;
  test.Set(m_a, m_b, after);
  area += test.GetArea();
  test.Set(m_b, m_c, after);
  area += test.GetArea();
  test.Set(m_c, m_a, after);
  area += test.GetArea();
  
  cout << "Area(2) " << area << " of " << GetArea() << endl;
  if (area/slack <= GetArea())
    return true;
 

  return false;
  
}

bool SolidTriangle::Collide(PhysObject & object) const
{
  int i;
  bool bHit = false;
  Coordinates c = object.GetCenter().GetVelocity();
  for (i=0; i<object.isize(); i++) {
    PhysMinimal & p = object[i];
    // Does it hit?
    if (Collision(p.GetPosition() + object.GetCenter().GetPosition(), p.GetPosition() + object.GetCenter().GetPosition())) {
      /* Coordinates m = p.GetVelocity();
      // m[0] = -2*(m[0]+c[0])*m_elast;
      //m[1] = -2*(m[1]+c[1])*m_elast;
      //m[2] = -2*(m[2]+c[2])*m_elast;
      m[0] = -(m[0])*m_elast;
      m[1] = -(m[1])*m_elast;
      m[2] = -(m[2])*m_elast;
      object.Impulse(i, m);
      cout << "Set IMPULSE " << i << " to " << m[0] << " " << m[1] << " " << m[2] << endl;*/
      object.Bounce(i, m_cross);
      bHit = true;
      break;
    }
  }
  return bHit;
}
