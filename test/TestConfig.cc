
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameConfig.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the HMM and SOM.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  GameConfig cc;

  cc.Read(aString);

  

  return 0;
}
