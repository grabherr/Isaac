
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/AntManipulator.h"
#include <math.h>



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
  eng.DoObjectCollision(false);
  


  svec<AntManipulator> manip;
  manip.resize(2);

  AntGlobal glob;
  eng.RegisterGlobal(&glob);

  MsgSceneNode node;

  node.Material(0).SetTexture("data/Textures/black.jpg");
  node.SetModel("data/Models/ant_anim.md2");
  node.SetPosition(StreamCoordinates(5300, 10, 4900));

  string name = glob.Register(&manip[0], true, node.GetModel());
  node.SetName(name);

  // No physics
  node.SetPhysMode(2);
  node.SetScale(25000.);

  //node.Animation().SetAnimation("walk");
  //node.Animation().SetSpeed(60);

  // We want to take control of the object
  node.SetRequestLoopBack(true);
  // Enable this if yu want to control the mesh! 
  node.SetRequestMesh(false);
  
  // Add it!
  eng.AddSceneNode(node, &manip[0]);


  //===============================================

  node.Material(0).SetTexture("data/Textures/black.jpg");
  //node.SetModel("data/Models/ant_anim.md2");
  node.SetModel("data/Models/ant_1.ms3d");
  node.SetPosition(StreamCoordinates(5000, 10, 4500));

  name = glob.Register(&manip[1], false, node.GetModel());
  node.SetName(name);

  // No physics
  node.SetPhysMode(2);
  node.SetScale(25000.);

  node.SetRequestLoopBack(true);
  node.SetRequestMesh(false);
  
  eng.AddSceneNode(node, &manip[1]);


 
  eng.Run();


  return 0;
}
