#ifndef INPCCONTROL_H
#define INPCCONTROL_H


#include "base/SVector.h"

// Interface for NPC control

class INPCControl
{
 public:
  INPCControl() {}
  virtual ~INPCControl() {}
  
  virtual void SetUp(int in, int out, int reward) = 0;
  
  virtual void SetRange(double min, double max) = 0;

  // Reward values should be between -1 and +1
  virtual void Process(svec<double> & out,
		       const svec<double> & in,
		       const svec<double> & reward,
		       double deltatime) = 0;

  virtual void Read(const string & fileName) = 0;
  
  virtual void Write(const string & fileName) = 0;
		      

  virtual void Print() = 0;

};





#endif //INPCCONTROL_H
