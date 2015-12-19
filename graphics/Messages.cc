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


static int g_unique = 0;

NameType::NameType() {
  m_bLighting = 0;
  m_scale = 10.;
  char tmp[256];
  sprintf(tmp, "object_%d", g_unique);
  m_name = tmp;
  g_unique++;
}

UpdatableMessage::UpdatableMessage() {
  m_dirty = true;
  char tmp[256];
  sprintf(tmp, "scenenode_%d", g_unique);
  m_name = tmp;
  g_unique++;
}

MsgSound::MsgSound()
{
  //char tmp[256];
  //sprintf(tmp, "sound_%d", g_unique);
  //m_name = tmp;
  //g_unique++;
}
