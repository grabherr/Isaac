
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"


int main(int argc,char** argv)
{
  // Use the argument parser to get values
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Example source code.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  // Retrieve values for the configuration file and scale
  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);

  // Run the engine
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);

  eng.Run();


  return 0;
}
