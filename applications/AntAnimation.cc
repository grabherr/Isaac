
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/AntManipulator.h"
#include <math.h>
#include "base/RandomStuff.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);
  commandArg<int> staticCmmd("-static","static ants", 1);
  commandArg<int> dynamicCmmd("-dynamic","dynamic ants", 1);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);
  P.registerArg(staticCmmd);
  P.registerArg(dynamicCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  int stat = P.GetIntValueFor(staticCmmd);
  int dyn = P.GetIntValueFor(dynamicCmmd);
  
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);
  
  int i;

  svec<AntManipulator> manip;
  cout << "Resize!" << endl;
  manip.resize(stat + dyn);
  cout << "Done! " << manip.isize() << endl;

  AntGlobal glob;
  eng.RegisterGlobal(&glob);

  

  int k = 0;
  for (i=0; i<dyn; i++) {      
    MsgSceneNode node;
    node.Material(0).SetTexture("data/Textures/black.jpg");
    node.SetModel("data/Models/ant_anim.md2");
    node.SetPosition(StreamCoordinates(5300, 10, 4900 + RandomFloat(300)));

    string name = glob.Register(&manip[k], true, node.GetModel());
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
    eng.AddSceneNode(node, &manip[k]);
    cout << "Register manip " << &manip[k] << endl;
    k++;
  }

  //===============================================
  for (i=0; i<stat; i++) {
    MsgSceneNode node;
    node.Material(0).SetTexture("data/Textures/black.jpg");
    //node.SetModel("data/Models/ant_anim.md2");
    node.SetModel("data/Models/ant_1.ms3d");
    node.SetPosition(StreamCoordinates(5000, 10, 4500-RandomFloat(300)));
    
    string name = glob.Register(&manip[k], false, node.GetModel());
    node.SetName(name);
    
    // No physics
    node.SetPhysMode(2);
    node.SetScale(25000.);
    
    node.SetRequestLoopBack(true);
    node.SetRequestMesh(false);
    
    eng.AddSceneNode(node, &manip[k]);
    cout << "Register manip (stat) " << &manip[k] << endl;
    k++;
  }

  
  for (i=0; i<manip.isize(); i++) {
    manip[i].StartThread();
    cout << "Start thread " << &manip[i] << endl;
  }
 
  eng.Run();


  return 0;
}
