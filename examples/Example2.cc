
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>


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
  eng.DoObjectCollision(false);
  

  // Method 1: Explicitely define each vertex, texture coords etc.
  MeshModel m;
  m.SetTexture("data/Textures/rock1.jpg");

  m.AbsCoords() = Coordinates(5400, 400, 5400);
  m.SetName("explicit");
  m.SetScale(200);

  m.AddVertex(Coordinates(0,0,0)); 
  m.AddVertex(Coordinates(1,0,0)); 
  m.AddVertex(Coordinates(1,1,0)); 
  m.AddVertex(Coordinates(0,1,0)); 
  m.AddVertex(Coordinates(1,0,1)); 
  m.AddVertex(Coordinates(1,1,1)); 
  m.AddVertex(Coordinates(0,1,1)); 
  m.AddVertex(Coordinates(0,0,1)); 
  m.AddVertex(Coordinates(0,1,1)); 
  m.AddVertex(Coordinates(0,1,0)); 
  m.AddVertex(Coordinates(1,0,1)); 
  m.AddVertex(Coordinates(1,0,0)); 

  m.AddNormal(Coordinates(-1,-1,-1));
  m.AddNormal(Coordinates( 1,-1,-1));
  m.AddNormal(Coordinates( 1, 1,-1));
  m.AddNormal(Coordinates(-1, 1,-1));
  m.AddNormal(Coordinates( 1,-1, 1));
  m.AddNormal(Coordinates( 1, 1, 1));
  m.AddNormal(Coordinates(-1, 1, 1));
  m.AddNormal(Coordinates(-1,-1, 1));
  m.AddNormal(Coordinates(-1, 1, 1));
  m.AddNormal(Coordinates(-1, 1,-1));
  m.AddNormal(Coordinates( 1,-1, 1));
  m.AddNormal(Coordinates( 1,-1,-1));
 
  m.AddTexCoord(Coordinates(0,1,0));
  m.AddTexCoord(Coordinates(1,1,0));
  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(0,0,0));
  m.AddTexCoord(Coordinates(0,1,0));
  m.AddTexCoord(Coordinates(0,0,0));
  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(1,1,0));
  m.AddTexCoord(Coordinates(0,1,0));
  m.AddTexCoord(Coordinates(1,1,0));
  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(0,0,0));

  m.AddIndex(0,2,1);
  m.AddIndex(0,3,2);
  m.AddIndex(1,5,4);
  m.AddIndex(1,2,5);
  m.AddIndex(4,6,7);
  m.AddIndex(4,5,6);
  m.AddIndex(7,3,0);
  m.AddIndex(7,6,3);
  m.AddIndex(9,5,2);
  m.AddIndex(9,8,5);
  m.AddIndex(0,11,10);
  m.AddIndex(0,10,7);

  m.SetPhysMode(1);
  eng.AddMeshModel(m);

  // Method 2: have the client load the model and send back details
  MsgSceneNode node;
  node.SetName("ball2");
  node.Material(0).SetTexture("data/Models/green.jpg");
  node.SetModel("data/Models/ball.ms3d");
  node.SetPosition(StreamCoordinates(5300, 450, 4900));

  // No physics
  node.SetPhysMode(2);

  node.SetScale(15.);
  // We want to take control of the object
  node.SetRequestLoopBack(true);
  
  // Add it!
  eng.AddSceneNode(node);


  eng.Run();


  return 0;
}
