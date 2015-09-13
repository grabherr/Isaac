#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H


#include "base/ThreadHandler.h"
#include "graphics/Messages.h"




class SharedAudioData
{
public:
  SharedAudioData() {
  }

  void SetDie(bool bDie) {
    m_mutex.Lock();
    m_bDie = bDie;
    m_mutex.Unlock();
  }

  bool IsDie() {
    m_mutex.Lock();
    bool bDie = m_bDie;
    m_mutex.Unlock();
    return bDie;
  }
 

private:
  
  ThreadMutex m_mutex;
  bool m_bDie;
};


class AudioThread : public IOneThread
{
public:
  AudioThread() {
    m_pData = NULL;
    m_id = -1;
  }

  AudioThread(SharedAudioData * p, int id) {
    m_pData = p;
    m_id = id;
  }

  void SetData(SharedAudioData * p, int id) {
    m_pData = p;
    m_id = id;
  }
protected:

  virtual bool OnDie() {
    return true;
  }

  virtual bool OnDo(const string & msg);

  virtual bool OnInitialize(const string & msg);

private:
  SharedAudioData * m_pData;
  int m_id;
};





#endif //AUDIOTHREAD_H
