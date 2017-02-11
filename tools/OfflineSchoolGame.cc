#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "game/SchoolLogic.h"

#include "game/SGPlay.h"
#include "game/Character.h"
#include "base/RandomStuff.h"


int main( int argc, char** argv )
{

  int i, j;
  AllCharacters all;
  
  SchoolCharacter s;

  SchoolLogic logic;
  svec<double> prop;
  
  s.SetName("Eva");
  s.SetStrength(0.7);
  s.SetAttract(0.5);
  s.SetGender(0.8);
  all.GetVector(prop, "Eva");
  s.Properties() = prop;
  logic.push_back(s);

  s.SetName("Franz");
  s.SetStrength(0.6);
  s.SetAttract(0.5);
  s.SetGender(0.6);
  all.GetVector(prop, "Franz");
  s.Properties() = prop;
  logic.push_back(s);

  s.SetName("Georg");
  s.SetStrength(0.8);
  s.SetAttract(0.7);
  s.SetGender(-0.8);
  all.GetVector(prop, "Georg");
  s.Properties() = prop;
  logic.push_back(s);

  s.SetName("Mona");
  s.SetStrength(0.3);
  s.SetAttract(0.4);
  s.SetGender(-0.7);
  all.GetVector(prop, "Mona");
  s.Properties() = prop;
  logic.push_back(s);


  svec<Character> characters;
  
  cout << "Characters:" << endl;


  svec<double> test;
  logic[0].AsVec(test);
 
  for (i=0; i<logic.isize(); i++) {
    Character cc;
    cc.SetupPeople(test.isize(), i);
    cc.SetName(logic[i].GetName());
    cc.SetCoords(Coordinates(RandomFloat(50), 0, 0));
    characters.push_back(cc);
    
    cout << "#" << i << endl;
    logic[i].Print();
  }
  
  
  bool b = true;
  while (b) {
    logic.BeginRound();
    cout << endl;
  
    cout << "-------------- Begin new round --------------" << endl;
    //cout << "Begin new round..." << endl;
    // Feed data
    for (i=0; i<logic.isize(); i++) {
      svec<double> input;
      logic[i].AsVec(input);
      for (j=0; j<logic.isize(); j++) {
	characters[j].FeedNeutral(input, characters[i].GetCoords(), i);
      }
     //logic[i].Print();
    }

    // Move them
    for (i=0; i<logic.isize(); i++) {
      int des = characters[i].GetDesire();
      int avoid = characters[i].GetAvoid();
      double act = characters[i].GetAct();
      svec<double> input;
      logic[i].AsVec(input);
      Coordinates self = characters[i].GetCoords();
 
      cout << "Process " << logic[i].GetName() << endl;
      cout << "Desire " << des << " Avoid " << avoid << " Act " << act << " Coords ";
      self.Print();
      
      if (des >= 0) {
	const Coordinates & other = characters[des].GetCoords();
	self += (other - self).Einheitsvector()*0.5;
	characters[i].SetCoords(self);

	// Interact here!!!!!
	if ((self - other).Length() < 10.) {
	  cout << "INTERACTION " << i << " <-> " << des << " act " << act << endl;
	  svec<double> input_other;
	  logic[des].AsVec(input_other);
	  
 	  logic[i].SetTarget(des);
	  logic[i].SetInteract(act);
	  characters[des].FeedAction(input, act);
	  characters[i].FeedDone(input, act);
	}
      }
      if (avoid >= 0) {
	const Coordinates & other = characters[avoid].GetCoords();
	self -= (other - self).Einheitsvector()*0.3;
	characters[i].SetCoords(self);	
      }      
    }
    
    logic.EndRound();

    for (i=0; i<logic.isize(); i++) {
      characters[i].SetScore(logic[i].GetStrength());
      characters[i].Print();
    }
        



    cout << "-------------- End round --------------------" << endl;
    logic.Print();
  }
  
  return 0;
}
