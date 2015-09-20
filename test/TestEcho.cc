
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

//=======================================================================

class EchoManipulator : public IManipulator
{
public:
  EchoManipulator() {
    m_time = 0.;
    m_speed = 0.5;
  }
  virtual ~EchoManipulator() {
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

    m_time += deltatime * m_speed;
    Coordinates cc;
    cc[1] = 5;
    cc[0] = m_center[0] + 18*sin(m_time);
    cc[2] = m_center[2] + 10*cos(m_time);
    p.MoveTo(cc);

    const string & name = o.GetName();
    Sound & sound = p.GetSound();
    cout << "NAME: " << name << endl;
    sound.UpdateAdd(name, 
		    "data/Sounds/happy.wav",
		    cc);


  
  }
  
  void SetCoordinates(const Coordinates & c, double speed) {
    m_center = c;
    m_speed = speed;
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
  }
  
  private:
  Coordinates m_center;
  double m_time;
  double m_speed;
};





void AddModel(GameEngine & eng)
{
  MusicManipulator * manip = new MusicManipulator;

  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(4000, 
					100, 
					4000));
  anim.SetLighting(true);
  anim.SetTexture("Temp/Gramophone/bd416241.png");
  //anim.SetTexture2("Temp/Gramophone/f6aab574.png");
  anim.SetModel("Temp/Gramophone/Gramophone.obj");
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
 
  //MusicManipulator manip;

  AddModel(eng);

  for (int i=0; i<5; i++) {
    EchoManipulator * manip = new EchoManipulator;
    AnimatedSceneNode anim;
    anim.SetLighting(true);
    anim.SetTexture("data/Models/red.jpg");
    anim.SetModel("data/Models//ball.ms3d");
    anim.SetScale(3);
    anim.SetPhysMode(2);
    manip->SetCoordinates(Coordinates(500 + 20*i + RandomFloat(30.), RandomFloat(40), 500 + 20*i + RandomFloat(40.)), 0.8*(double)i);
    eng.AddAnimatedModel(anim, manip);
  }

  for (int i=0; i<5; i++) {
    EchoManipulator * manip = new EchoManipulator;
    AnimatedSceneNode anim;
    anim.SetLighting(true);
    anim.SetTexture("data/Models/red.jpg");
    anim.SetModel("data/Models//ball.ms3d");
    anim.SetScale(3);
    anim.SetPhysMode(2);
    manip->SetCoordinates(Coordinates(0 + 20*i + RandomFloat(30.), RandomFloat(40), 0 + 20*i + RandomFloat(40.)), 0.8*(double)i);
    eng.AddAnimatedModel(anim, manip);
  }

  for (int i=0; i<5; i++) {
    EchoManipulator * manip = new EchoManipulator;
    AnimatedSceneNode anim;
    anim.SetLighting(true);
    anim.SetTexture("data/Models/red.jpg");
    anim.SetModel("data/Models//ball.ms3d");
    anim.SetScale(3);
    anim.SetPhysMode(2);
    manip->SetCoordinates(Coordinates(500 + 20*i + RandomFloat(30.), RandomFloat(40), 0 + 20*i + RandomFloat(40.)), 0.8*(double)i);
    eng.AddAnimatedModel(anim, manip);
  }
  for (int i=0; i<5; i++) {
    EchoManipulator * manip = new EchoManipulator;
    AnimatedSceneNode anim;
    anim.SetLighting(true);
    anim.SetTexture("data/Models/red.jpg");
    anim.SetModel("data/Models//ball.ms3d");
    anim.SetScale(3);
    anim.SetPhysMode(2);
    manip->SetCoordinates(Coordinates(0 + 20*i + RandomFloat(30.), RandomFloat(40), 500 + 20*i + RandomFloat(40.)), 0.8*(double)i);
    eng.AddAnimatedModel(anim, manip);
  }

  eng.Run();


  return 0;
}
