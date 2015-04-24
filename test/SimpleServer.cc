
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "util/Keyboard.h"

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
 

  char c = (char)get_kbhit();

  SyncConnServer server(aString);

  while (true) {
    string request;
    server.WaitForRequest(request);
    cout << "GOT Request: " << request << endl;
    cout << "Collect answer... ";
    string keyhit;
    /*while (!get_kbhit()) {
      usleep(100);
    }
    keyhit = getchar();*/
    cout << "Sending." << endl;
    server.SendResult("d");
    cout << "done." << endl;
  }

  return 0;
}
