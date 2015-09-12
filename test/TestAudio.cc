
#include <string>

#include "base/CommandLineParser.h"
#include "audio/audioout.h"
#include "audio/Wavefile.h"

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
  
  while(w.GetBuffer(pBuffer)) {
    cout << "Add buffer" << endl;
    ao->AddBuffer(pBuffer);
  }
  ao->Stop();

  delete pBuffer;

  return 0;
}
 
