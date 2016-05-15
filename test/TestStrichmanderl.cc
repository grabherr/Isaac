
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "npc/SkeletonPhys.h"
#include "npc/NNet.h"



class SMLeg
{
public:
  SMLeg() {}

  void SetUp(int in, int out) {
    m_nn.Setup(40, in + out);
    m_off = in;
    m_last.resize(in+out);
  }
  void SetRange(int i, double min, double max) {
    m_nn.ReSetup(i, min, max);
  }

  void Retrieve(svec<double> & out, const svec<double> & in) {
    NPCIO tmp;
    tmp.resize(m_last.isize());
    int i;
    for (i=0; i<in.isize(); i++) {
      tmp[i] = in[i];
      tmp.SetValid(i, true);
    }
    for (i=0; i<out.isize(); i++) {
      tmp[i+in.isize()] = in[i];
      tmp.SetValid(i+in.isize(), false);
    }
    m_nn.Retrieve(tmp);
    m_last = tmp;
    for (i=0; i<in.isize(); i++) {
      m_last.SetValid(i, true);
    }
    for (i=in.isize(); i<tmp.isize(); i++) {
      out[i-in.isize()] = tmp[i];
      m_last.SetValid(i, true);
    }

    //m_nn.Print();
  }

  void Print() const {
    m_nn.Print();
  }
  void Learn() {
    m_nn.Learn(m_last, 0.5);
    //m_nn.LearnButPreserve(m_last);


  }

  void LearnAvoid() {
    m_nn.LearnAvoid(m_last, 0.1);
  }

private:
  NeuralNetwork m_nn;
  int m_off;
  NPCIO m_last;

};

class SMBrain
{
public:
  SMBrain() {
    m_legs.resize(6);
    m_out = 0;
  }
  void SetUp(int in, int out) {
    int i;
    for (i=0; i<m_legs.isize(); i++)
      m_legs[i].SetUp(in, out);

    m_out = out;
  }

   void SetRange(int index, int i, double min, double max) {
    //int j;
    //for (j=0; j<m_legs.isize(); j++)
    m_legs[index].SetRange(i, min, max);
  }
  void SetRange(int i, double min, double max) {
    int j;
    for (j=0; j<m_legs.isize(); j++)
      m_legs[j].SetRange(i, min, max);
  }

  void Retrieve(svec<double> & out, const svec<double> & in) {
    int i, j;
    int k = 0;
    for (i=0; i<m_legs.isize(); i++) {
      svec<double> tmp;
      tmp.resize(m_out, 0.);    
      m_legs[i].Retrieve(tmp, in);
      
      for (j=0; j<tmp.isize(); j++) {
	out[k] = tmp[j];
	k++;
      }
    }
    //m_legs[0].Print();
  }

  void Learn() {
    int j;  
    //m_legs[0].Print();
   for (j=0; j<m_legs.isize(); j++)
      m_legs[j].Learn();
   //m_legs[0].Print();
  }

  void LearnAvoid() {
    int j;
    for (j=0; j<m_legs.isize(); j++)
      m_legs[j].LearnAvoid();
  }

  void Learn(int j) {
    m_legs[j].Learn();
  }

  void LearnAvoid(int j) {
    m_legs[j].LearnAvoid();
  }



private:
  svec<SMLeg> m_legs;
  int m_out;
};




class AttractInfo
{
public:
  AttractInfo() {
    m_index = -1;
  }

  const string & GetName() const {return m_name;}
  const Coordinates &GetCoords() const {return m_coords;}
  double GetLen() const {return m_len;}
  double GetPull() const {return m_pull;}
  int GetIndex() const {return m_index;}

  void SetName(const string & s) {m_name = s;}
  Coordinates & Coords() {return m_coords;}
  void SetLen(double d) {m_len = d;}
  void SetPull(double d) {m_pull = d;}
  void SetIndex(int i) {m_index = i;}

private:
  string m_name;
  Coordinates m_coords;
  double m_len;
  double m_pull;
  int m_index;
};




void SetupSkeleton(NPCSkeletonWithPhysics & s);
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
class StrichManipulator : public IManipulator
{
public:
  StrichManipulator() {
    m_dist = -1;
    m_plus = -35;
    m_lastSpeed = 0.;
    m_lastX = -1;
    m_frame = 0;
    m_leg = 0;
    m_trials = 0;
    m_lastDist = 0.;
    m_absFrame = 0;
    m_lastMove = 0;
    m_sign = 1.;
    m_start = false;
    m_elapsed = 0.;
    m_elapsed_total = 0.;
    m_dir = 1;
  }
  virtual ~StrichManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    //return;
  
    int i;
    PhysObject & p = m_skeleton.Physics();
    PhysMinimal & middle = p.GetCenterDirect();

  
    //GetName
    MsgSceneNode & node1 = o.MessageSceneNode();
    const string & name = node1.GetName();
    cout << "Updating object " << name << endl;

    int index = GetInfo(name);
    if (index != -1) {
      const AttractInfo & f = m_info[index];
      PhysObject & pp = o.GetPhysObject();
      PhysMinimal & mm = pp.GetCenterDirect();      
      mm.SetPosition(f.GetCoords());
      return;
    }

    PhysObject & pp1 = o.GetPhysObject();
    PhysMinimal & mm1 = pp1.GetCenterDirect();      
    //mm1.SetPosition(mm1.GetPosition() + Coordinates(0, 0, -deltatime*27));

    //pp1.SetEngRotation(Coordinates(0, m_elapsed_total, 0));
  
    m_elapsed += deltatime;
    m_elapsed_total += deltatime;

    if (m_elapsed > 0.3) {
      m_dir = -m_dir;
      m_elapsed = 0.;
      cout << "SWITCH TO " << m_dir << endl;
    }

    for (i=0; i<p.AttractorCount(); i++) {      
      Coordinates cc = node1.GetPosition()/20 + p.Attractor(i).GetPosition()*5/4;
      AttractInfo & f = m_info[i];
      f.Coords() = cc;
      f.SetLen(p.Attractor(i).GetLen());
    }

    cout << "TOTAL ELAPSED " << m_elapsed_total << endl;
    if (m_elapsed_total > 1.6) {
      cout << "ACTIVE" << endl;
      p.Attractor(10).SetActive(true);
    }
    if (m_elapsed_total > 4.6) {    
      p.Attractor(11).SetActive(true);
    }
    if (m_elapsed_total > 8.) {
      p.Attractor(10).SetActive(false);
    }

    if (m_elapsed_total > 6.6) {
      p.Attractor(11).SetActive(false);
    }
    if (m_dir == 1) {
      p.Attractor(0).SetActive(true);
      p.Attractor(1).SetActive(true);
      p.Attractor(2).SetActive(false);
      p.Attractor(3).SetActive(false);

      p.Attractor(4).SetActive(false);
      p.Attractor(5).SetActive(false);
      p.Attractor(6).SetActive(true);
      p.Attractor(7).SetActive(true);
    } else {

      p.Attractor(0).SetActive(false);
      p.Attractor(1).SetActive(false);
      p.Attractor(2).SetActive(true);
      p.Attractor(3).SetActive(true);
 
      p.Attractor(4).SetActive(true);
      p.Attractor(5).SetActive(true);
      p.Attractor(6).SetActive(false);
      p.Attractor(7).SetActive(false);
   }
      //p.Attractor(i).SetForce(f.GetPull());
      

      //f.SetPull(
      // DATA EXCHANGE HERE
    

    //m_a1 = node1.GetPosition()/20 + /*middle.GetPosition() +*/ p.Attractor(0).GetPosition()*5/4;
    //m_a2 = node1.GetPosition()/20 + /*middle.GetPosition() +*/ p.Attractor(1).GetPosition()*5/4;

    /*
    cout << "Printing attractors" << endl;
    p.Attractor(0).GetPosition().Print();
    p.Attractor(1).GetPosition().Print();
    cout << "Center: ";
    p.GetCenter().Print();
    cout << "Printing bones" << endl;
    for (int i=0; i<p.isize(); i++) {
      p[i].GetPosition().Print();
    }
    */
   
    cout << "Doing skeletton stuff." << endl;
  
    m_frame++;
 
    m_skeleton.UpdateAndSync(deltatime);

  
    int offset = 15;

    double factor = 20.;
    
    double plus = -deltatime*0.5;
 
   

    //m_skeleton.UpdateToPhys(deltatime, true);
    

    SceneNodeMeshPhysics phys;
    MSkeleton makeSkeleton;
    makeSkeleton.MakeSkeleton(phys, m_skeleton);
    //phys.SetRotation(Coordinates(0, deltatime, 0));
  

    MsgSceneNode & node = o.MessageSceneNode();
    

    node.Mesh(0) = phys;
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {

 


    return; // Do nothing

  
  }
  void SetSkeleton(const NPCSkeletonWithPhysics &s) { 
    m_skeleton = s;
  }

  void SetupNN(const svec<double> & range) {
    
    m_brain.SetUp(30, 4);
    int i;
    for (i=0; i<6; i++)
      m_brain.SetRange(i, 0, 10.);
    for (i=6; i<30; i++)
      m_brain.SetRange(i, -2*PI_P, 2*PI_P);

    // Differential...?
    int k = 0;
    double slack = 14.;
    cout << "Range, size=" << range.isize() << endl;
    for (int j = 0; j<6; j++) {
      for (i=30; i<34; i++) {
	if (i==33)
	  m_brain.SetRange(j, i, 0, 10);
	else
	  m_brain.SetRange(j, i, 1, 2*range[k]);
	cout << "RANGE " << range[k]-slack << " - " <<  range[k]+slack << " avg " <<  range[k] << endl;
	k++;
      }
    }

     // Optimize speed...?

  
  }

  void AddAttractor(const AttractInfo & f) {
    m_info.push_back(f);
  }

  int GetAttractorCount() const {return m_info.isize();}
  const AttractInfo & GetAttractor(int i ) const {return m_info[i];}
 
private:
  Coordinates m_center;
  Coordinates m_lastPos;
  NPCSkeletonWithPhysics m_skeleton;
  NPCSkeletonWithPhysics m_skeletonTemplate;
  double m_dist;
  double m_plus;

  //NeuralNetwork m_nn;
  //NPCIO m_io;
  //NPCIO m_iol;
  double m_lastSpeed;
  double m_lastX;

  int m_frame;

  int m_leg;
  int m_trials;

  SMBrain m_brain;

  double m_lastDist;
  double m_lastMove;
  int m_absFrame;
  double m_sign;
  bool m_start;
  //Coordinates m_a1;
  //Coordinates m_a2;

  svec<AttractInfo> m_info;

  int GetInfo(const string & name) const {
    for (int i=0; i<m_info.isize(); i++) {
      if (m_info[i].GetName() == name)
	return i;
    }
    return -1;
  }
  double m_elapsed;
  double m_elapsed_total;
  int m_dir;
};


//=======================================================================
void SetupSkeleton(NPCSkeletonWithPhysics & s, StrichManipulator & manip)
{
  int i;
  NPCBone root;
  root.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); // Root/butt
  root.Root() = Coordinates(0, 2, 0);
  s.AddBone(root);

  
  NPCBone aa;

  // Legs 1,2 3,4
  aa.SetRelCoords(NPCBoneCoords(1., 0.3, 0.2, -PI_P/2+0.3)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1., 0., -0.2, -PI_P/2)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(1., 0.3, -0.2+PI_P, -PI_P/2+0.3)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(1., 0., -0.2, -PI_P/2)); 
  s.AddBone(aa);

  // Spine 5
  aa.SetRelCoords(NPCBoneCoords(1., 0., 0, PI_P/2)); 
  s.AddBone(aa);

  
  // Arms 6,7 8,9
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., 0, -0.4)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P/2, 0.)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P, -0.4)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(0.6, 0., PI_P/2, 0.)); 
  s.AddBone(aa);
  
  // Forward
  aa.SetRelCoords(NPCBoneCoords(0.3, 0., PI_P/2, 0.)); 
  s.AddBone(aa);

  
  

  s.SetParentChild(0, 1);
  s.SetParentChild(1, 2);
  s.SetParentChild(0, 3);
  s.SetParentChild(3, 4);
  s.SetParentChild(0, 5);

  s.SetParentChild(5, 6);
  s.SetParentChild(5, 8);
  s.SetParentChild(6, 7);
  s.SetParentChild(8, 9);
  
  s.SetParentChild(5, 10);
  //s.SetParentChild(0, 10);

 
  s.Scale(10.);
  s.SetAbsOffset(Coordinates(0., 60., 0));
  s.SetFloorY(0.);

  
  s.SetupPhysics();

  PhysObject & phys = s.Physics();

  phys.Connect(PhysConnection(0, 10, 0.01));
  //phys.Connect(PhysConnection(5, 10, 0.01));
  phys.Fixate();

  cout << "After fixate, center is ";
  phys.GetCenter().GetPosition().Print();
  cout << "Print all coordinates:" << endl;
  for (i=0; i<phys.isize(); i++)
    phys[i].GetPosition().Print();

  

  PhysAttractor at;
  at.SetIndex(1);
  at.Position() = phys[1].GetPosition();
  at.Position()[2] -= 2.0;
  at.AddAttach(0);
  at.AddAttach(5);
  at.AddAttach(10);
  at.SetActive(false);
  at.SetForce(25.);
  at.SetBack(25.);
  phys.AddAttractor(at);

  at.Position() = phys[2].GetPosition();
  at.Position()[2] -= 2.5;
  at.SetIndex(2);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[1].GetPosition();
  at.Position()[2] += 2.8;
  //at.Position()[1] += 1.8;
  at.SetIndex(1);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[2].GetPosition();
  at.Position()[2] += 4.8;
  at.Position()[1] += 2.8;
  at.SetIndex(2);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  //---------------------------------------------------

  at.SetIndex(3);
  at.Position() = phys[3].GetPosition();
  at.Position()[2] -= 2.0;
  at.AddAttach(0);
  at.AddAttach(5);
  at.AddAttach(10);
  at.SetActive(false);
  at.SetForce(25.);
  // at.SetBack(0.);
  phys.AddAttractor(at);

  at.Position() = phys[4].GetPosition();
  at.Position()[2] -= 2.5;
  at.SetIndex(4);
  at.SetForce(30);
  at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[3].GetPosition();
  at.Position()[2] += 2.8;
  //at.Position()[1] += 1.8;
  at.SetIndex(3);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  at.Position() = phys[4].GetPosition();
  at.Position()[2] += 4.8;
  at.Position()[1] += 2.8;
  at.SetIndex(4);
  at.SetForce(25);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);


  //==================================================
  at.Position() = phys[0].GetPosition();
  at.SetIndex(0);
  at.SetForce(22);
  //at.SetBack(0.);
  at.SetActive(true);
  phys.AddAttractor(at);
  at.Position() = phys[5].GetPosition();
  at.SetIndex(5);
  at.SetForce(52);
  //at.SetBack(0.);
  at.SetActive(true);
  phys.AddAttractor(at);


  at.Position() = phys[5].GetPosition();
  at.Position()[1] += 2.4;
  at.SetIndex(9);
  at.SetForce(10);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);
 
  at.Position() = phys[5].GetPosition();
  at.Position()[2] += 22;
  at.Position()[0] -= 22;
  at.SetIndex(7);
  at.SetForce(20);
  //at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  for (i=0; i<phys.AttractorCount(); i++) {
    char name[256];
    sprintf(name, "attract%d", i);
    AttractInfo info;
    info.SetName(name);
    info.Coords() = phys.Attractor(i).GetPosition();
    info.SetPull(phys.Attractor(i).GetForce());
    info.SetIndex(i);
    manip.AddAttractor(info);
  }
}


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  int i;
  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng("localhost", false);
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);
  
  StrichManipulator manip2;
  NPCSkeletonWithPhysics s;
  SetupSkeleton(s, manip2);
  
 


  MsgSceneNode node;
  node.SetName("skeleton");
  //node.Material(0).SetTexture("data/Models/green.jpg");
  node.Material(0).SetTexture("data/Textures/allblack.jpg");
  //node.SetModel("data/Models/ball.ms3d");
  node.SetPosition(StreamCoordinates(5300, 450, 7900));

  MSkeleton makeSkeleton;
  makeSkeleton.MakeSkeleton(node.Mesh(0), s);

  // No physics
  node.SetPhysMode(1);

  node.SetScale(25.);
  // We want to take control of the object
  node.SetRequestLoopBack(true);
 
  // Enable this if yu want to control the mesh! 
  node.SetRequestMesh(false);


  /*
  svec<double> range;
  for (i=15; i<15+6*4; i++) {
    range.push_back(s.GetPhysics().GetConnection(i).GetDistance());
    cout << "Got " << s.GetPhysics().GetConnection(i).GetDistance() << " " << range[range.isize()-1] << endl;
  }


  manip2.SetupNN(range);*/


  manip2.SetSkeleton(s);
 
  // Add it!
  eng.AddSceneNode(node, &manip2);

  
  for (i=0; i<manip2.GetAttractorCount(); i++) {
    MsgSceneNode node2;
    node2.SetName(manip2.GetAttractor(i).GetName());
    node2.Material(0).SetTexture("data/Models/green.jpg");
    node2.SetModel("data/Models/ball.ms3d");
    //node2.SetPosition(StreamCoordinates(5325, 2061.7, 4894.79));
      
    node2.SetPhysMode(2);
    node2.SetScale(2.);
    node2.SetRequestLoopBack(true);
    eng.AddSceneNode(node2, &manip2);
  }
  
    //node2.SetName("attract1");
    //eng.AddSceneNode(node2, &manip2);


  eng.Run();


  return 0;
}
