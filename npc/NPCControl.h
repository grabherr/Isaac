#ifndef NPCCONTROL_H
#define NPCCONTROL_H


#include "npc/INPCControl.h"
#include "npc/NNet.h"
#include "npc/NLOSys.h"
#include "util/mutil.h"

// Implementation of NPC control

class NPCControl //: public INPCControl
{
 public:
  NPCControl();
  ~NPCControl();
  
  void SetUp(int in, int out, int reward);
  
  void SetRange(double min, double max);

  // Reward values should be between -1 and +1
  void Process(svec<double> & out,
	       const svec<double> & in,
	       const svec<double> & reward,
	       double deltatime);

  void Read(const string & fileName);
  void Write(const string & fileName);
		      
  void Read(CMReadFileStream & s);
  void Write(CMWriteFileStream & s);

  void Print();

 protected:
  double m_time;
  double m_cycletime;
  NLOProblemSolver m_solver;
  svec<double> m_curr;
  svec<double> m_reward;
  int m_cycle;
  int m_cycleCount;
  
};



class NPCControlComplex
{
 public:
  NPCControlComplex();
  ~NPCControlComplex();

  void SetNumControls(int n);
  void SetUp(int i, int in, int out, int reward);
  
  void SetRange(double min, double max);

  // Reward values should be between -1 and +1
  void Process(svec<double> & out,
	       const svec<double> & in,
	       const svec<double> & reward,
	       double deltatime);

  void Read(const string & fileName);
  void Write(const string & fileName);
		      
  void Read(CMReadFileStream & s);
  void Write(CMWriteFileStream & s);

  void Print();
 private:
  svec<NPCControl> m_ctrls;
  svec<double> m_allIn;
  svec<double> m_allOut;
  svec<double> m_allReward;
};


#endif


