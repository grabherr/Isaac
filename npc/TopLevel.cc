#include "npc/TopLevel.h"


TopLevel::TopLevel() {
  m_time = 0.;
  m_cycletime = 0.;
  m_lastScore = 0.; 

}

void TopLevel::resize(int in, int out, int score) {
  int neurons = 20;
  int layers = 1;
  int size = 10;
  
  m_nn.Setup(neurons, size*(in+out+score), layers);  
  m_nn.ReSetup(-1, 1);
  m_nn.SetTimeShift(in+out+score);  
  
  m_curr.resize(in, out, score);
  m_hist.resize(size);
}


void TopLevel::Update(IOEntity & io, double deltatime, double score)
{
  int i, j;

  m_time += deltatime;
  m_cycletime += deltatime;

  if (m_cycletime > 0.5) {
    m_hist.push_back(m_curr);
    m_curr.reset();
    m_cycletime = 0.;
  }
  

  // Fill io here
  if (m_hist.full()) {
    svec<double> full;
    m_hist.GetData(full);

    NPCIO seq;
    seq.resize(full.isize());
    seq = full;
    seq.SetAllValid(true);

    m_nn.Learn(seq);

    int n = m_hist.GetData(full, 1);
    seq = full;
    for (i=seq.isize()-n; i<seq.isize(); i++)
      seq.SetValid(i, false);
   
    double score;
    
    m_nn.Retrieve(seq, score);
    IOVector hyp = m_hist;
    
    hyp.SetData(seq.data());
    
    
  }

  // ============
  
  m_curr += io;
}
  
