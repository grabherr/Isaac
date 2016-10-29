#include "npc/Skeleton.h"
#include "base/FileParser.h"

Coordinates NPCRotate(const Coordinates & a, const Coordinates & s) {
  Coordinates out;
  SphereCoordinates mySphere = a.AsSphere();
  SphereCoordinates sSphere = s.AsSphere();
  
  
  double dphi = mySphere.phi() + sSphere.phi();
  double dtheta = mySphere.theta() + sSphere.theta();

  mySphere.SetPhi(dphi);
  mySphere.SetTheta(dtheta);
  
  out.FromSphere(mySphere);
  
  return out;
}

Coordinates NPCRotateS(const Coordinates & a, const SphereCoordinates & sSphere) {
  Coordinates out;
  SphereCoordinates mySphere = a.AsSphere();

  
  double dphi = mySphere.phi() + sSphere.phi();
  double dtheta = mySphere.theta() + sSphere.theta();

  mySphere.SetPhi(dphi);
  mySphere.SetTheta(dtheta);
  
  out.FromSphere(mySphere);
  
  return out;
}

Coordinates NPCRotatePhi(const Coordinates & a, const SphereCoordinates & sSphere) {
  Coordinates out;
  SphereCoordinates mySphere = a.AsSphere();

  
  double dphi = mySphere.phi() + sSphere.phi();

  mySphere.SetPhi(dphi);
  
  out.FromSphere(mySphere);
  
  return out;

  /*
  Coordinates axis;
  axis[0] = 0;
  axis[1] = 1;
  axis[2] = 0;
  
  SphereCoordinates mySphere = a.AsSphere();

  double u = axis[0];
  double v = axis[1];
  double w = axis[2];

  double x = a[0];
  double y = a[1];
  double z = a[2];
  double theta = -sSphere.theta()+mySphere.theta();
  
  Coordinates out;

  out[0] = u*(u*x+v*y+w*z)*(1-cos(theta))+x*cos(theta)+(-w*y+v*z)*sin(theta);
  out[1] = v*(u*x+v*y+w*z)*(1-cos(theta))+y*cos(theta)+(w*x-u*z)*sin(theta);
  out[2] = w*(u*x+v*y+w*z)*(1-cos(theta))+z*cos(theta)+(-v*x+u*y)*sin(theta);

  //out[0] = 
  
  return out;*/
}

Coordinates NPCRotateS2(const Coordinates & a, const SphereCoordinates & sSphere) {

  Coordinates axis;
  axis[0] = cos(sSphere.phi()-PI_P/2);
  axis[2] = sin(sSphere.phi()-PI_P/2);

  double u = axis[0];
  double v = axis[1];
  double w = axis[2];

  double x = a[0];
  double y = a[1];
  double z = a[2];
  double theta = sSphere.theta();
  
  Coordinates out;

  out[0] = u*(u*x+v*y+w*z)*(1-cos(theta))+x*cos(theta)+(-w*y+v*z)*sin(theta);
  out[1] = v*(u*x+v*y+w*z)*(1-cos(theta))+y*cos(theta)+(w*x-u*z)*sin(theta);
  out[2] = w*(u*x+v*y+w*z)*(1-cos(theta))+z*cos(theta)+(-v*x+u*y)*sin(theta);

  //out[0] = 
  
  return out;
}

Coordinates NPCRotateByAxis(const Coordinates & a, const Coordinates & axis, double angle)
{
  double u = axis[0];
  double v = axis[1];
  double w = axis[2];

  double x = a[0];
  double y = a[1];
  double z = a[2];
  double theta = angle;
  
  Coordinates out;

  out[0] = u*(u*x+v*y+w*z)*(1-cos(theta))+x*cos(theta)+(-w*y+v*z)*sin(theta);
  out[1] = v*(u*x+v*y+w*z)*(1-cos(theta))+y*cos(theta)+(w*x-u*z)*sin(theta);
  out[2] = w*(u*x+v*y+w*z)*(1-cos(theta))+z*cos(theta)+(-v*x+u*y)*sin(theta);
  return out;

}


void NPCBone::UpdateChildren(NPCSkeleton & s, const Coordinates & tip, const Coordinates & root)
{
  int i;
  //return;

  Coordinates dir = tip - root;
  
  for (i=0; i<m_children.isize(); i++) {
    NPCBone & n = s[m_children[i]];
    SphereCoordinates & rel = n.Rel().SCoords();
    SphereCoordinates & corr = n.Correction().SCoords();
  
    SphereCoordinates dirc;
    dirc = dir.AsSphere();
    
    Coordinates relc;
    relc.FromSphere(rel/* - corr*/);
    Coordinates newabs;
    
    if (m_last.r() > 0.) {
      dirc.Switch(m_last);
      //newabs = NPCRotateS2(relc, dirc);
      
      newabs = NPCRotatePhi(relc, dirc);
      newabs = NPCRotateS2(newabs, dirc);
    }

    
    m_last = dirc;
    
    n.GetCoords() = newabs + tip;    
    n.SetBaseCoords(tip);

    cout << "From SCoords (deri) ";
    n.GetCoords().Print();
    
    Coordinates delta;
    
    n.UpdateChildren(s, n.GetCoords(), n.GetBaseCoords());    
  }

}

bool NPCNerve::Move(NPCSkeleton & s, double speed)
{
  int i;
  bool b = false;
  if (m_move + speed < m_max && m_move + speed > m_min) {
    m_move += speed;
    b = true;
  }
  //cout << "MOVE Speed " << speed << endl;
  for (i=0; i<m_index.isize(); i++) {
    //NPCBoneCoords rr;
    ///rr = m_relMove[i]*speed;
    //cout << "MOVE rot ";
    //rr.Print();
   
    s.AddToBoneRot(m_index[i], m_relMove[i]*speed);
  }
  return b;
}

void NPCSkeleton::Read(const string & fileName)
{
  FlatFileParser parser;


  int i;
  parser.Open(fileName);
  parser.ParseLine();
  int n = parser.AsInt(0);
  //m_bones.resize(n);
  for (i=0; i<n; i++) {
    //NPCBone & b = m_bones[i];
    parser.ParseLine();
    double a = parser.AsFloat(1);
    double b = parser.AsFloat(2);
    double c = parser.AsFloat(3);
    AddToBoneRot(i, NPCBoneCoords(0, a, b, c));
  }
}

void NPCSkeleton::Write(const string & fileName)
{
  FILE * p = fopen(fileName.c_str(), "w");
  fprintf(p, "%d\n", m_bones.isize());
  for (int i=0; i<m_bones.isize(); i++) {
    NPCBoneCoords & b = m_bones[i].Rel();
    NPCBoneCoords & base = m_baseline[i].Rel();
    fprintf(p, "%d %f %f %f\n", i, b.Yaw()-base.Yaw(),
	    b.Pitch()-base.Pitch(), b.Roll()-base.Roll());
  }
  fclose(p);
}
  
void NPCSkeleton::RotateAll(const Coordinates & axis, double angle)
{
  int i;
  for (i=0; i<isize(); i++) {
    NPCBone & n = (*this)[i];
    Coordinates tip = n.GetCoords();
    Coordinates base = n.GetBaseCoords();

    tip = NPCRotateByAxis(tip, axis, angle);
    n.GetCoords() = tip;
    
    base = NPCRotateByAxis(base, axis, angle);
    n.SetBaseCoords(base);
  }
}

//==============================================
void NPCSkeleton::Update(double deltatime)
{
  //return;
  
  int i;
  //if (m_bFirst)
  //m_absPos = m_base;
    
  double lowest = 10000;
  double diff = 10000;
  Coordinates center;
  Coordinates floor1, floor2;
  int onFloor = 0;
  int onFloorOrClose = 0;
  for (i=0; i<m_bones.isize(); i++) {
    Coordinates cc = m_bones[i].GetCoords();
    cc += m_base;
    center += cc;
    if (cc[1] <= 0.) {
      floor2 = floor1;
      floor1 = cc;
      onFloor++;
    }
    if (cc[1] <= 0.1) {
      onFloorOrClose++;
    }
    
    cout << "Physical bone coords for " << i << ": ";
    cc.Print();
    if (cc[1] < lowest) {
      lowest = cc[1];
      diff = -cc[1];
    }
  }

  Coordinates axis;
  double angle = 0;

  double rotF = 0.1;
  if (onFloor == 1 || (onFloor == 2 && floor1 == floor2)) {
     Coordinates for_angle = (center - floor1).Einheitsvector()*m_gravity;
    SphereCoordinates ccc = for_angle.AsSphere();
    if (ccc.theta() > PI_P || ccc.theta() < -PI_P)
      ccc.Switch();

    m_rotImp = ccc;
    
    cout << "New floor for Rot Imp ";
    floor1.Print();
  }
  cout << "Rot Imp: ";
  m_rotImp.Print();

  if (onFloor > 2) {
    m_rotImp.phi() = 0.;
    m_rotImp.theta() = 0.;
    m_rotImp.r() = 0.;
  }
  
  if (onFloor == 2 && floor1 != floor2) {
     double c = sqrt((floor1[0]-floor2[0])*(floor1[0]-floor2[0])+
		    (floor1[0]-floor2[2])*(floor1[0]-floor2[2]));
    
    double a = sqrt((floor1[0]-center[0])*(floor1[0]-center[0])+
		    (floor1[0]-center[2])*(floor1[0]-center[2]));
    
    double b = sqrt((center[0]-floor2[0])*(center[0]-floor2[0])+
		    (center[0]-floor2[2])*(center[0]-floor2[2]));
    
    double s = 0.5*(a+b+c);
    double h = 2./c*sqrt(s*(s-a)*(s-b)*(s-c));
    double d = sqrt(b*b - h*h);

    if (a > b) {
      d = c - d;
    }

    Coordinates mid = floor1 + floor2*d/c;
    
    Coordinates for_angle = (center - mid).Einheitsvector()*m_gravity;
    SphereCoordinates ccc = for_angle.AsSphere();
    if (ccc.theta() > PI_P || ccc.theta() < -PI_P)
      ccc.Switch();

    //m_rotImp = ccc;
   
  }
  if (onFloorOrClose >= 3) {
    m_rotImp.theta() = 0;
    m_rotImp.phi() = 0;
    m_rotImp.r() = 0;
  }
  
  //AddToBoneRot(0, NPCBoneCoords(0, m_rotImp.phi()*deltatime, m_rotImp.theta()*deltatime, 0));
  AddToBoneRot(0, NPCBoneCoords(0, 0, 0, 0));
  RotateAll(axis, angle);
  
  cout << "LOWEST: " << lowest << " Base: " << m_base[1] << " Abs: " << m_absPos[1] << " Imp: " << m_imp[1] <<  endl;
  if (lowest > 0) 
    m_imp[1] -= m_gravity*deltatime*10;
  
  m_absPos += m_imp*deltatime;
  m_base += m_imp*deltatime;
  double damp = 14.;
  if (lowest < 0) {
    //m_absPos[1] = -0.001;
    m_absPos[1] -= lowest/damp;
    m_base[1] -= lowest/damp;
    m_imp[1] = 0.;
    if (m_absPos[1] > m_lastAbsPos[1])
      m_imp = (m_absPos - m_lastAbsPos)/deltatime;
    
  }
  
  
  m_lastAbsPos = m_absPos;
  
  /*
  if (m_x.isize() == 0) {
    SyncTo(deltatime);
  }


  
  for (i=0; i<m_x.isize(); i++) {
    cout << "Phys coords for " << i << ": ";
    m_x[i].Print();
    m_v[i].Print();

    Coordinates before = m_x[i];
 
    Coordinates cc;
    bool b = m_coll.Constraint(m_x[i]);
    if (b) {
      Coordinates diff = m_v[i]*deltatime;
      //m_x[i] = before;
      
      AddToBase(diff*-1);
      m_v[i] = Coordinates(0, 0, 0);
      cout << "Stuck" << endl;
    } else {
      (m_v[i])[1] -= deltatime*m_gravity;
      Coordinates diff = m_v[i]*deltatime;
      m_x[i] += diff;   
      if (i == 0) {
	AddToBase(diff);
	cout << "Add to base " << endl;
	
      }
      cout << "Moving ";
      m_v[i].Print();
      m_x[i].Print();
    }
    }*/
}

void NPCSkeleton::SyncTo(double deltatime)
{
  int i;
  bool bV = true;
  if (m_x.isize() == 0) {
    m_x.resize(m_bones.isize());
    m_v.resize(m_bones.isize());    
    bV = false;
  }

  for (i=0; i<m_bones.isize(); i++) {
    Coordinates c = m_bones[i].GetCoords();
    if (bV) {
      Coordinates v = c - m_x[i];
      m_v[i] += v * deltatime;
    }
    m_x[i] = c;
    /*if (!bV) {
      cout << "Setup " << i << " ";
      c.Print();
      }*/
  }
}

double NPCSkeleton::PhysDist()
{
  int i;
  double dist = 0.;

  for (i=0; i<m_bones.isize(); i++) {
    Coordinates c = m_bones[i].GetCoords();
    dist += (c - m_x[i]).Length();
  }
  return dist;
}

void NPCSkeleton::MoveBones()
{
  int i;

  // Lateral movement
  Coordinates lat = m_x[0] - m_bones[0].Root();
  cout << "Moving center from" << endl;
  m_bones[0].Root().Print();
  AddToBase(lat);
  cout << "to";
  m_bones[0].Root().Print();
  
  cout << "Moving bones." << endl;
  for (i=0; i<m_bones.isize(); i++) {
    //cout << "Try bone " << i << endl;
    MoveOneBone(i);
  }
  
}

void NPCSkeleton::MoveOneBone(int i)
{
  throw;
}
