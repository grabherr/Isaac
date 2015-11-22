
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"

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

  int i, j;

  /*  MeshModel triMesh;

  MTriangleMesh meshMaker;
  meshMaker.SetBoundaries(0, 0, 20, 20, 1);
  meshMaker.GetMesh(triMesh);

  triMesh.SetTexture("data/Textures/water1.jpg");
  triMesh.AbsCoords() = Coordinates(1400, 1400, 1400);
  triMesh.SetName("triangles");
  triMesh.SetScale(200);
  triMesh.SetPhysMode(0);
  eng.AddMeshModel(triMesh);
  
  cout << "Print triangle mesh" << endl;
  for (int i=0; i<triMesh.VertexCount(); i++) {
    triMesh.GetVertex(i).Print();
    triMesh.GetTextCoord(i).Print();
  }
  cout << "Indices:" << endl;
  for (int i=0; i<triMesh.IndexCountTotal(); i++) {
    if (i % 3 == 0)
      cout << endl;
    cout << triMesh.GetIndexTotal(i) << endl;
    }*/

  MeshModel m;
  m.SetTexture("data/Textures/rock1.jpg");

  m.AbsCoords() = Coordinates(400, 400, 400);
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

  m.SetPhysMode(0);

  //eng.AddMeshModel(m);

  MeshModel flat, block, leaf;

  flat.AbsCoords() = Coordinates(800, 300, 1800);
  flat.SetScale(20);
  MRectangle rr;
  MTriangle tt;
  //tt.GetMesh(flat, StreamCoordinates(10, 10, 20));
  rr.GetMesh(flat, StreamCoordinates(10, 10, 20));
  flat.SetTexture("data/Textures/water1.jpg");
  flat.SetName("flat");
  eng.AddMeshModel(flat);

  block.AbsCoords() = Coordinates(1500, 300, 1000);
  block.SetScale(20);
  MBlock bb;
  bb.GetMesh(block, StreamCoordinates(10, 10, 20));
  block.SetTexture("data/Textures/concrete1.jpg");
  block.SetRotImp(StreamCoordinates(0, 500, 0));
  block.SetName("block");
  eng.AddMeshModel(block);


  leaf.AbsCoords() = Coordinates(800, 400, 1800);
  leaf.SetScale(20);
  MLeaf ll;
  ll.GetMesh(leaf, StreamCoordinates(10, 10, 10));
  leaf.SetTexture("Temp/CurrantLeafB.jpg");
  //leaf.SetRotImp(StreamCoordinates(0, 500, 0));
  leaf.SetName("leaf");
  eng.AddMeshModel(leaf);

  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(4400, 300, 4400));
  anim.SetRotImp(StreamCoordinates(0, 5000, 0));
  anim.SetTexture("data/Models/black.jpg");
  anim.SetModel("data/Models/ball.ms3d");
  anim.SetName("ball");
  anim.SetTransparent(200);
  eng.AddAnimatedModel(anim);

  eng.Run();


  return 0;
}
