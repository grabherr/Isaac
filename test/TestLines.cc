
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/Plant.h"


void Draw(const StreamCoordinates & a1, 
	  const StreamCoordinates & b1, 
	  double width,
	  GameEngine & eng)
{
  StreamCoordinates a = a1*3;
  StreamCoordinates b = b1*3;

  StreamCoordinates offset = StreamCoordinates(4500, 100., 4500);
  a += offset;
  b += offset;

  MeshModel m;
  // m.SetTexture("data/Textures/black.jpg");
  m.SetTexture("data/Textures/grass1-small.jpg");
  MLine line;
  line.SetCoords(a,
		 b, 
		 width*7);
  line.GetMesh(m);
  //m.SetName("one");
  m.SetPhysMode(2);

  eng.AddMeshModelSilent(m);
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

  int i;
  RandomFloat(1000);
  RandomFloat(1000);
  RandomFloat(1000);
  RandomFloat(1000);
  RandomFloat(1000);
 
  for (int j=0; j<30; j++) {

    SimpleTree tree;
    double x = RandomFloat(1000);
    double y = RandomFloat(1000);
    tree.AddTrunk(Coordinates(x,0,y), Coordinates(x,20,y));
    bool bTips = false;
    char name[256];
    //int n = 70;
    int n = 25;
    for (i=0; i<n; i++) {
      cout << "ROUND " << i << endl;
      //if (i == n - 3)
      //bTips = true;
      if (i < 12)
	tree.AddBranches(0.0);
      else
	tree.AddBranches(0.6, bTips);
      tree.Grow();
      //tree.Print();
    }
    
    for (i=0; i<tree.isize(); i++) {
      const Branch & b = tree[i];
      Draw(b.Bottom(), 
	   b.Top(),
	   b.GetWidth(),
	   eng);
      
    }
  }



  eng.Run();


  return 0;
}
