#include "engine/FrameClock.h"
#include <stdio.h>
#include <sys/time.h>
//#include "util/SysTime.h"
#include <unistd.h>

FrameClock::FrameClock()
{  
  m_last = GetAbs();
  SetFrameRate(60);
}
  
void FrameClock::SetFrameRate(int rateInHertz)
{
  m_interval = 1000000/rateInHertz;
}
 
long long FrameClock::GetElapsed()
{
  return (GetAbs() - m_last);
}

void FrameClock::Update()
{
  m_last = GetAbs();
}

long long FrameClock::GetAbsolute()
{
  return GetAbs();
}

double FrameClock::GetSec()
{
  double d = (double)GetAbs();
  return d / 1000000;
}

long long FrameClock::WaitUntilNextFrame()
{
  int skipped = 0;
  int wait = m_interval - GetElapsed();
  //cout << "Wait: " << wait << " elapsed: " << GetElapsed() << endl;
  while (wait < 0) {
    wait += m_interval;
    skipped += m_interval;
  }
  usleep(wait);

  Update();
  return skipped;
}

long long FrameClock::GetAbs()
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec*1000000 + tv.tv_usec;
}
