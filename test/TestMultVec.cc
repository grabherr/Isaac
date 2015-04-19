
#include <string>

#include "base/CommandLineParser.h"
#include "base/MultVector.h"
#include "base/FileParser.h"
#include "core/Cerebellum.h"

int main(int argc,char** argv)
{

  commandArg<string> aStringCmmd("-i","input file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the HMM and SOM.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);

  int i;

  multvec<double> vec;
  
  vec.resize(10, 10, 10);

  double a = vec[20];
  vec(3, 3, 5) = 10.;
  cout << vec(3, 3, 5) << endl;

  Cerebellum c;
  c.Setup(20, 20);
  c.SetCost(0., 10, 19);
  c.SetCost(5., 9, 5);
  c.SetCost(5., 10, 5);
  c.SetCost(5., 11, 5);
    
  svec<PathItem> out;
  c.FindPath(out, 10, 0, 0, 0, 10, 19, 0, 0);
    
  return 0;
}
