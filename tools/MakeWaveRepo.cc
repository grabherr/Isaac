#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Waves.h"



int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-o","output file");
  commandArg<int> bCmmd("-t","time", 512);
  commandArg<int> xCmmd("-x","x size base", 512);
  commandArg<int> yCmmd("-y","y size base", 256);
  commandLineParser P(argc,argv);
  P.SetDescription("Generates a wave repository.");
  P.registerArg(fileCmmd);
  P.registerArg(bCmmd);
  P.registerArg(xCmmd);
  P.registerArg(yCmmd);
 
  P.parse();
  
  string outName = P.GetStringValueFor(fileCmmd);
  int fulltime = P.GetIntValueFor(bCmmd);
  int xx = P.GetIntValueFor(xCmmd);
  int yy = P.GetIntValueFor(yCmmd);

  SingleWaveForm s;

  int i;

 
  s.SetSource(Coords2D(xx+xx/2, yy+yy/2));

  Canvas c, c2;
  c.resize(xx*3, yy*3);
  c.PaintAll(0.5, 0.5, 0.5, 0.5);


  WavePainter painter;
  painter.Paint(c, s, 400);
  SparseCanvas sparse;
  sparse.FromCanvas(c, 0.49, 0.51);
 
  // ======================================
  c2.resize(xx, yy);
  c2.PaintAll(0.5, 0.5, 0.5, 0.5);
  sparse.AddToCanvas(c2, xx, yy, 0.5, true);


  c2.GreyScale();

  Bitmap out;
  c2.ToBitmap(out);
  out.Write("wave2.bmp");


  // Test the repo

  WaveRepo repo;
  repo.SetSize(xx*3, yy*3);
  SingleWaveForm s2;

  
  s2.SetSource(Coords2D(repo.X()/2, repo.Y()/2));
  cout << "Building repo..." << endl;
  for (double time=1; time<fulltime; time += 1.5) {
    cout << "Add " << time << " of " << fulltime << endl;
    repo.AddWave(s2, time);
  }
  cout << "Write repo." << endl;
  repo.Write(outName);
  cout << "Done." << endl;
  


 

  return 0;
}
