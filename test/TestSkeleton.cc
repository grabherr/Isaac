
#include "visual/Whiteboard.h"

#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "base/SVector.h"
#include "visual/Color.h"
#include "npc/Skeleton.h"


void OneFrame(const string & o, const NPCSkeleton & t)
{
  double x_offset = 20;
  double y_offset = 20;


  int i, j, k;
  ns_whiteboard::whiteboard board;

 
  double x_max = 400.;
  double y_max = 400.;
  double scale = 200.;

  board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(0, 0), 
				      ns_whiteboard::xy_coords(2*x_offset+x_max, 2*y_offset+y_max), 
				      color(0.9,0.9,0.9)));


  for (i=0; i<t.isize(); i++) {
    const NPCBone & b = t[i];
    b.Print();

    if (b.GetParent() < 0) {
      cout << "No parent, i=" << i << endl;
      continue;
    }
    const NPCBone & p = t[b.GetParent()];
    double x1 = scale*p.GetCoords().X()+x_max/2;
    double y1 = scale*p.GetCoords().Y()+y_max/2;
    //double x1 = scale*p.GetBase().X()+x_max/2;
    //double y1 = scale*p.GetBase().Y()+y_max/2;
    double x2 = scale*b.GetCoords().X()+x_max/2;
    double y2 = scale*b.GetCoords().Y()+y_max/2;
    cout << "Draw " << x1 << " " << y1 << " - " << x2 << " " << y2 << endl;
    cout << "Raw ";
    b.GetCoords().Print();

    board.Add( new ns_whiteboard::line( ns_whiteboard::xy_coords(x_offset+x1, y_offset+y1), 
					ns_whiteboard::xy_coords(x_offset+x2, y_offset+y2), 
					b.GetWidth(),
					color(0.,0.,0.)));
  }
 
  ofstream out(o.c_str());
  
  ns_whiteboard::ps_display display(out, x_max + 2 * x_offset, y_max + 2 * y_offset);
  board.DisplayOn(&display);
 
}


int main( int argc, char** argv )
{
  /*
  commandArg<string> iStringO("-i","input");
  commandArg<string> aStringO("-o","outfile (post-script)");
 
  
  commandLineParser P(argc,argv);
  P.SetDescription("Animation example");

  P.registerArg(iStringO);
  P.registerArg(aStringO);

  P.parse();
  */

  //SimpleTree tree;
  //tree.AddTrunk(Coordinates(250,0,0), Coordinates(250,20,0));
  /*
  NPCBoneCoords coords;
  coords.SetBase(Coordinates(1, 1, 1));
  coords.Set(1, PI_P/2, PI_P/2, PI_P/2); 
  //coords.Set(1, 0, PI_P/2, 0); 
  cout << "Get the coordinates." << endl;
  Coordinates eu = coords.GetCoords();
  cout << "Print the coordinates." << endl;
  eu.Print();
  return 0;*/


  int i, j;
  NPCSkeleton s;

  /*
  RotMatrix r1, r2;

  r1.Set(0, 0, 1.5);
  r1.Set(1, 1, 1.5);
  r1.Set(2, 2, 1.5);

  r2.Set(0, 0, 1.);
  r2.Set(1, 0, 1.);
  r2.Set(2, 0, 1.);
  
  r2.Set(0, 1, 1.);
  r2.Set(1, 1, 1.);
  r2.Set(2, 1, 1.);
 
  r2.Set(0, 2, 1.);
  r2.Set(1, 2, 1.);
  r2.Set(2, 2, 1.);

  Coordinates cc(1., 0., 0);

  RotMatrix m = r1.MultMat(r2);
  m.Print();
  Coordinates cm1 = r1.MultVec(cc);
  cm1.Print();
  Coordinates cm2 = r2.MultVec(cc);
  cm2.Print();
  //return 0;

  NPCBone test; 
  //test.SetRelCoords(NPCBoneCoords(0, 0, PI_P/2.)); 
  //test.Print();
  //return 0;
  for (double angle = 0.; angle <2*PI_P; angle += 0.1) {
    cout << "Angle z=" << angle << endl;
    test.SetRelCoords(NPCBoneCoords(1, angle, angle, PI_P/4.)); 
    cout << "RESULT: ";
    test.GetCoords().Print();
    test.Print();
  }
  //test.GetCoords().Print();
  return 0;
  */
  
  NPCSkeleton primitive;
  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); 
  primitive.AddBone(aa);
  //aa.SetRelCoords(NPCBoneCoords(1, PI_P/4, PI_P/4, PI_P/2)); 
  aa.SetRelCoords(NPCBoneCoords(0.3, 0, 0, 0)); 
  primitive.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(0.3, 0, 0, PI_P/4)); 
  primitive.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.3, 0, 0, PI_P/1.5)); 
  primitive.AddBone(aa);

  primitive.SetParentChild(0, 1);
  primitive.SetParentChild(1, 2);
  primitive.SetParentChild(2, 3);
  primitive.Print();
  OneFrame("test.ps", primitive);
  int kk = 1000;

  for (i=0; i<120; i++) {
    //primitive.AddToBoneRot(0, NPCBoneCoords(0., 0, 0.05, 0.));
    primitive.AddToBoneRot(0, NPCBoneCoords(0., 0., 0.05, 0.));
    primitive.AddToBoneRot(2, NPCBoneCoords(0., 0., 0., 0.01));
    primitive.AddToBoneRot(3, NPCBoneCoords(0., 0., 0., -0.015));
    primitive.Print();
    char name[256];
    sprintf(name, "tmp%d.ps", kk);
    OneFrame(name, primitive);
    kk++;
  }

  return 0;
  

  NPCBone a;
  // Root
  a.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); 
  
  s.AddBone(a);
  a.SetRelCoords(NPCBoneCoords(0.5, 0, 0, PI_P/2.));

 
  s.AddBone(a);
  a.SetRelCoords(NPCBoneCoords(0.5, 0, 0, PI_P/2.+0.2));
  s.AddBone(a);
  a.SetRelCoords(NPCBoneCoords(0.5, 0, 0, PI_P/2.-0.2));
  s.AddBone(a);

  a.SetRelCoords(NPCBoneCoords(0.5,  PI_P/4, PI_P/4, PI_P/4.));
  s.AddBone(a);
  s.AddBone(a);
  s.AddBone(a);

 
  s.SetParentChild(0, 1);
  s.SetParentChild(0, 2);
  s.SetParentChild(0, 3);

  s.SetParentChild(1, 4);
  s.SetParentChild(2, 5);
  s.SetParentChild(3, 6);
 

  //s.Print();

  s.AddToBoneRot(0, NPCBoneCoords(0., -PI_P/4., -PI_P/4., -PI_P/4.));

  //s.Print();
  //s.AddToBoneRot(4, NPCBoneCoords(0., PI_P/4., 0., 0.));
  //s.Print();
  //OneFrame("test.ps", s);
  //return 0;
  int k = 1000;
  double d = 0.05;
  for (double a=0.; a<2*3.14; a += d) {
    s.AddToBoneRot(0, NPCBoneCoords(0., 0, d, 0.));
    //s.AddToBoneRot(0, NPCBoneCoords(0., 0, 0, d));

    //s.AddToBoneRot(4, NPCBoneCoords(0., d/3., 0., 0.));
    //s.AddToBoneRot(5, NPCBoneCoords(0., 0, d/3., 0.));
    //s.AddToBoneRot(6, NPCBoneCoords(0., 0, 0., d/3.));

    char name[256];
    sprintf(name, "tmp%d.ps", k);
    OneFrame(name, s);
    k++;
  }

  return 0;
}
