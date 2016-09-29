
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/BodyParts.h"
#include "npc/SkeletonPhys.h"


void SetupSkeleton(NPCSkeletonWithPhysics & s)
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

  aa.SetRelCoords(NPCBoneCoords(1., 0.3, 0.2, -PI_P/2-0.3)); 
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

  aa.SetRelCoords(NPCBoneCoords(0.6, -0., PI_P, -0.4)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P/2, 0.)); 
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
  


 
  s.Scale(10.);
  s.SetAbsOffset(Coordinates(0., 60., 0));
  s.SetFloorY(0.);

 
  s.SetupPhysics();

  
  PhysObject & phys = s.Physics();
  double elast = 0.01;

  phys.Connect(PhysConnection(0, 2, elast));
  phys.Connect(PhysConnection(0, 4, elast));
  phys.Connect(PhysConnection(5, 1, elast));
  phys.Connect(PhysConnection(5, 3, elast));

  phys.Connect(PhysConnection(5, 7, elast));
  phys.Connect(PhysConnection(5, 9, elast));
  phys.Connect(PhysConnection(0, 6, elast));
  phys.Connect(PhysConnection(0, 8, elast));


  for (i=0; i<phys.isize(); i++) {
    for (int j=i+1; j<phys.isize(); j++) {
      phys.Connect(PhysConnection(i, j, 0.9));
      
    }
  }


  /*
  cout << "Hidden connections from " <<  phys.GetConnectCount() << endl;

  phys.Connect(PhysConnection(13, 1, elast));
  phys.Connect(PhysConnection(14, 1, elast));
  phys.Connect(PhysConnection(0, 7, elast));
  phys.Connect(PhysConnection(13, 7, elast));

  phys.Connect(PhysConnection(13, 2, elast));
  phys.Connect(PhysConnection(14, 2, elast));
  phys.Connect(PhysConnection(0, 8, elast));
  phys.Connect(PhysConnection(13, 8, elast));

  phys.Connect(PhysConnection(13, 3, elast));
  phys.Connect(PhysConnection(14, 3, elast));
  phys.Connect(PhysConnection(0, 9, elast));
  phys.Connect(PhysConnection(13, 9, elast));

  phys.Connect(PhysConnection(13, 4, elast));
  phys.Connect(PhysConnection(14, 4, elast));
  phys.Connect(PhysConnection(0, 10, elast));
  phys.Connect(PhysConnection(13, 10, elast));

  phys.Connect(PhysConnection(13, 5, elast));
  phys.Connect(PhysConnection(14, 5, elast));
  phys.Connect(PhysConnection(0, 11, elast));
  phys.Connect(PhysConnection(13, 11, elast));

  phys.Connect(PhysConnection(13, 6, elast));
  phys.Connect(PhysConnection(14, 6, elast));
  phys.Connect(PhysConnection(0, 12, elast));
  phys.Connect(PhysConnection(13, 12, elast));

 
  cout << "Hidden connections to " <<  phys.GetConnectCount() << endl;
  phys.Connect(PhysConnection(13, 14, elast));

  phys.Connect(PhysConnection(13, 15, elast));
  phys.Connect(PhysConnection(14, 15, elast));

  */

  phys.Fixate();
}




int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);


  MsgLightNode light;
  light.SetPosition(StreamCoordinates(2300, 1600, 3000));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2200, 1000, 2900));
  eng.AddLight(light);
  eng.AddLight(light);
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2400, 1800, 3000));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2600, 1400, 3100));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2700, 1400, 2700));
  eng.AddLight(light);

  light.SetPosition(StreamCoordinates(2700, 600, 3000));
  eng.AddLight(light);

  MsgSceneNode node;

  node.SetScale(60.);
  int n = 1;
  node.SetMaterialCount(n);

  for (int x = 0; x<n; x++) {
    node.Material(x).SetTexture("data/Textures/grey.jpg");
    //node.Material(x).SetTexture("data/Models/red.jpg");
    node.Material(x).SetLighting(true);
    node.Material(x).SetShinyness(30);
  }


  //node.SetModel("../../IsaacSchoolGame/Isaac_School_OBJ_Triangulate.obj");
  node.SetModel("applications_data/schoolgame/Isaac_School_OBJ_Triangulate.x");

  node.SetPosition(StreamCoordinates(2300, 450, 2900));
  //node.SetRotation(Coordinates(3.14/2, 0., 3.14));
 
  node.SetPhysMode(2);
  
  node.SetRequestLoopBack(false);
  node.SetRequestMesh(false);

  node.SetName("school");  
  eng.AddSceneNode(node);

  //=================================================================

  MsgSceneNode stat;

  stat.SetScale(60.);
  stat.SetMaterialCount(n);


  svec<string> names;
  names.push_back("ProfessorBerger");
  names.push_back("ProfessorHuber");
  names.push_back("Eva");
  names.push_back("Franz");
  names.push_back("Georg");
  names.push_back("Manfred");
  names.push_back("Mona");
  names.push_back("Sonja");
  names.push_back("Susi");

  for (int i=0; i<names.isize(); i++) {
    string name = names[i];

    double a = 2300 + RandomFloat(800)-400;
    double c = 3500 + RandomFloat(800)-400;
    
    for (int x = 0; x<n; x++) {
      
      stat.Material(x).SetTexture("applications_data/schoolgame//CharacterHeads/" + name + "/OBJ/" + name + "_texture_closed.jpg");
      stat.Material(x).SetLighting(true);
    }
    
    stat.SetModel("applications_data/schoolgame//CharacterHeads/" + name + "/OBJ/" + name + "_closed.obj");

    double kidscale = 1.;
    if (i >=3) {
      kidscale -= RandomFloat(0.3);
    }
    
    stat.SetPosition(StreamCoordinates(a, 609-(1.-kidscale)*70, c));
    stat.SetRotation(Coordinates(0., 3.14 + RandomFloat(2.)-1., 0.));
    stat.SetPhysMode(2);
    stat.SetScale(5.);
    
    stat.SetRequestLoopBack(false);
    stat.SetRequestMesh(false);
    
    stat.SetName("prop");  
    eng.AddSceneNode(stat);
    
    //===============================================
    // Body
    NPCSkeletonWithPhysics s;
    SetupSkeleton(s);
    
    MsgSceneNode body;
    body.Material(0).SetTexture("data/Textures/allblack.jpg");
    body.SetPosition(StreamCoordinates(a, 550, c));
    
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(body.Mesh(0), s);
    
    body.SetPhysMode(2);
    body.SetScale(5.*kidscale);
    body.SetRequestLoopBack(true);
    body.SetRequestMesh(false);
    
    eng.AddSceneNode(body);
  }

  eng.Run();


  return 0;
}
