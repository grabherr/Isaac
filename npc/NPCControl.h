#ifndef NPCCONTROL_H
#define NPCCONTROL_H


#include "npc/INPCControl.h"
#include "npc/NNet.h"
#include "npc/NLOSys.h"

// Implementation of NPC control

class NPCControl : public INPCControl
{
 public:
  NPCControl();
  virtual ~NPCControl();
  
  virtual void SetUp(int in, int out, int reward);
  
  virtual void SetRange(double min, double max);

  // Reward values should be between -1 and +1
  virtual void Process(svec<double> & out,
		       const svec<double> & in,
		       const svec<double> & reward,
		       double deltatime);

  virtual void Read(const string & fileName);
  
  virtual void Write(const string & fileName);
		      

  virtual void Print();

 protected:
  double m_time;
  double m_cycletime;
  NLOProblemSolver m_solver;
  svec<double> m_curr;
  svec<double> m_reward;
  int m_cycle;
  int m_cycleCount;
  
};

#endif


