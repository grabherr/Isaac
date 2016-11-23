#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "base/RandomStuff.h"


class Trial
{
public:
  Trial() {
    m_score = 0.;
  }


  void resize(int n, double d) {
    m_val.resize(n, d);
  }
  int isize() const {return m_val.isize();}
  const double & operator[] (int i) const {return m_val[i];}
  double & operator[] (int i) {return m_val[i];}

  const double & Score() const {return m_score;}
  double Score() {return m_score;}

  double Fit(const svec<double> & s, int from, int to) const {
    int i;
    double d = 0.;
    double max = 0.;
    double n = 0.;
    int off = to - m_val.isize();
    for (i=from; i<=to; i++) {
      if (i-off >=0 && i-off < m_val.isize()) {
	d += m_val[i-off]*s[i];
	n += 1.;
	max += s[i]*s[i];
      }
    }
    double score = d/max;
    return score;
  }
  
private:
  svec<double> m_val;
  double m_score;
  
};


class AdaptiveNetwork
{
public:
  AdaptiveNetwork() {
    m_frame = 0;
    m_frameLast = 0;
    m_futureCount = 0;
  }

  double Feed(double score) {
    m_scores.push_back(score);

    int i;
      
    if (score != 0.) {
      int last = ExtendBack();
      int best = -1;
      double max = 0.;
      for (i=0; i<m_trials.isize(); i++) {
	double d = m_trials[i].Fit(m_action, last, m_frame);
	if (d > max) {
	  max = d;
	  best = i;
	}
      }
      if (best != -1) {
	m_future.clear();
	m_futureCount = 0;
	double diff = score - m_trials[i].Score();
	if (diff > 0.) {
	  for (i=last; i<=m_frame; i++) {
	    m_future.push_back(m_action[i]*1.05);
	  }
	}
      }
    }
    
    double a = GetAction();
    m_action.push_back(a);
    
    m_frame++;
    return a;
  }

private:
  double GetAction() {
    throw;
  }
  
  int ExtendBack() {
    double sum = 0.;
    double sumLast = 0.;
    int i;
    
    for (i=m_scores.isize()-1; i>=0; i--) {
      sum += m_scores[i];
      if (sum*sumLast < 0)
	return i;
      sumLast = sum;
    }
    if (m_scores.isize() == 0)
      return -1;
    return 0;
  }

  
  
  int m_frame;
  int m_frameLast;
  int m_futureCount;
  svec<double> m_scores;
  svec<double> m_action;
  svec<double> m_future;
  svec<Trial> m_trials;
  
};



//==================================================================

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
    m_time = 0.;
    m_score = 0.;
    m_scoreMax = 0.;
  }

  
  double Test(double a) {
    double r = sin(m_time);
    m_time += 0.2;

    m_score += a*r;
    m_scoreMax += r*r;

    double ret = 0.;
    
    if (RandomFloat(1.) < 0.05) {
      ret = m_score/m_scoreMax;
      m_score = 0.;
      m_scoreMax = 0.;
    }
    
    return ret;
  }

private:
  double m_time;
  double m_score;
  double m_scoreMax;
  
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


  int i;

 
  
  return 0;
}
