
#include <string>

#include "base/CommandLineParser.h"
#include "audio/audioout.h"
#include "audio/Wavefile.h"
#include "audio/SpatialAudio.h"


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input wav file");
  commandArg<string> bStringCmmd("-i2","input wav file 2");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the audio.");
  P.registerArg(aStringCmmd);
  P.registerArg(bStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  string bString = P.GetStringValueFor(bStringCmmd);
  IAudioOut * ao = CreateAudio();

  MultiSourceAudio mult;
  mult.SetSampleRate(44100);
 
  ao->SetSampleRate(44100);
  ao->SetChannels(2);

  ao->Start();

  int bufSize = ao->GetBufferSize();
  mult.SetBufferSize(bufSize, 2);
 
  char * pBuffer = new char[bufSize];
  
  double y = -10.;
  double z = -12.;
  double d1 = 0.1;
  double d2 = 0.05;

  mult.AddAudioSource(Coordinates(3, 1, 0), aString);
  mult.AddAudioSource(Coordinates(0, 1, 2), bString);

  //FILE * pOut = fopen("out.pcm", "wb");
  cout << "Start loop." << endl;
   while(mult.GetSound(pBuffer)) {
    cout << "Add buffer" << endl;
    //Coordinates pos = Coordinates(y, 2, 0);
    //spat.AddSound(pBuffer, pos, 0, 2);
    y += d1;
    z += d2;
    if (y > 10.)
      d1 = -0.3;
    if (y < -10.)
      d1 = 0.3;
    if (z > 12.)
      d2 = -0.1;
    if (z < -9.)
      d2 = 0.1;

    cout << "POS: " << y << endl;
    mult.SetPosition(0, Coordinates(y, 0, 0));
    mult.SetPosition(1, Coordinates(0, 0, z));
    //spat.GetSound(pBuffer);

    ao->AddBuffer(pBuffer);
    //fwrite(pBuffer, bufSize, 1, pOut);
  }
  ao->Stop();

  //fclose(pOut);
  delete pBuffer;

  return 0;
}
 
