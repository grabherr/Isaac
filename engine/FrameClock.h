// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef FRAMECLOCK_H
#define FRAMECLOCK_H

#include "base/StringUtil.h"

class FrameClock
{
 public:
  FrameClock();
  
  void SetFrameRate(int rateInHertz);
  
  // Returns time since last call
  long long GetElapsed();

  // Returns time in seconds
  double GetSec();

  // Absolute time
  long long GetAbsolute();

  // Updates current time
  void Update();

  // Returns the time elapsed (can be more than a frame
  // if skipped)
  long long WaitUntilNextFrame();

 private:
  long long GetAbs();

  long long m_interval;
  long long m_last;
};




#endif 

