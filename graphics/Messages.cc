#include "graphics/Messages.h"

#include <stdio.h>
#include <sys/time.h>
#include "util/SysTime.h"

void TimeStamp::Update()
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  m_ms = tv.tv_sec;
  GetTime(m_r);
}
