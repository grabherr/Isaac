
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>



class ScentMap : public IGlobal
{
public:
  ScentMap(int size, int actual) {
    int i;
    m_data.resize(actual);
    for (i=0; i<m_data.isize(); i++)
      m_data[i].resize(actual, 0.);
    m_scale = (double)size/(double)actual;
    m_decay = 0.1;
  }

  virtual void StartFrame(double deltatime) {
  }

  virtual void EndFrame(double deltatime) {
    cout << "DECAY" << endl;
    int i, j;
    for (i=0; i<m_data.isize(); i++) {
      svec<double> & d = m_data[i];
      for (j=0; j<d.isize(); j++)
	d[j] *= 1. - m_decay*deltatime;
    }
  }

  void Add(double x, double z, double v) {
    int i = x/m_scale;
    int j = z/m_scale;
    
    if (i < 0 || j < 0 || i >= m_data.isize() || j <= m_data[0].isize())
      return;

    (m_data[i])[j] += v;
  }

  double Get(double x, double z) const {
    int i = x/m_scale;
    int j = z/m_scale;
    if (i < 0 || j < 0 || i >= m_data.isize() || j <= m_data[0].isize())
      return 0;

    return (m_data[i])[j];
 }


private:
  svec < svec < double > > m_data;
  double m_scale;
  double m_decay;
};



class AntManipulator : public IManipulator
{
public:
  AntManipulator() {
    m_rotY = 0;
    m_pMap = NULL;
    m_totaltime = 0.;
    m_wait = 0.2;
    m_counter = 0;
  }

  void SetWaitTime(double d) {
    m_wait = d;
  }

  void SetMap(ScentMap * p) {
    m_pMap = p;
  }

  virtual ~AntManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {   
    
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    m_totaltime += deltatime;
    if (m_totaltime < m_wait) {
      m.SetPosition(Coordinates(90, 15, 90));
      return;
    }

    if (RandomFloat(1) > 0.1)
      m_counter++;

    if (m_counter == 2) {
      o.MessageSceneNode().SetModel("data/Models/ant_1.ms3d");
    }
    if (m_counter == 4) {
      o.MessageSceneNode().SetModel("data/Models/ant_2.ms3d");
    }
    if (m_counter == 6) {
      o.MessageSceneNode().SetModel("data/Models/ant_3.ms3d");
    }
    if (m_counter == 8) {
      o.MessageSceneNode().SetModel("data/Models/ant_0.ms3d");
      m_counter = 0;
    }

    //if (m_center[0] == 0.)
    //  m_center = m.GetPosition();
    //m_lastPos = m.GetPosition();

    double rr = 0.5;
    if (RandomFloat(1.) < 0.05)
      m_rotY += (rr - RandomFloat(2*rr));
    
    //m_rotY = 3.1415/2;

    MsgSceneNode & msn = o.MessageSceneNode();
    ///msn.Animation().SetAnimation("run");
 
    msn.SetRotation(StreamCoordinates(0, m_rotY, 0));
    //cout << "Rot " << m_rotY << endl;
    Coordinates update = m.GetPosition();
    double speed = 10.*deltatime;
    cout << "Ant position (before) " << update[0] <<  " " << update[2] << " " << m_rotY << endl;
    update[0] += speed*cos(m_rotY+3.1415/2);
    update[2] += -speed*sin(m_rotY+3.1415/2);

    cout << "Ant position (after)  " << update[0] <<  " " << update[2] << " " << m_rotY << endl;
    if (update[0] < 0.) {
      update[0] = 0.;
      m_rotY -= 3.1415; 
    }
    if (update[2] < 0.) {
      update[2] = 0.;
      m_rotY -= 3.1415; 
    }
    if (update[0] > 980) {
      update[0] = 980;
      m_rotY -= 3.1415; 
    }
    if (update[2] > 980) {
      update[2] = 980;
      m_rotY -= 3.1415; 
    }


    // Water
   if (update[0] > 170 && update[0] < 600 && update[2] > 500 && update[2] < 720) {
     m_rotY -= 3.1415; 
   }
    

    m.SetPosition(update);
    //cout << "Manipulator " << p.GetPhysMode() << endl;
    //update.Print();

  
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  double m_rotY;
  ScentMap * m_pMap;
  double m_totaltime;
  double m_wait;
  int m_counter;
};

//====================================================
class WaterManipulator : public IManipulator
{
public:
  WaterManipulator() {
    m_count = 1200;
    m_abs = 0;
    m_first = true;
  }
  virtual ~WaterManipulator() {
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
    m_campos_raw = c;
    m_campos = c - cp;
    m_campos.Flip();


  }

  void SetBase(const string & b) {
    m_base = b;
  }

  virtual void Update(GamePhysObject & o, double deltatime) {
    m_abs++;
    //if (m_abs % 2 == 0)
    //return;
    int i;

    if (m_count >=1500)
      m_count = 1200;
  
    MsgSceneNode & msn = o.MessageSceneNode();

    if (m_first) {
      m_orig = msn;
      m_first = false;
    }

    //==========================================================

    SphereCoordinates s = m_campos.AsSphere();
    //cout << "Sphere coords " << s.phi() << " " << s.theta() << " " << s.r() << endl;

    //s.SetTheta(s.theta());

    double ff = 1.-cos(s.theta());
    if (ff < 0.)
      ff = 0.;
    //cout << "ff=" << ff << endl;
    
    for (i=0; i<m_orig.Mesh(0).GetTexCoordCount(); i++) {
      const StreamCoordinates & from = m_orig.Mesh(0).GetTextCoordConst(i);
      StreamCoordinates & to = msn.Mesh(0).GetTextCoord(i);

      // Individual edges =======================================
      PhysObject & pp = o.GetPhysObject();
      Coordinates cp = pp.GetPosition();   
      StreamCoordinates edge = m_orig.Mesh(0).GetVertex(i) * msn.GetScale();
      edge += cp;
      cout << "Edge " << endl;
      msn.GetPosition().Print();
      edge.Print();
      m_campos_raw.Print();
      StreamCoordinates rel = m_campos_raw - edge;     
      rel.Print();
      rel.Flip();
      s = rel.AsSphere();
      s.SetTheta(s.theta()*1.3);
      ff = 1.-cos(s.theta());
      cout << "ff=" << ff << endl;
      if (ff < 0.)
	ff = 0.;
      //=========================================================




      to = from;
      double ffX = cos(s.phi())*ff;
      double ffY = sin(s.phi())*ff;
      
      to[0] = 0.5+(to[0]-0.5)/2.;
      to[1] = 0.5+(to[1]-0.5)/2.;   
      to[0] -= 0.25*ffX;
      to[1] += 0.25*ffY;
      //cout << "Update tex coords from " << from[0] << " " << from[1] << " to " << to[0] << " " << to[1] << endl;
    } 




    //==========================================================


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
  Coordinates m_center;
  Coordinates m_lastPos;
  int m_count;
  int m_abs;
  string m_base;

  MsgSceneNode m_orig;
  bool m_first;
  Coordinates m_campos;
  Coordinates m_campos_raw;

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
  
  MsgLightNode light;
  light.SetPosition(StreamCoordinates(5300, 1200, 4900));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5400, 1800, 5400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 1800, 3400));
  eng.AddLight(light);

  int i;

  // Add water
  
  WaterManipulator manip_w;
  manip_w.SetBase("calm_surface");
  MsgSceneNode puddle;
  puddle.SetName("water");
 
  puddle.SetModel("data/Models/block7.ms3d");
  puddle.SetPosition(StreamCoordinates(8400, 260, 12200));
  //puddle.SetRotation(StreamCoordinates(3.14/2., 0, 0.));
  puddle.SetPhysMode(1);

  puddle.SetScale(45.);
  puddle.SetRequestLoopBack(true);
  puddle.SetRequestMesh(true);
 
  puddle.Material(0).SetLighting(false);

  eng.AddSceneNode(puddle, &manip_w);



  ScentMap scentmap(32, 512);

  int n = 2;
  svec<AntManipulator> antmanips;
  antmanips.resize(n);

  for (i=0; i<antmanips.isize(); i++) {
    antmanips[i].SetMap(&scentmap);

    if (i > 0)
      antmanips[i].SetWaitTime(RandomFloat(20.));
    
    MsgSceneNode node;
    node.Material(0).SetTexture("data/Textures/black.jpg");
    //node.Material(0).SetTexture("data/Models/blue.jpg");
    node.SetModel("data/Models/ant_0.ms3d");
    //node.SetModel("/home/manfred/Work/Temp/Ants/ant9.md2");
    //node.SetModel("/home/manfred/Work/revamp/Isaac/data/Models/raven1.md2");

    node.SetPosition(StreamCoordinates(5300, 280, 4900));
    //node.SetPosition(StreamCoordinates(0, 0, 0));
    
    // No physics
    node.SetPhysMode(2);
    
    node.SetScale(16000.);
    //node.SetScale(1.6);
    //node.SetScale(6.);
     
    node.Animation().SetAnimation("run");
    //node.Animation().SetAnimation("fly");
    node.Animation().SetSpeed(30);
    
    // We want to take control of the object
    node.SetRequestLoopBack(true);
    
    // Enable this if yu want to control the mesh! 
    node.SetRequestMesh(false);
    
    node.Material(0).SetLighting(false);
    //node.Material(0).SetShinyness(30);
 
    // Add it!
    eng.AddSceneNode(node, &antmanips[i]);
    //eng.AddSceneNode(node);
  }

  eng.RegisterGlobal(&scentmap);

  eng.Run();


  return 0;
}
