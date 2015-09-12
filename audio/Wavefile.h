#ifndef WAVEFILE_H
#define WAVEFILE_H

#include "audio/IWaveSource.h"
#include "base/SVector.h"
#include "util/mutil.h"

class WaveFile : public IWaveSource
{
 public:
  WaveFile() {
    m_buffSize = 0;
    m_sampleRate = 0;
    m_numChannels = 0;
  }

  virtual void SetBufferSize(int size);
  virtual bool GetBuffer(char * buff);

  virtual int SampleRate();
  virtual int NumChannels();

  void Open(const string & fileName);
  void Close();


 protected:
  int m_buffSize;
  int m_sampleRate;
  int m_numChannels;
 
  CMReadFileStream m_waveFile;

};








#endif //WAVEFILE_H
