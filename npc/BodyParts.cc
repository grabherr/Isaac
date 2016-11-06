#include "npc/BodyParts.h"
#include "base/FileParser.h"

void BodyMap::Read(const string & fileName)
{
  FlatFileParser parser;

  m_name.clear();
  m_coords.clear();
  
  int i;
  parser.Open(fileName);
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    m_name.push_back(parser.AsString(0));
    m_coords.push_back(NPCBoneCoords(parser.AsFloat(1),
				     parser.AsFloat(2),
				     parser.AsFloat(3),
				     parser.AsFloat(4)));
  }
}

NPCBoneCoords BodyMap::Get(const string & name, const NPCBoneCoords & def)
{
  //return def;
  
  int i;
  NPCBoneCoords out = def;
  for (i=0; i<m_name.isize(); i++) {
    if (m_name[i] == name) {
      out += m_coords[i];
      if (m_coords[i].Radius() != 0)
	out.Radius() = m_coords[i].Radius();
      return out;
    }
  }
  return out;
}


////////////////////////////////////////////////

void BodyBuilder::GetHand(NPCSkeleton & primitive, bool bMirror)
{
  double scale = 0.3;
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
  aa.SetRelCoords(NPCBoneCoords(0.001, 0, 0.01, 0)); // Root/butt
  primitive.AddBone(aa);

  double notZeroPhi = 0.001;
  double notZeroTheta = 0.001;
  // Torso 1-2
  aa.SetRelCoords(m_map.Get("torso1", NPCBoneCoords(1.2, notZeroPhi, notZeroTheta, 0))); 
  //aa.SetLimits(NPCBoneCoords(0., -0., -0., -0.),
  //	       NPCBoneCoords(0., 0., 0., 0.));
  primitive.AddBone(aa);
  aa.SetRelCoords(m_map.Get("torso2", NPCBoneCoords(0.5, notZeroPhi, notZeroTheta, 0))); 
  //aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  //	       NPCBoneCoords(0., .1, .1, .1));
 
  primitive.AddBone(aa);
 

  // Shoulder 3-4
  aa.SetRelCoords(m_map.Get("leftshoulder", NPCBoneCoords(.62, notZeroPhi, notZeroTheta, 0))); 
  //aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  //	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);
 
  aa.SetRelCoords(m_map.Get("rightshoulder", NPCBoneCoords(.62, notZeroPhi, notZeroTheta, 0))); 
  //aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  //	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);

  aa.SetHaveLimit(false);
  
  // Left arm 5-6
  aa.SetRelCoords(m_map.Get("leftupperarm", NPCBoneCoords(0.91, notZeroPhi, notZeroTheta, 0 /*+0.3*/))); 
  //aa.SetLimits(NPCBoneCoords(0., -PI_P/2, -PI_P/2, 3*PI_P/2),
  //	       NPCBoneCoords(0., PI_P/2, PI_P/2, PI_P/2));
  primitive.AddBone(aa);

  aa.SetHaveLimit(false);
  aa.SetRelCoords(m_map.Get("leftlowerarm", NPCBoneCoords(0.82, notZeroPhi, notZeroTheta, 0))); 
  primitive.AddBone(aa);

  // Right arm 7-8
  aa.SetRelCoords(m_map.Get("rightupperarm", NPCBoneCoords(0.91, notZeroPhi, notZeroTheta, 0/*-0.3*/))); 
  //aa.SetLimits(NPCBoneCoords(0., -PI_P/2, -PI_P/2, 3*PI_P/2),
  //	       NPCBoneCoords(0., PI_P/2, PI_P/2, PI_P/2));
  primitive.AddBone(aa);
  aa.SetHaveLimit(false);
  aa.SetRelCoords(m_map.Get("rightlowerarm", NPCBoneCoords(0.82, notZeroPhi, notZeroTheta, 0))); 
  primitive.AddBone(aa);

  aa.SetHaveLimit(false);

  // Pelvis 9-10
  aa.SetRelCoords(m_map.Get("leftpelvis", NPCBoneCoords(0.515, notZeroPhi, notZeroTheta, 0))); 
  //aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  //	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);
  aa.SetRelCoords(m_map.Get("rightpelvis", NPCBoneCoords(0.515, notZeroPhi, notZeroTheta, 0))); 
  //aa.SetLimits(NPCBoneCoords(0., -.1, -.1, -.1),
  //	       NPCBoneCoords(0., .1, .1, .1));
  primitive.AddBone(aa);
  aa.SetHaveLimit(false);
 
  // Left leg 11-12
  aa.SetRelCoords(m_map.Get("leftupperleg", NPCBoneCoords(1.48, notZeroPhi, notZeroTheta, 0/*-0.05*/))); 
  primitive.AddBone(aa);
  aa.SetRelCoords(m_map.Get("leftlowerleg", NPCBoneCoords(1.4, notZeroPhi, notZeroTheta, 0))); 
  primitive.AddBone(aa);

  // Right leg 13-14
  aa.SetRelCoords(m_map.Get("rightupperleg", NPCBoneCoords(1.48, notZeroPhi, notZeroTheta, 0/*+0.05*/))); 
  primitive.AddBone(aa);
  aa.SetRelCoords(m_map.Get("rightlowerleg", NPCBoneCoords(1.4, notZeroPhi, notZeroTheta, 0))); 
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

  NPCBoneCoords empty;
  empty.Radius() = 0.;

  primitive.AddToBoneRot(1, NPCBoneCoords(0, PI_P/2, 0, 0)); 
 
  primitive.AddToBoneRot(3, NPCBoneCoords(0, 0, PI_P/2, 0)); //m_map.Get("leftshoulder_post", empty)); 
  primitive.AddToBoneRot(4, NPCBoneCoords(0, 0, 3*PI_P/2, 0));  //m_map.Get("rightshoulder_post", empty)); 
  primitive.AddToBoneRot(5, NPCBoneCoords(0, 0, PI_P/2, 0));  //m_map.Get("leftupperarm_post", empty));
  primitive.AddToBoneRot(6, NPCBoneCoords(0, 0, 0, 0));  //m_map.Get("leftlowerarm_post", empty)); 
  primitive.AddToBoneRot(7, NPCBoneCoords(0, 0, PI_P/2, 0));  //m_map.Get("rightupperarm_post", empty)); 
  primitive.AddToBoneRot(8, NPCBoneCoords(0, 0, 0, 0));  //m_map.Get("rightlowerarm_post", empty)); 
  primitive.AddToBoneRot(9, NPCBoneCoords(0, 0, PI_P/2, 0));  //m_map.Get("leftpelvis_post", empty)); 
  primitive.AddToBoneRot(10, NPCBoneCoords(0, 0, -PI_P/2, 0));  //m_map.Get("rightpelvis_post", empty)); 
  primitive.AddToBoneRot(11, NPCBoneCoords(0, 0, PI_P/2, 0));  //m_map.Get("leftupperleg_post", empty)); 
  //primitive.AddToBoneRot(12, NPCBoneCoords(0, PI_P/4, 0, 0));  //m_map.Get("leftlowerleg_post", empty)); 
  primitive.AddToBoneRot(13, NPCBoneCoords(0, 0, PI_P/2, 0));  //m_map.Get("rightupperleg_post", empty)); 
  //primitive.AddToBoneRot(14, NPCBoneCoords(0, -PI_P/4, 0, 0));  //m_map.Get("rightlowerleg_post", empty)); 


  
  primitive.AddToBoneRot(1, m_map.Get("torso1_post", empty)); 
  primitive.AddToBoneRot(2, m_map.Get("torso2_post", empty)); 
  primitive.AddToBoneRot(3, m_map.Get("leftshoulder_post", empty)); 
  primitive.AddToBoneRot(4, m_map.Get("rightshoulder_post", empty)); 
  primitive.AddToBoneRot(5, m_map.Get("leftupperarm_post", empty));
  primitive.AddToBoneRot(6, m_map.Get("leftlowerarm_post", empty)); 
  primitive.AddToBoneRot(7, m_map.Get("rightupperarm_post", empty)); 
  primitive.AddToBoneRot(8, m_map.Get("rightlowerarm_post", empty)); 
  primitive.AddToBoneRot(9, m_map.Get("leftpelvis_post", empty)); 
  primitive.AddToBoneRot(10, m_map.Get("rightpelvis_post", empty)); 
  primitive.AddToBoneRot(11, m_map.Get("leftupperleg_post", empty)); 
  primitive.AddToBoneRot(12, m_map.Get("leftlowerleg_post", empty)); 
  primitive.AddToBoneRot(13, m_map.Get("rightupperleg_post", empty)); 
  primitive.AddToBoneRot(14, m_map.Get("rightlowerleg_post", empty)); 
  
}

void BodyBuilder::GetSimple(NPCSkeleton & s)
{
  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0.001, 0, 0, 0)); // Root/butt
  s.AddBone(aa);
 
  aa.SetRelCoords(m_map.Get("torso1", NPCBoneCoords(1.0, 0, 0., 0))); 
  s.AddBone(aa);

  
  aa.SetRelCoords(m_map.Get("torso2", NPCBoneCoords(1.0, 0, 0, 0))); 
  s.AddBone(aa);

  /*aa.SetRelCoords(m_map.Get("torso3", NPCBoneCoords(1.0, 2, PI_P/3, 0))); 
  s.AddBone(aa);
  */
  
  s.SetParentChild(0, 1);
  s.SetParentChild(1, 2);
  //s.SetParentChild(2, 3);

  
  s.SetBaseCoords(Coordinates(0, 10+ 29, 0));
  s.SetBaseline();
}

void BodyBuilder::GetSpider(NPCSkeleton & s)
{

  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0.001, 0.001, 0.01, 0)); // Root/butt
  s.AddBone(aa);


  
  // Left
  aa.SetRelCoords(NPCBoneCoords(1.0, 1.2, PI_P/4, 0)); 
  s.AddBone(aa);

  
  aa.SetRelCoords(NPCBoneCoords(1.0, 0.2, PI_P/4, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(1.0, -0.2, PI_P/4, 0)); 
  s.AddBone(aa);
  
  aa.SetRelCoords(NPCBoneCoords(1.0, -1.2, PI_P/4, 0)); 
  s.AddBone(aa);

  //Right 
  aa.SetRelCoords(NPCBoneCoords(1.0, 1.2+PI_P, PI_P/4, 0)); 
  s.AddBone(aa);
  
  aa.SetRelCoords(NPCBoneCoords(1.0, 0.2+PI_P, PI_P/4, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(1.0, -0.2+PI_P, PI_P/4, 0)); 
  s.AddBone(aa);
  
  aa.SetRelCoords(NPCBoneCoords(1., -1.2+PI_P, PI_P/4, 0)); 
  s.AddBone(aa);

  //Left lower
  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);
  
  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);


  //Right lower
  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);
  
  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(2.0, 0, PI_P/2, 0)); 
  s.AddBone(aa);
  

  
  s.SetParentChild(0, 1);
  s.SetParentChild(0, 2);
  s.SetParentChild(0, 3);
  s.SetParentChild(0, 4);
  s.SetParentChild(0, 5);
  s.SetParentChild(0, 6);
  s.SetParentChild(0, 7);
  s.SetParentChild(0, 8);
  
  s.SetParentChild(1, 9);
  s.SetParentChild(2, 10);
  s.SetParentChild(3, 11);
  s.SetParentChild(4, 12);
  s.SetParentChild(5, 13);
  s.SetParentChild(6, 14);
  s.SetParentChild(7, 15);
  s.SetParentChild(8, 16);
  
 
  s.SetBaseCoords(Coordinates(0, 10+ 29, 0));
  s.SetBaseline();
  NPCNerveCostume & c = s.Nerves();

  NPCNerve n;
  
  n.Clear();
  n.SetName("up");
  n.AddBone(1, NPCBoneCoords(0., .0, -.1, 0));
  n.AddBone(3, NPCBoneCoords(0., .0, -.1, 0));
  n.AddBone(7, NPCBoneCoords(0., .0, -.1, 0));
  n.AddBone(5, NPCBoneCoords(0., .0, -.1, 0));
  
  n.AddBone(2, NPCBoneCoords(0., -.0, .1, 0));
  n.AddBone(4, NPCBoneCoords(0., -.0, .1, 0));
  n.AddBone(8, NPCBoneCoords(0., -.0, .1, 0));
  n.AddBone(6, NPCBoneCoords(0., -.0, .1, 0));
  n.SetMinMax(-3, 3);
  c.AddNerve(n);

  
  n.Clear();
  n.SetName("forward");
  n.AddBone(1, NPCBoneCoords(0., .3, 0, 0));
  n.AddBone(3, NPCBoneCoords(0., .3, 0, 0));
  n.AddBone(7, NPCBoneCoords(0., -.3, 0, 0));
  n.AddBone(5, NPCBoneCoords(0., -.3, 0, 0));
  
  n.AddBone(2, NPCBoneCoords(0., -.3, 0, 0));
  n.AddBone(4, NPCBoneCoords(0., -.3, 0, 0));
  n.AddBone(8, NPCBoneCoords(0., .3, 0, 0));
  n.AddBone(6, NPCBoneCoords(0., .3, 0, 0));
  n.SetMinMax(-1.3, 1.3);
  c.AddNerve(n);


  /*
  n.Clear();
  n.SetName("turn");
  n.AddBone(0, NPCBoneCoords(0., .02, 0, 0));
  n.SetMinMax(-2000, 2000);
  c.AddNerve(n);
  */
  
  
  /*
  n.Clear();
  n.SetName("forward_right");
  n.AddBone(1, NPCBoneCoords(0., .2, 0, 0));
  n.AddBone(3, NPCBoneCoords(0., .2, 0, 0));
  n.AddBone(8, NPCBoneCoords(0., .2, 0, 0));
  n.AddBone(6, NPCBoneCoords(0., .2, 0, 0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);

  n.Clear();
  
  n.SetName("forward_left");  
  n.AddBone(2, NPCBoneCoords(0., -.2, 0, 0));
  n.AddBone(4, NPCBoneCoords(0., -.2, 0, 0));
  n.AddBone(5, NPCBoneCoords(0., -.2, 0, 0));
  n.AddBone(7, NPCBoneCoords(0., -.2, 0, 0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);

  n.Clear();
  n.SetName("udfb");
  n.AddBone(1, NPCBoneCoords(0., .2, -.1, 0));
  n.AddBone(3, NPCBoneCoords(0., .2, -.1, 0));
  n.AddBone(8, NPCBoneCoords(0., .2, -.1, 0));
  n.AddBone(6, NPCBoneCoords(0., .2, -.1, 0));
  
  n.AddBone(2, NPCBoneCoords(0., -.2, .1, 0));
  n.AddBone(4, NPCBoneCoords(0., -.2, .1, 0));
  n.AddBone(7, NPCBoneCoords(0., -.2, .1, 0));
  n.AddBone(5, NPCBoneCoords(0., -.2, .1, 0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("look");
  n.AddBone(1, NPCBoneCoords(0., -.3, -.5, 0));
  n.AddBone(8, NPCBoneCoords(0., .3, -.5, 0));
  n.AddBone(9, NPCBoneCoords(0., .3, 0, 0));
  n.AddBone(16, NPCBoneCoords(0., -.3, 0, 0));
  n.SetMinMax(-0.4, 1.4);
  c.AddNerve(n);
  */
  
}

void BodyBuilder::GetFigure(NPCSkeleton & s)
{
  NPCSkeleton leftHand, rightHand, leftFoot, rightFoot;
  NPCSkeleton body;

  GetHand(rightHand, false);
  GetHand(leftHand, true);
  GetFoot(rightFoot, false);
  GetFoot(leftFoot, true);
  
  GetBody(s);

  /*
  rightHand.AddToBoneRot(0, NPCBoneCoords(0, 0, 0, -PI_P/2));
  leftHand.AddToBoneRot(0, NPCBoneCoords(0, 0, PI_P, 3*PI_P/2));
  s.Attach(rightHand, 6);
  s.Attach(leftHand, 8);
  */

  s.SetBaseCoords(Coordinates(0, 10+ 29, 0));
  
  //=======================================================
  s.SetBaseline();
  //=======================================================
  
  
  NPCNerveCostume & c = s.Nerves();

  NPCNerve n;
  
  
  n.Clear();
  n.SetName("turn");
  n.AddBone(3, NPCBoneCoords(0., .2, 0, 0));
  n.AddBone(4, NPCBoneCoords(0., .2, 0, 0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);

  n.Clear();
  n.SetName("walk");
  n.AddBone(9, NPCBoneCoords(0., 0.3, 0, 0));
  n.AddBone(10, NPCBoneCoords(0., 0.3, 0, 0));
  n.AddBone(11, NPCBoneCoords(0., 1, 0, 0));
  n.AddBone(13, NPCBoneCoords(0., 1, 0, 0));
  n.AddBone(5, NPCBoneCoords(0., 0.5, 0, 0.));
  n.AddBone(7, NPCBoneCoords(0., 0.5, 0, 0.));
  n.SetMinMax(-1.2, 1.2);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("squat");
  
  n.AddBone(11, NPCBoneCoords(0., -1, 0, 0));
  n.AddBone(13, NPCBoneCoords(0., 1, 0, 0));
  n.AddBone(12, NPCBoneCoords(0., 0, 1.9, 0));
  n.AddBone(14, NPCBoneCoords(0., 0, 1.9, 0));
  
  n.AddBone(5, NPCBoneCoords(0., -0.5, -0.2, 0.));
  n.AddBone(7, NPCBoneCoords(0., +0.5, 0.2, 0.));
  n.SetMinMax(-1.7, 1.7);
  c.AddNerve(n);

  n.Clear();
  n.SetName("left_knee");
  n.AddBone(12, NPCBoneCoords(0., 0, 0.5, 0.0));
  n.SetMinMax(0, 3);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("right_knee");
  n.AddBone(14, NPCBoneCoords(0., 0, -0.5, 0.0));
  n.SetMinMax(0, 3);
  c.AddNerve(n);

  n.Clear();
  n.SetName("left_arm_out");
  n.AddBone(5, NPCBoneCoords(0., .0, -.5, 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("right_arm_out");
  n.AddBone(7, NPCBoneCoords(0., .0, -.5, 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);

  n.Clear();
  n.SetName("left_arm_up");
  n.AddBone(5, NPCBoneCoords(0., -.5, .0, 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("right_arm_up");
  n.AddBone(7, NPCBoneCoords(0., -.5, .0, 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);


  n.Clear();
  n.SetName("left_hand_up");
  n.AddBone(6, NPCBoneCoords(0., .0, .5, 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("right_hand_up");
  n.AddBone(8, NPCBoneCoords(0., .0, -.5, 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);

  n.Clear();
  n.SetName("left_leg");
  n.AddBone(11, NPCBoneCoords(0., 0.5, 0., 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("right_leg");
  n.AddBone(13, NPCBoneCoords(0., 0.5, 0., 0.0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);

  
  n.Clear();
  n.SetName("bend");
  n.AddBone(1, NPCBoneCoords(0., 0, -0.6, 0));
  n.SetMinMax(-2, 2);
  c.AddNerve(n);


  
  n.Clear();
  n.SetName("arms_up");
  n.AddBone(5, NPCBoneCoords(0., 1, 0, 0));
  //n.AddBone(6, NPCBoneCoords(0., -1, 0, 1));
  n.AddBone(7, NPCBoneCoords(0., 1, 0, 0));
  //n.AddBone(8, NPCBoneCoords(0., 1, 0, 1));
  c.AddNerve(n);

  n.Clear();
  n.SetName("arms_bend");
  n.AddBone(6, NPCBoneCoords(0., 1, 0, 0));
  n.AddBone(8, NPCBoneCoords(0., 1, 0, 0));
  c.AddNerve(n);

  n.Clear();
  n.SetName("arms_twist");
  n.AddBone(5, NPCBoneCoords(0., 0, 0, -1));
  n.AddBone(7, NPCBoneCoords(0., 0, 0, 1));
  c.AddNerve(n);

  n.Clear();
  n.SetName("leftarm_fb");
  n.AddBone(5, NPCBoneCoords(0., 1, 0, 0));
  c.AddNerve(n);

  n.Clear();
  n.SetName("rightarm_fb");
  n.AddBone(7, NPCBoneCoords(0., 1, 0, 0));
  c.AddNerve(n);
 
  n.Clear();
  n.SetName("leftarm_io");
  n.AddBone(5, NPCBoneCoords(0., 0, 0, 1));
  c.AddNerve(n);

  n.Clear();
  n.SetName("rightarm_io");
  n.AddBone(7, NPCBoneCoords(0., 0, 0, -1));
  c.AddNerve(n);

  n.Clear();
  n.SetName("lefthand_fb");
  n.AddBone(6, NPCBoneCoords(0., 1, 0, 0));
  c.AddNerve(n);
  
  n.Clear();
  n.SetName("righthand_fb");
  n.AddBone(8, NPCBoneCoords(0., 1, 0, 0));
  c.AddNerve(n);
}
