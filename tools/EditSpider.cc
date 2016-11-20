
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"





class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
    m_index = 0;
    m_frame = 0;
    m_rot = 0;

    m_fb = -0.3;
    m_updown = .06;
    m_bToggle = true;
    m_init = 0;
    m_time = 0.;
  }
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  void SetKey(const string & key) {
    m_key = key;
  }
  
  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    m_frame++;
    
    int offset = 15;

    m_time += deltatime;
    
    //double angle = 0.02;
    double angle = 0.04;
    //double angle1 = 0.04;
    double x = 0.;
    double y = 0.;
    if (m_key == "B") {
      x = angle;
    }
    if (m_key == "G") {
      x = -angle;
    }
    if (m_key == "N") {
      y = angle;
    }
    if (m_key == "H") {
      y = -angle;
    }
    if (m_key == "TAB" && m_key != m_lastKey) {
      m_index++;
      if (m_index >= m_skeleton.GetNerves().isize())
	m_index = 0;
    }
    if (m_key == "BACK" && m_key != m_lastKey) {
      m_index--;
      if (m_index < 0)
	m_index = m_skeleton.GetNerves().isize()-1;
     
    }
    cout << "Nerves: " << m_skeleton.GetNerves().isize() << endl;
    m_init++;
    int ff = 10;
    
    if (m_init == ff || (m_frame + m_init) % (2*ff) == 0) {
      //if (m_init == 90 || (m_frame + m_init) % 180 == 0) {
      if (m_bToggle)
	m_updown = -m_updown;
      else
	m_fb = -m_fb;
      
      m_bToggle = !m_bToggle;
    }

    double upd = sin(7*m_time);
    double forback = -cos(7*m_time);

    svec<double> features;
    m_skeleton.MakeFeatureVector(features, deltatime);
 
    m_skeleton.Move("up", upd*0.3);
    m_skeleton.Move("forward", forback*0.3);

    cout << "Feature: ";
    for (int y=0; y<features.isize(); y++)
      cout << " " << features[y];
    cout << " out: " << upd << " " << forback << endl;

    
    /*
    if (m_bToggle)
      m_skeleton.Move("up", m_updown);
    else
      m_skeleton.Move("forward", m_fb);
    */


    m_skeleton.Move(m_index, x);
    m_skeleton.Move(m_index+1, y);

    //m_skeleton.Move("up", deltatime);
    //m_skeleton.Move("forward", deltatime);
    m_skeleton.Update(deltatime);

  
    
    //m_rot = 0.1*deltatime;
    //m_skeleton.RotateAll(Coordinates(1, 0, 1), deltatime);
    
    
    if (m_frame % 100 == 0) {
      m_skeleton.Write(m_save);
      cout << "SAVED!!" << endl;
    }
    
    char msg[1024];
    double theMove = m_skeleton.GetNerves()[m_index].GetMove();
    string name = m_skeleton.GetNerves()[m_index].GetName();
     
    sprintf(msg, "Nerve: %d (%s); move=%f;\n", m_index, name.c_str(), theMove);
    
    
    
    SceneNodeMeshPhysics phys;
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(phys, m_skeleton);
    MsgSceneNode & node = o.MessageSceneNode();

    StreamCoordinates rr = node.GetRotation();
    node.SetRotation(rr+m_skeleton.RelRot());
    //node.SetRotation(rr+Coordinates(0, 1, 0)*deltatime);

    
    
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();
    Coordinates pp = m.GetPosition();
    if (m_frame < 5) {
      m_basePos = pp;
    }
    m.SetPosition(m_basePos+m_skeleton.AbsPos());
    
   
    node.SetMessage(msg);
    
    node.Mesh(0) = phys;
    m_lastKey = m_key;
    m_key = "";
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }
  void SetSkeleton(const NPCSkeleton &s) { 
    m_skeleton = s;
  }

  void SetSaveName(const string & s) {
    m_save = s;
  }
private:
  Coordinates m_basePos;
  Coordinates m_center;
  Coordinates m_lastPos;
  NPCSkeleton m_skeleton;
  int m_index;
  string m_key;
  string m_lastKey;
  string m_save;
  int m_frame;
  double m_rot;
  double m_updown;
  double m_fb;
  bool m_bToggle;
  int m_init;
  double m_time;
};


class KeyCtrl : public IGlobal
{
public:
  KeyCtrl(MyManipulator * p) {
    m_pManip = p;
  }
  
  virtual void StartFrame(double deltatime) {
  }
  
  virtual void EndFrame(double deltatime) {
  }

  virtual void KeyPressed(const string & s) {
    cout << "Got message Key pressed: " << s << endl;
    if (m_pManip != NULL)
      m_pManip->SetKey(s);
  }
  
private:
  MyManipulator * m_pManip;
};
 





int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<string> cStringCmmd("-c","body config file", "");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(cStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  string cString = P.GetStringValueFor(cStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng("localhost", false);
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);

  
  // Make skeleton
  BodyBuilder bb;
  if (cString != "")
    bb.ReadConfig(cString);
  
  NPCSkeleton s;
 
  bb.GetSpider(s);
  s.Scale(10.);

  
  MyManipulator manip2;

  int k = 0;
  char name[256];
  while (true) {
    sprintf(name, "skeleton%d.txt", k);
    FILE * p = fopen(name, "r");
    if (p) {
      fclose(p);
      k++;
      continue;
    }
    break;
  }

  manip2.SetSaveName(name);
  
  MsgSceneNode node;
  node.SetName("skeleton");
  //node.Material(0).SetTexture("data/Models/green.jpg");
  node.Material(0).SetTexture("data/Textures/allblack.jpg");
  //node.SetModel("data/Models/ball.ms3d");
  node.SetPosition(StreamCoordinates(5300, 1400, 4900));

  MSkeleton makeSkeleton;
  makeSkeleton.MakeSkeleton(node.Mesh(0), s);

  // No physics
  node.SetPhysMode(1);

  node.SetScale(25.);
  // We want to take control of the object
  node.SetRequestLoopBack(true);
 
  // Enable this if yu want to control the mesh! 
  node.SetRequestMesh(false);

  manip2.SetSkeleton(s);
 
  // Add it!
  eng.AddSceneNode(node, &manip2);


  KeyCtrl keyCtrl(&manip2);
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
