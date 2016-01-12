
#include "visual/Whiteboard.h"

#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "base/SVector.h"
#include "visual/Color.h"
#include "npc/Plant.h"


void OneFrame(const string & o, const Tree & t)
{
  double x_offset = 20;
  double y_offset = 20;


  int i, j, k;
  ns_whiteboard::whiteboard board;

 
  double x_max = 300.;
  double y_max = 300.;


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

  SimpleTree tree;
  tree.AddTrunk(Coordinates(150,0,0), Coordinates(150,20,0));

  tree.AddBranches();
  tree.AddBranches();


  int i;
  for (i=0; i<150; i++) {
    cout << "ROUND " << i << endl;
    tree.AddBranches();
    tree.Grow();
    tree.Print();
    char name[256];
    sprintf(name, "tmp%d.ps", i); 
    OneFrame(name, tree);
  }

  OneFrame("tree.ps", tree);

  return 0;
}