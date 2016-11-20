
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "npc/SkeletonControl.h"
#include "npc/NPCHMM.h"





class SkeletonManipulator : public IManipulator
{
public:
  SkeletonManipulator() {
    m_index = 0;
    m_frame = 0;
    m_rot = 0;
    m_lastDiff = 0;
    m_init = 0;
    m_reward = 0.;
    m_currDiff = 0.;
    m_time = 0.;
    m_lastTime = 0.;
  }
  virtual ~SkeletonManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    m_camPos = c;
    //PhysObject & p = self.GetPhysObject();
    //PhysMinimal & m = p.GetCenterDirect();
    //Coordinates pp = m.GetPosition();
    //m_camPos = c - pp;
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
    
    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();
    Coordinates pp = m.GetPosition();
    if (m_frame < 5) {
      m_basePos = pp;
    }
    m.SetPosition(m_basePos+m_skeleton.AbsPos());
    Coordinates spiderPos = m_basePos+m_skeleton.AbsPos();

 
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
    m_init++;
    //Coordinates spiderPos = m_skeleton.AbsPos();

    string learn = "n/a";
    bool bRetrieve = true;

    bool bAction = false;
    int skip = 16;
    if (m_frame % skip == 0)
      bAction = true;
    //bAction = true;

    if (m_out.isize() == 0) {
      svec<double> features;
      m_skeleton.MakeFeatureVector(features, m_time-m_lastTime);
      m_ctrl.Retrieve(m_out, features, 0);
      m_lastFeatures.resize(features.isize(), 0.);
    }

    //double reward = 0.;
    double moveWeight = skip;
    if (bAction) {
    
      svec<double> features;
      m_skeleton.MakeFeatureVector(features, m_time-m_lastTime);
      //features[0] = sin(m_time*3.14*2);
      //features[1] = cos(m_time*3.14*2);
      double mm = 0.;
      for (int z=0; z<features.isize(); z++) {
	mm += (features[z] - m_lastFeatures[z])*(features[z] - m_lastFeatures[z]);
      }
      mm = sqrt(mm)/(double)features.isize()-0.2;
	
      cout << "Award " << m_reward << " " << mm << endl;
      m_lastFeatures = features;
      
      //m_reward += mm;
      m_ctrl.Retrieve(m_out, features, m_reward);
      m_ctrl.Print();
    
      //cout << "Features" << endl;
      //for (i=0; i<features.isize(); i++)
      //	cout << i << ": " << features[i] << endl;
      bool bGood = true;
      for (i=0; i<m_skeleton.GetNerves().isize(); i++) {
	double data = m_out[i];
	bool b = m_skeleton.Move(i, data*deltatime*moveWeight);
	if (!b)
	  bGood = false;
      }
      if (!bGood)
	m_ctrl.UnSuccess();


      m_lastTime = m_time;
      
      //m_ctrl.LearnAvoid(1.);
      //m_lastDiff = spiderPos.Length();
      //m_lastDiff = m_camPos.Length();
      SuccessFeature fs;
      //m_reward = (spiderPos - m_camPos).Length() - (m_lastPos - m_camPos).Length();
      m_lastDiff = (m_lastPos - m_lastCamPos).Length();
      m_currDiff = (spiderPos - m_camPos).Length();
      m_reward = m_currDiff - m_lastDiff;
      m_reward /= 5.;
      /* if (m_reward > 1.)
	m_reward = 1.;
      if (m_reward < -1.)
	m_reward = -1.;
      */
      
      //m_reward = m_camPos.Length();
      //if (!bGood)
      //m_reward = -1.;
      fs.resize(1);
      fs[0] = m_reward;
      
      m_ctrl.SetSuccess(fs);

      m_ctrl.LearnOrAvoid();  
	
      cout << "SPIDER " << m_currDiff << " " << m_lastDiff << endl;
      spiderPos.Print();      
      m_lastPos.Print();
      m_camPos.Print();
      m_lastCamPos.Print();
      
      m_lastPos = spiderPos;
      m_lastCamPos = m_camPos;
 
    } else {
      for (i=0; i<m_skeleton.GetNerves().isize(); i++) {
	double data = m_out[i];
	double dd = (m_skeleton.GetNerves())[i].GetMove();
	//double back = -dd*0.05;
	bool b = m_skeleton.Move(i, data*deltatime*moveWeight);
	//m_skeleton.Move(i, back*deltatime);
      }
    }

    m_skeleton.Move(m_index, x);
    
    m_skeleton.Update(deltatime);


    
     
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
      sprintf(tmptmp, "%2.3f ... %s\n", m_out[i], m_skeleton.GetNerves()[i].GetName().c_str());
      strcat(msg, tmptmp);
    } 
    
    if (bAction) {
      cout << "Update lastDiff" << endl;
      m_lastDiff = m_currDiff;
    }
     
    SceneNodeMeshPhysics phys;
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(phys, m_skeleton);
    MsgSceneNode & node = o.MessageSceneNode();

    StreamCoordinates rr = node.GetRotation();
    node.SetRotation(rr+m_skeleton.RelRot());
    //node.SetRotation(rr+Coordinates(0, 1, 0)*deltatime);

    
    
   
    node.SetMessage(msg);
    
    node.Mesh(0) = phys;
    m_lastKey = m_key;
    m_key = "";
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }
  void SetSkeleton(const NPCSkeleton &s) {
    cout << "SetSkeleton start" << endl;
    int i;
    m_skeleton = s;
    const NPCNerveCostume & nerves = m_skeleton.GetNerves();

    m_ctrl.SetNumControls(nerves.isize());
    for (i=0; i<nerves.isize(); i++) {
      m_ctrl[i].SetName(nerves[i].GetName());
      m_ctrl[i].SetIndex(i);
    }
    int size = m_skeleton.GetFeatDim();
    m_ctrl.SetUp(size, 1);
    m_ctrl.SetRange(-2., 2.);
    cout << "SetSkeleton end" << endl;
  }

  //void SetSaveName(const string & s) {
  //  m_save = s;
  //}
private:
  Coordinates m_basePos;
  Coordinates m_center;
  Coordinates m_lastPos;
  Coordinates m_camPos;
  Coordinates m_lastCamPos;
  NPCSkeleton m_skeleton;
  int m_index;
  string m_key;
  string m_lastKey;
  //string m_save;
  int m_frame;
  double m_rot;
  int m_init;
  svec<double> m_out;
  double m_time;
  double m_lastTime;
  //SkeletonControl m_ctrl;
  NPCControl m_ctrl;
  double m_lastDiff;
  double m_currDiff;
  //double m_currDiff;
  double m_reward;
  svec<double> m_lastFeatures;
};


class KeyCtrl : public IGlobal
{
public:
  KeyCtrl(SkeletonManipulator * p) {
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
  SkeletonManipulator * m_pManip;
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
