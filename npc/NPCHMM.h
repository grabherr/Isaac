#ifndef NPCHMM_H
#define NPCHMM_H

#include "base/SVector.h"
#include "npc/SkeletonControl.h"
#include "npc/NNet.h"

class NPCState
{
 public:
  NPCState() {   
    m_score = 0;
    //m_hist.resize(100, -1);
  }

  void SetHistSize(int n) {
    m_hist.resize(n, -1);
  }
  
  double & Score() {return m_score;}
  const double & Score() const {return m_score;}

  void operator -= (double s) {
    m_score -= s;
  }
  
  void AddHist(int n) {
    int i;
    for (i=1; i<m_hist.isize(); i++) {
      m_hist[i-1] = m_hist[i];
    }
    m_hist[m_hist.isize()-1] = n;
  }

  const svec<int> & History() const {return m_hist;}
 private:
  
  
  double m_score;
  svec<int> m_hist;
};


class NPCHMM
{
 public:
  NPCHMM() {
    m_stay = 0.;
    m_trans = 5.;
    m_lastMin = 0;
    m_histSize = 100;
  }

  void Setup(int n) {
    m_states.resize(n);

    for (int i=0; i<n; i++)
      m_states[i].SetHistSize(m_histSize);

  }
  


  void Update(const svec<double> & dist);
  const svec<int> & TraceBack() const {return m_states[m_lastMin].History();}

  int LastBest() const {return m_lastMin;}

  int GetHistSize() const {return m_histSize;}
  
 private:
  svec<NPCState> m_states;
  double m_stay;
  double m_trans;
  int m_lastMin;
  int m_histSize;
};



class TimeSOM
{
 public:
  TimeSOM() {
    m_dim = 0;
    m_lastBest = 0;
  }

  int GetHistSize() const {return m_hmm.GetHistSize();}
  
  void SetUp(int in) {
    m_nn.Setup(40, in);
    m_nn.SetDecay(0.999);
    m_nn.SetBeta(0.3);
    m_dim = in;
    m_hmm.Setup(m_nn.isize());    
  }
  
  void SetRange(int i, double min, double max) {
    m_nn.ReSetup(i, min, max);
  }
  
  void SetRange(double min, double max) {
    m_nn.ReSetup(min, max);
  }
  

  bool IsNew(const svec<double> & in) {
    NPCIO tmp;
    tmp.resize(in.isize());
    int i;
    for (i=0; i<in.isize(); i++) {
      tmp[i] = in[i];
      tmp.SetValid(i, true);
    }
    //cout << "IsNew size=" << in.isize() << " " << tmp.isize() << endl;
    //m_nn.Print();
    //int best1 = m_nn.Best(tmp);
    //cout << "Best" << endl;
    m_nn.Learn(tmp);
    int best = m_nn.Best(tmp);
    const svec<double> & all = m_nn.AllDist();
    //cout << "Update" << endl;
    m_hmm.Update(all);
    const svec<int> & tb = m_hmm.TraceBack();
    //cout << "Check" << endl;
    if (m_hmm.LastBest() == m_lastBest/* &&
	tb[tb.isize()-1] == m_lastBest &&
	tb[tb.isize()-2] == m_lastBest &&
	tb[tb.isize()-3] != m_lastBest*/)
      return false;

    m_lastBest = m_hmm.LastBest();
    return true;
  }
  
  int Delay() {return 3;}
  const svec<int> & TraceBack() const {return m_hmm.TraceBack();}

 private:
  NeuralNetwork m_nn;
  int m_lastBest;
  int m_dim;
  NPCHMM m_hmm;
};


class SuccessFeature;


//=======================================================================
//=======================================================================
//============================== Top Level Class ========================
//=======================================================================
class HMMControl
{
public:
  HMMControl() {
    m_out = 0;
    m_frame = 0;
  }

  void SetNumControls(int n) {
    m_controls.resize(n);
  }
  
  void SetUp(int in, int out) {
    m_time.SetUp(in+1); // One for the success
    m_success.resize(m_time.GetHistSize(), 0.);
    int i;

    // Reserve space for self-feedback
    for (i=0; i<m_controls.isize(); i++)
      m_controls[i].SetUp(in /*+ isize()*/, out);
    m_out = out;

    //cout << "CTRL " << m_success.isize() << " " << m_time.GetHistSize() << endl;
  }

  void SetRange(int index, double min, double max, int i) {
    m_controls[index].SetRange(i, min, max);
  }
  
  void SetRange(double min, double max, int i) {
    int j;
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].SetRange(i, min, max);
  }
  
  void SetRange(double min, double max) {
    int j;
    for (j=0; j<m_controls.isize(); j++) {
      m_controls[j].SetRange(min, max);
    }
  }

  


  int isize() const {return m_controls.isize();}
  MotorControl & operator[] (int i) {return m_controls[i];}
  const MotorControl & operator[] (int i) const {return m_controls[i];}

  void Print() {
    cout << "Printing motor control" << endl;
    for (int i=0; i<m_controls.isize(); i++) {
      cout << "NN " << m_controls[i].GetName() << endl;
      m_controls[i].Print();
    }
    cout << endl;
  }

  void SetSuccessSize(int n) {
  }
 
  void SetSuccess(const SuccessFeature & f);
  void Retrieve(svec<double> & out, const svec<double> & in_raw, double success); 

  void LearnOrAvoid();
  void UnSuccess();
  
    
private:
  TimeSOM m_time;
  svec<MotorControl> m_controls;
  int m_out;
  int m_frame;
  svec<double> m_success;
};



#endif 
