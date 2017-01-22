
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "game/CharManip.h"




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

  
  // Make skeleton
  BodyBuilder bb;
  if (cString != "")
    bb.ReadConfig(cString);
  
  NPCSkeleton s;
  s.DoPhysics(false);
  
  bb.GetFigure(s);
  s.Scale(10.);
  s.MoveTowards(1, 0.02, 0.05);
  
  CharManipulator manip2;

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


  // Add HEAD here
   HeadManipulator headManip;

  MsgSceneNode stat;

  //stat.SetScale(600.);
 
      
  stat.Material(0).SetTexture("applications_data/schoolgame//CharacterHeads/Eva/OBJ/Eva_texture_closed.jpg");
  stat.Material(0).SetLighting(true);    
    
  stat.SetModel("applications_data/schoolgame//CharacterHeads/Eva/OBJ/Eva_closed.obj");
	
  stat.SetPosition(StreamCoordinates(3000, 600, 300));
  stat.SetRotation(Coordinates(0., 3.14 + RandomFloat(2.)-1., 0.));
  stat.SetPhysMode(2);
  stat.SetScale(28.);
    
  stat.SetRequestLoopBack(true);
  stat.SetRequestMesh(false);
  
  stat.SetName("Eva_head");  
  eng.AddSceneNode(stat, &headManip);


  //====================================================================================
  MsgSceneNode item;
  stat.SetModel("applications_data/schoolgame/Items/Milchpackerl.obj");
  stat.SetPosition(StreamCoordinates(3000, 600, 800));
  stat.SetRotation(Coordinates(0., 3.14 + RandomFloat(2.)-1., 0.));
  stat.SetPhysMode(2);
  stat.SetScale(15.);
    
  stat.SetRequestLoopBack(true);
  stat.SetRequestMesh(false);
  
  stat.SetName("milk");
  ItemManipulator item_milk;
  eng.AddSceneNode(stat, &item_milk);
  //=====================================================================================
  
  manip2.SetSaveName(name);
  
  MsgSceneNode node;
  node.SetName("skeleton");
  //node.Material(0).SetTexture("data/Models/green.jpg");
  node.Material(0).SetTexture("data/Textures/allblack.jpg");
  //node.SetModel("data/Models/ball.ms3d");
  node.SetPosition(StreamCoordinates(5300, 1400, 4900));

  MSkeleton makeSkeleton;
  makeSkeleton.MakeSkeleton(node.Mesh(0), s);

  // No physics
  node.SetPhysMode(1);

  node.SetScale(20.);
  // We want to take control of the object
  node.SetRequestLoopBack(true);
 
  // Enable this if yu want to control the mesh! 
  node.SetRequestMesh(false);

  manip2.SetSkeleton(s);
 
  // Add it!
  eng.AddSceneNode(node, &manip2);
  
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


  CharGlobCtrl keyCtrl(&manip2, &headManip, &item_milk);
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
