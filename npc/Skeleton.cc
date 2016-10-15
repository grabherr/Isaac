#include "npc/Skeleton.h"
#include "base/FileParser.h"

void NPCBone::UpdateChildren(NPCSkeleton & s, const NPCBoneCoords & delta)
{
  int i;

  Coordinates me = GetCoords();

  for (i=0; i<m_children.isize(); i++) {
    //cout << "Updating children" << endl;
    NPCBone & n = s[m_children[i]];
    n.AddToAbsCoords(delta);
    n.SetBaseCoords(me);
    n.UpdateChildren(s, delta);
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
    fprintf(p, "%d %f %f %f\n", i, b.RX()-base.RX(),
	    b.RY()-base.RY(), b.RZ()-base.RZ());
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
  for (i=0; i<m_bones.isize(); i++) {
    Coordinates cc = m_bones[i].GetCoords();
    cc += m_base;
    cout << "Physical bone coords for " << i << ": ";
    cc.Print();
    if (cc[1] < lowest) {
      lowest = cc[1];
      diff = -cc[1];
    }
  }
  
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
  NPCBone & bone = m_bones[i];
  NPCBoneCoords test;
  
  NPCBone & deriv = m_bones[i];
 

  //Coordinates tip = deriv.GetCoords();
  double delta = 0.001;
  
   //cout << "Val: " << val << " Tip: ";
   //tip.Print();
   
   double dist = PhysDist();
   //cout << "Initial distance: " << dist << endl;
   
   
   deriv.Rel().RX() += delta; 
   double dist_derivX = dist - PhysDist();
   deriv.Rel().RX() -= delta;
   
   deriv.Rel().RY() += delta;  
   double dist_derivY = dist - PhysDist();
   deriv.Rel().RY() -= delta;
   
   deriv.Rel().RZ() += delta; 
   double dist_derivZ = dist - PhysDist();
   deriv.Rel().RZ() -= delta;
  
   double lastdist = dist;
  
   double scale = 1.;
   int counter = 0;
   while (true) {
     deriv.Rel().RX() += dist_derivX;
     deriv.Rel().RY() += dist_derivY;
     deriv.Rel().RZ() += dist_derivZ;
     
     dist = PhysDist();
     
     //cout << "Testing: " << dist << " " << lastdist << endl;
     if (dist > lastdist) {
       deriv.Rel().RX() -= dist_derivX;
       deriv.Rel().RY() -= dist_derivY;
       deriv.Rel().RZ() -= dist_derivZ;
       break;
     }
     if (dist < 0.001)
       break;
     lastdist = dist;
     
     // WARNING: This is STUPID!!!
     if (counter > 20)
       break;
     counter++;
   }
   bone = deriv;


   NPCBoneCoords nothing;
   AddToBoneRot(i, nothing);
}
