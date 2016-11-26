#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "npc/TempNN.h"

double Abs(double a)
{
  if (a < 0)
    return -1;
  return a;
}



int main( int argc, char** argv )
{

  /* commandArg<string> fileCmmd("-i","input file");
  commandArg<int> bCmmd("-from","from column");
  commandArg<int> eCmmd("-to","to column");
  commandArg<bool> nCmmd("-newline","add newline", false);
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(bCmmd);
  P.registerArg(eCmmd);
  P.registerArg(nCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  int from = P.GetIntValueFor(bCmmd);
  int to = P.GetIntValueFor(eCmmd);
  bool bN = P.GetBoolValueFor(nCmmd);*/

  int i, j;
  
  NeuralNetwork nn;
  
  nn.Setup(10, 5, 3);
  nn.ReSetup(-1, 1);
  nn.SetTimeShift(1);
  
  NPCIO seq;
  NPCIO out;
  seq.resize(5);
  seq.SetAllValid(true);
  seq[0] = 1.;
  seq[1] = 0.5;
  seq[2] = 0.;
  seq[3] = -0.5;
  seq[4] = -1;

  for (i=0; i<200; i++) {
    
    double tmp = seq[0];
    for (int j=1; j<seq.isize(); j++) {
      seq[j-1] = seq[j];
    }
    seq[seq.isize()-1] = tmp;
    cout << "Learn " << i << endl;
    out = seq;
    nn.Retrieve(out);
    nn.Learn(seq);
    nn.Print();

  }
  
  return 0;
}
