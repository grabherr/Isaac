#include <string>
#include "base/CommandLineParser.h"
#include "npc/Motivator.h"

int main( int argc, char** argv )
{
  /*
  commandArg<string> fileCmmd("-i","input file");
  commandArg<string> oCmmd("-o","output file");
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(oCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string outName = P.GetStringValueFor(oCmmd);
  */

  Motivator m;

  svec<EnvFactor> env;
  env.resize(2);
  env[0].SetName("punch");
  env[0].SetVal(-0.25);
  env[0].SetName("food");
  env[1].SetVal(0.);

  m.Setup(3, env.isize());
  m[0].SetName("happy");
  m[1].SetName("painfree");
  m[2].SetName("dominant");

  m[0].SetVal(0.8);
  m[1].SetVal(1.0);
  m[2].SetVal(0.6);

  m.AddToMatrix("painfree", env);

  env[0].SetVal(0.);
  env[1].SetVal(0.1);
  m.AddToMatrix("happy", env);
  
  return 0;
}
