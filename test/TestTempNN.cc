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

class Func
{
public:
  Func() {
    m_frame = 0;
    m_counter = 0; 
    m_time = 0;
    m_score = 0;
  }


  double Test(double a, double b) {

    double d = 0.;
    switch (m_counter) {
    case 0:
      d = Abs(a - (double)m_frame) + Abs(b);
      cout << "(0) Have a=" << a << " -> " << m_frame;
      cout << " b=" << b << " -> 0" << " dist: " << d << endl;
      break;
    case 1:
      d = Abs(b - (double)m_frame) + Abs(5-a);
      cout << "(1) Have a=" << a << " -> " << 5;
      cout << " b=" << b << " -> " << m_frame <<  " dist: " << d << endl;
      break;
    case 2:
      d = Abs(a - (double)(5-m_frame)) + Abs(5-b);
      cout << "(2) Have a=" << a << " -> " << 5-m_frame;
      cout << " b=" << b << " -> 5" <<  " dist: " << d << endl;
      break;
    case 3:
      d = Abs(b - (double)(5-m_frame)) + Abs(a);
      cout << "(3) Have a=" << a << " -> " << 0;
      cout << " b=" << b << " -> " << 5-m_frame <<  " dist: " << d << endl;
      break;
    }

    
    m_frame++;
    if (m_frame == 5) {
      m_counter++;
      if (m_counter == 4)
	m_counter = 0;
      m_frame = 0;
    }
    m_score += d;
    double max = 15*2+20*2;
    cout << "Frame " << m_frame << " counter " << m_counter << endl;
    if (m_counter == 0 && m_frame == 0) {
      double dd =  1. - 2*m_score/max;
      cout << "RETURN " << dd << endl;
      m_score = 0.;
      return dd;
      
    } else {
      return 0;
    }
  }

private:
  double m_time;
  int m_frame;
  int m_counter;
  double m_score;
};


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

  TNN tnn;
  tnn.Setup(0, 2);

  Func f;
    
  double time = 0.;
  double lastSucc = 0.;
  int k = 0;
  while (true) {

    svec<double> out, in;
    cout << "call Feed" << endl;
    tnn.Feed(out, in, lastSucc);

    lastSucc = f.Test(out[0], out[1]);
    
     

  }

  
  return 0;
}
