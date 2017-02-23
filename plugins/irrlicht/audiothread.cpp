#include "audiothread.h"
#include "audio/audioout.h"
#include "audio/Wavefile.h"
#include "audio/SpatialAudio.h"
#include "engine/FrameClock.h"

bool AudioThread::OnDo(const string & msg) {
  IAudioOut * ao = CreateAudio();
  ao->SetSampleRate(44100);
  ao->SetChannels(2);
  MultiSourceAudio mult;
  mult.SetSampleRate(44100);
 

  ao->Start();
  
  cout << "Start audio." << endl;
  int bufSize = ao->GetBufferSize();
  mult.SetBufferSize(bufSize, 2);
 
  char * pBuffer = new char[bufSize];
 
  int i;


  svec<SourceData> d;
  m_pData->Get(d);
  
  // That should be IN the loop!!
  for (i=0; i<d.isize(); i++) {
    if (d[i].HasChanged()) {
      //NOT CORRECT!!!!
      mult.AddAudioSource(d[i].GetName(), d[i].Coords(), d[i].WavFile());	
      cout << "Adding audio source: " << d[i].WavFile() << endl;
      d[i].SetChanged(false);
    }
  }
  //==========================================
  FrameClock clock;
  double delay = 0.03;
  //double delay = 0.1;
  double aclock = clock.GetSec();
  double last = clock.GetSec();
  double frame = mult.GetFrameTime();
  double begin = last;
  bool bFirst = true;
  //==========================================


  while(true) {
    if (m_pData->IsDie()) {
      cout << "AUDIO DIED!!" << endl;
      break;
    }

    //==========================================
    svec<SourceData> d;
    m_pData->Get(d);
    for (i=0; i<d.isize(); i++) {
      mult.SyncAddAudioSource(d[i].GetName(), d[i].Coords(), d[i].WavFile());	
    }
    m_pData->RemoveAllSources();
    //==========================================




    //cout << "Get/play buffer." << endl;
    Coordinates camPos = m_pData->GetCamPos();
    Coordinates camRot = m_pData->GetCamRot();
    mult.SetPosition(camPos);
    mult.SetRotation(camRot);

    //cout << "Get composite buffer." << endl;
    mult.GetSound(pBuffer);
    //cout << "done." << endl;

    //=================================================
    if (bFirst) {
      bFirst = false;
      last = clock.GetSec();
      aclock = last;      
    }
    double now = clock.GetSec();
    double wait =  aclock - now - delay;
    //cout << "time " << aclock - begin << " real " << now - begin << " wait " << wait << " frame " << frame << endl;
    int ms = 1000000 * wait;
    //sec = now;
    if (wait > 0 && ms > 0)
      usleep(ms);
 
    //=================================
    ao->AddBuffer(pBuffer);
    //=================================
    aclock += frame;

    
   }

  ao->Stop();
  
  delete pBuffer;


  DeleteAudio(ao);
  
  return true;
}

bool AudioThread::OnInitialize(const string & msg) {
  return true;
}
