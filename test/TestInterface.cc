
#include <string>

#include "base/CommandLineParser.h"
#include "interface/iau.h"


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

  IAU * pIAU = CreateIAU();
  pIAU->AddCapability("run", 1.);
  
  int i, j;
  RelativeCoordinates relPos;
  RelativeCoordinates preyPos;
  
  preyPos.Set(20, 10, 0);


  Element prey_learn;
  ElementProperties prop_learn;
  prop_learn[0] = 1.;
  prey_learn.SetProperties(prop_learn);
  prey_learn.SetCoordinates(preyPos);
 
  pIAU->Hint(prey_learn, 0., 1., 0.);

  for (i=0; i<100; i++) {
    double pain = 0.;
    double reward = 0.;
    double collision = 0.;

    double health = 1.;

    pIAU->StartFrame();
    pIAU->SetIAUDirection(relPos);
    pIAU->SetIAUStatus(pain, 
		       reward, 
		       collision);
    pIAU->SetIAUHealth(health);

    // Add one element
    Element prey;
    ElementProperties prop;
    prop[0] = 1.;
    prey.SetProperties(prop);
    prey.SetCoordinates(preyPos);
    pIAU->AddElement(prey);
 
    pIAU->CloseFrame();

    RelativeCoordinates move = pIAU->GetDirection();
    preyPos -= move;

 }

  DestroyIAU(pIAU);

  return 0;
}
