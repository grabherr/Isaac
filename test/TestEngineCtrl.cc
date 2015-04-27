
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "util/Keyboard.h"

#include "base/CommandLineParser.h"
#include "graphics/EngineComm.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  /*commandArg<string> aStringCmmd("-hostname","name of the server running the host");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the socket communication between computers.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);

  int i;
  int max = 10000000;
 

  char c = (char)get_kbhit();


  StreamCommTransmitter * pTrans = GetTransmitter(aString, DEFPORT);
  StreamCommReceiver * pRec = GetReceiver(DEFPORT + 1);
 
  while (true) {
    DataPacket d;
    while (pRec->Get(d)) {  
      string msg;
      double x, y, z;
      d.Read(msg);
      d.Read(x);
      d.Read(y);
      d.Read(z);
      cout << msg << " " << x << " " << y << " " << z << endl;
    }
    
    DataPacket f;
    f.Write("control");
    f.Write("left");
    pTrans->Send(f);
    usleep(100);

  }
  */

  return 0;
}
