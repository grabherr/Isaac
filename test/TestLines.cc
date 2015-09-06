
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"

void Draw(const StreamCoordinates & a1, const StreamCoordinates & b1, GameEngine & eng)
{
  StreamCoordinates a = a1;
  StreamCoordinates b = b1;

  StreamCoordinates offset = StreamCoordinates(4500, 100., 4500);
  a += offset;
  b += offset;

  MeshModel m;
  m.SetTexture("data/Textures/black.jpg");
  MLine line;
  line.SetCoords(a,
		 b, 
		 30.);
  line.GetMesh(m);
  //m.SetName("one");
  m.SetPhysMode(2);

  eng.AddMeshModel(m);
}

int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);


  Draw(StreamCoordinates(0, 0, 0), 
       StreamCoordinates(0, 500, 0),
       eng);

  Draw(StreamCoordinates(-600, 500, 0), 
       StreamCoordinates(800, 500, 0),
       eng);

  Draw(StreamCoordinates(-600, 500, 0), 
       StreamCoordinates(-600, 800, 0),
       eng);

  Draw(StreamCoordinates(800, 500, 0), 
       StreamCoordinates(800, 900, 0),
       eng);
 
  Draw(StreamCoordinates(-600, 800, 200), 
       StreamCoordinates(-600, 800, -200),
       eng);

  Draw(StreamCoordinates(1000, 900, 300), 
       StreamCoordinates(600, 900, -300),
       eng);

  //===========================================
  Draw(StreamCoordinates(-600, 800, 200), 
       StreamCoordinates(-600, 1200, 200),
       eng);
  Draw(StreamCoordinates(-600, 800, -200), 
       StreamCoordinates(-600, 1300, -200),
       eng);

  Draw(StreamCoordinates(1000, 900, 300), 
       StreamCoordinates(1000, 1300, 300),
       eng);
  Draw(StreamCoordinates(600, 900, -300), 
       StreamCoordinates(600, 1100, -300),
       eng);

  eng.Run();


  return 0;
}
