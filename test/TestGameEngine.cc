
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the game engine.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);

  eng.SetupMap(0);
  eng.Run();


  return 0;
}
