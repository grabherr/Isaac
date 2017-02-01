
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "game/CharManip.h"
#include <unistd.h>

class Measures
{
public:
  Measures() {}
  string name;
  string sex;
  double spine;
  double sholder;
  double arm;
  double wrist;
  double pelvis;
  double leg;
  double ankle;
};


class AllCharacters
{
public:
  AllCharacters() {
    FlatFileParser parser;
    double scale = 0.032;

    parser.Open("applications_data/schoolgame/measures.txt");
    while (parser.ParseLine()) {
      if (parser.GetItemCount() == 0)
	continue;
      Measures m;
      m.name = parser.AsString(0);
      m.sex = parser.AsString(1);
      parser.ParseLine();
      m.spine = parser.AsFloat(parser.GetItemCount()-1)*scale;
      parser.ParseLine();
      m.sholder = parser.AsFloat(parser.GetItemCount()-1)*scale/2;
      parser.ParseLine();
      m.arm = parser.AsFloat(parser.GetItemCount()-1)*scale;
      parser.ParseLine();
      m.wrist = parser.AsFloat(parser.GetItemCount()-1)*scale;
      parser.ParseLine();
      m.pelvis = parser.AsFloat(parser.GetItemCount()-1)*scale/2;
      parser.ParseLine();
      m.leg = parser.AsFloat(parser.GetItemCount()-1)*scale;      
      parser.ParseLine();
      m.ankle = parser.AsFloat(parser.GetItemCount()-1)*scale;
  
      m_maxln.push_back(m);
    }
  }
  
  ~AllCharacters() {
    int i;
    for (i=0; i<m_p.isize(); i++)
      delete m_p[i];
    for (i=0; i<m_p.isize(); i++)
      delete m_h[i];
    for (i=0; i<m_i.isize(); i++)
      delete m_i[i];
  }

  svec<CharManipulator*> Char() {return m_p;}
  svec<HeadManipulator*> Head() {return m_h;}
  svec<ItemManipulator*> Item() {return m_i;}

  void AddSchool(GameEngine & eng);

  void AddCharacter(GameEngine & eng,
		    BodyBuilder & bb,
		    const StreamCoordinates & pos,
		    const string & name,
		    const string & modelName);

  
  const svec<Measures> & GetMaxln() const {return m_maxln;}

private:
  int MaxlIndex(const string & name) {
    int i;
    for (i=0; i<m_maxln.isize(); i++) {
      if (m_maxln[i].name == name)
	return i;
    }
    return -1;
  }
  
  svec<CharManipulator*> m_p;
  svec<HeadManipulator*> m_h;
  svec<ItemManipulator*> m_i;
  svec<Measures> m_maxln;
};

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


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<string> cStringCmmd("-c","body config file", "");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(cStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  string cString = P.GetStringValueFor(cStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng("localhost", false);
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);


  int i;
  
  // Make skeleton
  BodyBuilder bb;
  if (cString != "")
    bb.ReadConfig(cString);
  

  int k = 0;
  char name[256];
  while (true) {
    sprintf(name, "skeleton%d.txt", k);
    FILE * p = fopen(name, "r");
    if (p) {
      fclose(p);
      k++;
      continue;
    }
    break;
  }


  AllCharacters all;

  all.AddSchool(eng);
  sleep(2);
  all.AddCharacter(eng, bb, StreamCoordinates(6300, 1400, 7900), "Susi", "female");
  all.AddCharacter(eng, bb, StreamCoordinates(6600, 1400, 8300), "Franz", "male");
  all.AddCharacter(eng, bb, StreamCoordinates(5200, 1400, 7000), "Georg", "male");

  sleep(2);

  all.AddCharacter(eng, bb, StreamCoordinates(7000, 1400, 7000), "Headmaster", "");
  all.AddCharacter(eng, bb, StreamCoordinates(7200, 1400, 7000), "ProfessorBerger", "");
  all.AddCharacter(eng, bb, StreamCoordinates(6700, 1400, 7100), "ProfessorHuber", "");

  sleep(2);
  
  all.AddCharacter(eng, bb, StreamCoordinates(3200, 1400, 7100), "Eva", "");
  all.AddCharacter(eng, bb, StreamCoordinates(3500, 1400, 7200), "Manfred", "");

  sleep(2);

  all.AddCharacter(eng, bb, StreamCoordinates(5000, 1400, 8300), "Mona", "");
  all.AddCharacter(eng, bb, StreamCoordinates(5400, 1400, 9400), "Sonja", "");
  all.AddCharacter(eng, bb, StreamCoordinates(5300, 1400, 10600), "Susi", "");

  sleep(2);

  //============================================================

  //====================================================================================
  MsgSceneNode stat;
  stat.SetModel("applications_data/schoolgame/Items/Milchpackerl.obj");
  stat.SetPosition(StreamCoordinates(3000, 900, 2000));
  stat.SetRotation(Coordinates(0., 3.14 + RandomFloat(2.)-1., 0.));
  stat.SetPhysMode(2);
  stat.SetScale(15.);
    
  stat.SetRequestLoopBack(true);
  stat.SetRequestMesh(false);
  
  stat.SetName("milk");
  ItemManipulator item_milk;
  eng.AddSceneNode(stat, &item_milk);
  //all.AddItem(
  //=====================================================================================



  
  MsgLightNode light;
  light.SetRadius(200000);
  light.SetPosition(StreamCoordinates(4600, 2600, 4400));
  eng.AddLight(light);
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5600, 2600, 5500));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(1600, 2600, 5500));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5500, 2600, 500));
  eng.AddLight(light);

  light.SetPosition(StreamCoordinates(6500, 2800, 6000));
  eng.AddLight(light);

  CharGlobCtrl keyCtrl;

  for (i=0; i<all.Char().isize(); i++) {
    keyCtrl.AddFigure(all.Char()[i], all.Head()[i]);
  }
  //for (i=0; i<all.Item().isize(); i++) {
  //  keyCtrl.AddItem(all.Item()[i]);
  //}
 

  
  //keyCtrl.AddFigure(&manip2, &headManip);
  keyCtrl.AddItem(&item_milk);
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
