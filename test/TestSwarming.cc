
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "base/RandomStuff.h"

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {}
  virtual ~MyManipulator() {
    m_pMe = NULL;
    m_inter = true;
    m_setFlyUp = false;
    m_setNormal = true;
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {

    PhysObject & p = self.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();


    m_pMe = &self;
    cout << "+++++++++++++++++++++" << endl;
    cout << "Got camera position";
    c.Print();
    m_lastPos.Print();
    cout << "+++++++++++++++++++++" << endl;
    if (m_pMe == NULL)
      return;
    double mass = m_pMe->GetPhysObject().GetTotalMass();
    Coordinates dir = m_lastPos - c;
    double l = dir.Length();
 

 


    if (l < 75.) {
      
      //if (RandomFloat(1.) < 0.01) {
      const string & name = self.GetName();
      Sound & sound = p.GetSound();
      cout << "NAME: " << name << endl;
      sound.UpdateAdd(name, 
		      "data/Sounds/crow2.wav",
		      m.GetPosition());




      m_inter = false;
      cout << "Help, fleeing!!!" << endl;
      Coordinates flee = m_pMe->GetPhysObject().GetLatImpulse() + dir.Einheitsvector() * 2500000;
      double cap = 45000.;
      //if (flee.Length() > cap) {
      //flee /= flee.Length();
      //flee *= cap;
      //}
      cout << "Flee " << flee.Length() << endl;
      flee.Print();
      m_pMe->GetPhysObject().SetLatImpulse(flee);

      
      if (!m_setFlyUp)
	m_pMe->GetPhysObject().SetAnimation("pain");
      else 
	m_pMe->GetPhysObject().SetAnimation("");
      m_setFlyUp = true;
      m_setNormal = false;
    } else {
      m_inter = true;
      cout << "Before, ptr=" << m_pMe << endl;
      cout << " mass " << mass << endl;
      cout << m_pMe->GetPhysObject().GetAnimation() << endl;
      if (!m_setNormal)
	m_pMe->GetPhysObject().SetAnimation("fly");
      else 
	m_pMe->GetPhysObject().SetAnimation("");
      cout << "After." << endl;
      m_setFlyUp = false;
      m_setNormal = true;
    }

  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    m_pMe = &o;

    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    if (m_center[0] == 0.) {
      m_center = m.GetPosition();
      double max = 5. * mass;
      Coordinates l(RandomFloat(max), RandomFloat(max), RandomFloat(max));     
      p.SetLatImpulse(l);
    }
    m_lastPos = m.GetPosition();
   
    double change = 25*mass;
    Coordinates lat = p.GetLatImpulse();
    lat[0] += RandomFloat(change) - change/2;
    lat[1] += RandomFloat(change/2) - change/4;
    lat[2] += RandomFloat(change) - change/2;
    p.SetLatImpulse(lat);




    /*
    cout << "Manipulator y=" << m.GetPosition()[1] << endl;
    double up = 1.;
    if (m.GetPosition()[1] > 100.) {
      up = -1.;
    }
    double v = 5. * mass * deltatime;
    Coordinates l = p.GetLatImpulse();
    l[1] += v * up;
    p.SetLatImpulse(l);
    cout << "Manipulator imp " << mass << " " << deltatime << " " << v << ": ";
    p.GetLatImpulse().Print();*/

    
    /*
    double phi = m.GetPosition()[1] / 20.;
    double x = 10. * cos(phi);
    double z = 13. * sin(phi);
    Coordinates update = m_center + Coordinates(x, m.GetPosition()[1], z);
    update[1] = m.GetPosition()[1];
    m.SetPosition(update);
    cout << "Manipulator pos ";
    update.Print();
    */

    /*
    if (m.GetPosition()[1] < 21.) {
      double v = 35. * mass;
      p.SetLatImpulse(Coordinates(0, v, 0));
      //void SetRotImpulse(const Coordinates &c) {m_rotImp = c;}
    }
    
    if (m.GetPosition()[1] > 180.) {
      double v = -35. * mass;
      p.SetLatImpulse(Coordinates(0, v, 0));
      //void SetRotImpulse(const Coordinates &c) {m_rotImp = c;}
      }*/
 
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
     m_pMe = &self;
   //return;
    if (!m_inter) {
      cout << "Not interacting, fleeing." << endl;
      return;
    }

    PhysObject & p = other.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();
    Coordinates dir = m_lastPos - m.GetPosition();
    double l = dir.Length();
    double fac = l;
    l -= 15.;
    if (l<0.)
      fac = -fac/10.;

    double acc = p.GetLatImpulse().Einheitsvector().Scalar(dir.Einheitsvector());
    
   //if (acc > 0.)
    //  fac *= (1.-acc);
    //else
    //fac *= (acc - 1.);
    Coordinates pull = p.GetLatImpulse() + dir.Einheitsvector() * fac * 5;
    double cap = 45000.;
    cout << "Pull " << pull.Length() << endl;
    if (pull.Length() > cap) {
      pull /= pull.Length();
      pull *= cap;
    }
    
    //if (l > 25 || l < -25)
    p.SetLatImpulse(pull);

  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  GamePhysObject * m_pMe;
  bool m_inter;
  bool m_setFlyUp;
  bool m_setNormal;
  
};


void AddModel(GameEngine & eng)
{
  MyManipulator * manip = new MyManipulator;

  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(RandomFloat(8000), 
					RandomFloat(8000), 
					RandomFloat(8000)));
  anim.SetLighting(true);
  anim.SetTexture("data/Models/raven.jpg");
  anim.SetModel("data/Models/raven.md3");
  anim.SetAnimation("fly");
  anim.SetAnimationSpeed(30.);
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
 
  MyManipulator manip;

  for (int k=0; k<7; k++)
  // for (int k=0; k<2; k++)
    AddModel(eng);
  //AddModel(eng);
  //AddModel(eng);
  //AddModel(eng);
  //AddModel(eng);

  /*
  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(4400, 300, 4400));
  //anim.SetRotImp(StreamCoordinates(0, 5000, 3000));
  anim.SetLighting(true);
   //anim.SetTexture("data/Models/black.jpg");
  anim.SetTexture("data/Models/raven.jpg");
  anim.SetModel("data/Models/raven.md3");
  //anim.SetModel("data/Models/snake.md2");
  anim.SetAnimation("fly");
  anim.SetAnimationSpeed(30.);
  anim.SetScale(5);
  //anim.SetModel("data/Models/ball.ms3d");
  anim.SetName("ball");
  anim.SetPhysMode(2);
  eng.AddAnimatedModel(anim, &manip);

  MyManipulator manip2;
  MyManipulator manip3;
  MyManipulator manip4;

  anim.SetName("ball2");
  //anim.SetRotImp(StreamCoordinates(0, 0, 0));
  //anim.SetRotImp(StreamCoordinates(2000, -6000, 0));
  //anim.SetTexture("data/Models/black.jpg");
  anim.SetCoordinates(StreamCoordinates(4900, 350, 4900));
  eng.AddAnimatedModel(anim, &manip2);

  anim.SetName("ball3");
  //anim.SetRotImp(StreamCoordinates(2000, -1000, -40000));
  // anim.SetTexture("data/Models/blue.jpg");
  anim.SetCoordinates(StreamCoordinates(4900, 400, 5300));
  eng.AddAnimatedModel(anim, &manip3);

  anim.SetName("ball4");
  //anim.SetRotImp(StreamCoordinates(2000, 00, 30000));
  //anim.SetTexture("data/Models/green.jpg");
  anim.SetCoordinates(StreamCoordinates(5300, 450, 4900));
  eng.AddAnimatedModel(anim, &manip4);
  */

  eng.Run();


  return 0;
}
