#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "base/RandomStuff.h"
#include "npc/NLOSys.h"

double Score(const svec<double> & a, const svec<double> & b)
{
  double max = 0.;
  double proj = 0.;

  double na = 0.;
  double nb = 0.;

  int i;
  for (i=0; i<a.isize(); i++) {
    na += a[i]*a[i];
    nb += b[i]*b[i];
  }

  na = sqrt(na);
  nb = sqrt(nb);
  double d = 0.;
  for (i=0; i<a.isize(); i++) {
    proj += a[i]*b[i]/na/nb;
    max += a[i]*a[i]/na/na;
    d += (a[i]-b[i])*(a[i]-b[i]);
  }
  d = sqrt(d/(double)a.isize());
  return 1. - d;
}


int main( int argc, char** argv )
{

  int i, j;
  
  NLOProblemSolver solver;
  int n = 5;
  int dim = 2;
  solver.SetCycleNumDim(n, 2);

  svec<double> testX;
  testX.resize(n, 0);
  svec<double> testY;
  testY.resize(n, 0);

  testX[0] = 1.;
  testX[1] = 0.5;
  testX[2] = 0.;
  testX[3] = -.5;
  testX[4] = -1.;

  testY[0] = 1.;
  testY[1] = 0.5;
  testY[2] = 0.;
  testY[3] = -.5;
  testY[4] = -1.;

  int index = 0;
  double score = 0.;

  int counter = 0;
  
  for (i=0; i<10000; i++) {
    svec<double> out;

    solver.Get(out);
    
    svec<double> test;
    test.resize(dim);
    test[0] = testX[index];
    test[1] = testY[index];
    index++;

    cout << "+++++++++++++++++++++++" << endl;
    cout << "Cycle " << index << " score: " << score << endl;
    for (j=0; j<test.isize(); j++) {
      cout << test[j] << "\t" << out[j] << endl;
    }

    
    if (index >= testX.isize()) {
      index = 0;
      //solver.SetScore(score/(double)testX.isize());
      //score = 0.;
    }
    counter++;
    if (counter > 5 && RandomFloat(1.) < 0.2) {
      cout << "****************************************" << endl;
      cout << "Cycle END " << i << " score: " << score/(double)counter << endl;
      solver.SetScore(score/(double)counter);
      score = 0.;
      counter = 0;
    }
    
    double s = Score(test, out);
    score += s;


  }
  
}
