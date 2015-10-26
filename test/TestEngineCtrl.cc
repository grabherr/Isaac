
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
  ectrl.SetGraphicsEngine("/home/manfred/Work/irrlicht-trunk/bin/Linux/32.IrrlichtServer");

  MsgTerrain terr;
  terr.Set("/home/manfred/Work/Models/Terrain/terrain-heightmap-empty.bmp",
	   "/home/manfred/Work/Bots/Images/sweden1.jpg",
	   "/home/manfred/Work/irrlicht-trunk/media/detailmap3.jpg",
	   "/home/manfred/Work/Bots/Images/bluesky_up.jpg",
	   "/home/manfred/Work/Bots/Images/bluesky_dn.jpg",
	   "/home/manfred/Work/Bots/Images/bluesky_lf.jpg",
	   "/home/manfred/Work/Bots/Images/bluesky_rt.jpg",
	   "/home/manfred/Work/Bots/Images/bluesky_ft.jpg",
	   "/home/manfred/Work/Bots/Images/bluesky_bk.jpg",
	   "/home/manfred/Work/irrlicht-trunk/media/skydome.jpg");


  ectrl.SetTerrain(terr);

  ectrl.StartGraphics(800, 600, false);

  AnimatedSceneNode m;
  Coordinates cc;
  cc[0] = 2700*2 - 230;
  cc[1] = 0;
  cc[2] = 2600*2;
  m.SetCoordinates(cc);

  m.SetModel("/home/manfred/Work/Models/snake/snake.md2");
  m.SetTexture("/home/manfred/Work/Models/snake/snake.jpg");
  m.SetAnimation("attack");
  m.SetAnimationSpeed(45);
  m.SetName("1");

  AnimatedSceneNode m2;
  Coordinates cc2;
  cc2[0] = 2700*2 - 230;
  cc2[1] = 0;
  cc2[2] = 2600*2;
  m2.SetCoordinates(cc2);
  Coordinates dir2;
  dir2[0] = 0;
  dir2[1] = 0;
  dir2[2] = 1;
  m2.SetDirection(dir2);

  m2.SetModel("/home/manfred/Work/Models/Monkey/monkey.md2");
  m2.SetTexture("/home/manfred/Work/Models/Monkey/brute.jpg");
  m2.SetAnimation("attack");
  m2.SetAnimationSpeed(45);
  m2.SetName("2");


  SceneNode t;
  Coordinates cc1;
  cc1[0] = 700;
  cc1[1] = 0;
  cc1[2] = 2900;
  t.SetCoordinates(cc1);

  t.SetMesh("/home/manfred/Work/Bots/Models/Elm/Elm5.3ds");
  t.SetTexture1("/home/manfred/Work/Bots/Models/Elm/iTrees-02_Bark-WillowOld-02.jpg");
  t.SetTexture2("/home/manfred/Work/Bots/Models/Elm/itrees-02-leaf_002.jpg");
 

  ectrl.AddAnimatedNode(m);
  ectrl.AddAnimatedNode(m2);
  ectrl.AddNode(t);
  cout << "Add" << endl;

  int k = 0;

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
      //cout << msg << " " << x << " " << y << " " << z << endl;
    }
    usleep(1000);
    k++;
    cc[0] += 0.1;
    cc2[2] += 0.1;
    m.SetCoordinates(cc);
    m2.SetCoordinates(cc2);
    if (k == 3000) {
      cout << "RUN" << endl;
      m.SetAnimation("run");
    }
    if (k == 5000) {
      cout << "IDLE" << endl;
      m.SetAnimation("idle");
    }
   if (k == 6000) {
      cout << "IDLE" << endl;
      m.SetAnimation("attack");
    }
    ectrl.UpdateAnimatedNode(m);
    ectrl.UpdateAnimatedNode(m2);

  }

  return 0;
}
