#include <string>
#include "base/CommandLineParser.h"
#include "npc/TopLevel.h"
#include "base/RandomStuff.h"
#include "physics/Coordinates.h"



double GetMilkScore(double & relPhi,
		    const Coordinates & oldPos,
		    const Coordinates & realPos,
		    const Coordinates & target,
		    double currRot)
{
  Coordinates rel = target - realPos;
  rel[1] = 0.;
  
  Coordinates relOld = target - oldPos;
  relOld[1] = 0.;

  double score = relOld.Length() - rel.Length();
  SphereCoordinates s = rel.AsSphere();
  //s.phi() += PI_P/2;

  cout << "MILK " << target[0] << " " << target[2] << " maxl " << realPos[0] << " " << realPos[2] << " was " << oldPos[0] << " " << oldPos[2] << " phi " << currRot << " phi targ " << s.phi() << endl; 
  //cout << "MILK2 " << rel.Length() << " " << relOld.Length() << endl;

  relPhi = (s.phi()-currRot)/PI_P;
  while(relPhi > 1) {
    relPhi -= 2;
  }
  while (relPhi < -1) {
    relPhi += 2;
  }

  //score = 1./(rel.Length()+1);

  
  return score;
  
}


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
  tt.resize(1, 1, 1, 30);
  TopLevel ttneg;
  ttneg.resize(1, 1, 1, 30);
  
 
  int i;
  double last = 0.;
  double score = 0.;

  svec<double> seq;

  Coordinates target, pred, pred1;

  target[0] = 10.;
  target[2] = 25;
  
  cout << "Training." << endl;

  double angle = 0.;
  for (i=0; i<10000; i++) {
    IOEntity tmp;
    tmp.resize(1, 1, 1);


    SphereCoordinates s;
    s.phi() = angle;
    s.theta() = PI_P/2;
    s.r() = 0.1;

    Coordinates plus;
    plus.FromSphere(s);
    pred += plus;

    
    double input;
    score = GetMilkScore(input,
			 pred1,
			 pred,
			 target,
			 angle);

    score *= 10.;
    
    cout << "Predator coords " << pred[0] << " " << pred[2] << " phi=" << angle/PI_P << " soll=" << input << " score " << score << endl;
    //pred.Print();
    pred1 = pred;
    tmp.in(0) = input;
    tmp.score(0) = score;
    IOEntity tmp2;
    tmp2 = tmp;
    
    cout << "Update w/ score " << score << endl;
    tt.Update(tmp, 0.6, score);
    //ttneg.Update(tmp2, 0.6, -score);
    
    //angle += tmp.out(0)*PI_P*0.01;
    angle = tmp.out(0)*PI_P;

    //if (i > 100)
    //angle = -tmp2.out(0)*PI_P;

    
    cout << "Train " << i << " angle: " << angle << " delta: " << input << " score: " << score << endl;
    //angle += input*PI_P;
  }
  cout << "Testing" << endl;


  /*
  for (i=0; i<seq.isize(); i++) {
    IOEntity tmp;
    tmp.resize(1, 1, 1);
    tmp.in(0) = seq[i];

    tt.Update(tmp, 0.6, score);

    score = 1. - (tmp.out(0)-seq[i])*(tmp.out(0)-seq[i]);    
    cout << "Trial " << i << " in: " << seq[i] << " out: " << tmp.out(0) << " score: " << score << endl;

    }*/
  
  return 0;
}
