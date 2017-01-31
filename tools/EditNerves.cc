
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

 
    //double angle = 0.02;
    double angle = 0.04;
    double x = 0.;
    if (m_key == "B") {
      x = angle;
    }
    if (m_key == "G") {
      x = -angle;
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
    m_skeleton.Move(m_index, x);
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
    
    m_headPos = m_skeleton[1].GetCoordsPlusDelta()+m_basePos+m_skeleton.AbsPos();
    m_headPos[1] += 6.8;
    m_headRot = m_skeleton.RelRot() + node.GetRotation();
    //m_headRot[1] += 0.6;
    
  
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

  const Coordinates & HeadPos() const {return m_headPos;}
  const Coordinates & HeadRot() const {return m_headRot;}


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
  Coordinates m_headPos;
  Coordinates m_headRot;
};

class HeadManipulator;



//==================================================================
//==================================================================
//==================================================================
//==================================================================


class HeadManipulator : public IManipulator
{
public:
  HeadManipulator() {}
  virtual ~HeadManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    MsgSceneNode & n = o.MessageSceneNode();
    StreamCoordinates pos = n.GetPosition();
    StreamCoordinates rot = n.GetRotation();

    pos = m.GetPosition();


    //pos[1] += 0.2;
    //rot[1] += 0.02;
    
    //cout << "ROT/POS " << pos[1] << " " << rot[1] << endl; 

    //n.SetPosition(pos);
    m.SetPosition(m_pos);
    n.SetRotation(m_rot);

  
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }

  void SetCoords(const Coordinates & pos, const Coordinates & rot) {
    m_pos = pos;
    m_rot = rot;
    cout << "Head coords: " << m_pos[0] << " " << m_pos[1] << " " << m_pos[2] << endl;
  }

private:
  Coordinates m_pos;
  Coordinates m_rot;
  Coordinates m_center;
  Coordinates m_lastPos;
};



class KeyCtrl : public IGlobal
{
public:
  KeyCtrl(MyManipulator * p, HeadManipulator * pHead) {
    m_pManip = p;
    m_pHead = pHead;
  }
  
  virtual void StartFrame(double deltatime) {
  }
  
  virtual void EndFrame(double deltatime) {
    m_pHead->SetCoords(m_pManip->HeadPos(), m_pManip->HeadRot());
  }

  virtual void KeyPressed(const string & s) {
    cout << "Got message Key pressed: " << s << endl;
    if (m_pManip != NULL)
      m_pManip->SetKey(s);
  }
  
private:
  MyManipulator * m_pManip;
  HeadManipulator * m_pHead;
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
  s.DoPhysics(false);
  
  bb.GetFigure(s);
  s.Scale(10.);
  //s.Move(1, -0.2);
  //s.Move(3, 0.5);
  //s.Move(4, 0.5);
  //s.Update(0.1);

  
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


  // Add HEAD here
   HeadManipulator headManip;

  MsgSceneNode stat;

  //stat.SetScale(600.);
 
      
  stat.Material(0).SetTexture("applications_data/schoolgame//CharacterHeads/Eva/OBJ/Eva_texture_closed.jpg");
  stat.Material(0).SetLighting(true);    
    
  stat.SetModel("applications_data/schoolgame//CharacterHeads/Eva/OBJ/Eva_closed.obj");
	
  stat.SetPosition(StreamCoordinates(3000, 600, 300));
  stat.SetRotation(Coordinates(0., 3.14 + RandomFloat(2.)-1., 0.));
  stat.SetPhysMode(2);
  stat.SetScale(28.);
    
  stat.SetRequestLoopBack(true);
  stat.SetRequestMesh(false);
  
  stat.SetName("Eva_head");  
  eng.AddSceneNode(stat, &headManip);



  
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

  node.SetScale(20.);
  // We want to take control of the object
  node.SetRequestLoopBack(true);
 
  // Enable this if yu want to control the mesh! 
  node.SetRequestMesh(false);

  manip2.SetSkeleton(s);
 
  // Add it!
  eng.AddSceneNode(node, &manip2);
  
  MsgLightNode light;
  light.SetRadius(200000);
  light.SetPosition(StreamCoordinates(4600, 2600, 4400));
  eng.AddLight(light);
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(5600, 2600, 5500));
  eng.AddLight(light);


  KeyCtrl keyCtrl(&manip2, &headManip);
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
