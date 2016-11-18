#include "npc/SkeletonControl.h"


void MotorControl::PruneQueue()
{
  if (m_trainQueue.isize() < m_maxQueue)
    return;
  
  int n = 20;
  for (int i=n; i<m_trainQueue.isize(); i++)
    m_trainQueue[i-n] = m_trainQueue[i];
  m_trainQueue.resize(m_trainQueue.isize()-n);
  m_frameStart += n;
}

void MotorControl::AddToQueue(const NPCIO & f)
{
  m_trainQueue.push_back(f);
  PruneQueue();
}

bool LearnControl::GetLearnFrames(int & start, int & end, double & weight)
{
  //return false;
  
  int i;
  int bad = 0;
  end = -1;
  start = -1;
  weight = 0.;
  for (i=m_hist.isize()-1; i>=0; i--) {
    if (m_hist[i].Good() < 0.) {
      if (end != -1) {
	start = i + 1  + m_frameStart;
	weight = GetWeightHi(weight, end-start+1);
	if (end - start >=3 )
	  return true;
	else
	  return false;
      }
      
      bad++;
      if (bad > 5)
	return false;
      continue;
    }
    if (end == -1)
      end = i + m_frameStart;
    weight += m_hist[i].Good();
  }

  if (end < 0)
    return false;
  
  start = m_frameStart;
  weight = GetWeightHi(weight, end-start+1);
  return true;
}

bool LearnControl::GetAvoidFrames(int & start, int & end, double & weight)
{
  return false;
  
  int i;
  int good = 0;
  end = -1;
  start = -1;
  weight = 0.;
  for (i=m_hist.isize()-1; i>=0; i--) {
    if (m_hist[i].Good() >= 0.) {
      if (end != -1) {
	start = i + 1  + m_frameStart;
	weight = GetWeightLo(weight, end-start+1);
	return true;
      }
      
      good++;
      if (good > 0)
	return false;
      continue;
    }
    if (end == -1)
      end = i + m_frameStart;
    weight -= m_hist[i].Good();
  }
  if (end < 0)
    return false;
  
  //start = m_frameStart;
  start = end;
  weight = GetWeightHi(weight, end-start+1);
  return true;
}

#define LEARNINT 10

void SkeletonControl::SetSuccess(const SuccessFeature & f)
{
  int i;
  int mis = m_frame - m_learn.Frame();
  if (f[0] > 0.1)
    m_useOpt = false;

  if (f[0] < 0.)
    m_lf = LEARNINT;
  
  if (mis == 1) {
    m_learn.Add(f);
    return;
  }
  if (mis > 1) {
    SuccessFeature dummy;
    dummy.resize(f.isize());
    for (i=1; i<mis; i++)
      m_learn.Add(dummy);
    m_learn.Add(f);      
  } else {
    cout << "ERROR: SkeletonControl::SetSuccess" << endl;
    throw;
  }
}


void SkeletonControl::LearnOrAvoid()
{
  int j;

  int from, to;
  double weight = 1.;

  if (m_lf < LEARNINT) {
    m_lf++;
    return;
  }
  m_lf = 0;
  
  if (m_learn.GetLearnFrames(from, to, weight)) {
    // DEBUG!!!!!!!!!!!!!!!!!!!!!!
    // DEBUG!!!!!!!!!!!!!!!!!!!!!!
    // DEBUG!!!!!!!!!!!!!!!!!!!!!!
    // DEBUG!!!!!!!!!!!!!!!!!!!!!!
    //weight = 1.;
    // DEBUG!!!!!!!!!!!!!!!!!!!!!!
    // DEBUG!!!!!!!!!!!!!!!!!!!!!!
    cout << "Learning frames " << from << " - " << to << " w=" << weight << " curr=" << m_frame << endl;
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].Learn(weight, from, to);
  }
  
  if (m_learn.GetAvoidFrames(from, to, weight)) {
    
    cout << "Learn/avoid frames " << from << " - " << to << " w=" << weight << " curr=" << m_frame << endl;
  
    for (j=0; j<m_controls.isize(); j++)
      m_controls[j].LearnAvoid(weight, m_frame, m_frame);
  }
  
}
