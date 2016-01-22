
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
    m_first = true;
  }
  virtual ~MyManipulator() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}

  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    MsgSceneNode & msn = self.MessageSceneNode();
    PhysObject & p = self.GetPhysObject();
    Coordinates cp = p.GetPosition();   
    //cp[0] -= 10.;
    //cp[2] -= 10.;

    const StreamCoordinates & pos = msn.GetPosition();
    cout << "My position:  " << cp[0] <<  " " << cp[1] << " " << cp[2] << " from " << p.isize() << endl;
    cout << "Cam position: " << c[0] <<  " " << c[1] << " " << c[2] << endl;   
    m_campos = c - cp;
    m_campos.Flip();

  }

  void SetBase(const string & b) {
    m_base = b;
  }

  virtual void Update(GamePhysObject & o, double deltatime) {
    int i;

    MsgSceneNode & msn = o.MessageSceneNode();
    if (m_first) {
      m_orig = msn;
      m_first = false;
    }

    PhysObject & p = o.GetPhysObject();

    if (msn.MeshCount() > 0) {
      cout << "Have vertices: " << msn.Mesh(0).VertexCount() << endl;
      cout << "Have text coords: " << msn.Mesh(0).GetTexCoordCount() << endl;
    } else {
      cout << "No meshes." << endl;
    }

    SphereCoordinates s = m_campos.AsSphere();
    cout << "Sphere coords " << s.phi() << " " << s.theta() << " " << s.r() << endl;

    double ff = 1.-cos(s.theta());
    if (ff < 0.)
      ff = 0.;
    cout << "ff=" << ff << endl;
    
    for (i=0; i<m_orig.Mesh(0).GetTexCoordCount(); i++) {
      const StreamCoordinates & from = m_orig.Mesh(0).GetTextCoordConst(i);
      StreamCoordinates & to = msn.Mesh(0).GetTextCoord(i);
      to = from;
      double ffX = cos(s.phi())*ff;
      double ffY = sin(s.phi())*ff;
      to[0] = 0.5+(to[0]-0.5)/2.;
      to[1] = 0.5+(to[1]-0.5)/2.;   
      to[0] -= 0.25*ffX;
      to[1] += 0.25*ffY;
      cout << "Update tex coords from " << from[0] << " " << from[1] << " to " << to[0] << " " << to[1] << endl;
    } 

    
    

    return;

    if (m_count >=1500)
      m_count = 1200;
  
    char name[256];
    sprintf(name, "data/%s%d.bmp", m_base.c_str(), m_count);
    msn.Material(0).SetTexture(name);
    //msn.SetRotation(StreamCoordinates(-3.14/2., 0, 0.));

    m_count++;
    // NOTE: we should keep track of the fframe rate/timing here,
    // which we don't, for simplicity!
   
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return;

    
  }

private:
  Coordinates m_campos;
  Coordinates m_lastPos;
  int m_count;
  string m_base;
  MsgSceneNode m_orig;
  bool m_first;

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
  

  // Let's add some more realistic lighting here
  MsgLightNode light;
  light.SetPosition(StreamCoordinates(4000, 1200, 4900));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5000, 1800, 4400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 1800, 3400));
  eng.AddLight(light);
 
  


  // One for the bottom
  MyManipulator manip2;
  manip2.SetBase("calm_caustic");
  MsgSceneNode node;
  node.Material(0).SetTexture("data/Textures/grass1.jpg");
  node.SetModel("data/Models/block7.ms3d");
  node.SetPosition(StreamCoordinates(5300, 340, 4900));
  //node.SetRotation(StreamCoordinates(3.14/2., 0, 0.));
  node.SetPhysMode(1);

  node.SetScale(15.);
  node.SetRequestLoopBack(true);
  node.SetRequestMesh(true);
   
  node.Material(0).SetLighting(true);

  eng.AddSceneNode(node, &manip2);

 
  eng.Run();


  return 0;
}
