#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "npc/SkeletonControl.h"
#include "npc/NPCHMM.h"


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

  int size = 1;
  svec<double> move;
  move.resize(size, 0);

  int i;

  
  NPCControl ctrl;
  ctrl.SetNumControls(1);
  
  ctrl[0].SetName("one");
  ctrl[0].SetIndex(0);
  
  ctrl.SetUp(size, 1);
  ctrl.SetRange(-2., 2.);

  double time = 0.;
  double lastSucc = 0.;
  while (true) {
    svec<double> out;
    out.resize(1, 0);

    double lasttime = time;
    time += 0.08;

    svec<double> inc;
    //inc.resize(size, 0);
    double upd = sin(lasttime);
    double forback = -cos(lasttime);

    move[0] = forback;
    
    //inc[0] = upd;
    //inc[1] = forback;

    /*
    for (i=0; i<move.isize(); i++) {
      move[i] += inc[i] * 0.02;
      move[i] = inc[i];
      if (move[i] > 3.)
	move[i] = 3.;
      if (move[i] < -3.)
	move[i] = -3.;
	}*/
    
    double upd_expect = sin(time);
    double forback_expect = -cos(time);
   
    ctrl.Retrieve(out, move, lastSucc);

    //double dist = (out[0]-forback_expect);
    double dist = (out[0]-forback);
    dist *= dist;

    double d = (out[0] - forback);
    if (d < 0)
      d = -d;
    lastSucc = -(d - 1);
    cout << "DEBUG " << out[0] << " " << forback << " " << lastSucc << endl;
    
    /*
    if (out[0] < 0. && forback_expect < 0.) {
      lastSucc = 1./(1+dist);
    }
    if (out[0] > 0. && forback_expect > 0.) {
      lastSucc = 1./(1+dist);
      }*/
    
    ctrl.LearnOrAvoid();  
    
    cout << "Try " << out[0] << " <-> " << forback_expect << " success: " << lastSucc << endl;
    ctrl.Print();
  
    //ctrl.SetSuccess(0);
    

  }

  
  return 0;
}
