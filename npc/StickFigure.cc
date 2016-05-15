#include "npc/StickFigure.h"
#include "physics/Physics.h"

//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//=======================================================================
void SetupSkeleton(NPCSkeletonWithPhysics & s, StrichManipulator & manip)
{
  int i;
  NPCBone root;
  root.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); // Root/butt
  root.Root() = Coordinates(0, 2, 0);
  s.AddBone(root);

  
  NPCBone aa;

  // Legs 1,2 3,4
  aa.SetRelCoords(NPCBoneCoords(1., 0.3, 0.2, -PI_P/2+0.3)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1., 0., -0.2, -PI_P/2)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(1., 0.3, -0.2+PI_P, -PI_P/2+0.3)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1., 0., -0.2, -PI_P/2)); 
  s.AddBone(aa);

  // Spine 5
  aa.SetRelCoords(NPCBoneCoords(1., 0., 0, PI_P/2)); 
  s.AddBone(aa);

  
  // Arms 6,7 8,9
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., 0, -0.4)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P/2, 0.)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P, -0.4)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P/2, 0.)); 
  s.AddBone(aa);
  
  // Forward
  aa.SetRelCoords(NPCBoneCoords(0.3, 0., PI_P/2, 0.)); 
  s.AddBone(aa);

  
  

  s.SetParentChild(0, 1);
  s.SetParentChild(1, 2);
  s.SetParentChild(0, 3);
  s.SetParentChild(3, 4);
  s.SetParentChild(0, 5);

  s.SetParentChild(5, 6);
  s.SetParentChild(5, 8);
  s.SetParentChild(6, 7);
  s.SetParentChild(8, 9);
  
  s.SetParentChild(5, 10);
  //s.SetParentChild(0, 10);

 
  s.Scale(10.);
  s.SetAbsOffset(Coordinates(0., 60., 0));
  s.SetFloorY(0.);

  
  s.SetupPhysics();

  PhysObject & phys = s.Physics();

  phys.Connect(PhysConnection(0, 10, 0.01));
  //phys.Connect(PhysConnection(5, 10, 0.01));
  phys.Fixate();

  cout << "After fixate, center is ";
  phys.GetCenter().GetPosition().Print();
  cout << "Print all coordinates:" << endl;
  for (i=0; i<phys.isize(); i++)
    phys[i].GetPosition().Print();

  

  PhysAttractor at;
  at.SetIndex(1);
  at.Position() = phys[1].GetPosition();
  at.Position()[2] -= 2.0;
  at.AddAttach(0);
  at.AddAttach(5);
  at.AddAttach(10);
  at.SetActive(false);
  at.SetForce(25.);
  at.SetBack(25.);
  phys.AddAttractor(at);

  at.Position() = phys[2].GetPosition();
  at.Position()[2] -= 2.5;
  at.SetIndex(2);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[1].GetPosition();
  at.Position()[2] += 2.8;
  //at.Position()[1] += 1.8;
  at.SetIndex(1);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[2].GetPosition();
  at.Position()[2] += 4.8;
  at.Position()[1] += 2.8;
  at.SetIndex(2);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  //---------------------------------------------------

  at.SetIndex(3);
  at.Position() = phys[3].GetPosition();
  at.Position()[2] -= 2.0;
  at.AddAttach(0);
  at.AddAttach(5);
  at.AddAttach(10);
  at.SetActive(false);
  at.SetForce(25.);
  // at.SetBack(0.);
  phys.AddAttractor(at);

  at.Position() = phys[4].GetPosition();
  at.Position()[2] -= 2.5;
  at.SetIndex(4);
  at.SetForce(30);
  at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[3].GetPosition();
  at.Position()[2] += 2.8;
  //at.Position()[1] += 1.8;
  at.SetIndex(3);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[4].GetPosition();
  at.Position()[2] += 4.8;
  at.Position()[1] += 2.8;
  at.SetIndex(4);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);


  //==================================================
  at.Position() = phys[0].GetPosition();
  at.SetIndex(0);
  at.SetForce(22);
  //at.SetBack(0.);
  at.SetActive(true);
  phys.AddAttractor(at);
  at.Position() = phys[5].GetPosition();
  at.SetIndex(5);
  at.SetForce(52);
  //at.SetBack(0.);
  at.SetActive(true);
  phys.AddAttractor(at);


  at.Position() = phys[5].GetPosition();
  at.Position()[1] += 2.4;
  at.SetIndex(9);
  at.SetForce(10);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);
 
  at.Position() = phys[5].GetPosition();
  at.Position()[2] += 22;
  at.Position()[0] -= 22;
  at.SetIndex(7);
  at.SetForce(20);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  for (i=0; i<phys.AttractorCount(); i++) {
    char name[256];
    sprintf(name, "attract%d", i);
    AttractInfo info;
    info.SetName(name);
    info.Coords() = phys.Attractor(i).GetPosition();
    info.SetPull(phys.Attractor(i).GetForce());
    info.SetIndex(i);
    manip.AddAttractor(info);
  }
}
