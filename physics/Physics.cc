#define FORCE_DEBUG
 
#include "physics/Physics.h"

double abs(double a)
{
  if (a < 0)
    return -a;
  return a;    
}

double offset(double x, double d)
{
  if (x < 0.)
    d = -d;
  x -= d;
  return x;
}

void PhysObject::MoveTo(const Coordinates & c)
{
  m_center.SetPosition(c);

  Fixate();
}

Coordinates PhysObject::GetCenterPos()
{
  int i, j;
  double weight = 0;
  for (i=0; i<m_objects.isize(); i++) {
    weight += m_objects[i].GetMass();
  }
  m_center.SetMass(weight);
  Coordinates newCenter;
  for (i=0; i<m_objects.isize(); i++) {
    const PhysMinimal & p = m_objects[i];
    Coordinates c;
    c = p.GetPosition();
    newCenter += c * p.GetMass() / weight;   
  }
  return newCenter;
}

double PhysObject::UpdateImpulseEnergy()
{
  Coordinates cc;
  Coordinates xx;
  m_energy = 0.;
  double totalMass = 0.;
  //cout << "Update impulses (lateral & rotation): " << endl;
  for (int i=0; i<m_objects.isize(); i++) {
    Coordinates toCenter = m_objects[i].GetPosition() - m_center.GetPosition();
    Coordinates toCenterE = toCenter.Einheitsvector();
    double p = toCenterE.Scalar(m_objects[i].GetVelocity()) * m_objects[i].GetMass();
    double v = m_objects[i].GetVelocity().Length();

    cc += m_objects[i].GetVelocity()*m_objects[i].GetMass();
    //cout << " update " << i << " ";
    m_objects[i].GetVelocity().Print();

    //cout << "   add " << toCenterE[1]*p << " " <<  toCenterE[1] << endl;

    m_energy += m_objects[i].GetMass() * v * v / 2.;
    totalMass += m_objects[i].GetMass();
    
    Coordinates tangential = m_objects[i].GetVelocity() - toCenterE *  m_objects[i].GetVelocity().Scalar(toCenterE);
    Coordinates transversal = m_objects[i].GetVelocity() - tangential;
    double test = tangential.Scalar(toCenter);
    Coordinates x = toCenter.CrossProduct(tangential) * m_objects[i].GetMass();
    xx += x;  

  }
  
  
  //cout << "Impulse (lateral): ";
  m_latImp = cc;
  //m_latImp.Print(); 

  //cout << "Impulse (rotational): ";
  m_rotImp = xx;
  //m_rotImp.Print(); 
  return totalMass;
}

void PhysObject::AdjustImpulseEnergy()
{
  Coordinates tmpLat = m_latImp;
  Coordinates tmpRot = m_rotImp;
  //cout << "Adjusting to" << endl;
  m_latImp.Print();
  m_rotImp.Print();

  double totalMass = UpdateImpulseEnergy();
  
  Coordinates latVDiff = m_latImp - tmpLat;
  Coordinates rotVDiff = m_rotImp - tmpRot;
  //cout << "Differential: " << endl;
  //cout << " abs: ";
  //m_latImp.Print();
  //cout << " lat: ";
  //latVDiff.Print();
  //cout << " rot: ";
  //rotVDiff.Print();
  latVDiff /= totalMass;
 
  int i;
  // Lateral update
  Coordinates total, after;
  //cout << "Centerpos ";
  //m_center.GetPosition().Print();
  for (i=0; i<m_objects.isize(); i++) {
    Coordinates toCenter = m_objects[i].GetPosition() - m_center.GetPosition();
    Coordinates toCenterE = toCenter.Einheitsvector();
    double p = toCenterE.Scalar(m_objects[i].GetVelocity()) * m_objects[i].GetMass();
    double v = m_objects[i].GetVelocity().Length();

    //cout << "  before " << m_objects[i].Velocity()[1] << " - " << latVDiff[1] << endl;
    total += m_objects[i].Velocity()*m_objects[i].GetMass()/totalMass;
    //cout << " adjust " << i << " ";
    m_objects[i].GetVelocity().Print();

    //cout << "NO lateral update ";
    //m_objects[i].Velocity().Print();
    m_objects[i].Velocity() -= latVDiff;
    after += latVDiff;
  }
  //cout << "Total ";
  //total.Print();
  //cout << "After ";
  //after.Print();

  //====================================================
  UpdateImpulseEnergy();
  rotVDiff = m_rotImp - tmpRot;
  //cout << "Differential (after update): " << endl;
  //cout << " abs: ";
  //m_latImp.Print();
  //cout << " lat: ";
  //latVDiff.Print();
  //cout << " rot: ";
  //rotVDiff.Print();
  latVDiff /= totalMass;

  //====================================================

  Coordinates tmp;
  Coordinates xxx;
  double l = rotVDiff.Length();

  // Count objects that are right in the center
  double div = 0.;
  double totalDist = 0.;
  for (i=0; i<m_objects.isize(); i++) {
    if (m_objects[i].GetPosition() != m_center.GetPosition())
      div += 1.;
    Coordinates toCenter = (m_objects[i].GetPosition() - m_center.GetPosition());
    totalDist += toCenter.Length();
  }

  // Rotational update
  for (i=0; i<m_objects.isize(); i++) {
    if (m_objects[i].GetPosition() == m_center.GetPosition())
      continue;
    Coordinates toCenter = (m_objects[i].GetPosition() - m_center.GetPosition());
    
    Coordinates toCenterE = toCenter.Einheitsvector();

    Coordinates tangential = m_objects[i].GetVelocity() - toCenterE *  m_objects[i].GetVelocity().Scalar(toCenterE);
    Coordinates lateral = m_objects[i].GetVelocity() - tangential;

    Coordinates ehB = rotVDiff - toCenterE * rotVDiff.Scalar(toCenterE);
    Coordinates y = ehB.CrossProduct(toCenter).Einheitsvector();
    double r = toCenter.Length();
    double fac = ehB.Length();
    double ab = fac/r/m_objects[i].GetMass();
    ab *= r/totalDist;
    if (toCenter.Length() == 0)
      ab = 0.;

    //cout << "fac " << fac << " r=" << r << " f/r " << fac/r << endl;
    Coordinates yy = y * ab;
    
    //cout << "Add rot impulse: " << endl;
    //yy.Print();
    //===============================================
    m_objects[i].Velocity() -= yy;
    //===============================================
    //m_objects[i].Velocity().Print();
    xxx -= yy;
  }
  //cout << "Sum" << endl;
  //xxx.Print();
  //tmpRot.Print();

  cout << "Sanity check" << endl;
  UpdateImpulseEnergy();
  m_latImp = tmpLat;
  m_rotImp = tmpRot;
  //tmp.Print();
  //xxx.Print();
}

void PhysObject::AdjustCoordinates()
{

}

void PhysObject::Fixate()
{
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
    newVel += p.GetVelocity() * p.GetMass() / weight;
    newCenter += c * p.GetMass() / weight;   
    //cout << "Add " << newCenter[1] << " " << (m_center.GetPosition())[1] << " vel " << newVel[1] << " delta " << p.GetVelocity()[1]  << endl;
  }
  //cout << "Center pos ";
  // newCenter.Print();
  //cout << endl;

  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    Coordinates & c = p.Position();
    c -= newCenter;
  }

  // DEBUG!!!!!!!
  newCenter += m_center.GetPosition();
  m_center.SetPosition(newCenter);
  m_center.Velocity() = m_latImp / m_center.GetMass();

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
  m_bImpulse = true;

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

  m_bImpulse = true;
  PhysMinimal & min = m_objects[index];
  Coordinates vel = min.GetVelocity() + m_center.GetVelocity();
  double len = vel.Length();
  Coordinates e2 = direction.Einheitsvector();
  
  double force = -vel.Scalar(e2);
  cout << "Force " << force << endl;

  Coordinates & plus = min.Velocity();
  for (int i=0; i<plus.isize(); i++) {
    cout << "Bounce " << i << " " << plus[i] << " + " << 2*e2[i]*force << endl;
    
    plus[i] += 2*e2[i]*force;
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

double PhysObject::Energy() const
{
  double energy = 0.;
  for (int i=0; i<m_objects.isize(); i++) {
    double v = m_objects[i].GetVelocity().Length();
    energy += m_objects[i].GetMass() * v * v / 2.;
  }
  cout << "Energy: " << energy << endl;
  return energy;
}

Coordinates PhysObject::GetTotalImpulse(double & totalMass)
{
  Coordinates cc;
  double energy = 0.;
  totalMass = 0.;
  cout << "Impulse: " << endl;
  for (int i=0; i<m_objects.isize(); i++) {
    Coordinates plus =  m_objects[i].GetVelocity()*m_objects[i].GetMass();
    cout << "Contrib: ";
    plus.Print();
    cc += plus;
    double v = m_objects[i].GetVelocity().Length();
    energy += m_objects[i].GetMass() * v * v / 2.;
    totalMass += m_objects[i].GetMass();
  }
  

  cout << "Impulse: ";
  cc.Print(); 
  return cc;
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
  Coordinates impulseMoved;

  Coordinates cPos = GetCenterPos();
  cout << "Moving elements" << endl;

  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    if (o.IsFixed())
      continue;
    Coordinates & x = o.Position();
    Coordinates old = x;
    Coordinates & v = o.Velocity();

    x += v * deltatime;
   }


  svec<PhysMinimal> tmp = m_objects;
  
  // Re-set positions
  cout << "Resetting pos" << endl;
 
  for (i=0; i<m_connect.isize(); i++) {
    const PhysConnection & pc = m_connect[i];
    const Coordinates & a = tmp[pc.GetFirst()].GetPosition();
    const Coordinates & b = tmp[pc.GetSecond()].GetPosition();
    Coordinates & an = tmp[pc.GetFirst()].Position();
    Coordinates & bn = tmp[pc.GetSecond()].Position();
    double dist = (a - b).Length();
    double corr = (dist - pc.GetDistance())/2.;
    Coordinates ee = (a - b).Einheitsvector();
    ee *= corr;
    an -= ee / (double)tmp[pc.GetFirst()].GetConnectCount();
    bn += ee / (double)tmp[pc.GetSecond()].GetConnectCount();
    //cout << "Fixed: " << (a - b).Length() << " " << (an - bn).Length() << " soll " << pc.GetDistance() << " corr " << corr;
    //cout << " dist " << dist << " diff " << dist - pc.GetDistance() << " ee " << ee.Length() << endl;
    //a.Print();
    //b.Print();
    //an.Print();
    //bn.Print();
    //cout << "Moved " << endl;
    //ee.Print();
  }

  m_objects = tmp;

  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o1 = m_objects[i];
    const Coordinates & x1 = o1.GetPosition();
    Coordinates v1 = o1.GetVelocity();
    

    PhysMinimal & no1 = tmp[i];
    Coordinates & nx1 = no1.Position();
    Coordinates & nv1 = no1.Velocity();

    double m1 = o1.GetMass();
    //cout << "Update " << i << endl;
    svec<double> projection;
    projection.resize(o1.GetConnectCount(), 0.);

    Coordinates ve = v1.Einheitsvector();

    double projectSum = 0.;
    for (j=0; j<o1.GetConnectCount(); j++) {
      PhysConnection & pc = m_connect[o1.GetConnect(j)];
      int c = pc.GetOther(i);
      PhysMinimal & o2 = m_objects[c];
      Coordinates xx = o2.GetPosition() - x1;
      Coordinates xe = xx.Einheitsvector();
      //cout << "WEIGHT to " << c << endl;
      //xe.Print();
      //ve.Print();
      double d = xe.Scalar(ve);
      if (d < 0)
	d = -d;
      //cout << "Scalar: " << d << endl;
      projection[j] = d;
      projectSum += d;
    }
    if (projectSum == 0.)
      projectSum = 1.;

    for (j=0; j<o1.GetConnectCount(); j++) {
      PhysConnection & pc = m_connect[o1.GetConnect(j)];
      int c = pc.GetOther(i);
      PhysMinimal & o2 = m_objects[c];
      if (o2.IsFixed())
	continue;
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
      double lenDiff = currDist - len;
      double f = lenDiff/elast/m2;
      double f1 = lenDiff/elast/m1;

  
      double weight = projection[j]/projectSum;
      if (projectSum == 0)
	weight = 0.;

      Coordinates delta = eh * f * weight * deltatime;
      //cout << "  connect " << i << " -> " << c << " -> weight " << weight << " f: " << f;
      //cout << " " << delta[0] << " " << delta[1] << " " << delta[2] << endl;
      //cout << "Before " << nv2[0] << " " << nv2[1] << endl;
      Coordinates before2 = nv2;
      Coordinates before1 = nv1;

      

      Coordinates result = eh * f * weight * deltatime;
      result += eh * f1 * weight * deltatime;
      double resDist = result.Length();
      double adjust = 1.;
 
      nv2 +=  eh * f * weight * deltatime * adjust;      
      nv1 -=  eh * f1 * weight * deltatime * adjust;      
      cout << "After  " << nv2[0] << " " << nv2[1] << endl;
 
      damp = 0.3;
      Coordinates relV = nv2 - nv1;
      Coordinates rel1 = relV * damp / m1;
      Coordinates rel2 = relV * damp / m2;
      
      //cout << "Subtract " << rel2.Length() << " " << rel1.Length() << " " << (v1-v2).Length() <<  endl;
      //cout << "Before " << (nv2 - nv1).Length() << endl; 
      nv2 -= rel2 * weight * deltatime;
      nv1 += rel1 * weight * deltatime;
      //cout << "After  " << (nv2 - nv1).Length() << endl; 
     
      impulseMoved += nv2 - before2;
      impulseMoved += nv1 - before1;

 
    }
  }
  m_objects = tmp;

  cout << "IMPULSE MOVED: " << endl;
  impulseMoved.Print();

 
  if (m_bImpulse) {
    UpdateImpulseEnergy();
  }

  AdjustImpulseEnergy();

  m_bImpulse = false;
 
  //m_center.Velocity().Print();

  // Gravity
  //cout << "NO GRAVITY, DISABLED!!!" << endl;
  cout << "Gravity " << gravity << endl;
  (m_center.Velocity())[1] -= gravity*deltatime;
  m_center.Position() += m_center.Velocity() * deltatime;
  m_latImp[1] -= gravity*deltatime*m_center.GetMass();

  Fixate();
 
  // Print all objects, we're done.
  cout << "Printing objects: " << endl;
  Energy();
  cout << "Delta time " << deltatime << endl;
  cout << "Center: " << endl;
  m_center.Print();
  cout << "Objects: " << endl;
  
  /*
  for (i=0; i<m_objects.isize(); i++) {
    m_objects[i].Print();
  }
  cout << "Connectors: " << endl;
  for (i=0; i<m_connect.isize(); i++) {
    cout << i << " -> " << m_connect[i].GetOther(i) << ": " << m_connect[i].GetDistance();
    const PhysMinimal & a = m_objects[m_connect[i].GetFirst()];
    const PhysMinimal & b = m_objects[m_connect[i].GetSecond()];
    cout << " real: " << (a.GetPosition() - b.GetPosition()).Length() << endl;

    }*/

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
      object.Bounce(i, m_cross);
      bHit = true;
      break;
    }
  }
  return bHit;
}
