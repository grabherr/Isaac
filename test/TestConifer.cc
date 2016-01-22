
#include "visual/Whiteboard.h"

#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "base/SVector.h"
#include "visual/Color.h"
#include "npc/Plant.h"
#include "base/RandomStuff.h"


void OneFrame(const string & o, const Tree & t)
{
  double x_offset = 20;
  double y_offset = 20;


  int i, j, k;
  ns_whiteboard::whiteboard board;

 
  double x_max = 800.;
  double y_max = 600.;
  board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(0, 0), 
				      ns_whiteboard::xy_coords(2*x_offset+x_max, 2*y_offset+y_max), 
				      color(0.9,0.9,0.9)));


  for (i=0; i<t.isize(); i++) {
    const Branch & b = t[i];

    board.Add( new ns_whiteboard::line( ns_whiteboard::xy_coords(x_offset+b.Bottom()[0], y_offset+b.Bottom()[1]), 
					ns_whiteboard::xy_coords(x_offset+b.Top()[0], y_offset+b.Top()[1]), 
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

  int i, j;
  int k = 1000;
  int n = 70;

  RandomFloat(1);
  
  for (j=0; j<2; j++) {
    Conifer tree;
    tree.AddTrunk(Coordinates(400,0,0), Coordinates(400,20,0));
    bool bTips = false;
    char name[256];

    for (i=0; i<n; i++) {
      cout << "ROUND " << i << endl;
      //if (i == n - 3)
      //bTips = true;
      if (i < 12)
	tree.AddBranches(0.0);
      else
	tree.AddBranches(0.6, bTips);
      tree.Grow();
      tree.Print();
      sprintf(name, "tmp%d.ps", k);
      k++;
      OneFrame(name, tree);
    }
    for (int x = 0; x<30; x++) {
      sprintf(name, "tmp%d.ps", k);
      k++;
      OneFrame(name, tree);
    }
     

  }

  //OneFrame("tree.ps", tree);

  return 0;
}
