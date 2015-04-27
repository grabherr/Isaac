
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "util/Keyboard.h"

#include "base/CommandLineParser.h"
#include "util/StreamComm.h"
#include "graphics/Messages.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  commandArg<string> aStringCmmd("-hostname","name of the server running the host");

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
 
  sleep(5);
  cout << "Adding fairy." << endl;

  DataPacket fairy;
  MessageHeader header;
  header.SetHeader("animatedmodel");
  header.ToPacket(fairy);
  Model m;
  Coordinates cc;
  cc[0] = 2700*2 - 230;
  cc[1] = 255*2;
  cc[2] = 2600*2;
  m.SetCoordinates(cc);
  m.SetModel("../../media/faerie.md2");
  m.SetTexture("../../media/faerie2.bmp");
  m.ToPacket(fairy);
  pTrans->Send(fairy);

  MessageHeader generic;

  while (true) {
    DataPacket d;
    while (pRec->Get(d)) {  
      string msg;
      double x, y, z;
      MessageHeader tmp;
      tmp.FromPacket(d);
      d.Read(msg);
      d.Read(x);
      d.Read(y);
      d.Read(z);
      cout << msg << " " << x << " " << y << " " << z << endl;
    }
    
    DataPacket f;
    generic.ToPacket(f);
    f.Write("control");
    f.Write("left");
    pTrans->Send(f);
    usleep(100);

  }

  return 0;
}
