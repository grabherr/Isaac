
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "engine/Manipulator.h"
#include <math.h>

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
    m_phi = 0.;
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

    Coordinates dir = p.GetDirection();
    m_phi += deltatime * 1.;
    if (m_phi > 2*3.1415926)
      m_phi -= 2*3.1415926;
    dir[0] = cos(m_phi);
    dir[2] = sin(m_phi);
    cout << "PHI " << m_phi << " " << dir[0] << " " << dir[2] << endl;

    p.SetDirection(dir);
    
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    // return;
 
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  double m_phi;
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
  
  LightNode light;
  light.SetPosition(StreamCoordinates(8000, 1600, 8000));
  light.SetRadius(100000);
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2400, 800, 2400));
  //eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 800, 3400));
  //eng.AddLight(light);
 
  //MyManipulator manip;
  MoveManipulator manip;
  manip.SetRotation(Coordinates(0., 1., 0.));
  manip.SetRotationSpeed(Coordinates(0., 1., 0.5));

  AnimatedSceneNode earth;

 
 
  earth.SetCoordinates(StreamCoordinates(2200, 1900, 2200));
  earth.SetScale(1600);
  earth.SetPhysMode(2);
  earth.SetShinyness(200.);
  MLeaf ll;
  earth.SetName("earth");
  earth.SetDirection(Coordinates(1., 0., 0.));
  earth.SetLighting(true);
  earth.SetTexture("data/Textures/8081_earthmap4kb.jpg");
  earth.SetModel("data/Models/sphere.obj");
  eng.AddAnimatedModel(earth, &manip);




  eng.Run();


  return 0;
}
