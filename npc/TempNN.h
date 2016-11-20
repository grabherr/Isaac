#ifndef TEMPNN_H
#define TEMPNN_H

#include "npc/NNet.h"


class TNN
{
 public:
  TNN() {
    m_in = 0;
    m_out = 0;
    m_remember = 30;
    m_flip = 0;
    m_total = 0;
  }

  void Setup(int in, int out);

  void Feed(svec<double> & out, const svec<double> & in, double lastScore);
  
 private:  
  void Make(NPCIO & o, const svec<double> & out, const svec<double> & in);
  
  NeuralNetwork m_nn;
  svec<NPCIO> m_hist;
  svec<double> m_scores;
  svec<double> m_success; // Per neuron
  int m_remember;
  int m_in;
  int m_out;
  double m_flip;
  svec<double> m_lastOut;
  int m_total;
};







#endif

