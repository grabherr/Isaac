
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the game engine.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);

  MsgLightNode light;
  light.SetPosition(StreamCoordinates(7000, 1600, 7000));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(7000, 1800, 7000));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(7000, 300, 7000));
  eng.AddLight(light);

  eng.Run();


  return 0;
}
