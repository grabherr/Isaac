#ifndef NLOSYS_H
#define NLOSYS_H

#include "base/SVector.h"
#include <math.h>
#include "base/RandomStuff.h"
#include "npc/NNet.h"
#include "util/mutil.h"


template <class T>
class RingBuffer
{
 public:
  RingBuffer() {
    int m_index = 0;
  }
  RingBuffer(int size) {
    m_index = 0;
    m_data.resize(size);
  }

  int isize() const {return m_data.isize();}
  void resize(int size) {
    m_index = 0;
    m_data.resize(size);
  }

  const T & operator [] (int i) const {
    int t = m_index + i;
    if (t >= m_data.isize())
      t -= m_data.isize();
    return m_data[t]; 
  }
  T & operator [] (int i) {
    int t = m_index + i;
    if (t >= m_data.isize())
      t -= m_data.isize();
    return m_data[t]; 
  }

  void push_back(const T & t) {
    m_data[m_index] = t;
    m_index++;
    if (m_index >= m_data.isize())
      m_index = 0;
  }
 
 protected:
  svec<T> m_data;
  int m_index;
};


template <class T>
class RingBufferPlus : public RingBuffer<T>
{
 public:
  RingBufferPlus () : RingBuffer<T>() {}
  RingBufferPlus (int size) : RingBuffer<T>(size) {}

  // 
  double Distance(int & index, const RingBufferPlus<T> & t) const {
    int i, j;
    index = -1;
    double max = -1.;
    for (j=0; j<t.isize()-RingBuffer<T>::isize(); j++) {
      double d = SingleDistance(j, t);
      if (max < 0. || d < max) {
	max = d;
	index = j;
      }
    }
    return max;
  }
  
  double SingleDistance(int shift, const RingBufferPlus<T> & t) const {
    if (t.isize()+shift > RingBuffer<T>::isize())
      throw;
    int i;
    double n = 0.;
    double d = 0.;

    for (i=0; i<t.isize(); i++) {
      d += (*this)[i+shift].Distance(t[i]);
      n += 1.;
    }
    return sqrt(d/n);
  }
};




class NLOVect
{
 public:
  NLOVect() {
    m_score = 0.;
  }

  int isize() const {return m_data.isize();}
  void resize(int size, double d) {
    m_data.resize(size, d);
  }
  const double & operator[] (int i) const {
    return m_data[i];
  }

  double & operator[] (int i) {
    return m_data[i];
  }

  double Distance(const NLOVect & n) const {
    double nn = 0.;
    double d = 0;
    for (int i=0; i<m_data.isize(); i++) {
      d += (m_data[i] - n[i])*(m_data[i] - n[i]);
      nn += 1.;
    }
    return sqrt(d/nn);
  }

  void operator += (const NLOVect & n) {
    for (int i=0; i<m_data.isize(); i++) {
      m_data[i] += n[i];
    }
  }
  void operator -= (const NLOVect & n) {
    for (int i=0; i<m_data.isize(); i++) {
      m_data[i] -= n[i];
    }
  }
  void operator *= (double d) {
    for (int i=0; i<m_data.isize(); i++) {
      m_data[i] *= d;
    }
  }
  void SetScore(double d) {
    m_score = d;
  }
  double GetScore() const {return m_score;}
  void operator = (double d) {
    for (int i=0; i<m_data.isize(); i++)
      m_data[i] = d;
  }
  const svec<double> & data() const {return m_data;}
  svec<double> & data() {return m_data;}
  
  void Read(CMReadFileStream & s) {
    s.Read(m_score);
    int n;
    s.Read(n);
    m_data.resize(n);
    for (int i=0; i<m_data.isize(); i++)
      s.Read(m_data[i]);
  }
  
  void Write(CMWriteFileStream & s) {
    s.Write(m_score);
    s.Write(m_data.isize());
    for (int i=0; i<m_data.isize(); i++)
      s.Write(m_data[i]);
  }

 private:
  svec<double> m_data;
  double m_score;
};

class ScoreBuffer
{
 public:
  ScoreBuffer() {
    m_decay = 0.95;
    m_scale = 1.;
  }
  
  double GetScale() const {return m_scale;}
  
  double Norm(double score, bool & bRescaled) {
    bRescaled = false;
    double d = score * m_scale;
    if (d > 1. || d < -1.) {
      if (d < 0.)
	d = -d;
      m_scale /= d;
      //m_scale /= 4.;
      bRescaled = true;
    
      //cout << "SCORE RESET" << endl;
    }
    double ret = m_scale*score;
    m_scale = m_decay*m_scale + (1.-m_decay);
    //cout << "SCORE " << score << " -> " << ret << endl;
    return ret;
  }
  void Read(CMReadFileStream & s) {
    s.Read(m_scale);
    s.Read(m_decay);
  }
  
  void Write(CMWriteFileStream & s) {
    s.Write(m_scale);
    s.Write(m_decay);
  }

 private:
  double m_scale;
  double m_decay;
};

class NLOProblemSolver
{
public:
  NLOProblemSolver() {
    m_maxGuess = 4;
    m_cycles = 0;
    m_frame = 0;
    m_dim = 0;
    m_indim = 0;
    m_addCount = 0;
  }

  void SetCycleNumDim(int cycles, int dim) {
    m_cycles = cycles;
    m_dim = dim;
    m_try.resize(cycles*dim, 0.);
    m_curr.resize(cycles*dim, 0.);
    m_curr.SetScore(-1);
    for (int i=0; i<cycles*dim; i++)
      m_try[i] = RandomFloat(1) - .5;

    m_nn.Setup(20, cycles*dim+1, 1);
    m_nn.ReSetup(-1, 1);

    
  }

  //void Set(const svec<double> & in);
  void Get(svec<double> & out);
  void SetScore(double s);

  void Read(CMReadFileStream & s);
  void Write(CMWriteFileStream & s);

  
private:
  void FromNN();
  
  void Change(svec<double> & v, double w) {
    /*for (int i=0; i<v.isize(); i++) {
      double diff = w*(RandomFloat(2)-1);
      v[i] += diff;
      }*/

    if (RandomFloat(1.) < 0.1) {
      int idx = RandomInt(v.isize());
      v[idx] = w*(RandomFloat(2)-1); 
    } else {
    
      int idx = RandomInt(m_cycles);
      for (int i=0; i<m_dim; i++) {
	double diff = w*(RandomFloat(2)-1);
	int x = m_cycles*i+idx;
	v[x] += diff;
      }
    }
    

    
  }
  
  int m_maxGuess;
  int m_cycles;
  NLOVect m_curr;
  NLOVect m_try;
  NLOVect m_learn;
  int m_frame;
  int m_dim;
  int m_indim;
  int m_addCount;
  ScoreBuffer m_scoreBuf;
  NeuralNetwork m_nn;
  NPCIO m_io;

};


//---------------------------------------------------
class Situation
{
 public:
  Situation() {}
  
};


#endif //NLOSYS_H

