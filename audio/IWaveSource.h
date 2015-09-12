#ifndef WAVESOURCE_H
#define WAVESOURCE_H


class IWaveSource
{
 public:
  virtual ~IWaveSource() {}

  virtual int SampleRate() = 0;
  virtual int NumChannels() = 0;

  virtual void SetBufferSize(int size) = 0;
  virtual bool GetBuffer(char * buff) = 0;
};








#endif //WAVESOURCE_H
