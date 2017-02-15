
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "game/CharManip.h"
#include "game/SGPlay.h"
#include <unistd.h>



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
  all.AddCharacter(eng, bb, StreamCoordinates(5000, 1400, 8300), "Mona", "");
  all.AddCharacter(eng, bb, StreamCoordinates(6600, 1400, 8300), "Franz", "male");
  all.AddCharacter(eng, bb, StreamCoordinates(5200, 1400, 7000), "Georg", "male");

  sleep(1);

  all.AddCharacter(eng, bb, StreamCoordinates(3200, 1400, 7100), "Eva", "");
  all.AddCharacter(eng, bb, StreamCoordinates(7200, 1400, 7000), "ProfessorBerger", "");
  all.AddCharacter(eng, bb, StreamCoordinates(6700, 1400, 7100), "ProfessorHuber", "");

  sleep(1);
  
  all.AddCharacter(eng, bb, StreamCoordinates(3500, 1400, 7200), "Manfred", "");
  all.AddCharacter(eng, bb, StreamCoordinates(7000, 1400, 7000), "Headmaster", "");

  sleep(1);

  all.AddCharacter(eng, bb, StreamCoordinates(6300, 1400, 7900), "Susi", "female");
  all.AddCharacter(eng, bb, StreamCoordinates(5400, 1400, 9400), "Sonja", "");
  //all.AddCharacter(eng, bb, StreamCoordinates(5300, 1400, 10600), "Susi", "");

  //sleep(2);

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

  keyCtrl[0].Read("applications_data/schoolgame/Mona.char");
  keyCtrl[1].Read("applications_data/schoolgame/Franz.char");
  keyCtrl[2].Read("applications_data/schoolgame/Georg.char");
  keyCtrl[3].Read("applications_data/schoolgame/Eva.char");
  
  //for (i=0; i<all.Item().isize(); i++) {
  //  keyCtrl.AddItem(all.Item()[i]);
  //}
 

  
  //keyCtrl.AddFigure(&manip2, &headManip);
  keyCtrl.AddItem(&item_milk);
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
