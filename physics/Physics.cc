//#define FORCE_DEBUG
 
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
  cout << "Move to " << endl;
  c.Print();
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
    //m_objects[i].GetVelocity().Print();

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
  //m_latImp.Print();
  //m_rotImp.Print();

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
    Coordinates toCenter = m_objects[i].GetPosition() /*- m_center.GetPosition()*/;
    Coordinates toCenterE = toCenter.Einheitsvector();
    double p = toCenterE.Scalar(m_objects[i].GetVelocity()) * m_objects[i].GetMass();
    double v = m_objects[i].GetVelocity().Length();

    //cout << "  before " << m_objects[i].Velocity()[1] << " - " << latVDiff[1] << endl;
    total += m_objects[i].Velocity()*m_objects[i].GetMass()/totalMass;
    //cout << " adjust " << i << " ";
    //m_objects[i].GetVelocity().Print();

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
  //====================================================
  
  latVDiff /= totalMass;


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

  //cout << "Sanity check" << endl;
  UpdateImpulseEnergy();
  m_latImp = tmpLat;
  m_rotImp = tmpRot;
  //tmp.Print();
  //xxx.Print();
}

void PhysObject::AdjustCoordinates()
{

}

void PhysObject::Center()
{
  int i, j;
  double weight = 0;
  for (i=0; i<m_objects.isize(); i++) {
    weight += m_objects[i].GetMass();
  }

  m_center.SetMass(weight);
  Coordinates newCenter;

  Coordinates centerPos;
  
  for (i=0; i<m_objects.isize(); i++) {
    const PhysMinimal & p = m_objects[i];
    Coordinates c;
    c = p.GetPosition();
    
    centerPos += c * p.GetMass() / weight;   
  }
  //cout << "Center object." << endl;
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    Coordinates & c = p.Position();
    c -= centerPos;
    //c.Print();
  }
}

void PhysObject::Fixate()
{
  int i, j;
  double weight = 0;
  for (i=0; i<m_objects.isize(); i++) {
    weight += m_objects[i].GetMass();
  }

  m_farthest = 0.;

  m_center.SetMass(weight);
  Coordinates newCenter;


  Coordinates newVel = m_center.GetVelocity();
  for (i=0; i<m_objects.isize(); i++) {
    const PhysMinimal & p = m_objects[i];
    Coordinates c;
    c = p.GetPosition();

    double dist = c.Length(m_center.GetPosition());
    if (dist > m_farthest)
      m_farthest = dist;

    newVel += p.GetVelocity() * p.GetMass() / weight;
    newCenter += c * p.GetMass() / weight;   
    //cout << "Add " << newCenter[1] << " " << (m_center.GetPosition())[1] << " vel " << newVel[1] << " delta " << p.GetVelocity()[1]  << endl;
  }
  //cout << "Center pos ";
  // newCenter.Print();
  //cout << endl;

  // TODO: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    Coordinates & c = p.Position();
    c -= newCenter;
  }

  // DEBUG!!!!!!!
  if (m_bElast) {
    newCenter += m_center.GetPosition();
    m_center.SetPosition(newCenter);
  }
  m_center.Velocity() = m_latImp / m_center.GetMass();

  // Give it some default direction
  m_rot = m_objects[0].GetPosition();
}

void PhysObject::Connect(const PhysConnection & c)
{
  int k;
  for (k=0; k<m_connect.isize(); k++) {
    if (m_connect[k] == c) {
      //cout << "Duplicate connection, omitting." << endl;
      return;
    }
  }

  int n = m_connect.isize();
  int i = c.GetFirst();
  int j = c.GetSecond();

  double dist = m_objects[i].Position().Length(m_objects[j].Position());
  //cout << "DIST " << dist << endl;
  
  m_connect.push_back(c);
  m_connect[m_connect.isize()-1].SetDistance(dist);

  //cout << "Connecting " << i << " <-> " << j << endl;
  m_objects[i].AddConnect(n);
  m_objects[j].AddConnect(n);
  
}

void PhysObject::ConnectToCenter(const PhysConnection & c)
{
  Fixate();
  Add(m_center);
  int i;
  PhysConnection tmp = c;
  int n = m_objects.isize()-1;
  m_objects[n].SetMass(1);
  for (i=0; i<n; i++) {
    tmp.Set(i, n);
    Connect(tmp);    
  }
 
}

void PhysObject::ConnectWithin(const PhysConnection & c, double dist)
{
  int i, j;
  PhysConnection tmp = c;
  for (i=0; i<m_objects.isize(); i++) {
    for (j=i+1; j<m_objects.isize(); j++) {      
      double d = (m_objects[i].Position()-m_objects[j].Position()).Length();
      if (d < dist) {
	cout << "Dist conn " << d << " " << i << " <-> " << j << endl; 
	tmp.Set(i, j);
	Connect(tmp);
      }
    }
  }
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
  m_bIsStopped = false;

  //cout << "Call Impulse" << endl;
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
  //cout << relVel[0] << " " << relVel[1] << " " << relVel[2] << endl;

  Coordinates & toCenter = m_center.Position();
  toCenter -= min2.Position();
  Coordinates toCenterEE = toCenter.Einheitsvector();
  double rest = toCenterEE.Scalar(einheit);
  //cout << "toCenter " << toCenter[0] << " " << toCenter[1] << " " << toCenter[2] << endl;
  //cout << "einheit  " << einheit[0] << " " << einheit[1] << " " << einheit[2] << endl;
  double centerMass = rest*(m_center.GetMass()-m2);
  double m2_orig = m2;
  m2 += centerMass;
  //cout << "centerMass " << centerMass << " rest " << rest << " " << m_center.GetMass() << endl;

  double v2a = 2*(m1*v)/(m1+m2_orig);
  double v1a = 2*(m1*v)/(m1+m2)-v;

  v2a = (m1*v-m1*v1a)/m2_orig;
  //cout << m1 << " " << v << " " << m2_orig << " " << v1a << endl;
  //cout << "v1a=" << v1a << " v2a=" << v2a << endl;
  
  for (i=0; i<velocity2.isize(); i++) {
    //cout << velocity2[i] << " " << v2a*einheit[i] << " " << relVel[i] << endl;
    velocity2[i] += v2a*einheit[i]; 
    //cout << velocity2[i] << endl;
  }

  for (i=0; i<velocity1.isize(); i++) {
    //cout << velocity1[i] << " " << v1a*einheit[i] << " " << relVel[i] << endl;
    velocity1[i] += v1a*einheit[i]-relVel[i]; 
  }

}

void PhysObject::Bounce(int index, const Coordinates & direction, double elast)
{
  //if (!m_bElast && index == m_lastBounce)
  //return;

  m_lastBounce = index;

  m_bImpulse = true;
  m_bIsStopped = false;

  PhysMinimal & min = m_objects[index];
  Coordinates vel = min.GetVelocity() + m_center.GetVelocity();
  double len = vel.Length();
  Coordinates e2 = direction.Einheitsvector();
  
  double force = -vel.Scalar(e2);
  //cout << "Force " << force << endl;

  Coordinates plus = min.Velocity();
  int i;

  double proj = direction.Scalar(min.GetVelocity() + m_center.GetVelocity());
  cout << "Bounce, scalar: " << proj << endl;
  if (proj > 0.) {
    cout << "NOT bouncing, object has bounced already." << endl; 
    return;
  }

  if (m_bElast) {
    for (int i=0; i<plus.isize(); i++) {
      //cout << "Bounce " << i << " " << plus[i] << " + " << 2*e2[i]*force << endl;    
      plus[i] += 2*e2[i]*force*elast;
    }
    min.Velocity() = plus;
  } else {
    plus = direction.Einheitsvector();

    Coordinates toCenter = min.GetPosition();
    Coordinates toCenterE = toCenter.Einheitsvector();
    double v = plus.Scalar(m_center.GetVelocity());
    double p = toCenterE.Scalar(plus) * m_center.GetMass() * (-2*v);
 
    Coordinates cc = toCenterE * p * elast;  
  
    double totalMass = m_center.GetMass();
    
    Coordinates tangential = plus - toCenterE /* * plus.Scalar(toCenterE)*/;
    Coordinates transversal = plus - tangential;

    Coordinates xx = toCenter.CrossProduct(tangential) * m_center.GetMass();
    //cout << "Applying bounce to index: " << index << endl; 
    //cout << "Elast " << elast << endl;
    Coordinates a = min.GetPosition() + m_center.GetPosition();
    //a.Print();
    //m_latImp.Print();
    //m_rotImp.Print();
    //cout << "Direction: ";
    //plus.Print();

    m_latImp += cc;
    m_center.Velocity() = m_latImp / totalMass;
    m_rotImp += xx;
    //cout << "Bounce:     ";
    //plus.Print();
    //cout << "Lateral:    ";
    //cc.Print();
    //m_latImp.Print();
    //cout << "Rotational: ";
    //xx.Print();
    //m_rotImp.Print();
    
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
  cout << "Scale: " << m_scale << endl;
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
  //cout << "Energy: " << energy << endl;
  return energy;
}

Coordinates PhysObject::GetTotalImpulse(double & totalMass)
{
  Coordinates cc;
  double energy = 0.;
  totalMass = 0.;
  //cout << "Impulse: " << endl;
  for (int i=0; i<m_objects.isize(); i++) {
    Coordinates plus =  m_objects[i].GetVelocity()*m_objects[i].GetMass();
    //cout << "Contrib: ";
    //plus.Print();
    cc += plus;
    double v = m_objects[i].GetVelocity().Length();
    energy += m_objects[i].GetMass() * v * v / 2.;
    totalMass += m_objects[i].GetMass();
  }
  

  //cout << "Impulse: ";
  //cc.Print(); 
  return cc;
}

void PhysObject::ApplyGravity(double deltatime, double gravity)
{
  (m_center.Velocity())[1] -= gravity*deltatime;
  m_center.Position() += m_center.Velocity() * deltatime;
  for (int i=0; i<m_objects.isize(); i++) {
    m_objects[i].Position() += m_center.Velocity() * deltatime;
  }
  m_latImp[1] -= gravity*deltatime*m_center.GetMass();
}

void PhysObject::Update(double deltatime, double gravity)
{
  if (m_bElast) {
    UpdateElast(deltatime, gravity);
  } else {
    UpdateFixed(deltatime, gravity);
  }
}

// Move objects according to the center
void PhysObject::UpdateFixed(double deltatime, double gravity)
{
  int i, j;
  
  Fixate();
  m_bIsStopped = true;
  if (m_bImpulse) {
    //UpdateImpulseEnergy();
  }

  AdjustImpulseEnergy();

  m_bImpulse = false;

  // Move all objects linearly
  ApplyGravity(deltatime, gravity);
  

  Fixate();

  // Rotate objects
  const Coordinates & center = m_center.Position();
  Coordinates rot = m_rotImp / m_center.GetMass();
  
  if (m_center.GetVelocity().Length() > m_stop)
    m_bIsStopped = false;

  //if (m_rotImp.Length() < 0.0001) {
  //  m_rotImp = Coordinates(0., 0., 0.);
  //  cout << "Rot imp = 0, skip update." << endl;
  //  return;
  //}

  rot *= deltatime;
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    //o.Print();
    Coordinates & x = o.Position();
    Coordinates & v = o.Velocity();
    if (v.Length() > m_stop)
      m_bIsStopped = false;

    Coordinates pos1 = x /* - center*/;
    SphereCoordinates sp = pos1.AsSphere();

    Coordinates toCenter = pos1;
    //cout << "To center" << endl;
    //toCenter.Print();
    Coordinates toCenterE = toCenter.Einheitsvector();
    Coordinates ehB = rot - toCenterE * rot.Scalar(toCenterE);
    Coordinates y = ehB.CrossProduct(toCenter).Einheitsvector();
    double r = toCenter.Length();
    double fac = ehB.Length();
    double ab = fac;
    Coordinates yy = pos1 + y * ab;
    //yy.Print();
    
    //cout << "Rel" << endl;
    SphereCoordinates s = yy.AsSphere();
    s.SetR(sp.r());
    s.SetPhi(Circle(s.phi()-sp.phi()));
    s.SetTheta(Circle(s.theta()-sp.theta()));
    
    //s.Print();
    Coordinates moved;
    moved.FromSphere(s);
    //moved.Print();
    
    //cout << "Pos" << endl;
    SphereCoordinates rr = pos1.AsSphere();
    //rr.Print();
    
    rr.SetPhi(Circle(s.phi() + rr.phi()));
    rr.SetTheta(Circle(s.theta() + rr.theta()));
    //cout << "Before: " << endl;
    //x.Print();
    //cout << x.Length() << endl;   
    //cout << "After: " << endl;
    //rr.Print();
    Coordinates cc;
    cc.FromSphere(rr);
    //cc.Print();
    Coordinates x_keep = x;
    double dist_orig = x.Length();

    x = /*center +*/ cc;
    double dist_new = x.Length();
    double diff = dist_new - dist_orig;
    if (diff < 0.)
      diff = -diff;
    if (diff > 0.0000001)
      cout << "ERROR" << endl;
    cout << x.Length() << endl;
    v = x - x_keep;
  }
  
  
  // Adjust for numerical errors
  for (i=0; i<m_connect.isize(); i++) {
    const PhysConnection & pc = m_connect[i];
    const Coordinates & a = m_objects[pc.GetFirst()].GetPosition();
    const Coordinates & b = m_objects[pc.GetSecond()].GetPosition();
    Coordinates & an = m_objects[pc.GetFirst()].Position();
    Coordinates & bn = m_objects[pc.GetSecond()].Position();
    double len_a = an.Length();
    double len_b = bn.Length();

    double dist = (a - b).Length();
    double corr = (dist - pc.GetDistance())/2.;
    Coordinates ee = (a - b).Einheitsvector();
    ee *= corr;
    an -= ee / (double)m_objects[pc.GetFirst()].GetConnectCount();
    bn += ee / (double)m_objects[pc.GetSecond()].GetConnectCount();

    double len_a_n = an.Length();
    double len_b_n = bn.Length();

    an = an * len_a / len_a_n;
    bn = bn * len_b / len_b_n;
  }
  


  Fixate();

  //if (m_bIsStopped)
  //  Stop();


  // Print all objects, we're done.
  cout << "Printing objects: " << endl;
  Energy();
  cout << "Delta time " << deltatime << endl;
  cout << "Center: " << endl;
  m_center.Print();
  cout << "Lateral impulse: " << endl;
  m_latImp.Print();
  cout << "Objects: " << endl;
  for (i=0; i<m_objects.isize(); i++)
    m_objects[i].Print();

}

void PhysObject::Stop()
{
  Coordinates n = Coordinates(0., 0., 0.);
  m_latImp = n;
  m_rotImp = n;
  m_center.Velocity() = n;

  for (int i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    Coordinates & v = o.Velocity();
    v = n;
  }
}


void PhysObject::UpdateElast(double deltatime, double gravity)
{
  int i, j;
  
  Fixate();
  Coordinates impulseMoved;

  Coordinates cPos = GetCenterPos();
  //cout << "Moving elements" << endl;

  m_bIsStopped = true;

  if (m_center.GetVelocity().Length() > m_stop)
    m_bIsStopped = false;

  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & o = m_objects[i];
    if (o.IsFixed())
      continue;
    Coordinates & x = o.Position();
    Coordinates old = x;
    Coordinates & v = o.Velocity();
    if (v.Length() > m_stop)
      m_bIsStopped = false;

    x += v * deltatime;
   }


  svec<PhysMinimal> tmp = m_objects;
  
  // Re-set positions
  //cout << "Resetting pos" << endl;
 
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
      //cout << "After  " << nv2[0] << " " << nv2[1] << endl;
 
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

  //cout << "IMPULSE MOVED: " << endl;
  //impulseMoved.Print();

 
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
 
  //if (gravity == 0. && m_bIsStopped)
  //Stop();

  Fixate();
 
  // Print all objects, we're done.
  cout << "Printing objects: " << endl;
  Energy();
  cout << "Delta time " << deltatime << endl;
  cout << "Center: " << endl;
  m_center.Print();
  //cout << "Objects: " << endl;
  


}

bool PhysObject::DoesCollide(PhysObject & o)
{
  // Quick check
  double dist = m_center.GetPosition().Length(o.GetCenter().GetPosition());
  dist -= 1.1*(GetFarthest() + o.GetFarthest());
  if (dist > 0.)
    return false;

  double elast = o.GetTotalMass() / (GetTotalMass() + o.GetTotalMass());
  int i;
  
  for (i=0; i<m_connectTriangles.isize(); i+= 3) {
    int a = m_connectTriangles[i];
    int b = m_connectTriangles[i+1];
    int c = m_connectTriangles[i+2];
    SolidTriangle t;
    t.Set(m_objects[a].GetPosition() + m_center.GetPosition(),
	  m_objects[b].GetPosition() + m_center.GetPosition(),
	  m_objects[c].GetPosition() + m_center.GetPosition());

    double dir = t.Cross().Scalar(m_objects[a].GetPosition());
    if (dir < 0.)
      t.InvertDirection();
    t.SetElasticity(elast);
    if (t.Collide(o)) {
      // ERROR: Find correct point at which they collide!
      // Change INDEX!!!!
      cout << "Inter-object collision detected!!! " << i << endl;
      Bounce(a, t.Cross() * -1., 1. - elast);      
      return true;
    }
  }

  return false;
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
  
  //cout << "Area(1) " << area << " of " << GetArea() << " before " << before[2] << " " << m_a[2] << endl;
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
  
  //cout << "Area(2) " << area << " of " << GetArea() << endl;
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
      cout << "Collides with " << i << endl;
      object.Bounce(i, m_cross, m_elast);
      bHit = true;
      break;
    }
  }
  return bHit;
}
