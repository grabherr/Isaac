
#include <string>

#include "base/CommandLineParser.h"
#include "audio/audioout.h"
#include "audio/Wavefile.h"
#include "audio/SpatialAudio.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input wav file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the audio.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  IAudioOut * ao = CreateAudio();


  WaveFile w;
  w.Open(aString);
  ao->SetSampleRate(w.SampleRate());
  ao->SetChannels(w.NumChannels());

  ao->Start();
  int bufSize = ao->GetBufferSize();
  w.SetBufferSize(bufSize);
  char * pBuffer = new char[bufSize];
  
  SpatialAudio spat;
  spat.SetStereoSpeakers();
  spat.SetSampleRate(w.SampleRate());
  spat.SetBufferSize(bufSize, 2);
  double y = -2.;
  double d = 0.01;
  while(w.GetBuffer(pBuffer)) {
    cout << "Add buffer" << endl;
    spat.NewFrame();
    Coordinates pos = Coordinates(y, 2, 0);
    spat.AddSound(pBuffer, pos, 0, 2);
    y += d;
    if (y > 2.)
      d = -0.01;
    if (y < -2.)
      d = 0.01;
    cout << "POS: " << y << endl;
    spat.GetSound(pBuffer);

    ao->AddBuffer(pBuffer);
  }
  ao->Stop();

  delete pBuffer;

  return 0;
}
 