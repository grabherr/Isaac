#include "npc/SkeletonPhys.h"

void NPCSkeletonWithPhysics::SetupPhysics() 
{
  int i, j;

  m_physics.resize(m_bones.isize());
  // Use the radius as a proxy for mass
  for (i=0; i<m_physics.isize(); i++) {
    double w = m_bones[i].Rel().Radius();
    if (w == 0)
      w = 1.;
    m_physics[i].SetMass(w);
    bool bYes = false;
    for (j=0; j<m_bones[i].GetChildCount(); j++) {
      int index = m_bones[i].GetChild(j);
      m_physics[i].AddToChildMass(m_bones[index].Rel().Radius());
      bYes = true;
    }
    if (bYes && m_physics[i].ChildMass() == 0)
      m_physics[i].AddToChildMass(1);
  }

}
void NPCSkeletonWithPhysics::ForceDiffOne(NPCBone & bone, int dim, double val)
{
  Coordinates tip = bone.GetCoords();
  NPCBone deriv = bone;
  double delta = 0.001;

  cout << "Val: " << val << " Tip: ";
  tip.Print();

  double dist = (tip[dim]-val)*(tip[dim]-val);

  deriv.Rel().RX() += delta;
  Coordinates tipDelta = deriv.GetCoords();
  double dist_derivX = dist - (tipDelta[dim]-val)*(tipDelta[dim]-val);

  deriv = bone;
  deriv.Rel().RY() += delta;
  tipDelta = deriv.GetCoords();
  double dist_derivY = dist - (tipDelta[dim]-val)*(tipDelta[dim]-val);

  deriv = bone;
  deriv.Rel().RZ() += delta;
  tipDelta = deriv.GetCoords();
  double dist_derivZ = dist - (tipDelta[dim]-val)*(tipDelta[dim]-val);

  double lastdist = dist;
  deriv = bone;

  double scale = 1.;
  int counter = 0;
  while (true) {
    deriv.Rel().RX() += dist_derivX;
    deriv.Rel().RY() += dist_derivY;
    deriv.Rel().RZ() += dist_derivZ;
    tipDelta = deriv.GetCoords();
    dist = (tipDelta[dim]-val)*(tipDelta[dim]-val);
    
    cout << "Testing: " << dist << endl;
    if (dist > lastdist)
      break;
    if (dist < 0.001)
      break;
    lastdist = dist;

    // WARNING: This is STUPID!!!
    if (counter > 20)
      break;
    counter++;
  }
  bone = deriv;
  
}

void NPCSkeletonWithPhysics::UpdateAndSync(double deltatime) 
{

  int i, j;
  deltatime /= 10.;
  
  double damp = 1. - 0.09*deltatime;

  //Coordinates the_root;
  m_rootSpeed[1] -= deltatime*m_gravity;
  AddToBase(m_rootSpeed);
  cout << "ROOT: ";
  m_bones[0].Root().Print();

  for (i=0; i<m_physics.isize(); i++) {
    NPCBoneCoords both = m_bones[i].Both();
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
      //double xplus = m_gravity*deltatime*m_physics[i].GetMass()*rel.Scalar(testX)/m_bones[i].Rel().Radius();
      //double zplus = m_gravity*deltatime*m_physics[i].GetMass()*rel.Scalar(testZ)/m_bones[i].Rel().Radius();

      // Check for ground...
      if (tip[1] + m_absoffset[1] <= 0.) {
	// BUG: Do something here!!!
	cout << "HIT FLOOR" << endl;
	cout << "  before: " << m_physics[i].RotImp().RX() << " " << m_physics[i].RotImp().RZ() << endl;	
	//m_physics[i].RotImp().RX() = -m_physics[i].RotImp().RX();
	//m_physics[i].RotImp().RZ() = -m_physics[i].RotImp().RZ();
	//m_physics[i].RotImp().RX() = -m_physics[i].RotImp().RX();
	//m_physics[i].RotImp().RZ() = -m_physics[i].RotImp().RZ();
	cout << "Call Force!" << endl;
	ForceDiffOne(m_bones[i], 1, -m_absoffset[1]);

	cout << "  after:  " << m_physics[i].RotImp().RX() << " " << m_physics[i].RotImp().RZ() << endl;      
      	m_physics[i].RotImp() *= 0.1;
	cout << "  rot imp ";
	m_physics[i].RotImp().Print();
      } else {
	m_physics[i].RotImp().RX() -= xplus;
	m_physics[i].RotImp().RZ() -= zplus;
      	m_physics[i].RotImp() *= damp;
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
