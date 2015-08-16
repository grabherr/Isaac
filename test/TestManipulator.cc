
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
    double v = 555. * mass * deltatime;
    Coordinates l = p.GetLatImpulse();
    l[1] += v * up;
    p.SetLatImpulse(l);

    double phi = m.GetPosition()[1] / 20.;
    double x = 10. * cos(phi);
    double z = 13. * sin(phi);
    Coordinates update = m_center + Coordinates(x, m.GetPosition()[1], z);
    update[1] = m.GetPosition()[1];
    m.SetPosition(update);
    cout << "Manipulator ";
    update.Print();

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
    // return;
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
 
  MyManipulator manip;

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
  m.SetLighting(true);
  MyManipulator manip5;
  eng.AddMeshModel(m, &manip5);

  MeshModel leaf;

 

  leaf.AbsCoords() = Coordinates(800, 400, 1800);
  leaf.SetScale(20);
  MLeaf ll;
  ll.GetMesh(leaf, StreamCoordinates(10, 10, 10));
  leaf.SetTexture("Temp/CurrantLeafB.jpg");
  //leaf.SetRotImp(StreamCoordinates(0, 500, 0));
  leaf.SetName("leaf");
  //eng.AddMeshModel(leaf, &manip);

  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(4400, 300, 4400));
  anim.SetRotImp(StreamCoordinates(0, 5000, 3000));
  anim.SetLighting(true);
   //anim.SetTexture("data/Models/black.jpg");
  anim.SetTexture("data/Models/red.jpg");
  anim.SetModel("data/Models/ball.ms3d");
  //anim.SetModel("data/Models/cube.ms3d");
  anim.SetName("ball");
  anim.SetPhysMode(0);
  eng.AddAnimatedModel(anim, &manip);

  MyManipulator manip2;
  MyManipulator manip3;
  MyManipulator manip4;

  anim.SetName("ball2");
  //anim.SetRotImp(StreamCoordinates(0, 0, 0));
  anim.SetRotImp(StreamCoordinates(2000, -6000, 0));
  anim.SetTexture("data/Models/black.jpg");
  anim.SetCoordinates(StreamCoordinates(4900, 350, 4900));
  eng.AddAnimatedModel(anim, &manip2);

  anim.SetName("ball3");
  anim.SetRotImp(StreamCoordinates(2000, -1000, -40000));
  anim.SetTexture("data/Models/blue.jpg");
  anim.SetCoordinates(StreamCoordinates(4900, 400, 5300));
  eng.AddAnimatedModel(anim, &manip3);

  anim.SetName("ball4");
  anim.SetRotImp(StreamCoordinates(2000, 00, 30000));
  anim.SetTexture("data/Models/green.jpg");
  anim.SetCoordinates(StreamCoordinates(5300, 450, 4900));
  eng.AddAnimatedModel(anim, &manip4);


  eng.Run();


  return 0;
}
