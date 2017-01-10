#include "npc/TopLevel.h"
#include "base/RandomStuff.h"

TopLevel::TopLevel() {
  m_time = 0.;
  m_cycletime = 0.;
  m_lastScore = 0.; 

}

void TopLevel::resize(int in, int out, int score) {
  int neurons = 10;
  int layers = 1;
  int size = 2;
  
  m_nn.Setup(neurons, size*(in+out+score), layers);  
  m_nn.ReSetup(-1, 1);
  m_nn.SetTimeShift(in+out+score);  
  
  m_curr.resize(in, out, score);
  m_project.resize(size);
  m_hist.resize(size);
  
  //cout << "Size " << m_hist.isize() << endl;
}


void TopLevel::Update(IOEntity & io, double deltatime, double score)
{
  int i, j;
  m_counter += 1.;
  
  m_time += deltatime;
  m_cycletime += deltatime;

  m_curr += io;

  if (m_cycletime > 0.5) {
    //cout << "..." << endl;
    if (!m_hist.full())
      m_hist.push_back(m_curr);
    m_curr.reset();
    m_cycletime = 0.;
  
  

    // Fill io here
    if (m_hist.full()) {
      //cout << "Full" << endl;
      
      IOEntity & last = m_hist[m_hist.isize()-1];
      for (i=0; i<io.scoresize(); i++)
	last.score(i) = io.score(i);
      
      svec<double> full;
      m_hist.GetData(full);
      
      
      NPCIO seq;
      seq.resize(full.isize());
      seq = full;
      seq.SetAllValid(true);
      
      m_nn.Learn(seq);
      cout << "Learn w/ " << endl;
      seq.Print();
      
      // Look one ahead
      int n = m_hist.GetData(full, 1);
      seq = full;
      // Ignore future input and output
      for (i=seq.isize()-n; i<seq.isize(); i++)
	seq.SetValid(i, false);

      // HACK!!!!!
      seq.SetValid(seq.isize()-1, true);
      seq[seq.isize()-1] = 1.;
      //==================================================
      
      double score;
      
      int index = m_nn.Retrieve(seq, score);
      
      m_nn.Print();
      
      IOVector hyp = m_hist;

      // Use the neural data!
      hyp.SetData(m_nn[index].Data());
      
      IOEntity out = hyp[hyp.isize()-1];
      out.Print();
      seq.Print();

      
      int nx = RandomInt(out.outsize());
      double guess = 1. - RandomFloat(2.);
      double w = 20/(20+m_counter);
      double before = out.out(nx);
      out.out(nx) = (1-w)*out.out(nx) + w*guess;
      for (i=0; i<io.outsize(); i++)
	io.out(i) = out.out(i);
      cout << "Before: " << before << " after " << out.out(nx) << " w=" << w << endl;
      m_hist.push_back(io);
    }
  }

  // ============
  
}
  
