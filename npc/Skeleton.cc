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

void NPCBone::SetCoords(const Coordinates & c)
{
  Coordinates a = c - m_root;
  Coordinates b = m_real - m_root;

  m_tipdelta = a - b;
  
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
      
      newabs = NPCRotatePhi(relc, dirc);
      newabs = NPCRotateS2(newabs, dirc);
    }

    if (m_last.r() > 0. && (dirc.theta() == 0 || dirc.theta() == -PI_P)) {
      m_last.theta() = dirc.theta();
    } else {
      m_last = dirc;
    }
    
    n.GetCoords() = newabs + tip;    
    n.SetBaseCoords(tip);

     
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
  
    for (i=0; i<m_index.isize(); i++) {   
      s.AddToBoneRot(m_index[i], m_relMove[i]*speed);
    }
  }
  return b;
}


void NPCSkeleton::MakeFeatureVector(svec<double> & features) const
{
  features.clear();

  int i;

  double weight_coords = 0.2;
  double weight_sphere = 0.03;
  double weight_move = 1.;
  
  
  for (i=0; i<isize(); i++) {
    const NPCBone & b = (*this)[i];
    Coordinates c = b.GetCoordsPlusDelta() * weight_coords;
    //features.push_back(c[0]);
    //features.push_back(c[1]);
    //features.push_back(c[2]);
    const SphereCoordinates & s = b.Rel().SCoords();
    //features.push_back(s.phi() * weight_sphere);
    //features.push_back(s.theta() * weight_sphere);    
  }
  for (i=0; i<m_nerves.isize(); i++) {
    features.push_back(m_nerves[i].CurrMove() * weight_move);
  }
}


void NPCSkeleton::Read(const string & fileName)
{
  FlatFileParser parser;


  int i;
  parser.Open(fileName);
  parser.ParseLine();
  int n = parser.AsInt(0);
  for (i=0; i<n; i++) {
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
  
void NPCSkeleton::RotateAll(const Coordinates & axis_raw, double angle)
{
  int i;
 
 
  if (axis_raw.Length() > 0 && angle != 0) {
    Coordinates axis = axis_raw.Einheitsvector();
    SphereCoordinates a = axis.AsSphere();
    m_xrot += angle*cos(a.phi()); 
    m_zrot += angle*sin(a.phi()); 
    m_angle += angle;
  }
  
  for (i=0; i<isize(); i++) {
    NPCBone & n = (*this)[i];
    Coordinates tip = n.GetCoords();
    Coordinates base = n.GetBaseCoords();

    base = NPCRotateByAxis(base, Coordinates(1, 0, 0), m_xrot);
    base = NPCRotateByAxis(base, Coordinates(0, 0, 1), m_zrot);
    n.SetBaseDelta(base);

    tip = NPCRotateByAxis(tip, Coordinates(1, 0, 0), m_xrot);
    tip = NPCRotateByAxis(tip, Coordinates(0, 0, 1), m_zrot);
   
    
    n.SetTipDelta(tip);
       
  }
}

//==============================================
void NPCSkeleton::Update(double deltatime)
{
  //return;
  
  int i;
     
  double lowest = 10000;
  double diff = 10000;
  Coordinates center;
  Coordinates floor1, floor2;
  int onFloor = 0;
  int onFloorOrClose = 0;
  Coordinates avgOnFloor;
  Coordinates floorMove;
  Coordinates floorMoveSpeed;
  int moveCount = 0;
  
  for (i=0; i<m_bones.isize(); i++) {
    Coordinates cc = m_bones[i].GetCoordsPlusDelta();
    cc[1] += m_base[1];
    center += cc;
    if (cc[1] <= 0.) {
      floor2 = floor1;
      floor1 = cc;
      onFloor++;
      avgOnFloor += cc;
      if ( m_bones[i].Floor()) {
	floorMove += cc - m_bones[i].GetLastCoords();
	moveCount++;
      } else {
	floorMoveSpeed += (cc - m_bones[i].GetLastCoords())*deltatime;
      }
      m_bones[i].SetFloor(true);
    } else {
      m_bones[i].SetFloor(false);
    }
    if (cc[1] <= 0.1) {
      onFloorOrClose++;
    }
    m_bones[i].GetLastCoords() = cc;
    if (cc[1] < lowest) {
      lowest = cc[1];
      diff = -cc[1];
    }
  }
  center /= (double)m_bones.isize();

  avgOnFloor /= (double)m_bones.isize();

  if (onFloor > 0)
    floorMoveSpeed /= onFloor;
  if (moveCount > 0)
    floorMove /= moveCount;
  
  //Coordinates axis;
  double angle = 0;

  if (onFloor != m_onFloor)
    m_rotSpeed = 0.;
  m_onFloor = onFloor;

  double rotF = 0.1;
  bool bDo = false;
  if (onFloor == 1 || (onFloor == 2 && floor1 == floor2)) {
    Coordinates for_angle = floor1 - center;
    Coordinates for_axis = for_angle;
    for_axis[1] = 0.;
    SphereCoordinates ccc = for_angle.AsSphere();
    if (ccc.theta() > PI_P || ccc.theta() < -PI_P)
      ccc.Switch();


    
    double ddd = m_gravity*sin(ccc.theta())/2/PI_P;
    m_axis = for_angle.CrossProduct(for_axis);
 
    //cout << "AXIS to cent " << ddd << ": ";
    //m_axis.Print();
    //cout << "AXIS for rot " << ddd << ": ";
    //m_axis.Print();
    //cout << "AXIS for base " << ddd << ": ";
    //m_base.Print();

    m_rotSpeed += 20*ddd*deltatime;
    
    floor1.Print();
  }

  if (onFloorOrClose > 2) {
    m_rotImp.phi() = 0.;
    m_rotImp.theta() = 0.;
    m_rotImp.r() = 0.;
    m_imp = Coordinates(0, 0, 0);
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
    
    Coordinates for_angle = (center - mid);

    Coordinates for_axis = for_angle;
    for_axis[1] = 0.;
    SphereCoordinates ccc = for_angle.AsSphere();
    if (ccc.theta() > PI_P || ccc.theta() < -PI_P)
      ccc.Switch();

 
    double ddd = m_gravity*sin(ccc.theta())/2/PI_P;
    m_axis = for_angle.CrossProduct(for_axis); 
 
    m_rotSpeed += 20*ddd*deltatime;
   
  }
  if (onFloorOrClose >= 3) {
    m_rotImp.theta() = 0;
    m_rotImp.phi() = 0;
    m_rotImp.r() = 0;
    m_rotSpeed = 0;
    m_imp = Coordinates(0, 0, 0);
  }
  
  AddToBoneRot(0, NPCBoneCoords(0, 0, 0, 0));


  
  RotateAll(m_axis.Einheitsvector(), m_rotSpeed*deltatime);


  if (lowest > 0) 
    m_imp[1] -= m_gravity*deltatime*20;
  
  m_absPos += m_imp*deltatime;
  m_base += m_imp*deltatime;
  double damp = 5.;
  if (lowest < 0) {
    m_absPos[1] -= lowest/damp;
    m_base[1] -= lowest/damp;
    m_imp[1] = 0.;
    if (m_absPos[1] > m_lastAbsPos[1]) {
      m_imp = (m_absPos - m_lastAbsPos)/deltatime;
    }

  }

  if (moveCount > 0) {
    m_absPos[0] += floorMove[0];
    m_absPos[2] += floorMove[2];
    m_imp[0] = 0.;
    m_imp[2] = 0.;
  } else {
    m_imp[0] = floorMoveSpeed[0] / deltatime;
    m_imp[2] = floorMoveSpeed[2] / deltatime;
  }
 
  
  m_lastAbsPos = m_absPos;
  
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
  AddToBase(lat);
  for (i=0; i<m_bones.isize(); i++) {
    MoveOneBone(i);
  }
  
}

void NPCSkeleton::MoveOneBone(int i)
{
  throw;
}
