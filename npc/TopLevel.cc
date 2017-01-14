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
  //m_nn.SetBeta(0.1);
  m_curr.resize(in, out, score);
  m_project.resize(size);
  m_hist.resize(size);
  
  //cout << "Size " << m_hist.isize() << endl;
}


void TopLevel::Update(IOEntity & io, double deltatime, double score)
{
  cout << "===================== ENTER UPDATE ==========================" << endl;
  cout << "io: " << io.in(0) << " score " << score << endl;
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
      
      //IOEntity & last = m_hist[m_hist.isize()-1];
      //for (i=0; i<io.scoresize(); i++)
      //last.score(i) = io.score(i);

      IOEntity & last = m_hist[m_hist.isize()-1];
      for (i=0; i<last.scoresize(); i++)
	last.score(i) = score;

      IOVector tmphist = m_hist;
      IOEntity & lasttmp = tmphist[tmphist.isize()-1];

      if (score < 0.) {
	cout << "Inverting..." << endl;
	for (i=0; i<lasttmp.scoresize(); i++)
	  lasttmp.out(i) *= -1;	
      }
      
      svec<double> full;
      tmphist.GetData(full);      
      
      NPCIO seq;
      seq.resize(full.isize());
      seq = full;
      seq.SetAllValid(true);
      cout << "INPUT" << endl;
      m_hist.Print();
      //seq.Print();

      //if (score > 0.55) {
      // It's shifted by 1 frame!!
      double s = m_bufPos.AddScore(score);
      if (s > 0.) {
	cout << "Learn w/ score " << score << " weight " << s << endl;	
	m_nn.Learn(seq, s);
      }
      /*
      if (score < 0.) {
	s = 0.2;
	cout << "Learn INVERSE w/ score " << score << " weight " << s << endl;	
	m_nn.Learn(seq, s);
	}*/
      //}
      
      // Look one ahead
      //int n = m_hist.GetData(full, 0);
      //seq = full;
      //cout << "To retrieve (not shifted): " << endl;
      //seq.Print();
      
      m_hist.push_back(io);

      cout << "FOR RETRIEVE (unshifted)" << endl;
      m_hist.Print();

      //DEBUGGGGG 
      int n = m_hist.GetData(full, 1);
      seq = full;
     // Ignore future input and output

      int nn = m_hist[i].isize();
      for (i=seq.isize()-nn; i<seq.isize(); i++) {
	//cout << "a " << kk << endl;
	seq.SetValid(i, false);
      }
      
      for (i=0; i<m_hist[m_hist.isize()-2].outsize(); i++) {
	int kk = nn * (m_hist.isize()-2) + m_hist[0].insize() + i;
	//cout << "a " << kk << " " << nn << " " << m_hist.isize()-2 << " " << m_hist[0].insize() << endl;
	seq.SetValid(kk, false);	
      }
      for (i=0; i<m_hist[m_hist.isize()-2].scoresize(); i++) {
	int kk = nn * (m_hist.isize()-2) + m_hist[0].insize() + m_hist[0].outsize() + i;
	int kk2 = nn * (m_hist.isize()-1) + m_hist[0].insize() + m_hist[0].outsize() + i;
	//cout << "b " << kk << endl;
	//seq.SetValid(kk, false);	
 	seq[kk] = 1.;	
 	seq[kk2] = 1.;	
	seq.SetValid(kk2, true);
      }
      
      cout << "To retrieve (shifted): " << endl;
      seq.Print();

      // HACK!!!!!
      //seq.SetValid(seq.isize()-1, true);
      //seq[seq.isize()-1] = 1.;
      //==================================================
      
      //double nn_score;
      //m_hist.push_back(io);
      //m_hist.GetData(full, 1);
      //seq.resize(full.isize());
      //seq = full;
      //cout << "FOR RETRIEVE" << endl;
      //seq.Print();
      double nn_score;
      int index = m_nn.Retrieve(seq, nn_score);
      cout << "Retrieved neuron " << index << " score " << nn_score << endl;
      m_nn.Print();
      
      //for (i=0; i<io.outsize(); i++)
      //io.out(i) = out.out(i);
     
      IOVector hyp = m_hist;
      // Use the neural data!
      hyp.SetData(m_nn[index].Data());
      cout << "RETURNED" << endl;
      hyp.Print();
      // DEBUGGGGG
      IOEntity out = hyp[hyp.isize()-2];
      double outscore = out.score(0);
     //out.Print();
  
      
      int nx = RandomInt(out.outsize());
      double guess = 1. - RandomFloat(2.);
      double w = 50*outscore/(50*outscore+m_counter);
      double before = out.out(nx);
      //if (outscore < 0.)
      //w = 0.9;
      
      out.out(nx) = (1-w)*out.out(nx) + w*guess;
      for (i=0; i<io.outsize(); i++)
	io.out(i) = out.out(i);
      for (i=0; i<io.scoresize(); i++)
	io.score(i) = 0;

      IOEntity & tocopy = m_hist[m_hist.isize()-1];
      for (i=0; i<io.outsize(); i++)
	tocopy.out(i) = io.out(i);
      
      
      //m_lastScore = score;
      
      
      cout << "Before: " << before << " after " << out.out(nx) << " w=" << w << endl;
      m_hist.GetData(full);
      seq.resize(full.isize());
      seq = full;
      cout << "OUTPUT (old)" << endl;
      seq.Print();
      
      cout << "FINAL" << endl;
      m_hist.Print();
     
       
    }
  }

  // ============
  
}
  
