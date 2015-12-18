
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
    m_count = 1200;
  }
  virtual ~MyManipulator() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  void SetBase(const string & b) {
    m_base = b;
  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {

    if (m_count >=1500)
      m_count = 1200;
  
    MsgSceneNode & msn = o.MessageSceneNode();
    char name[256];
    //sprintf(name, "data/wind%d.bmp", m_count);
    sprintf(name, "data/%s%d.bmp", m_base.c_str(), m_count);
    msn.Material(0).SetTexture(name);
    msn.SetRotation(StreamCoordinates(-3.14/2., 0, 0.));

   

    m_count++;

    /*  PhysObject & p = o.GetPhysObject();
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
    */
  
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return;

    
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  int m_count;
  string m_base;
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
  
  FILE * pTest = fopen("data/both1200.bmp", "r");
  if (pTest == NULL) {
    cout << "ERROR: Data not found. Please run ./TestCalmWater to generate the images!" << endl;
    return -1;
  }
  fclose(pTest);

  // Let's add some more realistic lighting here
  MsgLightNode light;
  light.SetPosition(StreamCoordinates(5300, 1200, 4900));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5400, 1800, 5400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 1800, 3400));
  eng.AddLight(light);
 

 
  MyManipulator manip2;
  manip2.SetBase("calm_caustic");
  MsgSceneNode node;
  node.SetName("block1");
 
  node.SetModel("data/Models/block6.ms3d");
  node.SetPosition(StreamCoordinates(5300, 340, 4900));
  node.SetRotation(StreamCoordinates(3.14/2., 0, 0.));
  node.SetPhysMode(2);

  node.SetScale(15.);
  node.SetRequestLoopBack(true);
 
  node.Material(0).SetLighting(true);

  eng.AddSceneNode(node, &manip2);

  //=======================================

  MyManipulator manip3;
  manip3.SetBase("calm");
  node.SetName("block2");
 
  node.SetModel("data/Models/block6.ms3d");
  node.SetPosition(StreamCoordinates(5300, 1550, 4900));
  node.SetRotation(StreamCoordinates(3.14/2., 0, 0.));
  node.SetPhysMode(2);

  node.SetScale(15.);
  node.SetRequestLoopBack(true);
 
  node.Material(0).SetLighting(true);

  eng.AddSceneNode(node, &manip3);


  eng.Run();


  return 0;
}
