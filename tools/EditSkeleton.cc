
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

    double x = 0.;
    double y = 0.;
    double z = 0.;

    double angle = 0.02;
    if (m_key == "B") {
      x = angle;
    }
    if (m_key == "N") {
      y = angle;
    }
    if (m_key == "M") {
      z = angle;
    }
    if (m_key == "G") {
      x = -angle;
    }
    if (m_key == "H") {
      y = -angle;
    }
    if (m_key == "J") {
      z = -angle;
    }
    if (m_key == "TAB" && m_key != m_lastKey) {
      m_index++;
      if (m_index >= m_skeleton.isize())
	m_index = 0;
    }
    if (m_key == "BACK" && m_key != m_lastKey) {
      m_index--;
      if (m_index < 0)
	m_index = m_skeleton.isize()-1;
     
    }
   
    m_skeleton.AddToBoneRot(m_index, NPCBoneCoords(0., x, y, z));

    if (m_frame % 100 == 0) {
      m_skeleton.Write(m_save);
      cout << "SAVED!!" << endl;
    }
    
    char msg[1024];
    const NPCBoneCoords & bone = m_skeleton[m_index].Rel();
    string name = "finger";
    switch(m_index) {
    case 0:
      name = "butt";
      break;
    case 1:
      name = "spine low";
      break;
    case 2:
      name = "spine high";
      break;
    case 3:
      name = "sholder left";
      break;
    case 4:
      name = "sholder right";
      break;
    case 5:
      name = "upper arm left";
      break;
    case 6:
      name = "lower arm left";
     break;
    case 7:
      name = "upper arm right";
      break;
    case 8:
      name = "lower arm right";
      break;
    case 9:
      name = "pelvis left";
      break;
    case 10:
      name = "pelvis right";
      break;
   case 11:
      name = "upper leg left";
      break;
   case 12:
      name = "lower leg right";
      break;
   case 13:
      name = "upper leg right";
      break;
   case 14:
      name = "lower leg right";
      break;
      
    }
    
    sprintf(msg, "Bone: %d (%s); phi=%f; theta=%f; rho=%f;\n", m_index, name.c_str(), bone.RX(), bone.RY(), bone.RZ());
    
     
    SceneNodeMeshPhysics phys;
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(phys, m_skeleton);
    MsgSceneNode & node = o.MessageSceneNode();
    
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
  Coordinates m_center;
  Coordinates m_lastPos;
  NPCSkeleton m_skeleton;
  int m_index;
  string m_key;
  string m_lastKey;
  string m_save;
  int m_frame;
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
  

  BodyBuilder bb;
  if (cString != "")
    bb.ReadConfig(cString);
  
  NPCSkeleton s;
 
  NPCSkeleton leftHand, rightHand, leftFoot, rightFoot;
  NPCSkeleton body;

  bb.GetHand(rightHand, false);
  bb.GetHand(leftHand, true);
  bb.GetFoot(rightFoot, false);
  bb.GetFoot(leftFoot, true);
  
  bb.GetBody(s);

  //rightHand.AddToBoneRot(0, NPCBoneCoords(0, 0, PI_P/4, -PI_P/4));
  //leftHand.AddToBoneRot(0, NPCBoneCoords(0, 0, PI_P/4, -PI_P/4));
  rightHand.AddToBoneRot(0, NPCBoneCoords(0, 0, 0, -PI_P/2));
  leftHand.AddToBoneRot(0, NPCBoneCoords(0, 0, PI_P, 3*PI_P/2));
  s.Attach(rightHand, 6);
  s.Attach(leftHand, 8);


  //=======================================================
  s.SetBaseline();
  //=======================================================
  
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
  node.SetPosition(StreamCoordinates(5300, 1450, 4900));

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
