#include "npc/SkeletonPhys.h"

void NPCSkeletonWithPhysics::SetupPhysics() 
{
  int i, j;
  m_phys.SetPhysMode(0);
  for (i=0; i<m_bones.isize(); i++) {
    PhysMinimal m;
    m.SetMass(1.);
    Coordinates cc = m_bones[i].GetCoords();
    m.SetPosition(cc);
    //cout "Setup ";
    //cc.Print();
    m_phys.Add(m);
  }
  for (i=0; i<m_bones.isize(); i++) {
    for (j=0; j<m_bones[i].GetChildCount(); j++) {
      m_phys.Connect(PhysConnection(i, m_bones[i].GetChild(j)));
    }
  }
  m_phys.Fixate();
  const PhysMinimal & mr = m_phys[0];
  const Coordinates & ccr = mr.GetPosition();
  const Coordinates & ccb = m_bones[0].Root();  
  m_offset = ccr;
  AddToBase(ccr - ccb);
}

void NPCSkeletonWithPhysics::UpdateAndSync(double deltatime) 
{
  int i, j;
  m_phys.Update(deltatime, m_gravity);
  const PhysMinimal & mr = m_phys[0];
  const Coordinates & ccr = mr.GetPosition();
  const Coordinates & ccb = m_bones[0].Root();  

  cout << "MERGE W/ PHYSICS" << endl;
   cout << "Base orig: ";
  ccb.Print();
  cout << "Base phys: ";
  ccr.Print();
  // Update the base
  //AddToBase(ccr-ccb);

  for (i=0; i<m_bones.isize(); i++) {
    const PhysMinimal & m = m_phys[i];
    const Coordinates & cc = m.GetPosition();

    int parent = m_bones[i].GetParent();
    if (parent < 0)
      continue;

    Coordinates base = m_bones[i].Root();
    Coordinates tip  = m_bones[i].GetCoords();
    
    Coordinates ist = tip - base;
    Coordinates soll = cc - base;

    double d = m_bones[i].Rel().Radius();
    double rz = acos(soll[0]/d);
    double ry = acos(soll[1]/d);
    double rx = acos(soll[2]/d);
 
    NPCBoneCoords cc_bones = m_bones[i].Both();
    NPCBoneCoords cc_phys; 
    cc_phys.RX() = rx;
    cc_phys.RY() = ry;
    cc_phys.RZ() = rz;
    cc_phys.Radius() = d;
    
    NPCBoneCoords rel = cc_phys - cc_bones;
    
    cout << "BONE " << i << endl;
    cout << "Skeleton: ";
    cc_bones.Print();
    cout << "Physics:  ";
    cc_phys.Print();
    cout << "Diff:     ";
    rel.Print();

    // Update the base???

    //AddToBoneRot(i, rel);
    
   
  }

  // Feed back positions to physics
  /*
  for (i=0; i<m_bones.isize(); i++) {
    PhysMinimal & m = m_phys[i];
    Coordinates cc = m_bones[i].GetCoords();
    m.SetPosition(cc);         
    }*/
  
}
