
#include <string>

#include "base/CommandLineParser.h"
#include "creature/Brain.h"
#include "world/SomeEntities.h"


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
  Entity empty = GetEmpty();
  MemoryUnit mem_empty(empty);
  cout << "Size: " << mem_empty.isize() << endl;
  Brain brain(mem_empty);

  Entity cactus = GetCactus();
  Entity tree = GetTree();
  Entity bush = GetBush();
  Entity fruit = GetFruit();
  Entity creature = GetCreature();
  Entity insect = GetInsect();
  
  tree.SetCoordinates(Coordinates(10., 5, 0.));
  cactus.SetCoordinates(Coordinates(2., 5, 0.));
  bush.SetCoordinates(Coordinates(-5., 8, 0.));
  fruit.SetCoordinates(Coordinates(-5., 8, 0.));
  insect.SetCoordinates(Coordinates(-3., 7, 0.));
  creature.SetCoordinates(Coordinates(0.01, 0., 0.));
  MemoryUnit test = MemoryUnit(cactus);
  test.Print();
  MemoryUnit test2 = MemoryUnit(fruit);
  test2.Print();
 
  //brain.Feed(MemoryUnit(cactus));
  //brain.DoneFeed();
  //return 0;

  int i;
  for (i=0; i<12; i++) {
    cout << "Start feed..." << endl;
    brain.StartFeed();
    
    cout << "Feed entities..." << endl;
    brain.Feed(MemoryUnit(cactus));
    //brain.Feed(MemoryUnit(tree));
    //brain.Feed(MemoryUnit(bush));
    brain.Feed(MemoryUnit(fruit));
    //brain.Feed(MemoryUnit(insect));
    cout << "Feed me..." << endl;
    brain.FeedMe(MemoryUnit(creature));
    
    cout << "Done feed..." << endl;
    brain.DoneFeed();
    
    MemoryUnit out;
    cout << "Get action..." << endl;
    brain.NextAction(out);
    
    out.Print();
  }

  return 0;
}
