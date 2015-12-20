#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Waves.h"



int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-o","output file");
  commandArg<int> bCmmd("-n","size");
  commandLineParser P(argc,argv);
  P.SetDescription("Generates a wave repository.");
  P.registerArg(fileCmmd);
  P.registerArg(bCmmd);
 
  P.parse();
  
  string outName = P.GetStringValueFor(fileCmmd);
  int n = P.GetIntValueFor(bCmmd);

  SingleWaveForm s;

  int i;

 
  s.SetSource(Coords2D(512+256, 256+128));

  Canvas c, c2;
  c.resize(512*3, 256*3);
  c.PaintAll(0.5, 0.5, 0.5, 0.5);


  WavePainter painter;
  painter.Paint(c, s, 400);
  SparseCanvas sparse;
  sparse.FromCanvas(c, 0.49, 0.51);
 
  // ======================================
  c2.resize(512, 256);
  c2.PaintAll(0.5, 0.5, 0.5, 0.5);
  sparse.AddToCanvas(c2, 512, 256, 0.5, true);


  c2.GreyScale();

  Bitmap out;
  c2.ToBitmap(out);
  out.Write("wave2.bmp");


  // Test the repo

  WaveRepo repo;
  repo.SetSize(512*3, 256*3);
  SingleWaveForm s2;

  
  s2.SetSource(Coords2D(repo.X()/2, repo.Y()/2));
  cout << "Building repo..." << endl;
  for (double time=1; time<256*2; time += 1.5) {
    cout << "Add " << time << " of " << 256*3 << endl;
    repo.AddWave(s2, time);
  }
  cout << "Write repo." << endl;
  repo.Write(outName);
  cout << "Done." << endl;
  


 

  return 0;
}
