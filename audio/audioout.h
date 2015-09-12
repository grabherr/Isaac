#ifndef AUDIOOUT_H
#define AUDIOOUT_H




class IAudioOut
{
 public:
  IAudioOut() {}
 
  virtual ~IAudioOut() {}

  virtual void PlayWav(const char * fileName) = 0;

  virtual void Start() = 0;
  virtual void AddBuffer(char * buff) = 0;
  virtual void Stop() = 0;
  virtual int GetBufferSize() = 0;
  virtual void SetSampleRate(int i) = 0;
  virtual void SetChannels(int i) = 0;
  

  
};


IAudioOut * CreateAudio();
void DeleteAudio(IAudioOut * p);

#endif 
