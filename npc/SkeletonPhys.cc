// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#include "npc/SkeletonPhys.h"
#include "physics/Plane.h"

void NPCSkeletonWithPhysics::SetupPhysics() 
{
  int i, j;


  // Set up physics object
  m_physObj.SetPhysMode(0);
  m_physObj.SetElast(true);
  //m_physObj.SetGravity(9.81);
  for (i=0; i<m_bones.isize(); i++) {
    PhysMinimal min;
    min.SetMass(1.);
    min.SetPosition(m_bones[i].GetCoords());
    m_physObj.Add(min);
    cout << "Added bone # " << i << " -> ";
    min.GetPosition().Print();
  }

  for (i=0; i<m_bones.isize(); i++) {
    for (j=0; j<m_bones[i].GetChildCount(); j++) {
      int index = m_bones[i].GetChild(j);
      m_physObj.Connect(PhysConnection(i, index));
    }
  }
  m_physObj.Fixate();


  for (i=0; i<m_bones.isize(); i++) {
    m_bones[i].SetOverride(m_physObj[i].GetPosition());
  }
  NPCBoneCoords nothing;
  AddToBoneRot(0, nothing);
 
  
  const Coordinates & center = m_physObj.GetCenter().GetPosition();
  const Coordinates & first = m_physObj[0].GetPosition()+m_physObj.GetCenter().GetPosition();

 
}
void NPCSkeletonWithPhysics::UpdateFromPhys(double deltatime)
{
  int i;


  cout << "UpdatePhys ++++++++++++++ " << m_bones.isize() << endl;
  for (i=0; i<m_bones.isize(); i++) {
    const Coordinates & p = m_physObj[i].GetPosition(); 
    Coordinates check = m_bones[i].GetCoords();
    
    //m_bones[i].SetOverride(p);
    m_bones[i].SetCoords(p);
 
    NPCBoneCoords nothing;
    AddToBoneRot(i, nothing);
  }
}

void NPCSkeletonWithPhysics::UpdatePhys(double deltatime)
{
  UpdateFromPhys(deltatime);
  UpdateToPhys(deltatime, false);
}

void NPCSkeletonWithPhysics::UpdateToPhys(double deltatime, bool bVelocity)
{
  return;

 
  int i;
  if (!bVelocity)
    return;
  Coordinates orig = m_physObj.GetCenter().GetPosition();

  Coordinates diff = m_bones[0].Root() - m_physObj[0].GetPosition() - m_physObj.GetCenter().GetPosition();

  bVelocity = false;


  
  for (i=0; i<m_bones.isize(); i++) {
    PhysMinimal & min = m_physObj[i];   
    if (bVelocity) {
      Coordinates diff = m_bones[i].GetCoords()-m_physObj.GetCenter().GetPosition() - min.GetPosition();
      diff /= deltatime;
      //diff /= 100.;
      min.Velocity() += diff;
      //cout << "Add velocity: ";
      //diff.Print();

      int pp = m_bones[i].GetParent();
      if (pp >= 0) {
	m_physObj[pp].Velocity() -= diff;
      }
    }
    min.SetPosition(m_bones[i].GetCoords()-m_physObj.GetCenter().GetPosition());   
  }

  m_physObj.Fixate();
  

  //Coordinates relMove = m_physObj.GetCenter().GetPosition() - orig;
  Coordinates relMove = orig - m_physObj.GetCenter().GetPosition();
  m_bones[0].Root() += relMove;
  NPCBoneCoords nothing;
  AddToBoneRot(0, nothing);


  
  for (i=0; i<m_bones.isize(); i++) {
    PhysMinimal & min = m_physObj[i];   
    min.SetPosition(m_bones[i].GetCoords()-m_physObj.GetCenter().GetPosition());   
  }

}

void NPCSkeletonWithPhysics::ForceDiffOne(NPCBone & bone, int dim, double val)
{
  throw;
}
void NPCSkeletonWithPhysics::ForceDiffCoords(NPCBone & bone, const Coordinates & target)
{
  throw;
}

void NPCSkeletonWithPhysics::UpdateAndSync(double deltatime) 
{

  int i, j;

  cout << "CENTER VELOCITY: ";
  m_physObj.GetCenter().GetVelocity().Print();
  m_physObj.Update(deltatime, 10.);
  //m_physObj.Update(deltatime, 0.);
 
  cout << "CENTER VELOCITY (Phys update): ";
  m_physObj.GetCenter().GetVelocity().Print();

  
  FlatPlane plane;
  plane.SetCoordinates(Coordinates(0, -20, 0));
  svec<Coordinates> touch;
  plane.Collide(m_physObj, deltatime, touch);
  cout << "CENTER VELOCITY (AFTER): ";
  m_physObj.GetCenter().GetVelocity().Print();
  
  for (i=0; i<m_bones.isize(); i++) {
    m_bones[i].Touch() = touch[i];
  }




  UpdatePhys(deltatime);
  return;


  deltatime /= 10.;
  
  double damp = 1. - 0.09*deltatime;

  //Coordinates the_root;
  m_rootSpeed[1] -= deltatime*m_gravity;
  AddToBase(m_rootSpeed);
  //cout << "ROOT: ";
  //m_bones[0].Root().Print();

  for (i=0; i<m_physics.isize(); i++) {
    NPCBoneCoords both = m_bones[i].Rel();
    Coordinates root = m_bones[i].Root();
    Coordinates tip = m_bones[i].GetCoords();
    Coordinates rel = (tip - root).Einheitsvector();
    Coordinates reltmp = Coordinates(rel[0], rel[2], rel[1]);
    SphereCoordinates s = reltmp.AsSphere();

 
    if (m_bones[i].Rel().Radius() > 0.) {
      Coordinates testX = Coordinates(1., 0., 0.);
      Coordinates testZ = Coordinates(0., 0., 1.);
      
      double xplus = -sin(s.phi())*sin(s.theta());
      double zplus = cos(s.phi())*sin(s.theta());

 
      
      cout << "Bone " << i << ": " << xplus << " " << zplus << " phi=" << s.phi() << " theta=" << s.theta() << endl;
      
      //zplus = 0;

      xplus *= m_gravity*deltatime*m_physics[i].GetMass()/m_bones[i].Rel().Radius();
      zplus *= m_gravity*deltatime*m_physics[i].GetMass()/m_bones[i].Rel().Radius();
      cout << "zplus raw: " << zplus << endl;
 
      // Check for ground...
      if (tip[1] + m_absoffset[1] <= 0.) {
	// BUG: Do something here!!!
	ForceDiffOne(m_bones[i], 1, -m_absoffset[1]);

      	m_physics[i].RotImp() *= 0.1;
	cout << "  rot imp ";
	m_physics[i].RotImp().Print();
      } else {
	//m_physics[i].RotImp().RX() -= xplus;
	//m_physics[i].RotImp().RZ() -= zplus;
      	//m_physics[i].RotImp() *= damp;
      }

      cout << "phi=" << s.phi() << " cos(phi)=" << cos(s.phi()) << " theta=" << s.theta() << " sin(theta)=" << sin(s.theta()) << endl;
      both.Print();
      (tip - root).Print();
      cout << "Add impulse, x: " << xplus << " z: " << zplus << ", total: ";
      m_physics[i].Print();
    } else {
      if (root[1] + m_absoffset[1] < 0.) {
	root[1] = -m_absoffset[1];
	m_rootSpeed[1] = 0.;
      }
    }

  }
  
  
  for (i=0; i<m_physics.isize(); i++) {
    for (j=0; j<m_bones[i].GetChildCount(); j++) {
      int index = m_bones[i].GetChild(j);
      double w = m_physics[i].GetMass()/m_physics[index].GetMass()/m_physics[i].ChildMass()/*/m_bones[index].Rel().Radius()*/;
      //w = 1.;
      w *= deltatime;
      cout << "w: " << w << endl;
      m_physics[index].RotImp() -= m_physics[i].RotImp() * w;
      m_physics[i].RotImp() *= (1.-w);
    }
    int parent = m_bones[i].GetParent();
    if (parent > 0) {
      double w2 = m_physics[i].GetMass()/m_physics[parent].GetMass()/*/m_bones[i].Rel().Radius()*/;
      //w2 = 1.;
      w2 *= deltatime;
      m_physics[parent].RotImp() -= m_physics[i].RotImp() * w2;
      m_physics[parent].RotImp() *= (1.-w2);
    }
  }

  for (i=0; i<m_physics.isize(); i++) {
    m_bones[i].Rel() += m_physics[i].RotImp();
    NPCBoneCoords nothing;
    AddToBoneRot(i, nothing);

  }
  
}
