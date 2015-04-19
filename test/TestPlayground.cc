
#include <string>

#include "base/CommandLineParser.h"
#include "core/SOM.h"
#include "core/HMM.h"
#include "world/Playground.h"
#include "base/FileParser.h"
#include "base/RandomStuff.h"
#include "creature/Isaac.h"
#include "visual/Whiteboard.h"

void Visual(const Playground & world, const Coordinates & botPos, bool bPain, const string & fileName)
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

  Playground world(30, 30);
  char tmp1[256];
  //sprintf(tmp1, "data/bot%d.ps", k);
  //k++;
  //Visual(world, botPos, false, tmp1);



  for (int i=0; i<world.SizeX(); i++) {
    for (int j=0; j<world.SizeX(); j++) {
      if (RandomFloat(1.) < 0.03) {
	world.Set(i, j, TREE);
      }
      if (RandomFloat(1.) < 0.05) {
	world.Set(i, j, WATER);
      }
      if (RandomFloat(1.) < 0.01) {
	world.Set(i, j, CACTUS);
      }
    }
  }


  svec<Isaac> isaac;

  isaac.resize(2);
 
  for (i=0; i<isaac.isize(); i++) {
    botPos.SetX(RandomInt(20));
    botPos.SetY(RandomInt(20));
    isaac[i].SetTemplate(world.EntityTemplate()); 
    world.RegisterIsaac(&isaac[i], botPos);
  }

  cout << "Training." << endl;
  for (i=0; i<10; i++) {
    Entity test;
    Reaction train;
    Reaction trainfriend;
    world.MakeEntity(test, EMPTY);
    cout << test.isize() << endl;
    train.SetPain(0.1);
    train.SetReward(0.);

    cout << "Feedback" << endl;
    isaac[0].Feedback(train, test, botPos);
    cout << "Done." << endl;
    

    world.MakeEntity(test, FOOD);
    train.SetPain(0.);
    train.SetReward(1.);

    Entity friendly;
    world.MakeEntity(friendly, ISAAC);

    trainfriend.SetPain(0.0);
    trainfriend.SetReward(0.4);
    isaac[0].Feedback(train, test, botPos);
    isaac[0].Feedback(train, test, botPos);
    isaac[0].Feedback(train, test, botPos);
    isaac[0].Feedback(train, test, botPos);
 
    isaac[1].Feedback(trainfriend, friendly, botPos);
    isaac[1].Feedback(trainfriend, friendly, botPos);
    isaac[1].Feedback(trainfriend, friendly, botPos);
  }

  bool bCont = false;
  cout << "Start." << endl;
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

      for (j=0; j<isaac.isize(); j++) {
	svec<Entity> ent;
	cout << "Unit # " << isaac[j].GetHandle() << endl;
	world.GetEntities(ent, isaac[j].GetHandle());
	cout << "Entities: " << ent.isize() << endl;
	//cout << "Current bot pos: " << botPos.X() << " " << botPos.Y() << endl;
	isaac[j].Feed(ent);
      
	PlaygroundAction action;
	isaac[j].GetAction(action);
      
	Reaction outcome;
	Entity ee;
	world.Try(outcome, ee, isaac[j].GetHandle(), action);
	isaac[j].Feedback(outcome, ee, botPos);
	char tmp[256];
	sprintf(tmp, "data/bot%d.ps", k);
	k++;
	bool b = false;
	if (outcome.Pain() > 0.2)
	  b = true;
      }
      //Visual(world, botPos, b, tmp);
    }
  }

  return 0;
}
