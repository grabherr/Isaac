
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>

class LeafManipulator : public IManipulator
{
public:
  LeafManipulator() {
    m_rate = 2.6;
    m_counter = 0.;
  }
  virtual ~LeafManipulator() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}

  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
  }


  virtual void Update(GamePhysObject & o, double deltatime) {
    int i;

    m_counter++;
    //if (m_counter % 2 == 0)
    //return;

    PhysObject & p = o.GetPhysObject();
    p.SetSkipUpdate(true);
    
    if (m_grow.isize() == 0) {
      m_rot = o.MessageSceneNode().GetRotation();
      m_rotcurr = m_rot;
      m_grow.resize(p.isize());
      Coordinates sub = p[0].Position();
      for (i=0; i<m_grow.isize(); i++) {
	StreamCoordinates & cc = m_grow[i];
	cc[0] = 1. + RandomFloat(0.003);
	cc[1] = 1. + RandomFloat(0.010);
	cc[2] = 1. + RandomFloat(0.005);
 	Coordinates & c = p[i].Position();
	c -= sub;
      }
      //p.GetCenterDirect().Position()[1] -= 0.5;
    }
    
    bool bMove = false;
    if (m_rate < 0.02) {
      bMove = true;
      p.SetPhysMode(2);
    } else {
      for (i=0; i<p.isize(); i++) {
	Coordinates & c = p[i].Position();
	StreamCoordinates & cc = m_grow[i];
	c[0] *= ((cc[0]+RandomFloat(0.001))*m_rate + 1. - m_rate);
	c[1] *= ((cc[1]+RandomFloat(0.001))*m_rate + 1. - m_rate);
	c[2] *= ((cc[2]+RandomFloat(0.001))*m_rate + 1. - m_rate);
	cout << "NOW " << c[2] << endl;
      }
      
      m_rate *= 0.992;
    }
    //p.Fixate();
    
    //p.SetEngRotation(StreamCoordinates(0, m_rotY, 0));

    //return;
    if (!bMove)
      return;

    if (m_counter % 4 == 0) {   
      m_rotmove[0] += -0.25+RandomFloat(0.5);
      m_rotmove[1] += -0.1+RandomFloat(0.2);
      m_rotmove[2] += -0.25+RandomFloat(0.5);
    }

    m_rotcurr += m_rotmove*deltatime;
   
    for (i=0; i<3; i++) {
      if (m_rotcurr[i] - m_rot[i] > 3.14/5 || m_rot[i] - m_rotcurr[i] > 3.14/5)
	m_rotmove[i] -= m_rotmove[i]*0.7;
    }

 
    //StreamCoordinates back = m_rot - m_rotcurr;
    m_rotcurr = m_rotcurr*(1.-deltatime*0.1) + m_rot*deltatime*0.1;
   

    o.MessageSceneNode().SetRotation(m_rotcurr);

 
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return;  
  }

private:
  svec<StreamCoordinates> m_grow;
  double m_rate;
  StreamCoordinates m_rot;
  StreamCoordinates m_rotcurr;
  StreamCoordinates m_rotmove;
  int m_counter;
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
  eng.DoTriangleCollision(false);
  eng.DoAllInteractions(false);
  // Let's add some more realistic lighting here
  MsgLightNode light;
  light.SetPosition(StreamCoordinates(4000, 1200, 4900));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5000, 1800, 4400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(3400, 1800, 3400));
  eng.AddLight(light);
 

 
  svec<LeafManipulator> manip;

  manip.resize(300);
  //manip.resize(5);

  int i;

  for (i=0; i<manip.isize(); i++) {
    MsgSceneNode node;
    node.Material(0).SetTexture("data/Textures/leaf.bmp");
    
    //node.SetModel("data/Models/block7.ms3d");
    
    //node.SetPosition(StreamCoordinates(5300, 340, 4900));
    node.SetPosition(StreamCoordinates(4000+RandomFloat(2000), 340, 4000+RandomFloat(2000)));
    
    SceneNodeMeshPhysics & m = node.Mesh(0);
    
    // Add vertices manually
    // Base
    m.AddVertex(StreamCoordinates(0., 0., 0.));
    m.AddTexCoord(Coordinates(0.5, 0., 0.));
    
    // Level 1
    m.AddVertex(StreamCoordinates(-.1, 0.3, 0.));
    m.AddTexCoord(Coordinates(0.2, 0.3, 0.));
    
    m.AddVertex(StreamCoordinates(.1, 0.3, 0.));
    m.AddTexCoord(Coordinates(0.7, 0.3, 0.));
    
    
    // Level 2
    m.AddVertex(StreamCoordinates(-.1, 0.7, 0.1));
    m.AddTexCoord(Coordinates(0.2, 0.7, 0.));
    
    m.AddVertex(StreamCoordinates(.1, 0.7, 0.1));
    m.AddTexCoord(Coordinates(0.7, 0.7, 0.));
    
    // Top
    m.AddVertex(StreamCoordinates(0., 1., 0.3));
    m.AddTexCoord(Coordinates(0.5, 1., 0.));
    
    // Base
    m.AddVertex(StreamCoordinates(0., 0., 0.));
    m.AddTexCoord(Coordinates(0.5, 0., 0.));
    
    // Level 1
    m.AddVertex(StreamCoordinates(-.1, 0.3, 0.));
    m.AddTexCoord(Coordinates(0.2, 0.3, 0.));
    
    m.AddVertex(StreamCoordinates(.1, 0.3, 0.));
    m.AddTexCoord(Coordinates(0.7, 0.3, 0.));
    
    
    // Level 2
    m.AddVertex(StreamCoordinates(-.1, 0.7, 0.1));
    m.AddTexCoord(Coordinates(0.2, 0.7, 0.));
    
    m.AddVertex(StreamCoordinates(.1, 0.7, 0.1));
    m.AddTexCoord(Coordinates(0.7, 0.7, 0.));
    
    // Top
    m.AddVertex(StreamCoordinates(0., 1., 0.3));
    m.AddTexCoord(Coordinates(0.5, 1., 0.));
    
    m.AddIndex(0, 1, 2);
    m.AddIndex(2, 1, 3);
    m.AddIndex(2, 3, 4);
    m.AddIndex(4, 3, 5);
    
    m.AddIndex(7, 6, 8);
    m.AddIndex(7, 8, 9);
    m.AddIndex(9, 8, 10);
    m.AddIndex(9, 10, 11);
    m.RecomputeNormals();
    
    node.SetRotation(StreamCoordinates(0., RandomFloat(2*3.14), 0.));
    node.SetPhysMode(1);
    
    node.SetScale(75.);
    node.SetRequestLoopBack(true);
    //node.SetRequestMesh(true);
    
    //node.Material(0).SetLighting(true);
    
    eng.AddSceneNode(node, &manip[i]);
  }
 
  eng.Run();


  return 0;
}
