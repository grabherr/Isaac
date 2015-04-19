
#include <string>

#include "base/CommandLineParser.h"
#include "world/Simple2D.h"
#include "base/FileParser.h"
#include "base/RandomStuff.h"
#include "creature/Isaac.h"


int main(int argc,char** argv)
{
  /*
  commandArg<string> aStringCmmd("-i","input file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the HMM and SOM.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  */
  /*
  int k = 1000;


  int i, j;
  Coordinates botPos;
  botPos.SetX(2);
  botPos.SetY(2);

  WorldSimple2D world(30, 30);


  // Let's randomply populate the world
  for (i=4; i<world.SizeX(); i++) {
    for (j=4; j<world.SizeX(); j++) {
      if (RandomFloat(1.) < 0.07) {
	world.Set(i, j, TREE);
      }
      if (RandomFloat(1.) < 0.1) {
	world.Set(i, j, WATER);
      }
      if (RandomFloat(1.) < 0.08) {
	world.Set(i, j, CACTUS);
      }
      if (RandomFloat(1.) < 0.02) {
	world.Set(i, j, FOOD);
      }
    }
  }
 
  Isaac isaac;
 
  IAutonomosUnit * pIAU = &isaac;

  world.Register(pIAU, botPos);
   
  // Let's teach him a few things upfront
  for (i=0; i<10; i++) {
    Entity test;
    Reaction train;
    world.MakeEntity(test, EMPTY);
    train.SetPain(0.1);
    train.SetReward(0.);
    pIAU->Feedback(train, test, botPos);
    
    world.MakeEntity(test, FOOD);
    train.SetPain(0.);
    train.SetReward(1.);
    pIAU->Feedback(train, test, botPos);
    pIAU->Feedback(train, test, botPos);
    pIAU->Feedback(train, test, botPos);
    pIAU->Feedback(train, test, botPos);
  }

  bool bCont = false;

  // Let's do this 10 times
  for (i=0; i<10; i++) {
    // Let's add food.
    for (int x=4; x<world.SizeX(); x++) {
      for (int y=4; y<world.SizeX(); y++) {
	if (RandomFloat(1.) < 0.02) {
	  world.Set(x, y, FOOD);
	}
      }
    }


    botPos.SetX(2);
    botPos.SetY(2);
    cout << "TRY " << i << endl;

    while (world.Food() > 0 || bCont) {
      world.Print();
      cout << "Round " << i << endl;
 
      svec<Entity> ent;
      world.GetEntities(ent, botPos);
      cout << "Entities: " << ent.isize() << endl;
      cout << "Current bot pos: " << botPos.X() << " " << botPos.Y() << endl;
      pIAU->Feed(ent);
      
      Simple2DAction action;
      pIAU->GetAction(action);
      
      Reaction outcome;
      Entity ee;
      world.Try(outcome, botPos, ee, pIAU->GetHandle(), action);
      pIAU->Feedback(outcome, ee, botPos);
      k++;
    }
    }*/

  return 0;
}
