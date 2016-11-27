
#include "npc/NPCControl.h"


NPCControl::NPCControl() {
  m_time = 0.;
  m_cycletime = 0.;
  m_cycle = 0;
  m_cycleCount = 4;
}

NPCControl::~NPCControl() {
}
  
void NPCControl::SetUp(int in, int out, int reward)
{
  m_solver.SetCycleNumDim(m_cycleCount, out);
  m_reward.resize(reward, 0.);
}
  
void NPCControl::SetRange(double min, double max)
{
}

void NPCControl::Process(svec<double> & out,
			 const svec<double> & in,
			 const svec<double> & reward,
			 double deltatime)
{
  int i;
  
  m_time += deltatime;
  m_cycletime += deltatime;
  
  
  //out[0] = sin(5*m_time);
  //out[1] = cos(5*m_time);

  for (i=0; i<reward.isize(); i++) {
    if (reward[i] < 1. && reward[i] > -1)
      m_reward[i] += reward[i];
  }
  
  if (m_curr.isize() == 0 || m_cycletime > 0.2) {
    m_solver.Get(m_curr);
    cout << "Solver curr " << m_curr[0] << " " << m_curr[1]  << endl;
    m_cycletime = 0.;
    m_cycle++;
    if (m_cycle == m_cycleCount) {
      m_cycle = 0;
      double score = 0;
      //for (i=0; i<m_cycleCount; i++) {
      score += m_reward[0]/10;
      m_reward[0] = 0.;
      //if (score > 1.)
      //score = 1.;
      if (score < -1.)
	score = -1.;
      
      m_solver.SetScore(score);
      cout << "Solver score=" << score << endl;
    }

  }

  out = m_curr;
  for (i=0; i<out.isize(); i++)
    out[i] *= 5.;
}

void NPCControl::Read(const string & fileName)
{
  throw;
}
  
void NPCControl::Write(const string & fileName)
{
  throw;
}
		      

void NPCControl::Print()
{
}
