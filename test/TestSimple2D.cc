
#include <string>

#include "base/CommandLineParser.h"
#include "core/SOM.h"
#include "core/HMM.h"
#include "world/Simple2D.h"
#include "base/FileParser.h"
#include "base/RandomStuff.h"
#include "core/SimpleBot.h"
#include "core/HMMBot.h"
#include "core/MemBot.h"
#include "visual/Whiteboard.h"

void Visual(const WorldSimple2D & world, const Coordinates & botPos, bool bPain, const string & fileName)
{
  int i, j;
  ns_whiteboard::whiteboard board;

  double offset = 20.;
  double x_max = 300;
  double y_max = 300;
  double scale = 10.;

  double bX = scale * botPos.X();
  double bY = scale * botPos.Y();

  for (i=0; i<world.SizeX(); i++) {
    for (j=0; j<world.SizeY(); j++) {
      char c = world.Get(i, j);
      double x = i*scale;
      double y = j*scale;

      color col;
      if (c == EMPTY)
	col = color(0.8, 0.8, 0.8);
      if (c == CACTUS)
	col = color(0., 0.8, .0);
      if (c == TREE)
	col = color(0.1, 0.1, .1);
      if (c == WATER)
	col = color(0., 0., .9);
      if (c == FOOD)
	col = color(0.8, 0.7, .0);

      board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(x + offset, y + offset + scale), 
					  ns_whiteboard::xy_coords(x + offset + scale, y + offset),
					  col) );

    }
  }
  color colB = color(0.99, 0.6, 0.6);
  if (bPain)
     colB = color(0.9, 0., 0.0);
 
  board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(bX + offset, bY + offset + scale), 
				      ns_whiteboard::xy_coords(bX + offset + scale, bY + offset),
				      colB) );

  ofstream out(fileName.c_str());  
  ns_whiteboard::ps_display display(out, x_max + 2 * offset, y_max + 2 * offset);
  board.DisplayOn(&display);
 

}

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
  int k = 1000;


  int i, j;
  Coordinates botPos;
  botPos.SetX(2);
  botPos.SetY(2);

  WorldSimple2D world(30, 30);
  char tmp1[256];
  sprintf(tmp1, "data/bot%d.ps", k);
  k++;
  Visual(world, botPos, false, tmp1);



  for (int i=4; i<world.SizeX(); i++) {
    for (int j=4; j<world.SizeX(); j++) {
      if (RandomFloat(1.) < 0.07) {
	world.Set(i, j, TREE);
      }
      if (RandomFloat(1.) < 0.1) {
	world.Set(i, j, WATER);
      }
      if (RandomFloat(1.) < 0.08) {
	world.Set(i, j, CACTUS);
      }
    }
  }
  world.Set(17, 9, CACTUS);
  world.Set(17, 10, CACTUS);
  world.Set(17, 11, CACTUS);
  world.Set(17, 12, CACTUS);
  world.Set(17, 13, CACTUS);
  world.Set(17, 14, CACTUS);
  world.Set(17, 15, CACTUS);
  world.Set(17, 16, CACTUS);
  world.Set(17, 17, CACTUS);
  world.Set(17, 18, CACTUS);
  world.Set(17, 19, CACTUS);
  world.Set(17, 20, CACTUS);
  world.Set(17, 21, CACTUS);
  world.Set(17, 22, CACTUS);
  world.Set(17, 23, CACTUS);
  world.Set(17, 24, CACTUS);
  world.Set(17, 25, CACTUS);

  SimpleBot simplebot;
  HMMBot hmmbot;
  MemBot membot;
 
  //IAutonomosUnit * pIAU = &simplebot;
  //IAutonomosUnit * pIAU = &hmmbot;
  IAutonomosUnit * pIAU = &membot;

  world.Register(pIAU, botPos);
   
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

  for (i=0; i<6; i++) {
    botPos.SetX(2);
    botPos.SetY(2);
    cout << "TRY " << i << endl;
    world.Set(24, 20-i, CACTUS);
    world.Set(24, 21-i, CACTUS);
    world.Set(24, 22-i, CACTUS);

    if (i >= 3) {
      bCont = true;
      if (i == 5) {
	world.Set(11, 26, CACTUS);
	world.Set(11, 27, CACTUS);
	world.Set(11, 28, CACTUS);
      }
    } else {
      world.Set(25, 20-i, FOOD);
      world.Set(25, 21-i, FOOD);
      world.Set(25, 22-i, FOOD);
      
    }


    while (world.Food() > 0 || bCont) {
      world.Print();
      cout << "Round " << i << endl;
 
      if (i < 5 && bCont && botPos.Y() > 15) {
	world.Set(25, 20, FOOD);
	world.Set(25, 21, FOOD);
	world.Set(25, 22, FOOD);
	cout << "FOOD: " << world.Food() << endl;
	bCont = false;
      }
      if (i == 5 && bCont && botPos.Y() > 24) {
	world.Set(12, 26, FOOD);
	world.Set(12, 27, FOOD);
	world.Set(12, 28, FOOD);
	cout << "FOOD: " << world.Food() << endl;
	bCont = false;
      }

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
      char tmp[256];
      sprintf(tmp, "data/bot%d.ps", k);
      k++;
      bool b = false;
      if (outcome.Pain() > 0.2)
	b = true;
      Visual(world, botPos, b, tmp);
    }
  }

  return 0;
}
