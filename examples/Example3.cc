
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {}
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    if (m_center[0] == 0.)
      m_center = m.GetPosition();
    m_lastPos = m.GetPosition();

    cout << "Manipulator y=" << m.GetPosition()[1] << endl;
    double up = 1.;
    if (m.GetPosition()[1] > 100.) {
      up = -1.;
    }
    double v = 5. * mass * deltatime;
    Coordinates l = p.GetLatImpulse();
    l[1] += v * up;
    p.SetLatImpulse(l);

    double phi = m.GetPosition()[1] / 20.;
    double x = 10. * cos(phi);
    double z = 13. * sin(phi);
    Coordinates update = m_center + Coordinates(x, m.GetPosition()[1], z);
    update[1] = m.GetPosition()[1];
    m.SetPosition(update);
    cout << "Manipulator " << p.GetPhysMode() << endl;
    update.Print();

  
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
};


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
  GameEngine eng("localhost", false);
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);
  


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
  MyManipulator manip1;
  eng.AddMeshModel(m, &manip1);

  MyManipulator manip2;

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
 
  // Enable this if yu want to control the mesh! 
  //node.SetRequestMesh(true);
 
 
  // Add it!
  eng.AddSceneNode(node, &manip2);


  eng.Run();


  return 0;
}
