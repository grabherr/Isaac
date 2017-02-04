#include "game/SGPlay.h"
#include <unistd.h>
#include <math.h>

void AllCharacters::AddSchool(GameEngine & eng)
{
  MsgSceneNode node;

  node.SetScale(300.);
  int n = 1;
  node.SetMaterialCount(n);

  for (int x = 0; x<n; x++) {
    node.Material(x).SetTexture("data/Textures/grey.jpg");
    node.Material(x).SetLighting(true);
    node.Material(x).SetShinyness(30);
  }


  node.SetModel("applications_data/schoolgame/Isaac_School_OBJ_Triangulate.x");

  node.SetPosition(StreamCoordinates(2300, 730, 3900));
 
  node.SetPhysMode(2);
  
  node.SetRequestLoopBack(false);
  node.SetRequestMesh(false);

  node.SetName("school");  
  eng.AddSceneNode(node);
}

void AllCharacters::GetVector(svec<double> & out, int i)
{
  const Measures & m = m_maxln[i];
  out.clear();
  if (m.sex == "female")
    out.push_back(0.9);
  if (m.sex == "male")
    out.push_back(-0.9);
  double scale = 0.5;
  out.push_back(m.spine*scale);
  out.push_back(m.sholder*scale);
  out.push_back(m.arm*scale);
  out.push_back(m.wrist*scale);
  out.push_back(m.pelvis*scale);
  out.push_back(m.leg*scale);
  out.push_back(m.ankle*scale);

}

void AllCharacters::AddCharacter(GameEngine & eng,
				 BodyBuilder & bb,
				 const StreamCoordinates & pos,
				 const string & name,
				 const string & modelName)
{  
		  
  NPCSkeleton s;
  s.DoPhysics(false);
  
  bb.GetFigure(s);


  int idx = MaxlIndex(name);
  Measures m = m_maxln[idx];
  s[1].Len() = m.spine - s[2].Len();
  cout << "DEBUG " << s[3].Len() << " " <<  m.sholder << endl;
  s[3].Len() = m.sholder;
  s[4].Len() = m.sholder;


  
  s[5].Len() = m.arm;
  s[6].Len() = m.arm;
  s[7].Len() = m.wrist;
  s[8].Len() = m.wrist;
  
  s[9].Len() = m.pelvis;
  s[10].Len() = m.pelvis;

  s[11].Len() = m.leg;
  s[12].Len() = m.leg;
  s[13].Len() = m.ankle;
  s[14].Len() = m.ankle;
 
  s.Scale(10.);
  s.MoveTowards(1, 0.02, 0.05);
  
  CharManipulator * manip2 = new CharManipulator;
  manip2->SetHeadPlus(-2.1);
  HeadManipulator * headManip = new HeadManipulator;
  m_p.push_back(manip2);
  m_h.push_back(headManip);

  //==============================================================
  //==============================================================
  //==============================================================
  //==============================================================
  if (m_p.isize() == 1)
    manip2->SetTagged(true);

  manip2->Name() = name;
  
  MsgSceneNode stat;

  char tName[1024];
  char oName[1024];
  sprintf(tName, "applications_data/schoolgame/CharacterHeads/%s/OBJ/%s_texture_closed.jpg", name.c_str(), name.c_str()); 
  
  stat.Material(0).SetTexture(tName);
  stat.Material(0).SetLighting(true);    
  
  sprintf(oName, "applications_data/schoolgame/CharacterHeads/%s/OBJ/%s_closed.obj", name.c_str(), name.c_str()); 
  stat.SetModel(oName);
	
  stat.SetPosition(pos);
  stat.SetPhysMode(2);
  stat.SetScale(14.);
    
  stat.SetRequestLoopBack(true);
  stat.SetRequestMesh(false);
  
  //stat.SetName("Eva_head");  
  eng.AddSceneNode(stat, headManip);

  
  manip2->SetSaveName(name);
  
  MsgSceneNode node;
  //node.SetName("skeleton");
  node.Material(0).SetTexture("data/Textures/allblack.jpg");
  node.SetPosition(pos);

  MSkeleton makeSkeleton;
  makeSkeleton.MakeSkeleton(node.Mesh(0), s);
  node.SetPhysMode(1);

  node.SetScale(10.);
  node.SetRequestLoopBack(true);
  node.SetRequestMesh(false);

  manip2->SetSkeleton(s);
  eng.AddSceneNode(node, manip2);
}
