#ifndef TMANIPULATOR_H
#define TMANIPULATOR_H



#include "engine/IManipulator.h"
#include "base/ThreadHandler.h"

class ThreadedManipulator;

class ManipulatorThread : public IOneThread
{
public:
  ManipulatorThread(ThreadedManipulator * p) {
    m_pM = p;
  }

  void Set(ThreadedManipulator * p) {
    m_pM = p;
  }

protected:

  virtual bool OnDie() {
    return true;
  }

  virtual bool OnDo(const string & msg);

  virtual bool OnInitialize(const string & msg) {
    return true;
  }


private:
  ThreadedManipulator * m_pM;
};



//======================================================
class ThreadedManipulator : public IManipulator
{
 public:
  ThreadedManipulator() : m_thread(this) {
    //cout << "Create manip " << this << endl; 
  }
  ThreadedManipulator(const ThreadedManipulator & m) : m_thread(this) {
    *this = m;
    //cout << "Create manip copy " << this << endl; 
  }

  virtual ~ThreadedManipulator() {}

  void StartThread() {
    cout << "Starting thread (manip) " << this << endl;
    m_thread.Set(this);
    m_thread.Initialize("");
    m_thread.Do("");
    cout << "Done" << endl;
  } 

  // This function will be called
  virtual void Process() = 0;

 protected:
  void Lock() {
    m_mutex.Lock();
  }
  void Unlock() {
    m_mutex.Unlock();
  }

  void WaitForSignal() {
    m_cond.Wait();
  }
  void SendSignal() {
    m_cond.Signal();
  }
  
  // Use the condition to wait/sync
  ManipulatorThread m_thread;
  ThreadCondition m_cond;
  ThreadMutex m_mutex;
};


inline bool ManipulatorThread::OnDo(const string & msg) {
  //cout << "Call do " << m_pM << endl;
  m_pM->Process();
  return true;
}


#endif //TMANIPULATOR_H
