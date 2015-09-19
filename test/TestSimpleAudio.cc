
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "base/RandomStuff.h"

class MusicManipulator : public IManipulator
{
public:
  MusicManipulator() {}
  virtual ~MusicManipulator() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
 
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();


    const string & name = o.GetName();
    Sound & sound = p.GetSound();
    cout << "NAME: " << name << endl;
    sound.UpdateAdd(name, 
		    "data/Sounds/happy.wav",
		    m.GetPosition());


  
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
  }
  
  private:
  
};


void AddModel(GameEngine & eng)
{
  MusicManipulator * manip = new MusicManipulator;

  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(RandomFloat(8000), 
					RandomFloat(8000), 
					RandomFloat(8000)));
  anim.SetLighting(true);
  anim.SetTexture("data/Models/blue.jpg");
  anim.SetModel("data/Models//ball.ms3d");
  //anim.SetAnimation("fly");
  //anim.SetAnimationSpeed(30.);
  anim.SetScale(3);
  anim.SetPhysMode(2);
  eng.AddAnimatedModel(anim, manip);

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
  eng.DoObjectCollision(false);
  
  LightNode light;
  light.SetPosition(StreamCoordinates(2000, 1600, 1400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2400, 800, 2400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 800, 3400));
  eng.AddLight(light);
 
  MusicManipulator manip;

  AddModel(eng);

  eng.Run();


  return 0;
}
