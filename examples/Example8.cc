
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

class SpeedManipulator : public IManipulator
{
public:
  SpeedManipulator() {
    m_time = 0.;
  }
  virtual ~SpeedManipulator() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
 
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();

    m_time += deltatime/2;
    Coordinates cc;
    cc[1] = 5;
    cc[0] = 200 + 180*sin(m_time);
    cc[2] = 200 + 180*cos(m_time);
    p.MoveTo(cc);

    const string & name = o.GetName();
    Sound & sound = p.GetSound();
    cout << "NAME: " << name << endl;
    sound.UpdateAdd(name, 
		    /*"data/Sounds/engine_rev.wav",*/
		    /*"data/Sounds/car3.wav",*/
		    "data/Sounds/Ambulance-sound.wav",
		    cc);


  
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
  }
  
  private: 
  double m_time;
};


class SpeedManipulator2 : public IManipulator
{
public:
  SpeedManipulator2() {
    m_time = 0.;
  }
  virtual ~SpeedManipulator2() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
 
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();

    m_time -= deltatime/3.;
    Coordinates cc;
    cc[1] = 5;
    cc[0] = 200 + 180*sin(m_time);
    cc[2] = 200 + 180*cos(m_time);
    p.MoveTo(cc);

    const string & name = o.GetName();
    Sound & sound = p.GetSound();
    cout << "NAME: " << name << endl;
    sound.UpdateAdd(name, 
		    "data/Sounds/engine_rev.wav",
		    /*"data/Sounds/car3.wav",*/
		    /*"data/Sounds/Ambulance-sound.wav",*/
		    cc);


  
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
  }
  
  private:
  double m_time;
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
  
  MsgLightNode light;
  light.SetPosition(StreamCoordinates(2000, 1600, 1400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2400, 800, 2400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 800, 3400));
  eng.AddLight(light);
 
  //MusicManipulator manip;

  //AddModel(eng);

  SpeedManipulator manip;
  SpeedManipulator2 manip2;

  MsgSceneNode anim;

  anim.Material(0).SetLighting(true);
  anim.Material(0).SetTexture("data/Models/red.jpg");
  anim.SetModel("data/Models/ball.ms3d");
  anim.SetScale(3);
  anim.SetPhysMode(2);
  //anim.SetName("car1");
  eng.AddSceneNode(anim, &manip);

  anim.Material(0).SetTexture("data/Models/blue.jpg");
  anim.SetName("car2");
  eng.AddSceneNode(anim, &manip2);



  eng.Run();


  return 0;
}
