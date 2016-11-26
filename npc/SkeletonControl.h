#ifndef SKELTONCONTROL_H
#define SKELTONCONTROL_H

#include "npc/Skeleton.h"
#include "npc/NNet.h"
#include "npc/NLOpt.h"
#include "npc/INPCControl.h"


class NLONode
{
 public:
  NLONode() {
    m_depth = 0;
    m_curr = 0.;
    m_maxDat = 4;
  }

  void AddDataPoint(double score, double val);
  double GetAdjust() const {return m_curr;}

  
 private:
  
  void Add(double score, double val, double diff) {
    int i;
    for (i=0; i<m_score.isize(); i++) {
      if (val == m_val[i])
	return;
    }

    m_depth++;
    if (m_score.isize() < m_maxDat) {
      m_score.push_back(score);
      m_val.push_back(val);
      m_diff.push_back(diff);
      return;
    }
    
    for (i=1; i<m_score.isize(); i++) {
      m_score[i-1] = m_score[i];
      m_val[i-1] = m_val[i];
      m_diff[i-1] = m_diff[i];
    }
    m_score[m_score.isize()-1] = score;
    m_val[m_score.isize()-1] = val;
    m_diff[m_score.isize()-1] = diff;
  }
  
  svec<double> m_score;
  svec<double> m_val;
  svec<double> m_diff;
  double m_curr;
  int m_depth;
  int m_maxDat;
};


class NonLinOpt
{
 public:
  NonLinOpt() {}

 private:
};


class MotorControl
{
public:
  MotorControl() {
    m_index = 0;
    m_maxQueue = 150;
    m_frame = 0;
    m_frameStart = 0;
    m_off = 0;
  }

  void SetUp(int in, int out) {
    m_nn.Setup(20, in + out);

    m_nl.resize(out);
    for (int i=0; i<out; i++)
      m_nl[i].resize(m_nn.isize());
    
    m_nn.SetDecay(0.9999);
    m_nn.SetBeta(0.3);
    m_nn.SetNeuronDistance(2.);
    m_off = in;
    m_last.resize(in+out);
  }
  
  void SetRange(int i, double min, double max) {
    m_nn.ReSetup(i, min, max);
  }
  void SetRange(double min, double max) {
    m_nn.ReSetup(min, max);
  }

  //-----------------------------------------------------
  void Retrieve(svec<double> & out,
		const svec<double> & in,
		const svec<double> & suggest,
		double weight) {
    NPCIO tmp;
    tmp.resize(m_last.isize());
    int i;
    for (i=0; i<in.isize(); i++) {
      cout << "Feed " << i << " " << in[i] << endl;
      tmp[i] = in[i];
      tmp.SetValid(i, true);
    }
    for (i=0; i<out.isize(); i++) {
      //tmp[i+in.isize()] = in[i];
      tmp.SetValid(i+in.isize(), false);
    }

    //cout << "Retrieve..." << endl;
    m_last = tmp;
 
    m_nn.Retrieve(tmp);
    int bi = m_nn.Best(tmp);
    m_last.SetNeuron(bi);
    cout << "Best neuron: " << bi << endl;
    
    for (i=0; i<out.isize(); i++) {
      m_last[i+in.isize()] = tmp[i+in.isize()];
    }
    
    for (i=0; i<in.isize(); i++) {
      m_last.SetValid(i, true);
    }
    
    //cout << "Retrieve......" << endl;
    for (i=in.isize(); i<tmp.isize(); i++) {
      cout << i-in.isize() << " " << out.isize() << endl;
      out[i-in.isize()] = tmp[i];

      // DEBUG
      //m_last.SetValid(i, true);
      m_last.SetValid(i, false);

      // 
      const NLONode & nl = (m_nl[i-in.isize()])[bi];
      m_last[i] += nl.GetAdjust();
      tmp[i] += nl.GetAdjust();
      out[i-in.isize()] += nl.GetAdjust();
     
      if (suggest.isize() > 0) {
	double d = suggest[i-in.isize()]*weight + (1.-weight)*tmp[i];
	cout << "Opt weight " << weight << " sugg " << suggest[i-in.isize()] << " out " << tmp[i] << " -> " << d << endl;
  	m_last[i] = d;
	tmp[i] = d;
	out[i-in.isize()] = d;
      }
    }
    cout << "Added to queue" << endl;
    AddToQueue(m_last);
    for (i=0; i<m_last.isize(); i++)
      cout << "Last " << i << " " << m_last[i] << endl;
    
  }

  void Print() const {
    m_nn.Print();
    cout << "Queue" << endl;
    int k = 0;
    for (int i=m_trainQueue.isize()-1; i>=0; i--) {
      const NPCIO & q = m_trainQueue[i];
      cout << i;
      for (int j=0; j<q.isize(); j++)
	cout << " " << q[j];
      cout << endl;
      k++;
      if (k == 10)
	break;
    }
  }
  
  void Learn(double score, int fromFrame, int toFrame) {
    int i, j;
    for (i=fromFrame; i<=toFrame; i++) {
      //for (int x=0; x<10; x++) {

      double learnweight = score;
      if (learnweight > 1.)
	learnweight = 1.;
      if (learnweight > 0.) {
	m_nn.Learn(Get(i), 1.);
	m_nn.Learn(GetInv(i), 1.);
      } else {
	m_nn.Learn(GetNeg(i), 1.);
	m_nn.Learn(GetInvNeg(i), 1.);
      }
      const NPCIO & q = Get(i);

      int neuron = q.GetNeuron();
      cout << "Stored neuron: " << neuron << endl;
      
      for (j=m_off; j<q.isize(); j++) {
	NLONode & nl = (m_nl[j-m_off])[neuron];
	nl.AddDataPoint(score, q[j]);
      }

      
      cout << "Learn from frame " << i << ": ";
      for (j=0; j<q.isize(); j++)
	cout << " " << q[j];
      cout << endl;
	//m_nn.Learn(GetInv(i), 0.5*weight);
	//}
    }
  }
  void AntiLearn(double weight, int fromFrame, int toFrame) {
    throw;
    for (int i=fromFrame; i<=toFrame; i++) {
      //for (int x=0; x<10; x++) {
	m_nn.Learn(GetInv(i), 0.5*weight);
	//m_nn.Learn(GetInv(i), 0.5*weight);
	//}
    }
  }
  void LearnSmart(double weight, int fromFrame, int toFrame, double s) {
    throw;
    int j;
    for (int i=fromFrame; i<=toFrame; i++) {
      NPCIO nn = Get(i);
      if (s > 0.) {
	for (j=m_off; j<nn.isize(); j++) {
	  nn[j] *= 2.;
	}
      } else {
	for (j=m_off; j<nn.isize(); j++) {
	  nn[j] = -2*nn[j];
	}
      }
      m_nn.Learn(nn, 0.5*weight);
    }
  }

  void LearnAvoid(double weight, int fromFrame, int toFrame) {
    throw;
    for (int i=fromFrame; i<=toFrame; i++) {
      m_nn.LearnAvoid(Get(i), 0.1*weight);
      const NPCIO & q = Get(i);
      cout << "Learn from frame (avoid) " << i << ": ";
      for (int j=0; j<q.isize(); j++)
	cout << " " << q[j];
      cout << endl;
    }
  }

  void SetName(const string & n) {
    m_name = n; 
  }
  const string & GetName() const {return m_name;}
  void SetIndex(int i) {
    m_index = i; 
  }
  int GetIndex() const {return m_index;}

  void PruneQueue();
  
private:
  void AddToQueue(const NPCIO & f);
  NPCIO Get(int frame) const {
    if (frame-m_frameStart >= m_trainQueue.isize()) {
      cout << "NN Frame ERROR!! " << frame << " " << m_frameStart << " " << m_trainQueue.isize() << endl;
      throw;
    }
    return m_trainQueue[frame-m_frameStart];
  }
  NPCIO GetInv(int frame) const {
    if (frame-m_frameStart >= m_trainQueue.isize()) {
      cout << "NN Frame ERROR!! " << frame << " " << m_frameStart << " " << m_trainQueue.isize() << endl;
      throw;
    }
    NPCIO tmp = m_trainQueue[frame-m_frameStart];
    for (int i=0; i<tmp.isize(); i++)
      tmp[i] = -tmp[i];
    return tmp;
  }
  NPCIO GetNeg(int frame) const {
    NPCIO tmp = Get(frame);
    tmp[tmp.isize()-1] *= -1;
    return tmp;
  }
  NPCIO GetInvNeg(int frame) const {
    NPCIO tmp = Get(frame);
    tmp[tmp.isize()-1] *= -1;
    return tmp;    
  }
  
  NeuralNetwork m_nn;
  int m_off;
  NPCIO m_last;
  svec<NPCIO> m_trainQueue;
  int m_maxQueue;
  int m_frame;
  int m_frameStart;
  int m_index;
  string m_name;
  svec < svec < NLONode > > m_nl;
};

class SuccessFeature
{
 public:
  SuccessFeature() {}

  int isize() const {return m_data.isize();}
  const double & operator[] (int i) const {return m_data[i];}
  double & operator[] (int i) {return m_data[i];}
  
  void resize(int n, double f = 0.) {
    m_data.resize(n, f);
  }

  double Good() const {
    int i;
    double g = 0.;
    for (i=0; i<m_data.isize(); i++) {
      g += m_data[i];
    }
    return g;
  }
      
      
 private:
  svec<double> m_data;
};

//======================================================
class LearnControl
{
 public:
  LearnControl() {
    m_frameStart = 0;
    m_hi = 0.;
    m_lo = 0.;
  }

  void Add(const SuccessFeature & f) {
    m_hist.push_back(f);
    if (m_hist.isize() < 200)
      return;
    
    int n = 20;
    for (int i=n; i<m_hist.isize(); i++)
      m_hist[i-n] = m_hist[i];
    
    m_hist.resize(m_hist.isize()-n);
    m_frameStart += n;    
  }

  int Frame() const {return m_frameStart + m_hist.isize();}
  
  bool GetLearnFrames(int & start, int & end, double & weight);
  bool GetAvoidFrames(int & start, int & end, double & weight);
  
 private:
  double GetWeightHi(double w, int frames) {
    return 0.5*(double)frames;
    w /= ((double)frames+1);
    if (w > m_hi)
      m_hi = w;
    else
      m_hi *= 0.95;
    
    return w/m_hi*sqrt((double)frames)*0.3;
  }
  double GetWeightLo(double w, int frames) {
    return 1.;
    w /= ((double)frames+1);
    if (w > m_lo)
      m_lo = w;
    else
      m_lo *= 0.95;     
    return w/m_lo*sqrt((double)frames)*0.3;
  }
  
  svec<SuccessFeature> m_hist;
  int m_frameStart;
  double m_hi;
  double m_lo;
};


//======================================================
class SkeletonControl
{
public:
  SkeletonControl() {
    m_frame = -1;
    m_sDim = 1;
    m_useOpt = true;
    m_lf = 0;
  }

  void SetNumControls(int n) {
    m_controls.resize(n);
    m_out = 0;
    m_lastOut.resize(n, 0.);
  }
  
  void SetUp(int in, int out) {
    int i;

    // Reserve space for self-feedback
    for (i=0; i<m_controls.isize(); i++)
      m_controls[i].SetUp(in /*+ isize()*/, out);

    m_out = out;
  
  }

  void SetRange(int index, double min, double max, int i) {
    m_controls[index].SetRange(i, min, max);
    m_opt.SetDim(m_out, min, max);
  }
  
  void SetRange(double min, double max, int i) {
    int j;
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].SetRange(i, min, max);
    m_opt.SetDim(m_out, min, max);
  }
  
  void SetRange(double min, double max) {
    int j;
    for (j=0; j<m_controls.isize(); j++) {
      m_controls[j].SetRange(min, max);
    }
    m_opt.SetDim(m_out, min, max);
  }

  
  void Retrieve(svec<double> & out, const svec<double> & in_raw, double d) {
    int i, j;
    int k = 0;

    svec<double> in = in_raw;

    out.resize(m_controls.isize()*m_out, 0.);

    /*
    for (i=0; i<m_lastOut.isize(); i++)
      in.push_back(m_lastOut[i]*0.01);
    */
    svec<double> suggest;

    double weight = 0.;
    weight = m_opt.Suggest(suggest, 0.);
    if (!m_useOpt)
      weight = 0.;

    
    int kk = 0;
    for (i=0; i<m_controls.isize(); i++) {
      svec<double> tmp;
      tmp.resize(m_out, 0.);
      svec<double> sugglocal;
      sugglocal.resize(m_out, 0.);
      for (int x =0; x<m_out; x++) {
	sugglocal[x] = suggest[kk];
	kk++;
      }
      in = in_raw;
      m_controls[i].Retrieve(tmp, in, sugglocal, weight);
      
      for (j=0; j<tmp.isize(); j++) {
	out[k] = tmp[j];
	k++;
      }
      
    }
    m_lastOut = out;
    m_frame++;
    //m_controls[0].Print();
  }

  void Learn(double weight) {
    int j;  
    //m_controls[0].Print();
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].Learn(weight, m_frame, m_frame);
    //m_controls[0].Print();
  }

  void LearnAvoid(double weight) {
    int j;
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].LearnAvoid(weight, m_frame, m_frame);
  }

  void Learn(int j, double weight) {
    m_controls[j].Learn(weight, m_frame, m_frame);
  }

  void LearnAvoid(int j, double weight) {
    m_controls[j].LearnAvoid(weight, m_frame, m_frame);
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
    m_sDim = n;
  }
 
  void SetSuccess(const SuccessFeature & f);

  void LearnOrAvoid();
  void UnSuccess() {
    m_opt.UnSuccess();
    m_useOpt = true;
  }
    
private:
  svec<MotorControl> m_controls;
  int m_out;
  svec<double> m_lastOut;
  int m_frame;
  int m_sDim;

  LearnControl m_learn;
  NLOptimizer m_opt;
  bool m_useOpt;
  int m_lf;
  
};






#endif
