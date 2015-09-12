#ifndef SPATIALAUDIO_H
#define SPATIALAUDIO_H


#include "base/SVector.h"
#include "physics/Coordinates.h"


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
  
  void Add(const char * buffer, int samples, int delay, int channel = 0, int numChannels = 1);

 private:
  Coordinates m_pos;
  svec<short> m_audioBuffer;
  svec<short> m_insDel;
  int m_lastDelay;
  int m_toCopy;
  int m_samples;
};


class SpatialAudio
{
 public:
  SpatialAudio() {

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
  

  int GetNumChannels() {return m_rec.isize();}
  AudioReceiver & GetReceiver(int i) {return m_rec[i];}

  Coordinates & Position() {return m_pos;}
  Coordinates & Rotation() {return m_rot;}

  void NewFrame();
  void AddSound(const char * buffer, const Coordinates & c, int channel = 0, int numChannels = 1);
  void GetSound(char * buffer);

 private:
  int m_bufferSize;
  int m_sampleRate;
  svec<AudioReceiver> m_rec;
  Coordinates m_pos;
  Coordinates m_rot;
};









#endif //SPATIALAUDIO_H

