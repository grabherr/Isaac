
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "base/FileParser.h"
#include <math.h>
#include "base/RandomStuff.h"
#include <unistd.h>

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {}
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    return;
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    double range = 5500;

    double x = RandomFloat(range) - range/2;
    double y = RandomFloat(range) - range/2;
    double z = RandomFloat(range) - range/2;

    Coordinates l = p.GetLatImpulse();
    l[0] += x;
    l[1] += y;
    l[2] += z;

    p.SetLatImpulse(l);

 
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return;
    PhysObject & p = other.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();
    Coordinates dir = m_lastPos - m.GetPosition();
    double l = dir.Length();
    
    l -= 25.;
    Coordinates pull = p.GetLatImpulse() + dir.Einheitsvector() * l * 50;
    
    p.SetLatImpulse(pull);

  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
};


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input config file");
  commandArg<string> mCmmd("-m","molecule input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(mCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  string molecule = P.GetStringValueFor(mCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);
  MsgLightNode light;
  light.SetPosition(StreamCoordinates(2000, 1600, 1400));
  eng.AddLight(light);

  MyManipulator manip;

  FlatFileParser parser;
  
  parser.Open(molecule);
  double off = 4500.;
  double mscale = 60.;
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;

    //usleep(1000);
    //sleep(1);

    if (parser.AsString(0) != "ATOM")
      continue;
    if (parser.AsString(0) == "ENDMDL")
      break;

    if (parser.GetItemCount() < 11)
      continue;

    double x = parser.AsFloat(6);
    double y = parser.AsFloat(7);
    double z = parser.AsFloat(8);

    x = x * mscale + off;
    y = y * mscale + off;
    z = z * mscale + off;

    AnimatedSceneNode anim;

    anim.SetCoordinates(StreamCoordinates(x, y, z));


    anim.SetTexture("data/Models/red.jpg");
    if (parser.AsString(2) =="H")
      anim.SetTexture("data/Models/black.jpg");
    if (parser.AsString(2) =="C")
      anim.SetTexture("data/Models/blue.jpg");
    
    if (parser.AsString(3) =="GLU")
      anim.SetTexture("data/Textures/GLU.jpg");
    

    anim.SetModel("data/Models/ball.ms3d");
    //anim.SetModel("data/Models/cube.ms3d");
    //anim.SetName("ball");
    anim.SetPhysMode(0);
    anim.SetLighting(1);

    eng.AddAnimatedModel(anim, &manip);

  }
  light.SetPosition(StreamCoordinates(2200, 1700, 1400));
  eng.AddLight(light);


  eng.Run();


  return 0;
}
