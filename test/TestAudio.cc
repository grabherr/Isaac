
#include <string>

#include "base/CommandLineParser.h"
#include "audio/audioout.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input wav file");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the audio.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  IAudioOut * ao = CreateAudio();

  ao->SetSampleRate(16000);
  ao->SetChannels(1);
  ao->PlayWav(aString.c_str());

  return 0;
}
 
