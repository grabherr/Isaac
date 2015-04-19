
#include <string>
#include <stdio.h>
#include <unistd.h>

#include "base/CommandLineParser.h"
#include "util/SyncConn.h"
#include "base/FileParser.h"

int main(int argc,char** argv)
{

  commandArg<string> aStringCmmd("-hostname","name of the server running the host");

  commandLineParser P(argc,argv);
  P.SetDescription("Testing the socket communication between computers.");
  P.registerArg(aStringCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);

  int i;
  int max = 10000000;
 
  char tmp[2048];

  for (int i=0; i<20; i++) {
    SyncConnServer server(aString);
    string request;
    server.WaitForRequest(request);
    cout << "GOT Request: " << request << endl;
    cout << "Type answer: ";
    cin >> tmp;
    server.SendResult(tmp);
  }

  return 0;
}
