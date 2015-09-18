#ifndef SPATIALAUDIO_H
#define SPATIALAUDIO_H


#include "base/SVector.h"
#include "physics/Coordinates.h"
#include "audio/Wavefile.h"


class AudioReceiver
{
 public:
  AudioReceiver() {
    m_lastDelay = 0;
    m_samples = 0;
    m_toCopy = 0;
  }

  void SetPosition(const Coordinates & c) {m_pos = c;}
  const Coordinates & GetPosition() const {return m_pos;}
  void NewFrame() {
    //for (int i=0; i<m_audioBuffer.isize(); i++) {
    //  m_audioBuffer[i] = 0;
    //}
  }
  void SetSampleCount(int c) {
    if (m_audioBuffer.isize() != c) {
      m_audioBuffer.resize(c, 0);
      m_insDel.resize(c, 0);
    }
  }

  void Write(char * buffer, int samples, int channel, int numChannels);
  
  void Add(const char * buffer, int samples, int delay, double dist, int channel = 0, int numChannels = 1);

 private:
  Coordinates m_pos;
  svec<short> m_audioBuffer;
  svec<short> m_insDel;
  int m_lastDelay;
  int m_toCopy;
  int m_samples;
};

//-----------------------------------------------------
class SpatialAudio
{
 public:
  SpatialAudio() {
    m_scale = 1.;
  }

  void SetBufferSize(int bufSize, int channels) {    
    m_bufferSize = bufSize/channels/2;
  }

  void SetSampleRate(int r) {
    m_sampleRate = r;
  }

  void AddReceiver(AudioReceiver & rec) {
    m_rec.push_back(rec);
  }

  // Default configurations
  void SetStereoSpeakers();
  void SetHeadphones();
  

  int GetNumChannels() const {return m_rec.isize();}
  AudioReceiver & GetReceiver(int i) {return m_rec[i];}

  Coordinates & Position() {return m_pos;}
  Coordinates & Rotation() {return m_rot;}

  void NewFrame();
  void AddSound(const char * buffer, const Coordinates & c, int channel = 0, int numChannels = 1);
  void GetSound(char * buffer, bool bClear = true);

  void SetName(const string & name) {
    m_name = name;
  }
  const string & GetName() const {return m_name;}

 private:
  int m_bufferSize;
  int m_sampleRate;
  svec<AudioReceiver> m_rec;
  Coordinates m_pos;
  Coordinates m_rot;
  double m_scale;
  string m_name;
};


class SourceInfo
{
 public:
  SourceInfo(int bufSize) {
    m_bufSize = bufSize;
    m_bLoop = true;
  }
  
  void SetLoop(bool b) {
    m_bLoop = b;
  }

  int GetBufSize() const {return m_bufSize;}

  ~SourceInfo() {
    if (m_fileName != "")
      m_w.Close();
  }

  bool GetBuffer(char * pBuffer) {
    if (m_fileName == "")
      return false;
    bool b = m_w.GetBuffer(pBuffer);
    
    if (!b && m_bLoop) {
      m_w.Close();
      m_w.Open(m_fileName);      
    }

    return b;
  }

  void SetWavFile(const string & s) {
    if (m_fileName != "")
      m_w.Close();
   
    m_fileName = s;
    m_w.Open(m_fileName);
    m_w.SetBufferSize(m_bufSize);
  }
  WaveFile & GetWavFile()  {return m_w;}

  const Coordinates & GetPosition() const {return m_pos;}
  void SetPosition(const Coordinates & c) {m_pos = c;}
 private:
  string m_fileName;
  int m_bufSize;
  WaveFile m_w;
  Coordinates m_pos;
  bool m_bLoop;
};


//--------------------------------------------------
class MultiSourceAudio
{
 public:
  MultiSourceAudio();

  ~MultiSourceAudio();

  
  void SetBufferSize(int bufSize, int channels);
  void SetSampleRate(int r);

  double GetFrameTime() const {
    return (double)m_bufferSize / (double)m_sampleRate;
  }

  SpatialAudio * AddAudioSource(const string & name,
				const Coordinates & c, 
				const string & fileName = "");

  void SyncAddAudioSource(const string & name,
			  const Coordinates & c, 
			  const string & fileName = "");
  
  void SetPosition(const Coordinates & pos);
  
  void SetRotation(const Coordinates & rot);

  SpatialAudio & operator [] (int i) {return *m_sources[i];}
  const SpatialAudio & operator [] (int i) const {return *m_sources[i];}
  int isize() const {return m_sources.isize();}

  SpatialAudio * Get(const string & name);
  
  void SetPosition(int i, const Coordinates & c) {
    m_info[i]->SetPosition(c);
  }
  
  bool GetSound(char * buffer);



 private:
  int Find(const string & name) {
    for (int i=0; i<m_sources.isize(); i++) {
      if (m_sources[i]->GetName() == name)
	return i;
    }
    return -1;
  } 


  int m_rawBufferSize;
  int m_bufferSize;
  int m_sampleRate;
  svec<SpatialAudio *> m_sources;
  svec<SourceInfo *> m_info;
  char * m_pBuffer;
  Coordinates m_pos;
  Coordinates m_rot;
  double m_scale;
};




#endif //SPATIALAUDIO_H

