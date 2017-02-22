#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H


#include "base/ThreadHandler.h"
#include "graphics/Messages.h"


class SourceData
{
 public:
  SourceData() {
    m_hasChanged = true;
  }

  const StreamCoordinates & Coords() const {return m_pos;}
  void SetCoords(const StreamCoordinates & c) {m_pos = c;}
  const string & WavFile() const {return m_fileName;}
  const string Name() const {return m_name;}
  
  void SetWavFile(const string & s) {
    if (s != m_fileName) {
      m_hasChanged = true;
      m_fileName = s;
    }
  }

  void SetChanged(bool b) {
    m_hasChanged = b;    
  }
  bool HasChanged() const {return m_hasChanged;}

  void SetName(const string & n) {m_name = n;}
  const string & GetName() const {return m_name;}
 private:
  StreamCoordinates m_pos;
  string m_fileName;
  string m_name;
  bool m_hasChanged;
};

class SharedAudioData
{
public:
  SharedAudioData() {
    m_bDead = false;
    m_bDie = false;
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
  
  void RemoveAllSources() {
    m_mutex.Lock();
    m_data.clear();
    m_mutex.Unlock();    
  }
  
  void RemoveSource(int index) {
    m_mutex.Lock();

    if (m_data.isize() < 2) {
      m_data.clear();
    } else {      
      m_data[index] = m_data[m_data.isize()-1];
      m_data.resize(m_data.isize()-1);
    }
    
    m_mutex.Unlock();    
  }
  
  void AddSource(const SourceData & s) {
    m_mutex.Lock();
    int i;
    for (i=0; i<m_data.isize(); i++) {
      if (m_data[i].GetName() == s.GetName()) {
	m_data[i] = s;
	m_mutex.Unlock();    
	return;
      }
    }
    m_data.push_back(s);
    m_mutex.Unlock();    
  }

  /*
  int isize() const {
    return m_data.isize();
    }*/
  
  //  SourceData & operator [] (int i) {return m_data[i];}
  void Get(svec<SourceData> & d) {
    m_mutex.Lock();
    d = m_data;
    m_mutex.Unlock();      
  }

  void SetWavFile(const string & name, const string & file) {
    m_mutex.Lock();
    int i = Index(name);
    if (i >= 0)
      m_data[i].SetWavFile(file);
    m_mutex.Unlock();       
  }

  void SetPosition(const string & name, const StreamCoordinates & c) {
    m_mutex.Lock();
    int i = Index(name);
    if (i >= 0)
      m_data[i].SetCoords(c);
    m_mutex.Unlock();       
  }

  StreamCoordinates GetCamPos() {
    m_mutex.Lock();
    StreamCoordinates tmp = m_camPos;
    //cout << "Get cam pos ";
    tmp.Print();
    m_mutex.Unlock();       
    return tmp;
  }

  StreamCoordinates GetCamRot() {
    m_mutex.Lock();
    StreamCoordinates tmp = m_camRot;
    m_mutex.Unlock();       
    return tmp;
  }
  void SetSetCamPos(const StreamCoordinates & c) {
    m_mutex.Lock();
    m_camPos = c;
    //cout << "Set cam pos ";
    m_camPos.Print();
    m_mutex.Unlock();           
  }

  void SetCamRot(const StreamCoordinates & c) {
    m_mutex.Lock();
    m_camRot = c;
    m_mutex.Unlock();     
  }



private:
  int Index(const string & name) {
    for (int i=0; i<m_data.isize(); i++) {
      if (m_data[i].Name() == name)
	return i;
    }
    return -1;
  }
  StreamCoordinates m_camPos;
  StreamCoordinates m_camRot;
   
  ThreadMutex m_mutex;
  bool m_bDie;
  bool m_bDead;
  svec<SourceData> m_data;
};

//====================================================
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
