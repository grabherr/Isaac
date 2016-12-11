#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "game/SchoolLogic.h"



int main( int argc, char** argv )
{

  int i, j;

  SchoolCharacter s;

  SchoolLogic logic;

  s.SetName("Andi");
  s.SetStrength(0.7);
  s.SetAttract(0.5);
  s.SetGender(0.8);
  logic.push_back(s);

  s.SetName("Klaus");
  s.SetStrength(0.6);
  s.SetAttract(0.5);
  s.SetGender(0.6);
  logic.push_back(s);

  s.SetName("Eva");
  s.SetStrength(0.8);
  s.SetAttract(0.7);
  s.SetGender(-0.8);
  logic.push_back(s);

  s.SetName("Birgit");
  s.SetStrength(0.3);
  s.SetAttract(0.4);
  s.SetGender(-0.7);
  logic.push_back(s);

  cout << "Characters:" << endl;
  for (i=0; i<logic.isize(); i++) {
    cout << "#" << i << endl;
    logic[i].Print();
  }
  
  
  bool b = true;
  while (b) {
    logic.BeginRound();
    cout << endl;
  
    cout << "-------------- Begin new round --------------" << endl;
    //cout << "Begin new round..." << endl;
    for (i=0; i<logic.isize(); i++) {
      logic[i].Print();
      cout << "Enter target index ('n' if none): ";
      string in;
      cin >> in;
      int n = -1;
      if (in != "n") {
	n = atol(in.c_str());
	if (n < 0 || n >= logic.isize()) {
	  n = -1;
	  cout << endl << "ERROR: Enter a number from 0 to " << logic.isize() << ", you moron!!" << endl;
	  cout << "ERROR: Skipping you this round..." << endl << endl;
	}
      }
      if (n == i) {
	n = -1;
	cout << endl << "ERROR: You cannot interact with yourself, you cheater!!" << endl;
	cout << "ERROR: Skipping you this round..." << endl << endl;
      }
      if (n >= 0) {
	cout << "Enter interaction (between -1 and +1): ";
	double aa;
	cin >> aa;

	if (aa >= -1 && aa <= 1) {
	  logic[i].SetTarget(n);
	  logic[i].SetInteract(aa);
	  cout << "Set interaction " << aa << " for target " << logic[n].GetName()  << endl;
	} else {
	  cout << endl << "ERROR: Enter a floating point number between -1 and +1, for example -0.5, you imbecille!!" << endl;
	  cout << "ERROR: Skipping you this round..." << endl << endl;
	}

      } else {
	cout << "Skipping this round." << endl;
      }
      
    }
        
    logic.EndRound();
    cout << "-------------- End round --------------------" << endl;
    logic.Print();
  }
  
  return 0;
}
