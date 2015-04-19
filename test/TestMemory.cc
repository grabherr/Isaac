
#include <string>

#include "base/CommandLineParser.h"
#include "core/Memory.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  /*commandArg<string> aStringCmmd("-i","input file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the HMM and SOM.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  */


  int i;


  Memory mem;

  svec<double> cactus, water, tree, food, nothing;
  Reaction r_cactus, r_water, r_tree, r_food, r_nothing;

  cactus.resize(4, 0.);
  water.resize(4, 0.);
  tree.resize(4, 0.);
  food.resize(4, 0.);
  nothing.resize(4, 0.);

  cactus[0] = 1.;
  water[1] = 1.;
  tree[2] = 1.;
  food[3] = 1.;

  r_cactus.SetPain(0.9);
  r_water.SetPain(0.2);
  r_tree.SetPain(0.5);
  r_nothing.SetPain(0.1);

  r_food.SetReward(1.);

  r_cactus.SetUnsuccess(.7);
  r_tree.SetUnsuccess(1.0);
  
  cout << "Learning (iterate)" << endl;
  for (i=0; i<20; i++) {
    mem.LearnAct(nothing, r_nothing);
    mem.LearnAct(cactus, r_cactus);
    mem.LearnAct(tree, r_tree);
    mem.LearnAct(water, r_water);
    mem.LearnAct(food, r_food);
  }


  cout << "Retrieve: " << endl;
  Reaction r;
  mem.RetrieveAct(r, nothing);
  cout << "nothing:" << endl;
  r.Print();
  mem.RetrieveAct(r, food);
  cout << "food:" << endl;
  r.Print();
  mem.RetrieveAct(r, water);
  cout << "water:" << endl;
  r.Print();
  mem.RetrieveAct(r, tree);
  cout << "tree:" << endl;
  r.Print();
  mem.RetrieveAct(r, cactus);
  cout << "cactus:" << endl;
  r.Print();



  return 0;
}
