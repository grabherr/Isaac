
#include <string>

#include "base/CommandLineParser.h"
#include "world/Simple2D.h"
#include "base/FileParser.h"
#include "base/RandomStuff.h"
#include "creature/LongTermMem.h"
#include "world/Playground.h"
#include "creature/MatchSOMs.h"

int main(int argc,char** argv)
{
  int i;

  Playground playground(10, 10);
  Entity food, isaac, cactus, tree, water, empty;
 
  playground.MakeEntity(food, FOOD);
  playground.MakeEntity(isaac, ISAAC);
  playground.MakeEntity(cactus, CACTUS);
  playground.MakeEntity(tree, TREE);
  playground.MakeEntity(water, WATER);
  playground.MakeEntity(empty, EMPTY);


  //Properties templ = playground.EntityTemplate();
  //templ.resize(4.);

  Reaction good, bad;

  bad.SetPain(0.9);
  bad.SetReward(0.);

  good.SetPain(0.0);
  good.SetReward(0.9);
  
  cout << "Start feeding..." << endl;

 
  MemoryUnit a, b;
  Action action;
  action.Add("forward", 0.);
  action.Add("backward", 0.);

  Reaction r;
  Coordinates cc;

  //a.Set(Entity(food.GetProperties(), cc), action, r, cc);
  //b.Set(Entity(enemy.GetProperties(), cc), action, r, cc);
  a.Set(food, action, r, cc);
  b.Set(cactus, action, r, cc);
  a.Enable(true, true, true, true);
  b.Enable(true, true, true, true);

  ShortTermMemory stmem(3, a, 5, 5);
  LongTermMemory ltmem(10, a, 15, 5);

  cout << "Feeding" << endl;
  stmem.Feed(a, good);
  stmem.Propagate();
  stmem.Feed(a, good);
  stmem.Propagate();
  stmem.Feed(a, good);
  stmem.Propagate();
  stmem.Feed(a, good);
  stmem.Propagate();
  stmem.Feed(b, bad);
  stmem.Propagate();
  stmem.Feed(b, bad);
  stmem.Propagate();
  stmem.Feed(b, bad);
  stmem.Propagate();
  stmem.Feed(b, bad);
  stmem.Propagate();
 
  cout << "Feeding to long-term memory." << endl;
  for (i=0; i<stmem.isize(); i++) {
    ltmem.Feed(stmem[i], r);
  }
  cout << "Done." << endl;

  stmem.Print();

  svec<Reaction> react; 
  svec<MemoryUnit> mu;
  stmem.MemoryDump(react, mu, a);
  
  for (i=0; i<react.isize(); i++) {
    cout << "Frame " << i << endl;
    cout << "Memory unit" << endl;
    mu[i].Print();
    cout << "Reaction" << endl;
    react[i].Print();
  }
  
  return 0;
}
