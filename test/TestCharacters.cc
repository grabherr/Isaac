
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "base/RandomStuff.h"




class MonkeyManipulator : public IManipulator
{
public:
  MonkeyManipulator() {
    m_mode = "run";
  }
  virtual ~MonkeyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();
    
    Coordinates pos = m.GetPosition();
    cout << "Monkey Pos " << pos[0] << endl;
    //pos[0] += deltatime * 20;
    //m.SetPosition(pos);
 

    //SphereCoordinates sp = dir.AsSphere();
    //double phi = sp.phi();
    //double theta = sp.theta();
    Coordinates dir = (m_snake - pos).Einheitsvector();
    //SphereCoordinates sp = dir.AsSphere();
    //double phi = sp.phi();
    //double theta = sp.theta();
    double theta = Angle(dir[2], dir[0]);

    double dist = (m_snake - pos).Length();

    if (m_mode == "run") {
      if (dist > 2.) {
	p.SetEngRotation(Coordinates(0, theta+3.1415/2., 0));
	m_last = dir;
	dir[0] = -deltatime * 15 * dir[0];
	dir[2] = -deltatime * 15 * dir[2];
	
      } else {
	dir[0] = -deltatime * 12 * m_last[0];
	dir[2] = -deltatime * 12 * m_last[2];
      }
      cout << "Monkey Running" << endl;
    }
    if (m_mode == "attack") {
      if (dist > 1.)
	p.SetEngRotation(Coordinates(0, theta-3.1415+3.1415/2., 0));
      dir[0] = deltatime * 8 * dir[0];
      dir[2] = deltatime * 8 * dir[2];
      cout << "Monkey Attacking" << endl;
   }
    m.SetPosition(pos+dir);


    p.SetAnimation("");

    if (RandomFloat(1.) < 0.01) {
      p.SetAnimation("attack");
      m_mode = "attack";
      //cout << "Changed anim!" << endl;
    }
    // if (RandomFloat(1.) < 0.01) {
    //  cout << "Changed anim!" << endl;
    //  p.SetAnimation("die");
    //}
    if (RandomFloat(1.) < 0.01) {
      cout << "Changed anim!" << endl;
      p.SetAnimation("run");
      m_mode = "run";
    }
      
   
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
 
    PhysObject & p = self.GetPhysObject();    
    PhysMinimal & m = p.GetCenterDirect();

    PhysObject & po = other.GetPhysObject();    
    PhysMinimal & mo = po.GetCenterDirect();
    double d = (m.GetPosition() - mo.GetPosition()).Length();
    m_snake = mo.GetPosition();
    

  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  Coordinates m_snake;
  Coordinates m_last;
  string m_mode;
};

class SnakeManipulator : public IManipulator
{
public:
  SnakeManipulator() {
    m_dist = 99999.;
    m_stay = false;
  }
  virtual ~SnakeManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();
    
    Coordinates pos = m.GetPosition();

    cout << "Snake Pos " << pos[0] << endl;

    Coordinates dir = (m_monkey - pos).Einheitsvector();
    
    cout << "Snake direction " << dir[0] << " " << dir[2] << endl;
    cout << "Deltatime: " << deltatime << endl;
    //SphereCoordinates sp = dir.AsSphere();
    //double phi = sp.phi();
    //double theta = sp.theta();

    double theta = Angle(dir[2], dir[0]);

     cout << "Snake theta " << theta << endl;
    double dist = (m_monkey - pos).Length();

    if (!m_stay) { 
      dir[0] = deltatime * 15 * dir[0];
      dir[2] = deltatime * 15 * dir[2];
    
      cout << "Snake direction (add) " << dir[0] << " " << dir[2] << endl;
      cout << "Snake  position " << pos[0]+dir[0] << " " << pos[2]+dir[2] << endl;
   
      m.SetPosition(pos+dir);
    }

    if (dist > 1.) {
      p.SetEngRotation(Coordinates(0, theta-3.14156/2, 0));
    } else {
      m_stay = true;
    }
    if (dist > 20.) {
      m_stay = false;
    }

    p.SetAnimation("");

    if (RandomFloat(1.) < 0.01) {
      p.SetAnimation("attack");
      cout << "Changed anim!" << endl;
    }
    if (RandomFloat(1.) < 0.01) {
      cout << "Changed anim!" << endl;
      p.SetAnimation("die");
    }
    if (RandomFloat(1.) < 0.01) {
      cout << "Changed anim!" << endl;
      p.SetAnimation("run");
    }
    m_dist = 99999.;
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    PhysObject & p = self.GetPhysObject();    
    PhysMinimal & m = p.GetCenterDirect();

    PhysObject & po = other.GetPhysObject();    
    PhysMinimal & mo = po.GetCenterDirect();
    double d = (m.GetPosition() - mo.GetPosition()).Length();
    if (d < m_dist) {
      m_dist = d;
      m_monkey = mo.GetPosition();
    }

  }

private:
  Coordinates m_monkey;
  double m_dist;
  Coordinates m_center;
  Coordinates m_lastPos;
  bool m_stay;
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

 
  MonkeyManipulator monkey;
  SnakeManipulator snake;

  MsgSceneNode anim;
  anim.SetPosition(StreamCoordinates(4400, 10, 4400));
  //anim.SetRotImp(StreamCoordinates(0, 5000, 0));
  //anim.SetTexture("data/Models/snake.jpg");
  //anim.SetModel("data/Models/snake.md2");
  anim.Material(0).SetTexture("data/Models/brute.jpg");
  anim.SetModel("data/Models/monkey.md2");
  anim.SetName("snake");
  anim.Animation().SetAnimation("run");
  anim.Animation().SetSpeed(50);
  anim.SetScale(3.2);
  anim.SetPhysMode(2);
  anim.SetRequestMesh(false);
  eng.AddSceneNode(anim, &snake);

    
  anim.SetPosition(StreamCoordinates(3400, 10, 3400));
  //anim.SetRotImp(StreamCoordinates(0, 5000, 0));
  anim.Material(0).SetTexture("data/Models/brute.jpg");
  anim.SetModel("data/Models/monkey.md2");
  anim.SetName("monkey5");
  anim.Animation().SetAnimation("run");
  anim.Animation().SetSpeed(50);
  anim.SetScale(3);
  anim.SetPhysMode(2);
  anim.SetRequestMesh(false);
  eng.AddSceneNode(anim, &monkey);
    

  eng.Run();


  return 0;
}
