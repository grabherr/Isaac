
#include <string>

#include "base/CommandLineParser.h"
#include "core/SOM.h"
#include "core/HMM.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  commandArg<string> aStringCmmd("-i","input file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the HMM and SOM.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);

  int i;

  return 0;
}
