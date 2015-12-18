
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
    m_rotY = 0;
  }
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
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

    m_rotY += deltatime;
    
    MsgSceneNode & msn = o.MessageSceneNode();
    //msn.Animation().SetAnimation("");
    if (m_rotY > 3.14) {
      m_rotY = 0.;      
      msn.Animation().SetAnimation("run");
      cout << "Set animation " << msn.Animation().GetAnimation () << endl;
   }

    //p.SetEngRotation(StreamCoordinates(0, m_rotY, 0));
    msn.SetRotation(StreamCoordinates(0, m_rotY, 0));
    cout << "Rot " << m_rotY << endl;


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
  double m_rotY;
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
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);
  


  MyManipulator manip2;

  MsgSceneNode node;
  node.SetName("ball2");
  node.Material(0).SetTexture("data/Models/brute.jpg");
  node.SetModel("data/Models/monkey.md2");
  node.SetPosition(StreamCoordinates(5300, 20, 4900));

  // No physics
  node.SetPhysMode(2);

  node.SetScale(5.);

  node.Animation().SetAnimation("attack");
  node.Animation().SetSpeed(30);

  // We want to take control of the object
  node.SetRequestLoopBack(true);
 
  // Enable this if yu want to control the mesh! 
  node.SetRequestMesh(false);
 
 
  // Add it!
  eng.AddSceneNode(node, &manip2);


  eng.Run();


  return 0;
}
