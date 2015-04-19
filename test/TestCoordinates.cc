
#include <string>

#include "base/CommandLineParser.h"
#include "physics/Coordinates.h"
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

  Coordinates k;
  
  k[0] = -1;
  k[1] = 1;
  k[2] = -1;

  SphereCoordinates s = k.AsSphere();
  cout << s.r() << " " << s.phi() << " " << s.theta()-PI_P/2. << endl;
  if (s.phi() > PI_P/2)
    cout << s.phi() - PI_P  << endl;
    
  return 0;
}
