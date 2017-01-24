#include "npc/TopLevel.h"
#include "base/RandomStuff.h"

TopLevel::TopLevel() {
  m_time = 0.;
  m_cycletime = 0.;
  m_lastScore = 0.; 
  m_counter = 0.;
}

void TopLevel::resize(int in, int out, int score, int neurons) {
 
  int layers = 1;
  int size = 2;
  
  m_nn.Setup(neurons, size*(in+out+score), layers);  
  m_nn.ReSetup(-1, 1);
  m_nn.SetTimeShift(in+out+score);  
  m_curr.resize(in, out, score);
  m_project.resize(size);
  m_hist.resize(size);
  
}

double TopLevel::Guesstimate(IOEntity & est, int level)
{
  int i;
  svec<NeuronDist> alldist;
  m_nn.GetDistSorted(alldist);
  for (i=0; i<alldist.isize(); i++) {
    cout << "  neuron " << alldist[i].Index() << " dist: " << alldist[i].Distance() << endl;
  }

  const svec<double> & top = m_nn[alldist[0].Index()].Data();
  const svec<double> & second = m_nn[alldist[1].Index()].Data();
  int is = 1;

  /*
  for (i=1; i<alldist.isize(); i++) {
    cout << "NEURON " << m_nn[i].GetHit() << endl;
    if (m_nn[i].GetHit() > 0.) {
      second = m_nn[i].Data();
      is = i;
      break;
    }
    }*/

  int offset = m_curr.isize()*level;

  est = m_curr;
  //IOEntity second = m_curr;
  for (i=0; i<est.isize(); i++) {
    est[i] = 0.;
    //second[i] = 0.;
  }
  
  double diff = 0.;
  double max = -1;
  for (i=0; i<est.scoresize(); i++) {
    int x = offset + i + est.insize() + est.outsize();
    est.score(i) = top[x] - second[x];
      
	
    diff += top[x] - second[x];
    if (top[x] > max)
      max = top[x];
  }

  for (i=0; i<est.outsize(); i++) {
    int x = offset + i + est.insize();
    est.out(i) = top[x] - second[x];
    if (diff < 0)
      est.out(i) = -est.out(i);
    
    //second.out(i) = second[x];
   // DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (est.out(i) > 1.)
      est.out(i) = 1;;
    if (est.out(i) < -1.)
      est.out(i) = -1.;

    double limit = 0.05;
    if (est.out(i) > 0. && est.out(i) < limit)
      est.out(i) = limit;
    if (est.out(i) < 0. && est.out(i) > -limit)
      est.out(i) = -limit;
    
  }

  cout << "Estimate improvement, raw, diff: " << diff << " selected second: " << is << endl;
  for (i=0; i<est.outsize(); i++) {
    cout << est.out(i) << endl;
  }
  return max;
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

       // It's shifted by 1 frame!!
      double s = m_bufPos.AddScore(score);
      if (s > 0.) {
	cout << "Learn w/ score " << score << " weight " << s << endl;	
	m_nn.Learn(seq, s);
      } else {
	double beta = m_nn.GetBeta();
	m_nn.Learn(seq, 0.2);	
	m_nn.SetBeta(beta);
      }
      
      m_hist.push_back(io);

      cout << "FOR RETRIEVE (unshifted)" << endl;
      m_hist.Print();

      //DEBUGGGGG 
      int n = m_hist.GetData(full, 1);
      seq = full;
     // Ignore future input and output

      int nn = m_hist[i].isize();
      for (i=seq.isize()-nn; i<seq.isize(); i++) {
	seq.SetValid(i, false);
      }
      
      for (i=0; i<m_hist[m_hist.isize()-2].outsize(); i++) {
	int kk = nn * (m_hist.isize()-2) + m_hist[0].insize() + i;
	seq.SetValid(kk, false);	
      }
      for (i=0; i<m_hist[m_hist.isize()-2].scoresize(); i++) {
	int kk = nn * (m_hist.isize()-2) + m_hist[0].insize() + m_hist[0].outsize() + i;
	int kk2 = nn * (m_hist.isize()-1) + m_hist[0].insize() + m_hist[0].outsize() + i;
	seq[kk] = 1.;	
 	seq[kk2] = 1.;	
	seq.SetValid(kk2, true);
      }
      
      cout << "To retrieve (shifted): " << endl;
      seq.Print();

      double nn_score;
      int index = m_nn.Retrieve(seq, nn_score);
      cout << "Retrieved neuron " << index << " score " << nn_score << endl;
      m_nn.Print();
  
      IOEntity guesstimate;
      Guesstimate(guesstimate);
      
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
      double w = 10*outscore/(10*outscore+m_counter+1.); ///DEBUGGGGG
      cout << "W " << w << " " << outscore << " " << m_counter << " " << 10*outscore+m_counter+1. << endl;
      double before = out.out(nx);

      if (outscore < 0)
	w = 0.;
      
      for (i=0; i<out.outsize(); i++) {
	out.out(i) += guesstimate.out(i);
	//cout << "out " << out.out(i) << " guess " <<  guesstimate.out(i) << endl;
	if (i == nx) {
	  out.out(nx) = (1-w)*out.out(nx) + w*guess;
	} else {
	  out.out(i) = (1-w)*out.out(i);
	}
      }
      
      for (i=0; i<io.outsize(); i++) {
	io.out(i) = out.out(i) + guesstimate.out(i);
	if (io.out(i) > 1.)
	  io.out(i) = 1.;
 	if (io.out(i) < -1.)
	  io.out(i) = -1.;
      }
      for (i=0; i<io.scoresize(); i++)
	io.score(i) = 0;

      IOEntity & tocopy = m_hist[m_hist.isize()-1];
      for (i=0; i<io.outsize(); i++)
	tocopy.out(i) = io.out(i);
      
      
      
      
      cout << "Before: " << before << " nx " << nx << " after " << out.out(nx) << " w=" << w << " os " << outscore << endl;
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
  
