
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "game/CharManip.h"
#include "game/SGPlay.h"
#include <unistd.h>


void AddItem(svec<ItemManipulator*> & item_milk,
	     GameEngine & eng,
	     const string & model,
	     const string & name,
	     const StreamCoordinates & c)
{
  //====================================================================================
  MsgSceneNode stat;
  stat.SetModel(model);
  stat.SetPosition(c);
  stat.SetRotation(Coordinates(0., 3.14 + RandomFloat(2.)-1., 0.));
  stat.SetPhysMode(2);
  stat.SetScale(11.);
  stat.Material(0).SetLighting(true);    

  stat.SetRequestLoopBack(true);
  stat.SetRequestMesh(false);
  
  //stat.SetName("milk");
  ItemManipulator * p = new ItemManipulator;
  p->SetName(name);
  item_milk.push_back(p);
  eng.AddSceneNode(stat, p);
}

int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<string> cStringCmmd("-c","body config file", "");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("School game.");
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

  AllCharacters all;
  //2700*2,1200,3500
  all.AddSchool(eng);
  sleep(2);
  all.AddCharacter(eng, bb, StreamCoordinates(5400, 1400, 3500), "Mona", "");
  all.AddCharacter(eng, bb, StreamCoordinates(6600, 1400, 8300), "Franz", "male");
  all.AddCharacter(eng, bb, StreamCoordinates(5200, 1400, 7000), "Georg", "male");

  sleep(1);

  all.AddCharacter(eng, bb, StreamCoordinates(3200, 1400, 7100), "Eva", "");
  all.AddCharacter(eng, bb, StreamCoordinates(2300, 1400, 3900), "Susi", "female");
  all.AddCharacter(eng, bb, StreamCoordinates(1400, 1400, 4400), "Sonja", "");

  sleep(1);
  
  all.AddCharacter(eng, bb, StreamCoordinates(1500, 1400, 7200), "Manfred", "");
  all.AddCharacter(eng, bb, StreamCoordinates(500, 1400, 7000), "Headmaster", "");

  sleep(1);
  all.AddCharacter(eng, bb, StreamCoordinates(400, 1400, 7000), "ProfessorBerger", "");
  all.AddCharacter(eng, bb, StreamCoordinates(300, 1400, 7100), "ProfessorHuber", "");

  //all.AddCharacter(eng, bb, StreamCoordinates(5300, 1400, 10600), "Susi", "");

  //sleep(2);

  //============================================================
  svec<ItemManipulator*> item_milk;
  //sleep(1);

  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Milchpackerl.obj",
	  "Milchpackerl",
	  StreamCoordinates(3000, 900, 3000));

  item_milk[0]->SetSound("applications_data/schoolgame/Sounds/Ambient.wav");
  

  /*
  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Jausengetraenk.obj",
	  "Jausengetraenk",
	  StreamCoordinates(-2000, 800, 3000));
  */
  //sleep(1);
 
  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Apfelbutzen.obj",
	  "Apfelbutzen",
	  StreamCoordinates(8000, 900, 2600));

  /*
  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Buch.obj",
	  "Buch",
	  StreamCoordinates(0, 1100, 6500));
  */
  
  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Fuellfeder.obj",
	  "Fuellfeder",
	  StreamCoordinates(9000, 900, 10000));


  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Jausenbrot.obj",
	  "Jausenbrot",
	  StreamCoordinates(-2300, 1100, 5000));
  
  sleep(1);
  
  
  /*
  AddItem(item_milk,
	  eng,
	  "applications_data/schoolgame/Items/Notizblock.obj",
	  "Notizblock",
	  StreamCoordinates(-2300, 1100, 9000));*/
  
  /*
  //====================================================================================
  MsgSceneNode stat;
  stat.SetModel("applications_data/schoolgame/Items/Milchpackerl.obj");
  stat.SetPosition(StreamCoordinates(3000, 900, 3000));
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
  */

  HeadManipulator targetManip;
  MsgSceneNode arrow;

  
  arrow.Material(0).SetTexture("data/Models/red.jpg");
  arrow.Material(0).SetLighting(true);    
  
  arrow.SetModel("applications_data/schoolgame/arrow.ms3d");
	
  arrow.SetPhysMode(2);
  arrow.SetScale(14.);
    
  arrow.SetRequestLoopBack(true);
  arrow.SetRequestMesh(false);
  eng.AddSceneNode(arrow, &targetManip);

  /*
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
  */
  
  CharGlobCtrl keyCtrl;

  for (i=0; i<all.Char().isize(); i++) {
    keyCtrl.AddFigure(all.Char()[i], all.Head()[i]);
  }

  keyCtrl[0].Read("applications_data/schoolgame/Mona.char");
  keyCtrl[1].Read("applications_data/schoolgame/Franz.char");
  keyCtrl[2].Read("applications_data/schoolgame/Georg.char");
  keyCtrl[3].Read("applications_data/schoolgame/Eva.char");
  keyCtrl[4].Read("applications_data/schoolgame/Mona.char");
  keyCtrl[5].Read("applications_data/schoolgame/Franz.char");
  
  //for (i=0; i<all.Item().isize(); i++) {
  //  keyCtrl.AddItem(all.Item()[i]);
  //}
 

  
  //keyCtrl.AddFigure(&manip2, &headManip);
  for (i=0; i<item_milk.isize(); i++)
    keyCtrl.AddItem(item_milk[i]);
  
  keyCtrl.SetTarget(&targetManip);
  
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
