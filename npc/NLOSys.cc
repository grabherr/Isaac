
#include "npc/NLOSys.h"

void NLOProblemSolver::Get(svec<double> & out)
{
  int i;
  out.resize(m_dim);

  /*
  m_curr[0] = -1.78032;
  m_curr[1] = 0.052431;
  m_curr[2] = -0.0695456;
  m_curr[3] = 0.170352;
  m_curr[4] = -0.474273;
  m_curr[5] = -0.181738;
  m_curr[6] = -0.507059;
  m_curr[7] = 0.0802566;
  for (i=0; i<m_try.isize(); i++)
    m_try[i] = 0;
  */

  
  for (i=0; i<m_dim; i++) {
    int idx = i*m_cycles+m_frame;
    //cout << "Index " << idx << endl;
    out[i] = m_curr[idx];
    out[i] += m_try[idx];

    
    
    //cout << "    " << idx << " ... " << m_curr[idx] << " " << m_try[idx] << " -> " << out[i] << endl;
  }

  //cout << "Internal index " << m_frame << endl;
  
  m_frame++;
  if (m_frame >= m_cycles)
    m_frame = 0;
}

void NLOProblemSolver::FromNN()
{
  double best = -1.;
  double worst = 1.;
  int i;
  int indexBest = -1;
  int indexWorst = -1;

  for (i=0; i<m_nn.isize(); i++) {
    const Neuron & n = m_nn[i];
    if (n[n.isize()-1] > best) {
      indexBest = i;
      best = n[n.isize()-1];
    }
    if (n[n.isize()-1] < worst) {
      indexWorst = i;
      worst = n[n.isize()-1];
    }
  }
  cout << "Solver best=" << best << " worst=" << worst << endl;
  if (best > 0) {
    for (i=0; i<m_try.isize(); i++) {
      m_try[i] = 0;
      m_curr[i] = (m_nn[indexBest])[i];
    }
  } else {
    for (i=0; i<m_try.isize(); i++) {
      m_try[i] = 0;
      m_curr[i] = -(m_nn[indexBest])[i];
    }
  }
}

void NLOProblemSolver::SetScore(double s)
{
  
  int i, j;
  bool bRescaled = false;
  s = m_scoreBuf.Norm(s, bRescaled);
  cout << "Solver score (internal) " << s << endl;
  if (bRescaled) {
    m_curr.SetScore(m_curr.GetScore()*m_scoreBuf.GetScale());
  }


  
  m_try.SetScore(s);
  if (m_curr.isize() == 0) {
    m_curr = m_try;
    m_try = 0.;
    Change(m_try.data(), 0.1);
    return;
  }
  
  
  double sdiff = m_try.GetScore() - m_curr.GetScore();

  double plusminus = 1.;
  if (sdiff < 0.)
    plusminus *= -1.;

  m_io.resize(m_curr.isize()+1);
  m_io.SetAllValid(true);
  for (i=0; i<m_curr.isize(); i++)
    m_io[i] = m_curr[i] + m_try[i];
  m_io[i] = s;
  m_nn.Learn(m_io);
  cout << "Print NN" << endl;
  m_nn.Print();
  
  //cout << "SDIFF " << sdiff << " try " <<  m_try.GetScore() << " curr " << m_curr.GetScore() << endl;
  if (sdiff > 0. /*&& m_frame >= m_maxGuess*/) {
    for (i=0; i<m_try.isize(); i++) {
      if (m_curr[i] + m_try[i] > 1. || m_curr[i] + m_try[i] < -1.) {
	m_try[i] = -m_try[i];
      }
      m_curr[i] += m_try[i];
    }
    cout << "Solver: continue..." << endl;
    m_addCount++;
  } else {
  
    m_try = 0.;

    if (s < 0. && m_curr.GetScore() < 0. && RandomFloat(1.) < 0.5) {
      cout << "Solver: get hypothesis from NN" << endl;
      FromNN();
    } else {

      Change(m_try.data(), (1.-m_curr.GetScore()));
      for (i=0; i<m_try.isize(); i++) {
	if (m_curr[i] + m_try[i] > 1. || m_curr[i] + m_try[i] < -1.) {
	  m_try[i] = -m_try[i];
	}
      }
      cout << "Solver: change guess... " << 1.-m_curr.GetScore() << endl;
    }
    m_addCount = 0;
   //AddTry();
  }
  m_curr.SetScore(s);
  //m_frame++;
}

void NLOProblemSolver::Read(CMReadFileStream & s)
{
  s.Read(m_maxGuess);
  s.Read(m_cycles);
  s.Read(m_dim);
  s.Read(m_indim);
  s.Read(m_addCount);
  SetCycleNumDim(m_cycles, m_dim);
  m_curr.Read(s);
  m_try.Read(s);
  m_learn.Read(s);
  m_scoreBuf.Read(s);
  m_nn.Read(s);
}

void NLOProblemSolver::Write(CMWriteFileStream & s)
{
  s.Write(m_maxGuess);
  s.Write(m_cycles);
  s.Write(m_dim);
  s.Write(m_indim);
  s.Write(m_addCount);
  m_curr.Write(s);
  m_try.Write(s);
  m_learn.Write(s);
  m_scoreBuf.Write(s);
  m_nn.Write(s);

}
