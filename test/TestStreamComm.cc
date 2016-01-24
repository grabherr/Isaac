
#include <string>
#include <stdio.h>
#include <unistd.h>

#include "base/CommandLineParser.h"
#include "util/StreamComm.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  commandArg<string> aStringCmmd("-hostname","name of the server running the host");
  commandArg<bool>  aBoolCmmd("-s","be the server",false);
  commandArg<string>  fileCmmd("-f","file name","");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the socket communication between computers.");
  P.registerArg(aStringCmmd);
  P.registerArg(aBoolCmmd);
  P.registerArg(fileCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  string fileName = P.GetStringValueFor(fileCmmd);
  bool aBool = P.GetBoolValueFor(aBoolCmmd);

  int i;
  int max = 10000000;

 
  if (aBool) {
    IPCache cache;
    string ip = cache.GetIP(aString);
    cout << "Will send data to " << aString << " at IP " << ip << endl;
    int n = 0;
    cout << "Will be the server (ctrl+c to exit, or will exit after 10 sends)!" << endl;
    StreamCommTransmitter * pTrans = NULL;
 
    pTrans = GetTransmitter(aString.c_str());
  
    for (i=0; i<100; i++) {
      DataPacket d;
      d.Write("Testing... ");
      d.Write(i);
      pTrans->Send(d);
    }
    cout << "Done sending, sleeping." << endl;
    while(true) {
      usleep(1000);
    }
    cout << "Haaa???" << endl;

    delete pTrans;
    return 0;
  }

  if (aString == "") {
    cout << "You must specify the host name!" << endl;
    return -1;
  }
  StreamCommReceiver * pRec = GetReceiver();

 
  while (true) {
    DataPacket d;
    if (pRec->Get(d)) {
      cout << "Got it!!" << endl;
      string s;
      int n;
      d.Read(s);
      d.Read(n);
      cout << s << " " << n << " from " << d.GetIPAddress() << endl;
    } else {
      usleep(100);
    }
  }
  
  delete pRec;
}
