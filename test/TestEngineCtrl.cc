
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "util/Keyboard.h"

#include "base/CommandLineParser.h"
#include "graphics/EngineComm.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  GUIEngineControl ectrl;

  AnimatedSceneNode m;
  Coordinates cc;
  cc[0] = 2700*2 - 230;
  cc[1] = 255*2;
  cc[2] = 2600*2;
  m.SetCoordinates(cc);
  m.SetModel("../../media/faerie.md2");
  m.SetTexture("../../media/faerie2.bmp");

  cout << "Add" << endl;
  sleep(5);
  ectrl.AddAnimatedNode(m);

  while (true) {
    DataPacket d;
    while (ectrl.GetDataPacket(d)) {  
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
    usleep(1000);
    
    cc[1] += 1.;
    m.SetCoordinates(cc);
    ectrl.UpdateAnimatedNode(m);

  }

  return 0;
}
