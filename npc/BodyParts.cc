#include "npc/BodyParts.h"


void BodyBuilder::GetHand(NPCSkeleton & primitive, bool bMirror)
{
  double scale = 1.;
  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); // Root
  primitive.AddBone(aa);

  // Finger
  aa.SetRelCoords(NPCBoneCoords(scale*0.6, 0, 0.3*1.2, 0.2)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.2, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.2, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.18, 0, 0, 0)); 
  primitive.AddBone(aa);

  // Finger
  aa.SetRelCoords(NPCBoneCoords(scale*0.6, 0, 0.1*1.2, 0.24)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.22, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.22, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.20, 0, 0, 0)); 
  primitive.AddBone(aa);

   // Finger
  aa.SetRelCoords(NPCBoneCoords(scale*0.6, 0, -0.1*1.2, 0.22)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.2, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.2, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.17, 0, 0, 0)); 
  primitive.AddBone(aa);

  // Finger
  aa.SetRelCoords(NPCBoneCoords(scale*0.6, 0, -0.3*1.2, 0.18)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.18, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.18, 0, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.16, 0, 0, 0)); 
  primitive.AddBone(aa);

  // Thumb
  aa.SetRelCoords(NPCBoneCoords(scale*0.7, 0., -0.8, -0.12)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.18, -1.3, 0, 0)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(scale*0.18, -1.3, 0, 0)); 
  primitive.AddBone(aa);


  primitive.SetParentChild(0, 1);
  primitive.SetParentChild(1, 2);
  primitive.SetParentChild(2, 3);
  primitive.SetParentChild(3, 4);

  primitive.SetParentChild(0, 5);
  primitive.SetParentChild(5, 6);
  primitive.SetParentChild(6, 7);
  primitive.SetParentChild(7, 8);

  primitive.SetParentChild(0, 9);
  primitive.SetParentChild(9, 10);
  primitive.SetParentChild(10, 11);
  primitive.SetParentChild(11, 12);

  primitive.SetParentChild(0, 13);
  primitive.SetParentChild(13, 14);
  primitive.SetParentChild(14, 15);
  primitive.SetParentChild(15, 16);

  primitive.SetParentChild(0, 17);
  primitive.SetParentChild(17, 18);
  primitive.SetParentChild(18, 19);

  if (bMirror) {
    primitive.Mirror();
    //primitive.AddToBoneRot(0, NPCBoneCoords(0, 0, PI_P/2, PI_P/2)); // Root

  }
}

void BodyBuilder::GetFoot(NPCSkeleton & primitive, bool bMirror)
{

  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); // Root
  primitive.AddBone(aa);

  // Toe 1-2
  aa.SetRelCoords(NPCBoneCoords(0.6, 0, 0.3*1.2, 0.2)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.2, 0, 0, 0)); 
  primitive.AddBone(aa);
 
  // Toe 3-4
  aa.SetRelCoords(NPCBoneCoords(0.6, 0, 0.1*1.2, 0.24)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.22, 0, 0, 0)); 
  primitive.AddBone(aa);

   // Toe 5-6
  aa.SetRelCoords(NPCBoneCoords(0.6, 0, -0.1*1.2, 0.22)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.2, 0, 0, 0)); 
  primitive.AddBone(aa);

  // Toe 7-8
  aa.SetRelCoords(NPCBoneCoords(0.6, 0, -0.3*1.2, 0.18)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.18, 0, 0, 0)); 
  primitive.AddBone(aa);

  // Toe 9-10
  aa.SetRelCoords(NPCBoneCoords(0.7, 0., -0.4*1.2, -0.12)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.18, -1.3, 0, 0)); 
  primitive.AddBone(aa);
 

  primitive.SetParentChild(0, 1);
  primitive.SetParentChild(1, 2);

  primitive.SetParentChild(0, 3);
  primitive.SetParentChild(3, 4);

  primitive.SetParentChild(0, 5);
  primitive.SetParentChild(5, 6);

  primitive.SetParentChild(0, 7);
  primitive.SetParentChild(7, 8);

  primitive.SetParentChild(0, 9);
  primitive.SetParentChild(9, 10);

  if (bMirror) {
    primitive.Mirror();
  }
}


void BodyBuilder::GetBody(NPCSkeleton & primitive)
{

  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); // Root/butt
  primitive.AddBone(aa);

  // Torso 1-2
  aa.SetRelCoords(NPCBoneCoords(1.7, 0, 0., PI_P/2)); 
  //aa.SetLimits(NPCBoneCoords(0., -0., -0., -0.),
  //	       NPCBoneCoords(0., 0., 0., 0.));
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1.2, 0, 0, PI_P/2)); 
  aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  	       NPCBoneCoords(0., .1, .1, .1));
 
  primitive.AddBone(aa);
 

  // Shoulder 3-4
  aa.SetRelCoords(NPCBoneCoords(.8, 0, 0., 0)); 
  aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);
 
  aa.SetRelCoords(NPCBoneCoords(.8, 0, 0, PI_P)); 
  aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);

  aa.SetHaveLimit(false);
  
  // Left arm 5-6
  aa.SetRelCoords(NPCBoneCoords(1.5, 0, 0., -PI_P/2+0.6)); 
  aa.SetLimits(NPCBoneCoords(0., -PI_P/2, -PI_P/2, 3*PI_P/2),
  	       NPCBoneCoords(0., PI_P/2, PI_P/2, PI_P/2));
  primitive.AddBone(aa);

  aa.SetHaveLimit(false);
  aa.SetRelCoords(NPCBoneCoords(1.5, 0, 0, -PI_P/2)); 
  primitive.AddBone(aa);

  // Right arm 7-8
  aa.SetRelCoords(NPCBoneCoords(1.5, 0, 0., -PI_P/2-0.6)); 
  aa.SetLimits(NPCBoneCoords(0., -PI_P/2, -PI_P/2, 3*PI_P/2),
  	       NPCBoneCoords(0., PI_P/2, PI_P/2, PI_P/2));
  primitive.AddBone(aa);
  aa.SetHaveLimit(false);
  aa.SetRelCoords(NPCBoneCoords(1.5, 0, 0, -PI_P/2)); 
  primitive.AddBone(aa);

  aa.SetHaveLimit(false);

  // Pelvis 9-10
  aa.SetRelCoords(NPCBoneCoords(0.7, 0, 0., 0)); 
  aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.7, 0, 0, PI_P)); 
  aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);
  aa.SetHaveLimit(false);
 
  // Left leg 11-12
  aa.SetRelCoords(NPCBoneCoords(2., 0, 0., -PI_P/2+0.1)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1.9, 0, 0, -PI_P/2)); 
  primitive.AddBone(aa);

  // Right leg 13-14
  aa.SetRelCoords(NPCBoneCoords(2., 0, 0., -PI_P/2-0.1)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1.9, 0, 0, -PI_P/2)); 
  primitive.AddBone(aa);
  

  primitive.SetParentChild(0, 1);
  primitive.SetParentChild(1, 2);

  primitive.SetParentChild(2, 3);
  primitive.SetParentChild(2, 4);

  
  primitive.SetParentChild(3, 5);
  primitive.SetParentChild(5, 6);

  primitive.SetParentChild(4, 7);
  primitive.SetParentChild(7, 8);

  
  primitive.SetParentChild(0, 9);
  primitive.SetParentChild(0, 10);

  primitive.SetParentChild(9, 11);
  primitive.SetParentChild(11, 12);

  primitive.SetParentChild(10, 13);
  primitive.SetParentChild(13, 14);
  

}
