#define FORCE_DEBUG

#include "physics/Physics.h"

void PhysObject::Fixate()
{
  m_center.Clear();
  int i, j;
  double weight = 0;
  for (i=0; i<m_objects.isize(); i++) {
    weight += m_objects[i].GetMass();
  }
  m_center.SetMass(weight);
  for (i=0; i<m_objects.isize(); i++) {
    const PhysMinimal & p = m_objects[i];
    for (j=0; j<p.GetPosition().isize(); j++) {
      (m_center.Position())[j] += (p.GetPosition())[j]*p.GetMass()/weight;
      (m_center.Velocity())[j] += (p.GetVelocity())[j]*p.GetMass()/weight;
    }
   
  }
  // Give it some default direction
  m_rot = m_objects[0].GetPosition();
}

void PhysObject::Connect(const PhysConnection & c)
{
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

  // Return impulse instead of changing things...
  for (i=0; i<velocity1.isize(); i++) {
    cout << velocity1[i] << " " << v1a*einheit[i] << " " << relVel[i] << endl;
    velocity1[i] += v1a*einheit[i]-relVel[i]; 
  }

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

void PhysObject::Update(double deltatime, double gravity)
{
  int i, j;

  
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

       double len = rel_x.Length();
       double f = len - pc.GetDistance();
	 
       Coordinates pull = rel_x_e;
       double fact = f*m*pc.GetElast()*deltatime*pc.GetDistance();
       fact *= 3000.;
       if (fact > 0.5)
	 fact = 0.5;

       for (int x=0; x<pull.isize(); x++)
	 pull[x] *= fact;
       
       // Damp
       double damp = sqrt(rel_v.Length());
       for (int x=0; x<pull.isize(); x++)
	 pull[x] *= damp;
       
       cout << "Pull: " << pull[0] << endl;

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

  // Gravity (should be last)
  
  Coordinates xp = m_center.GetPosition();

  double before = (m_center.Position())[2];
  for (j=0; j<xp.isize(); j++) {
    (m_center.Position())[j] += deltatime*(m_center.Velocity())[j];
  }
  // Gravity
  (m_center.Velocity())[2] -= gravity*deltatime;

  
  // Move all objects according to the center
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    //for (j=0; j<p.GetPosition().isize(); j++) {
    (p.Position())[2] += deltatime*(m_center.Velocity())[2];
    (p.Velocity())[2] -= gravity*deltatime;
      //}   
  }

  Fixate();

  

  /*
  cout << "Impulse" << endl; 
  // Spread impulse across objects
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & m = m_objects[i];
    Coordinates & pos = m_objects[i].Position();
    Coordinates & v = m_objects[i].Velocity();
    PhysMinimal & m2 = tmp[i];

    double div = (double)m.GetConnectCount();
    for (j=0; j<m.GetConnectCount(); j++) {
      int c = m_connect[m.GetConnect(j)].GetOther(i);
      PhysMinimal & m1 = tmp[c];
      Coordinates pos1 = m_objects[c].GetPosition();
      Coordinates v1 = m_objects[c].GetVelocity();
      
      pos1 -= pos;
      Coordinates vv = v;
      vv -= v1;

      Coordinates epos1 = pos1.Einheitsvector();
      Coordinates ev1 = vv.Einheitsvector();
      cout << epos1[0] << " " << epos1[1] << " " << epos1[2] << " " << endl;
      cout << ev1[0] << " " << ev1[1] << " " << ev1[2] << " " << endl;
      double lateral = epos1.Scalar(ev1);

      for (int k = 0; k<pos1.isize(); k++) {      
	m1.Velocity()[k] += lateral*vv[k]*m.GetMass()/(m.GetMass()+m1.GetMass())/div;
	m2.Velocity()[k] = m1.Velocity()[k];
	//m2.Velocity()[k] -= lateral*pos1[k]*m1.GetMass()/(m.GetMass()+m1.GetMass())/div;
	cout << "Pass on " << lateral << " " << vv[k] << " " << div << endl;
	cout << lateral*vv[k]*m.GetMass()/(m.GetMass()+m1.GetMass())/div << endl;

      }      
    }
  }

  //m_objects = tmp;
  Print();
  cout << "MOVE" << endl; 
  
  // Move objects
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & m = m_objects[i];
    Coordinates & pos = m_objects[i].Position();
    Coordinates & v = m_objects[i].Velocity();
    for (j=0; j<pos.isize(); j++) {
      pos[j] += v[j]*deltatime;
    }
  }

  // And re-position
  Print();
  cout << "POSITION" << endl; 
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & m = m_objects[i];
    Coordinates & pos = m_objects[i].Position();
  
    double div = (double)m.GetConnectCount();
    for (j=0; j<m.GetConnectCount(); j++) {
      PhysConnection & p = m_connect[m.GetConnect(j)];
      int c = p.GetOther(i);
      cout << "from " << i << " to " << c << endl;
      PhysMinimal & m1 = m_objects[c];
      Coordinates pos1 = m_objects[c].GetPosition();
      cout << pos1[0] << " " << pos[0] << endl;
      pos1 -= pos;
      Coordinates epos1 = pos.Einheitsvector();
      double len = pos1.Length();
      double soll = p.GetDistance();
      double pull = p.GetElast();
      double plast = p.GetDamp();
      cout << "Move " << i << " " << len << " " << plast << " " << soll << endl;
      p.SetDistance(len*plast + soll*(1.-plast));
      soll = p.GetDistance();

      for (int k=0; k<pos.isize(); k++) {
	pos[k] += epos1[k]*soll/len*pull/div;
      }
     
    }
  }
  Print();
  cout << "DONE" << endl;
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
    if (Collision(p.GetPosition(), p.GetPosition())) {
      Coordinates m = p.GetVelocity();
      m[0] = -2*(m[0]+c[0])*m_elast;
      m[1] = -2*(m[1]+c[1])*m_elast;
      m[2] = -2*(m[2]+c[2])*m_elast;
      object.Impulse(i, m);
      cout << "Set IMPULSE " << i << " to " << m[0] << " " << m[1] << " " << m[2] << endl;
      bHit = true;
    }
  }
  return bHit;
}
