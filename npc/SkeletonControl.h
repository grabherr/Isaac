#ifndef SKELTONCONTROL_H
#define SKELTONCONTROL_H

#include "npc/Skeleton.h"
#include "npc/NNet.h"


class MotorControl
{
public:
  MotorControl() {
    m_index = 0;
  }

  void SetUp(int in, int out) {
    m_nn.Setup(50, in + out);
    m_nn.SetDecay(0.99);
    m_off = in;
    m_last.resize(in+out);
  }
  
  void SetRange(int i, double min, double max) {
    m_nn.ReSetup(i, min, max);
  }
  void SetRange(double min, double max) {
    m_nn.ReSetup(min, max);
  }

  void Retrieve(svec<double> & out, const svec<double> & in) {
    NPCIO tmp;
    tmp.resize(m_last.isize());
    int i;
    for (i=0; i<in.isize(); i++) {
      tmp[i] = in[i];
      tmp.SetValid(i, true);
    }
    for (i=0; i<out.isize(); i++) {
      tmp[i+in.isize()] = in[i];
      tmp.SetValid(i+in.isize(), false);
    }
    m_nn.Retrieve(tmp);
    m_last = tmp;
    for (i=0; i<in.isize(); i++) {
      m_last.SetValid(i, true);
    }
    for (i=in.isize(); i<tmp.isize(); i++) {
      out[i-in.isize()] = tmp[i];
      m_last.SetValid(i, true);
    }

  }

  void Print() const {
    m_nn.Print();
  }
  void Learn(double weight) {
    m_nn.Learn(m_last, 0.5*weight);
  }

  void LearnAvoid(double weight) {
    m_nn.LearnAvoid(m_last, 0.1*weight);
  }

  void SetName(const string & n) {
    m_name = n; 
  }
  const string & GetName() const {return m_name;}
  void SetIndex(int i) {
    m_index = i; 
  }
  int GetIndex() const {return m_index;}
  
private:
  NeuralNetwork m_nn;
  int m_off;
  NPCIO m_last;
  NPCIO m_lastForTrain;
  int m_index;
  string m_name;

};

//======================================================
class SkeletonControl
{
public:
  SkeletonControl() {}

  void SetNumControls(int n) {
    m_controls.resize(n);
    m_out = 0;
    m_lastOut.resize(n, 0.);
  }
  
  void SetUp(int in, int out) {
    int i;

    // Reserve space for self-feedback
    for (i=0; i<m_controls.isize(); i++)
      m_controls[i].SetUp(in + isize(), out);

    m_out = out;
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

  
  void Retrieve(svec<double> & out, const svec<double> & in_raw) {
    int i, j;
    int k = 0;

    svec<double> in = in_raw;

    out.resize(m_controls.isize()*m_out, 0.);
    
    for (i=0; i<m_lastOut.isize(); i++)
      in.push_back(m_lastOut[i]*0.01);
    
    for (i=0; i<m_controls.isize(); i++) {
      svec<double> tmp;
      tmp.resize(m_out, 0.);    
      m_controls[i].Retrieve(tmp, in);
      
      for (j=0; j<tmp.isize(); j++) {
	out[k] = tmp[j];
	k++;
      }
      
    }
    m_lastOut = out;
    //m_controls[0].Print();
  }

  void Learn(double weight) {
    int j;  
    //m_controls[0].Print();
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].Learn(weight);
    //m_controls[0].Print();
  }

  void LearnAvoid(double weight) {
    int j;
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].LearnAvoid(weight);
  }

  void Learn(int j, double weight) {
    m_controls[j].Learn(weight);
  }

  void LearnAvoid(int j, double weight) {
    m_controls[j].LearnAvoid(weight);
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
  
private:
  svec<MotorControl> m_controls;
  int m_out;
  svec<double> m_lastOut;
};






#endif
