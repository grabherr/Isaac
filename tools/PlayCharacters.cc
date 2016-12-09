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
  s.SetAttract(0.6);
  s.SetGender(0.6);
  logic.push_back(s);

  s.SetName("Eva");
  s.SetStrength(0.8);
  s.SetAttract(0.8);
  s.SetGender(-0.8);
  logic.push_back(s);

  s.SetName("Birgit");
  s.SetStrength(0.3);
  s.SetAttract(0.3);
  s.SetGender(-0.7);
  logic.push_back(s);


  logic.BeginRound();
 
  logic.Get("Andi").SetTarget(1);
  logic.Get("Andi").SetInteract(-0.5);

  logic.Get("Eva").SetTarget(2);
  logic.Get("Eva").SetInteract(0.5);
  
  logic.EndRound();

  logic.Print();
  
  return 0;
}
