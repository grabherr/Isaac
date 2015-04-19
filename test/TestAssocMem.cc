
#include <string>

#include "base/CommandLineParser.h"
#include "world/Simple2D.h"
#include "core/SpatAssocMem.h"
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
  botPos.SetX(0);
  botPos.SetY(0);

  WorldSimple2D world_train(5, 5);
  WorldSimple2D world_test(5, 5);
  char tmp1[256];
  sprintf(tmp1, "data/bot%d.ps", k);
  k++;
  //Visual(world, botPos, false, tmp1);

  world_train.Set(1, 1, TREE);
  world_train.Set(2, 1, TREE);
  world_train.Set(3, 1, TREE);
  world_train.Set(2, 2, CACTUS);
  world_train.Set(2, 3, CACTUS);
  
  world_test.Set(1, 1, TREE);
  world_test.Set(2, 1, TREE);
  world_test.Set(3, 1, TREE);
  //world_train.Set(2, 2, CACTUS);
  //world_train.Set(2, 3, CACTUS);
 	/*
  for (int i=0; i<world.SizeX(); i++) {
    for (int j=0; j<world.SizeX(); j++) {
      if (RandomFloat(1.) < 0.17) {
	world.Set(i, j, TREE);
	if (j > 1)
	  world.Set(i, j-1, WATER);
      }
      if (RandomFloat(1.) < 0.1) {
	world.Set(i, j, WATER);
      }
      if (RandomFloat(1.) < 0.08) {
	world.Set(i, j, CACTUS);
      }
    }
    }*/

  Entity cactus;
  cactus[3] = 1.;

  AssociativeMemory amem;
  Entity dummy;
  amem.Setup(4, 4);
  cout << "Training." << endl;
  world_train.Print();
  svec<Entity> ent, out;
  world_train.GetEntities(ent, botPos);
  cout << "Call Assoc." << endl;
  amem.Associate(out, ent, cactus);
  cout << "Print world entities (train)." << endl;
  for (i=0; i<ent.isize(); i++) {
    const Entity & ee = ent[i];
    cout << "x=" << ee.GetCoordinates().X() << " y=" <<  ee.GetCoordinates().Y() << endl;
    for (j=0; j<ee.isize(); j++) 
      cout << j << " " << ee[j] << endl;
  }

  cout << "Testing." << endl;
  world_test.Print();
  ent.clear();
  out.clear();
  world_test.GetEntities(ent, botPos);
  cout << "Call Assoc." << endl;
  amem.Associate(out, ent, cactus);
  //amem.Associate(out, ent);
  //amem.Associate(out, ent);
  //amem.Associate(out, ent);
  //amem.Associate(out, ent);
  cout << "Print world entities (test)." << endl;
  for (i=0; i<ent.isize(); i++) {
    const Entity & ee = ent[i];
    cout << "x=" << ee.GetCoordinates().X() << " y=" <<  ee.GetCoordinates().Y() << endl;
    for (j=0; j<ee.isize(); j++) 
      cout << j << " " << ee[j] << endl;
  }
  cout << "Print world entities (assoc)." << endl;
  for (i=0; i<out.isize(); i++) {
    const Entity & ee = out[i];
    cout << "x=" << ee.GetCoordinates().X() << " y=" <<  ee.GetCoordinates().Y() << endl;
    for (j=0; j<ee.isize(); j++) 
      cout << j << " " << ee[j] << endl;
  }

  
  cout << "All done" << endl;
  return 0;
}
