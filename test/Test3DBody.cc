
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"


class KeyCtrl : public IGlobal
{
public:
  KeyCtrl() {}
  virtual void StartFrame(double deltatime) {
  }
  virtual void EndFrame(double deltatime) {
  }

  virtual void KeyPressed(const string & s) {
    cout << "Got message Key pressed: " << s << endl;
  }

};
 



class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
    m_counter1 = 0.;
    m_counter2 = 0.;
    m_counter3 = 0.;
    m_counter4 = 0.;
    m_counter5 = 0.;
    m_counter6 = 0.;
    m_counter7 = 0.;
    m_counter8 = 0.;
    m_move1 = -0.6;
    m_move2 = -0.58;
    m_move3 = -0.56;
    m_move4 = -0.54;
    m_move5 = -0.59;
    m_move6 = -0.3;
    m_move7 = 1.0;
    m_move8 = 0.4;
    m_dir = 1;
  }
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    return;
    cout << "Call update!" << endl;
    //BodyBuilder bb;
    //NPCSkeleton s;
    //bb.GetHand(s);
    //s.Scale(60.);
    
    
    //s.AddToBoneRot(0, NPCBoneCoords(0., rot, 0., 0.));
    //m_skeleton.AddToBase(Coordinates(0.2, 0., 0.));
    //m_skeleton.Print();

    double move1 = m_move1*deltatime;
    double move2 = m_move2*deltatime;
    double move3 = m_move3*deltatime;
    double move4 = m_move4*deltatime;
    double move5 = m_move5*deltatime;
    double move6 = m_move6*deltatime;
    double move7 = m_move7*deltatime;
    double move8 = m_move8*deltatime;
    
    m_counter1 += move1;
    if (m_counter1 > 0 && m_move1 > 0) {
      m_move1 = -m_move1;
    }
    if (m_counter1 < -1.5 && m_move1 < 0) {
      m_move1 = -m_move1;
    }

    m_counter2 += move2;
    if (m_counter2 > 0 && m_move2 > 0) {
      m_move2 = -m_move2;
    }
    if (m_counter2 < -1.5 && m_move2 < 0) {
      m_move2 = -m_move2;
    }

    m_counter3 += move3;
    if (m_counter3 > 0 && m_move3 > 0) {
      m_move3 = -m_move3;
    }
    if (m_counter3 < -1.5 && m_move3 < 0) {
      m_move3 = -m_move3;
    }

    m_counter4 += move4;
    if (m_counter4 > 0 && m_move4 > 0) {
      m_move4 = -m_move4;
    }
    if (m_counter4 < -1.5 && m_move4 < 0) {
      m_move4 = -m_move4;
    }

    m_counter5 += move5;
    if (m_counter5 > 0 && m_move5 > 0) {
      m_move5 = -m_move5;
    }
    if (m_counter5 < -1.5 && m_move5 < 0) {
      m_move5 = -m_move5;
    }

    m_counter6 += move6;
    
    if (m_counter6 > 0.5 && m_move6 > 0) {
      m_move6 = -m_move6;
      //m_dir = -m_dir;
    }
    if (m_counter6 < -0.5 && m_move6 < 0) {
      m_move6 = -m_move6;
      //m_dir = -m_dir;
    }

    m_counter7 += move7;
    if (m_counter7 > 1.3 && m_move7 > 0) {
      m_move7 = -m_move7;
    }
    if (m_counter7 < .0 && m_move7 < 0) {
      m_move7 = -m_move7;
    }
    m_counter8 += move8;
    if (m_counter8 > 0 && m_move8 > 0) {
      m_move8 = -m_move8;
    }
    if (m_counter8 < -1.5 && m_move8 < 0) {
      m_move8 = -m_move8;
    }
   //move += m_counter;
    //m_skeleton.AddToBoneRot(0, NPCBoneCoords(0., deltatime*0.2, 0., 0.));

    int offset = 15;

    m_skeleton.AddToBoneRot(offset+2, NPCBoneCoords(0., 0., 0., move1));
    m_skeleton.AddToBoneRot(offset+3, NPCBoneCoords(0., 0., 0., move1));
    m_skeleton.AddToBoneRot(offset+4, NPCBoneCoords(0., 0., 0., move1));
    
    m_skeleton.AddToBoneRot(offset+6, NPCBoneCoords(0., 0., 0., move2));
    m_skeleton.AddToBoneRot(offset+7, NPCBoneCoords(0., 0., 0., move2));
    m_skeleton.AddToBoneRot(offset+8, NPCBoneCoords(0., 0., 0., move2));
    
    m_skeleton.AddToBoneRot(offset+10, NPCBoneCoords(0., 0., 0., move3));
    m_skeleton.AddToBoneRot(offset+11, NPCBoneCoords(0., 0., 0., move3));
    m_skeleton.AddToBoneRot(offset+12, NPCBoneCoords(0., 0., 0., move3));
    
    m_skeleton.AddToBoneRot(offset+14, NPCBoneCoords(0., 0., 0., move4));
    m_skeleton.AddToBoneRot(offset+15, NPCBoneCoords(0., 0., 0., move4));
    m_skeleton.AddToBoneRot(offset+16, NPCBoneCoords(0., 0., 0., move4));
    
    m_skeleton.AddToBoneRot(offset+18, NPCBoneCoords(0., 0., 0., -move5/1.2));
    m_skeleton.AddToBoneRot(offset+19, NPCBoneCoords(0., 0., 0., -move5/1.2));

    // Body
    m_skeleton.AddToBoneRot(2, NPCBoneCoords(0., 0., move6, 0.));
    //m_skeleton.AddToBoneRot(7, NPCBoneCoords(0., move7, 0., 0.));
    m_skeleton.AddToBoneRot(8, NPCBoneCoords(0., -move8, -move8/2, 0.));
    
    m_skeleton.AddToBoneRot(11, NPCBoneCoords(0., move6, move6, 0.));
    m_skeleton.AddToBoneRot(12, NPCBoneCoords(0., m_dir*move6, move6, 0.));
    
    m_skeleton.AddToBoneRot(13, NPCBoneCoords(0., -m_dir*move6, move6, 0.));
    m_skeleton.AddToBoneRot(14, NPCBoneCoords(0., move6, move6, 0.));
    
    SceneNodeMeshPhysics phys;
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(phys, m_skeleton);
    MsgSceneNode & node = o.MessageSceneNode();
    

    node.Mesh(0) = phys;
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }
  void SetSkeleton(const NPCSkeleton &s) { 
    m_skeleton = s;
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  double m_counter1;
  double m_counter2;
  double m_counter3;
  double m_counter4;
  double m_counter5;
  double m_counter6;
  double m_counter7;
  double m_counter8;
  double m_move1;
  double m_move2;
  double m_move3;
  double m_move4;
  double m_move5;
  double m_move6;
  double m_move7;
  double m_move8;
  NPCSkeleton m_skeleton;
  double m_dir;
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

  s.Scale(10.);

  MyManipulator manip2;

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


  KeyCtrl keyCtrl;
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
