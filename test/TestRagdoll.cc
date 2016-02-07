
#include "physics/Physics.h"
#include "visual/Whiteboard.h"
#include "base/CommandLineParser.h"
#include <math.h>


void OneFrame(const string & filename, const PhysObject & o)
{
  double x_offset = 20;
  double y_offset = 20;


  int i, j, k;
  ns_whiteboard::whiteboard board;

 
  double x_max = 300.;
  double y_max = 300.;
  board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(0, 0), 
				      ns_whiteboard::xy_coords(2*x_offset+x_max, 2*y_offset+y_max), 
				      color(0.9,0.9,0.9)));


  for (i=0; i<o.GetConnectCount(); i++) {
    const PhysConnection & c = o.GetConnection(i);

    const PhysMinimal & a = o[c.GetFirst()];
    const PhysMinimal & b = o[c.GetSecond()];
     
   
    Coordinates x = a.GetPosition() + o.GetCenter().GetPosition();
    Coordinates y = b.GetPosition() + o.GetCenter().GetPosition();
    cout << "Draw " << a.GetPosition()[0] << " " << a.GetPosition()[1] << endl;

    x *= 140;
    y *= 140;
    
    x[0] = 0.707*(x[0]+x[2]);
    y[0] = 0.707*(y[0]+y[2]);

    x[0] += x_max/2.;
    y[0] += x_max/2.;
    //x[1] += y_max/2;
    //y[1] += y_max/2;

    if (x[1] < 0) {
      x[1] = 0;
      cout << "ERROR!!!" << endl;
    }
    if (y[1] < 0) {
      y[1] = 0;
      cout << "ERROR!!!" << endl;
    }

    board.Add( new ns_whiteboard::line( ns_whiteboard::xy_coords(x_offset+x[0], y_offset+x[1]), 
					ns_whiteboard::xy_coords(x_offset+y[0], y_offset+y[1]), 
					1.,
					color(0.,0.,0.)));
    
  }
 
  ofstream out(filename.c_str());
  
  ns_whiteboard::ps_display display(out, x_max + 2 * x_offset, y_max + 2 * y_offset);
  board.DisplayOn(&display);
 
}




int main(int argc,char** argv)
{
  int i, j;

  PhysObject o;

  o.SetPhysMode(0);

  PhysMinimal head;     //0
  PhysMinimal neck;     //1
  PhysMinimal shoulder; //2, 13
  PhysMinimal ellbow;   //3, 14
  PhysMinimal wrist;    //4, 15
  PhysMinimal finger;   //5, 16

  PhysMinimal torso1;   //6
  PhysMinimal torso2;   //7

  PhysMinimal pelvis;   //8
  PhysMinimal pelvis_joint;   //9, 17
  PhysMinimal knee;     //10, 18
  PhysMinimal ankle;    //11, 19
  PhysMinimal toe;      //12, 20
  
  head.SetPosition(Coordinates(0., 2., 0));
  o.Add(head);
  neck.SetPosition(Coordinates(0., 1.8, 0));
  o.Add(neck);
  shoulder.SetPosition(Coordinates(-0.3, 1.8, 0));
  o.Add(shoulder);
  ellbow.SetPosition(Coordinates(-0.5, 1.5, 0));
  o.Add(ellbow);
  wrist.SetPosition(Coordinates(-0.9, 1.4, 0));
  o.Add(wrist);
  finger.SetPosition(Coordinates(-1., 1.4, 0));
  o.Add(finger);
  torso1.SetPosition(Coordinates(0., 1.5, -0.1));
  o.Add(torso1);
  torso2.SetPosition(Coordinates(0., 1.2, 0));
  o.Add(torso2);
  pelvis.SetPosition(Coordinates(0., 1.0, 0));
  o.Add(pelvis);
  pelvis_joint.SetPosition(Coordinates(-0.3, 1.0, 0));
  o.Add(pelvis_joint);
  knee.SetPosition(Coordinates(-0.3, 0.4, 0.1));
  o.Add(knee);
  ankle.SetPosition(Coordinates(-0.3, 0.1, 0));
  o.Add(ankle);
  toe.SetPosition(Coordinates(-0.3, 0.1, 0.1));
  o.Add(toe);
  
  shoulder.SetPosition(Coordinates(0.3, 1.8, 0));
  o.Add(shoulder);
  ellbow.SetPosition(Coordinates(0.5, 1.5, 0));
  o.Add(ellbow);
  wrist.SetPosition(Coordinates(0.9, 1.4, 0));
  o.Add(wrist);
  finger.SetPosition(Coordinates(1., 1.4, 0));
  o.Add(finger);
  
  pelvis_joint.SetPosition(Coordinates(0.3, 1., 0));
  o.Add(pelvis_joint);
  knee.SetPosition(Coordinates(0.3, 0.4, 0.15));
  o.Add(knee);
  ankle.SetPosition(Coordinates(0.3, 0.1, 0));
  o.Add(ankle);
  toe.SetPosition(Coordinates(0.3, 0.1, 0.1));
  o.Add(toe);
  cout << "Points: " << o.isize() << endl;

  o.Connect(PhysConnection(0,1));
  o.Connect(PhysConnection(1,2));
  o.Connect(PhysConnection(1,13));
  o.Connect(PhysConnection(2,13));
  o.Connect(PhysConnection(2,3));
  o.Connect(PhysConnection(13,14));
  o.Connect(PhysConnection(3,4));
  o.Connect(PhysConnection(14,15));
  o.Connect(PhysConnection(4,5));
  o.Connect(PhysConnection(15,16));
  o.Connect(PhysConnection(1,6));
  o.Connect(PhysConnection(6,7));
  o.Connect(PhysConnection(7,8));
  o.Connect(PhysConnection(8,9));
  o.Connect(PhysConnection(8,17));
  o.Connect(PhysConnection(9,17));
  o.Connect(PhysConnection(9,10));
  o.Connect(PhysConnection(10,11));
  o.Connect(PhysConnection(11,12));
  o.Connect(PhysConnection(17,18));
  o.Connect(PhysConnection(18,19));
  o.Connect(PhysConnection(19,20));


  for (i=0; i<o.isize(); i++) {
    PhysMinimal & b = o[i];
    b.SetMass(1.);
    cout << "Print " << i << " " << b.GetPosition()[0] << " " << b.GetPosition()[1] << endl;
  }
  o.Fixate();
  SolidTriangle tri1, tri2;
  tri1.Set(Coordinates(-10, 0 -10),
	   Coordinates(-10, 0, +10),
	   Coordinates(+10, 0, -10));
  tri2.Set(Coordinates(+10, 0, +10),
	   Coordinates(-10, 0, +10),
	   Coordinates(+10, 0, -10));

  for (i=1000; i<1600; i++) {
    cout << "Update: " << i << endl;
    o.Update(0.02, 9.81);
    tri1.Collide(o);
    tri2.Collide(o);

    if (i == 1100)
      o[0].SetVelocity(Coordinates(0.3, 7., 2));

    char name[256];
    sprintf(name, "tmp%d.ps", i);
    cout << "Write to " << name << endl;
    OneFrame(name, o);
  }

  return 0;
}
