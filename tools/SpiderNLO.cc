
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "npc/NPCControl.h"
#include "npc/TopLevel.h"

double GetMilkScore(double & relPhi,
		    const Coordinates & oldPos,
		    const Coordinates & realPos,
		    const Coordinates & target,
		    double currRot)
{

  Coordinates rel = target - realPos;
  rel[1] = 0.;
  
  Coordinates relOld = target - oldPos;
  relOld[1] = 0.;

  double score = relOld.Length() - rel.Length();
  SphereCoordinates s = rel.AsSphere();

  
  relPhi = (s.phi()-currRot)/PI_P;
  while(relPhi > 1) {
    relPhi -= 2;
  }
  while (relPhi < -1) {
    relPhi += 2;
  }

  return score;

}


class BallManipulator : public IManipulator
{
public:
  BallManipulator() {
    m_honk = "data/Sounds/magic.wav";
  }
  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    m_camPos = c;
  }
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();
    Coordinates pp = m.GetPosition();
    m_pos = pp;
    
    if (m_key == "SPACE") {
      m_pos = m_camPos;
      m_pos[1] = 10;
      cout << "KEY " << m_key << endl;
      m_key = "";
      Sound & sound = p.GetSound();
      sound.UpdateAdd("honk", 
		      m_honk,
		      m_camPos);
      
    } else {
      Sound & sound = p.GetSound();
      sound.UpdateAdd("honk", 
      	      "",
      	      m_camPos);      
    }
    m.SetPosition(m_pos);
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {}
  void SetKey(const string & key) {
    m_key = key;
  }
  
  const Coordinates & Pos() const {return m_pos;}


  
private:
  Coordinates m_camPos;
  Coordinates m_pos;
  string m_key;
  string m_honk;
};

class SkeletonManipulator : public IManipulator
{
public:
  SkeletonManipulator() {
    m_time = 0.;
    m_frame = 0;
    m_rot = 0;
    m_init = 0;
 
    m_lastTime = 0;
 
    m_lastDiff = 0;
    m_currDiff = 0;
    m_reward = 0;
    m_index = 0;
    m_honk = "data/Sounds/magic.wav";
    m_currRot = 0.;
    m_doRot = false;
    m_lastRot = 0.;
  }
  
  virtual ~SkeletonManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}

  const Coordinates & GetLastPos() const {return m_lastPos;}
  
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    m_camPos = c;
  }

  void SetKey(const string & key) {
    m_key = key;
  }
  
  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    int i;
    
    m_frame++;
    m_time += deltatime;
    
    int offset = 15;
    
  
    /*
    if (m_key == "SPACE") {
      cout << "KEY " << m_key << endl;
      m_key = "";
      Sound & sound = p.GetSound();
      sound.UpdateAdd("honk", 
		      m_honk,
		      m_camPos);
      
    } else {
      Sound & sound = p.GetSound();
      sound.UpdateAdd("honk", 
      	      "",
      	      m_camPos);      
	      }*/
 
    
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();
    Coordinates pp = m.GetPosition();
    if (m_frame < 5) {
      m_basePos = pp;
      m_realPos =  m_skeleton.AbsPos();
      m_lastCheck = m_skeleton.AbsPos();
      m_lastRelPos = m_realPos + m_basePos;
    }

    Coordinates deltaPos = m_skeleton.AbsPos() - m_lastCheck;
    double zz = deltaPos[1];
    deltaPos[1] = 0.;
    SphereCoordinates sTmp = deltaPos.AsSphere();
    sTmp.phi() += m_currRot;
    deltaPos.FromSphere(sTmp);
    deltaPos[1] = zz;
    Coordinates oldPos = m_basePos+m_realPos;
    m_realPos += deltaPos;
    Coordinates newPos = m_basePos+m_realPos;
  
    m.SetPosition(m_basePos+m_realPos);
    //m.SetPosition(m_basePos+m_skeleton.AbsPos());

    m_lastCheck = m_skeleton.AbsPos();

    //m.SetPosition(m_basePos+m_skeleton.AbsPos());
    //Coordinates spiderPos = m_basePos+m_skeleton.AbsPos();
    Coordinates spiderPos = m_basePos+m_realPos;
    
    m_init++;
 
    string learn = "n/a";

    m_lastDiff = (m_lastPos - m_lastCamPos).Length();
    m_currDiff = (spiderPos - m_camPos).Length();
  
    //m_reward = m_currDiff - m_lastDiff;
    m_reward = +(spiderPos[2] - m_lastPos[2])*0.7;
    //m_reward = 0.7*((m_lastPos - m_targetPos).Length() - (spiderPos - m_targetPos).Length());
    double xd = spiderPos[0] - m_lastPos[0];
    if (xd < 0)
      xd = -xd;
    m_reward -= xd*0.3;
      
    m_reward = 0.7*((m_lastPos - m_targetPos).Length() - (spiderPos - m_targetPos).Length());

    if (m_doRot) {
      m_reward = (spiderPos - m_lastPos).Length();
    }


    
    /*
    for (i=0; i<m_skeleton.GetNerves().isize(); i++) {
      double data = m_out[i];
      double dd = (m_skeleton.GetNerves())[i].GetMove();
      bool b = m_skeleton.Move(i, data*deltatime*moveWeight);
      }*/

    svec<double> features;
    m_skeleton.MakeFeatureVector(features, deltatime);

    svec<double> rew;
    rew.resize(1);
    rew[0] = m_reward;
    
    m_ctrl.Process(m_out,
		   features,
		   rew,
		   deltatime);

    for (i=0; i<m_skeleton.GetNerves().isize(); i++) {
      double data = m_out[i];
      bool b = m_skeleton.MoveTowards(i, data, deltatime);
    }
    
    m_skeleton.Update(deltatime);


    
    //-------------------------------------------------------------------
    if (m_frame == 800) {
      m_top.resize(1, 1, 1, 30);
      m_doRot = true;
    }
    double relPhi = 0.;
    double input;
    double score = GetMilkScore(input, m_lastPos, spiderPos, m_targetPos, m_currRot);
    if (m_doRot) {
      IOEntity ent;
      ent.resize(1, 1, 1);
      ent.in(0) = input;
      m_top.Update(ent, 0.6, score);
      double a = ent.out(0)*0.02;
      a *= PI_P;
      
      
      m_currRot += a;
      if (m_currRot < -PI_P)
	m_currRot += 2*PI_P;
      if (m_currRot > PI_P)
	m_currRot -= 2*PI_P;
      
    }
    
    //-------------------------------------------------------------------
    
    m_lastPos = spiderPos;
    m_lastCamPos = m_camPos;
    //m_reward /= 5.;
    cout << "REWARD: " << m_reward << " " << spiderPos[0] << " " << spiderPos[2] <<  endl;
   
    char msg[1024];
    double theMove = m_skeleton.GetNerves()[m_index].GetMove();
    string name = m_skeleton.GetNerves()[m_index].GetName();
     
    sprintf(msg, "Nerve: %d (%s); move=%f;\n%f, %f, %f ... %s\n%f %f %f camera\nlast: %2.2f curr: %2.2f reward: %2.2f\n", m_index,
	    name.c_str(), theMove, spiderPos[0], spiderPos[1], spiderPos[2], learn.c_str(),
	    m_camPos[0], m_camPos[1], m_camPos[2],
	    m_lastDiff, m_currDiff, m_reward);
    cout << "SPIDER check " << m_lastDiff << " " << m_currDiff << endl;
    char tmptmp[512];
    
    for (i=0; i<m_out.isize(); i++) {
      sprintf(tmptmp, "%2.3f ... %s\nROTATION: %f\n", m_out[i], m_skeleton.GetNerves()[i].GetName().c_str(), m_currRot);
      strcat(msg, tmptmp);
    } 
    
     
    SceneNodeMeshPhysics phys;
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(phys, m_skeleton);
    MsgSceneNode & node = o.MessageSceneNode();

    StreamCoordinates rr = node.GetRotation();
    //node.SetRotation(rr+m_skeleton.RelRot());
    //node.SetRotation(rr+m_skeleton.RelRot()+Coordinates(0, 1, 0)*m_currRot*-1);
    node.SetRotation(Coordinates(0, 1, 0)*m_currRot*-1);
    //node.SetRotation(rr+Coordinates(0, 1, 0)*deltatime);

 
    /*
      Coordinates deltaPos = m_skeleton.AbsPos() - m_lastCheck;
      double zz = deltaPos[1];
      deltaPos[1] = 0.;
      SphereCoordinates sTmp = deltaPos.AsSphere();
      sTmp.phi() += m_currRot;
      deltaPos.FromSphere(sTmp);
      deltaPos[1] = zz;
      Coordinates oldPos = m_basePos+m_realPos;
      m_realPos += deltaPos;
      Coordinates newPos = m_basePos+m_realPos;
      
      m.SetPosition(m_basePos+m_realPos);
      //m.SetPosition(m_basePos+m_skeleton.AbsPos());
      
      m_lastCheck = m_skeleton.AbsPos();
    */
    
    node.SetMessage(msg);
    
    node.Mesh(0) = phys;
    m_lastKey = m_key;
    m_key = "";
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing
    
    
  }
  void SetTargetPos(const Coordinates & c) {
    m_targetPos = c;
  }
    

  void SetSkeleton(const NPCSkeleton &s) {
    cout << "SetSkeleton start" << endl;
    int i;
    m_skeleton = s;
    const NPCNerveCostume & nerves = m_skeleton.GetNerves();

    int insize = m_skeleton.GetFeatDim();
    m_ctrl.SetUp(insize, nerves.isize(), 1);
    //for (i=0; i<nerves.isize(); i++) {
    //m_ctrl[i].SetName(nerves[i].GetName());
    //m_ctrl[i].SetIndex(i);
    //}
    m_ctrl.SetRange(-2., 2.);
    cout << "SetSkeleton end" << endl;

    
  }

 private:
  Coordinates m_basePos;
  Coordinates m_center;
  Coordinates m_lastPos;
  Coordinates m_camPos;
  Coordinates m_lastCamPos;
  NPCSkeleton m_skeleton;
  string m_key;
  string m_lastKey;
  int m_frame;
  double m_rot;
  int m_init;
  double m_time;
  double m_lastTime;
  NPCControl m_ctrl;
  double m_lastDiff;
  double m_currDiff;
  double m_reward;
  int m_index;
  svec<double> m_out;
  string m_honk;
  Coordinates m_targetPos;
  TopLevel m_top;
  double m_currRot;
  double m_lastRot;
  bool m_doRot;
  Coordinates m_lastCheck;
  Coordinates m_realPos;
  Coordinates m_lastRelPos;
};


class KeyCtrl : public IGlobal
{
public:
  KeyCtrl(SkeletonManipulator * p, BallManipulator * pBall) {
    m_pManip = p;
    m_pBall = pBall;
  }
  
  virtual void StartFrame(double deltatime) {
    m_pManip->SetTargetPos(m_pBall->Pos());
  }
  
  virtual void EndFrame(double deltatime) {
  }

  virtual void KeyPressed(const string & s) {
    cout << "Got message Key pressed: " << s << endl;
    if (m_pManip != NULL) {
      m_pManip->SetKey(s);
      m_pBall->SetKey(s);
    }
  }
  
private:
  SkeletonManipulator * m_pManip;
  BallManipulator * m_pBall;
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
  
  bb.GetSpider(s);
  s.Scale(10.);

  
  SkeletonManipulator manip2;

  
  int k = 0;

  /*
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

  manip2.SetSaveName(name);*/
  
  
  MsgSceneNode node;
  node.SetName("skeleton");
  //node.Material(0).SetTexture("data/Models/green.jpg");
  node.Material(0).SetTexture("data/Textures/allblack.jpg");
  //node.SetModel("data/Models/ball.ms3d");
  node.SetPosition(StreamCoordinates(5300, 1400, 1200));

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



  BallManipulator ballManip;
  MsgSceneNode node3;
  node3.SetName("ball2");
  node3.Material(0).SetTexture("data/Models/green.jpg");
  node3.SetModel("data/Models/ball.ms3d");
  node3.SetPosition(StreamCoordinates(5300, 10, 4900));

  node3.SetPhysMode(2);

  node3.SetScale(15.);
  node3.SetRequestLoopBack(true);
  node3.SetRequestMesh(false);
 
  eng.AddSceneNode(node3, &ballManip);



  

  KeyCtrl keyCtrl(&manip2, &ballManip);
  eng.RegisterGlobal(&keyCtrl);

  eng.Run();


  return 0;
}
