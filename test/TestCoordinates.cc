
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
  
  k[0] = 1;
  k[1] = 0;
  k[2] = 0;
  
  double phi = 0.;
  double theta = 0.;
  for (i=0; i<1000; i++) {
    phi = i/100.;
    double x = sin(phi);
    double y = cos(phi);
    double theta = atan(x/y);
    if (y < 0.)
      theta += 3.1415926;
    if (theta < 0.)
      theta += 2*3.1415926;
     
    cout << "phi " << phi << " x " << x << " y " << y << " theta " << theta << endl;
  }

  for (i=0; i<1000; i++) {
    SphereCoordinates s = k.AsSphere();
    cout << s.r() << " " << s.phi() << " " << s.theta()-PI_P/2. << endl;
    s.SetPhi(s.phi()+0.01);
    k.FromSphere(s);
    cout << k[0] << " " << k[1] << " " << k[2] << endl;
  }

  //if (s.phi() > PI_P/2)
  //cout << s.phi() - PI_P  << endl;
    
  return 0;
}
