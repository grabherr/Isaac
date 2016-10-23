#include "npc/Skeleton.h"
#include "base/FileParser.h"

Coordinates NPCRotate(const Coordinates & a, const Coordinates & s) {
  Coordinates out;
  SphereCoordinates mySphere = a.AsSphere();
  SphereCoordinates sSphere = s.AsSphere();
  
  
  double dphi = mySphere.phi() + sSphere.phi();
  double dtheta = mySphere.theta() + sSphere.theta();

  
  
  //if (dtheta < 0)
  //dphi -= PI_P;
  
  //cout << "phi diff " << dphi << endl;
  //cout << "theta diff " << dtheta << endl;
  mySphere.SetPhi(dphi);
  mySphere.SetTheta(dtheta);
  
  //cout << "sphere ";
  //mySphere.Print();
  out.FromSphere(mySphere);
  //out.Print();
  
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
    SphereCoordinates & abs = n.Rel().SCoords();
    //Coordinates localroot = n.GetBaseCoords;

    SphereCoordinates dirc;
    dirc = dir.AsSphere();
    //abs.phi() = dirc.theta();
      
    Coordinates relc;
    relc.FromSphere(rel);
    //relc[1] = 1.;

    
    Coordinates newabs = NPCRotate(relc, dir);
    //Coordinates newabsroot = NPCRotate(relc, dir);

    /*
    double scalar = newabs.Scalar(n.GetCoords()-n.GetBaseCoords());
    if (scalar < 0) {
      newabs[0] = -newabs[0];
      newabs[2] = -newabs[2];
      }*/
      
    
    //n.AddToAbsCoords(delta, spbase);
    n.GetCoords() = newabs + tip;    
    n.SetBaseCoords(tip);

    cout << "From SCoords (deri) ";
    n.GetCoords().Print();
    
    Coordinates delta;
    
    n.UpdateChildren(s, n.GetCoords(), n.GetBaseCoords());    
    //n.UpdateChildren(s, n.GetCoords(), root);    
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
  for (i=0; i<m_index.isize(); i++) {
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
  

//==============================================
void NPCSkeleton::Update(double deltatime)
{
  
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

  //if (onFloor == 0)
  double rotF = 0.1;
  if (onFloor == 1 || (onFloor == 2 && floor1 == floor2)) {
    double xd = floor1[0] - center[0];
    double zd = floor1[2] - center[2];
    double phi = PI_P/2;
    if (xd != 0.)
      phi = atan(zd/xd);
    double r = sqrt(xd*xd + zd*zd);
    m_rotImp[0] = r*cos(phi)*rotF;
    m_rotImp[2] = -r*sin(phi)*rotF;
    //m_lastRotImp = ??
    cout << "New floor for Rot Imp ";
    floor1.Print();
  }
  cout << "Rot Imp: ";
  m_rotImp.Print();


  if (onFloor == 2 && floor1 != floor2) {
    /*
    double xd = floor1[0] - center[0];
    double zd = floor1[2] - center[2];
    double phi = PI_P/2;
    if (xd != 0.)
      phi = atan(zd/xd);
    double r = sqrt(xd*xd + zd*zd);
    m_rotImp[0] = r*cos(phi);
    m_rotImp[2] = r*sin(phi);    */
    double c = sqrt((floor1[0]-floor2[0])*(floor1[0]-floor2[0])+
		    (floor1[0]-floor2[2])*(floor1[0]-floor2[2]));
    
    double a = sqrt((floor1[0]-center[0])*(floor1[0]-center[0])+
		    (floor1[0]-center[2])*(floor1[0]-center[2]));
    
    double b = sqrt((center[0]-floor2[0])*(center[0]-floor2[0])+
		    (center[0]-floor2[2])*(center[0]-floor2[2]));
    
    double s = 0.5*(a+b+c);
    double h = 2./c*sqrt(s*(s-a)*(s-b)*(s-c));
    
    double xd = floor1[0] - floor2[0];
    double zd = floor1[2] - floor2[2];
    double phi = PI_P/2;
    if (xd != 0.)
      phi = atan(zd/xd);
   
    //m_rotImp[0] = h*sin(phi)*rotF;
    //m_rotImp[2] = h*cos(phi)*rotF;
   
  }
  if (onFloorOrClose >= 3) {
    m_rotImp = Coordinates(0., 0., 0.);
  }
  
  AddToBoneRot(0, NPCBoneCoords(0, m_rotImp[0]*deltatime, 0, m_rotImp[2]*deltatime));
  
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
