
#include "npc/NLOSys.h"

void NLOProblemSolver::Get(svec<double> & out)
{
  int i;
  out.resize(m_dim);
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


void NLOProblemSolver::SetScore(double s)
{
  int i, j;
  bool bRescaled = false;
  s = m_scoreBuf.Norm(s, bRescaled);

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
  //cout << "SDIFF " << sdiff << " try " <<  m_try.GetScore() << " curr " << m_curr.GetScore() << endl;
  if (sdiff > 0. /*&& m_frame >= m_maxGuess*/) {
    for (i=0; i<m_try.isize(); i++) {
      m_curr[i] += m_try[i];
    }
    //cout << "Adding more..." << endl;
  } else {
  
    m_try = 0.;
    Change(m_try.data(), (1.-m_curr.GetScore()));
    //cout << "Change guess... " << 1.-m_curr.GetScore() << endl;
   //AddTry();
  }
  m_curr.SetScore(s);
  //m_frame++;
}
