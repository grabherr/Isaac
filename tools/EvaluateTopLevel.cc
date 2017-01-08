#include <string>
#include "base/CommandLineParser.h"
#include "npc/TopLevel.h"
#include "base/RandomStuff.h"

int main( int argc, char** argv )
{
  /*
  commandArg<string> fileCmmd("-i","input file");
  commandArg<string> oCmmd("-o","output file");
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(oCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string outName = P.GetStringValueFor(oCmmd);
  */
  TopLevel tt;

  IOEntity tmp;
  tmp.resize(1, 1, 1);

  int i;
  double last = 0.;
  double score = 0.;

  svec<double> seq;

  cout << "Training." << endl;
  
  for (i=0; i<100; i++) {
    double d = 0.5;
    if (RandomFloat(1.) < 0.5)
      d = -d;
    tmp.in(0) = d;
    //tmp.score(0) = score;
    seq.push_back(d);
    tt.Update(tmp, 0.6, score);

    score = 1. - (tmp.out(0)-d)*(tmp.out(0)-d);    
  }
  cout << "Testing" << endl;

  for (i=0; i<seq.isize(); i++) {
    tmp.in(0) = seq[i];

    tt.Update(tmp, 0.6, score);

    score = 1. - (tmp.out(0)-seq[i])*(tmp.out(0)-seq[i]);    
    cout << "Trial " << i << " in: " << seq[i] << " out: " << tmp.out(0) << " score: " << score << endl;

  }
  
  return 0;
}
