#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include "base/SVector.h"
#include "npc/TempNN.h"

class IOEntity
{
 public:
  IOEntity() {
    m_in = 0;
    m_out = 0;
    m_score = 0;
    m_accum = 0.;
  }

  void resize(int in, int out, int score) {
    m_in = in;
    m_out = out;
    m_score = score;
    m_data.resize(m_in+m_out+m_score, 0.);
  }

  int isize() const {return m_data.isize();}
  int insize() const {return m_in;}
  int outsize() const {return m_out;}
  int scoresize() const {return m_score;}

  const double & operator[] (int i) const {return m_data[i];}
  double & operator[] (int i) {return m_data[i];}
  
  const double & in(int i) const {return m_data[i];}
  double & in(int i) {return m_data[i];}

  const double & out(int i) const {return m_data[i+m_in];}
  double & out(int i) {return m_data[i+m_in];}

  const double & score(int i) const {return m_data[i+m_in+m_out];}
  double & score(int i) {return m_data[i+m_in+m_out];}
 
  void operator += (const IOEntity & v) {
    int i;
    double w1 = m_accum/(m_accum+1);
    double w2 = 1. - w1;
    for (i=0; i<m_data.isize(); i++) {
      double d = m_data[i]*w1 + v.m_data[i]*w2;
      //cout << m_data[i] << " -> " << d << " " << v.m_data[i] << " . " << m_accum << endl;
      m_data[i] = d;
    }
    m_accum += 1.;
  }
  void reset() {m_accum = 0.;}

  void Print() const {
    cout << "Entity:" << endl;
    int i;

    for (i=0; i<m_data.isize(); i++)
      cout << i << ": " << m_data[i] << endl;
  }
  
 private:
  svec<double> m_data;
  int m_in;
  int m_out;
  int m_score;
  double m_accum;
};

class IOVector
{
 public:

  IOVector() {
    m_counter = 0;
  }

  void resize(int n) {
    m_ent.resize(n);
  }

  int isize() const {return m_ent.isize();} 
  const IOEntity & operator[] (int i) const {return m_ent[i];}
  IOEntity & operator[] (int i) {return m_ent[i];}

  void push_back(const IOEntity & e) {
    //cout << m_counter << " " << isize() << endl;
    if (m_counter < isize()) {
      m_ent[m_counter] = e;
    } else {
      for (int i=1; i<isize(); i++)
	m_ent[i-1] = m_ent[i];
      m_ent[isize()-1] = e;
    }
    m_counter++;
  }

  bool full() const {
    return (m_counter >= isize());
  }
  
  void reset() {
    for (int i=0; i<m_ent.isize(); i++)
      m_ent[i].reset();
  }
  
  bool GetData(svec<double> & d, int shift = 0) const {
    d.resize(m_ent.isize()*m_ent[0].isize(), 0);
    int i, j;
    int k = 0;
  
    for (i=shift; i<m_ent.isize(); i++) {
      const IOEntity & e = m_ent[i];
      for (j=0; j<e.isize(); j++) {
	d[k] = e[j];
	k++;
      }
    }
    for (; k<d.isize(); k++)
      d[k] = 0.;
    return shift*m_ent[0].isize();
  }
  
  void SetData(const svec<double> & d) {
    int i, j;
    int k = 0;
    for (i=0; i<m_ent.isize(); i++) {
      IOEntity & e = m_ent[i];
      for (j=0; j<e.isize(); j++) {
	e[j] = d[k];
	k++;
      }
    }
  }

  void Print() const {
    cout << "Printing vector:" << endl;
    for (int i=0; i<m_ent.isize(); i++) {
      for (int j=0; j<m_ent[i].isize(); j++)
	cout << (m_ent[i])[j] << endl;
    }
  }
  
 private:
  //vec<double> m_data;
  svec<IOEntity> m_ent;
  int m_counter;
};


class TopScoreBuffer
{
 public:
  TopScoreBuffer() {
    m_hi = -1.;
    m_decay = 0.99;
    m_last = -1.;
  }

  // Returns the weight
  double AddScore(double d) {
    m_last = d;
    cout << "SCORE " << d << " curr " << m_last << " hi " << m_hi << " -> ";
    if (m_last > m_hi) {
      m_hi = m_last;
      m_last = d;
      cout << "1" << endl;
      return 1.;
    }
    if (m_last < 0 && m_hi > 0) {
      m_last = d;
      cout << "0" << endl;
      return 0.;
    }
    double diff = m_hi - m_last;
    m_last = d;
   
    m_hi *= m_decay;
    cout << 1./(1.+diff*200) << endl;
    return 1./(1.+diff*200);
  }

 private:
  double m_hi;
  double m_decay;
  double m_last;
};

//========================================================
class TopLevel
{
 public:
  TopLevel();

  void Update(IOEntity & io, double deltatime, double score);
  void resize(int in, int out, int score, int neurons = 10);

 private:
  double Guesstimate(IOEntity & est, int level = 0);  

  
  IOVector m_hist;
  IOVector m_project;
  IOEntity m_curr;
  double m_time;
  double m_cycletime;
  double m_lastScore;
  NeuralNetwork m_nn;
  double m_counter;
  TopScoreBuffer m_bufPos;
  
};



#endif //TOPLEVEL_H
