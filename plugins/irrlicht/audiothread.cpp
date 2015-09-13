#include "audiothread.h"
#include "audio/audioout.h"

bool AudioThread::OnDo(const string & msg) {
  IAudioOut * ao = CreateAudio();
  ao->SetSampleRate(44100);
  ao->SetChannels(2);
  
  ao->Start();
  
  while(true) {
    if (m_pData->IsDie()) 
      break;
  }

  ao->Stop();
  
  DeleteAudio(ao);
  
  return true;
}

bool AudioThread::OnInitialize(const string & msg) {
  return true;
}
