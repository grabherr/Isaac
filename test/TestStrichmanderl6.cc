
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









void SetupSkeleton(NPCSkeletonWithPhysics & s);

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
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
  }
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    //return;
  
    PhysObject & p = m_skeleton.Physics();
    PhysMinimal & middle = p.GetCenterDirect();

    //GetName
    MsgSceneNode & node1 = o.MessageSceneNode();
    const string & name = node1.GetName();
    cout << "Updating object " << name << endl;
    if (name == "attract1") {
      PhysObject & pp = o.GetPhysObject();
      PhysMinimal & mm = pp.GetCenterDirect();
      cout << "To position ";
      m_a1.Print();
      mm.SetPosition(m_a1);
      return;
    }
    if (name == "attract2") {
      PhysObject & pp = o.GetPhysObject();
      PhysMinimal & mm = pp.GetCenterDirect();
      cout << "To position ";
      m_a2.Print();
      mm.SetPosition(m_a2);
      return;
    }

    /*
    m_a1 = middle.GetPosition() + p.Attractor(0).GetPosition();
    m_a2 = middle.GetPosition() + p.Attractor(1).GetPosition();

    cout << "Attractor pos "; 
    p.Attractor(0).GetPosition().Print();
    cout << "Middle pos ";
    middle.GetPosition().Print();

    //m_a1 *= node1.GetScale();
    //m_a2 *= node1.GetScale();
    cout << "+++++++++ Scale " << node1.GetScale() << endl;
    */

    m_a1 = node1.GetPosition()/20 + /*middle.GetPosition() +*/ p.Attractor(0).GetPosition()*5/4;
    m_a2 = node1.GetPosition()/20 + /*middle.GetPosition() +*/ p.Attractor(1).GetPosition()*5/4;

    cout << "Printing attractors" << endl;
    p.Attractor(0).GetPosition().Print();
    p.Attractor(1).GetPosition().Print();
    cout << "Center: ";
    p.GetCenter().Print();
    cout << "Printing bones" << endl;
    for (int i=0; i<p.isize(); i++) {
      p[i].GetPosition().Print();
    }

    // m_a1[1] += 10.;
    //m_a2[1] += 10.;
    //return;

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
  Coordinates m_a1;
  Coordinates m_a2;

};


//=======================================================================
void SetupSkeleton(NPCSkeletonWithPhysics & s)
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
  aa.SetRelCoords(NPCBoneCoords(0.2, 0., PI_P/2, 0.)); 
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

 
  s.Scale(10.);
  s.SetAbsOffset(Coordinates(0., 60., 0));
  s.SetFloorY(0.);

  
  s.SetupPhysics();

  PhysObject & phys = s.Physics();

  phys.Connect(PhysConnection(0, 10, 0.01));
  phys.Fixate();

  cout << "After fixate, center is ";
  phys.GetCenter().GetPosition().Print();
  cout << "Print all coordinates:" << endl;
  for (i=0; i<phys.isize(); i++)
    phys[i].GetPosition().Print();

  

  PhysAttractor at;
  at.SetIndex(2);
  at.Position() = Coordinates(5.52637, 8.13247, 2.50331);
  at.AddAttach(0);
  at.AddAttach(5);
  at.AddAttach(10);
  at.SetActive(true);
  at.SetForce(15.);
  at.SetBack(15.);
  phys.AddAttractor(at);

  at.Position() = Coordinates(2.12289e-16, 10.469, 2.50331);
  at.SetIndex(9);
  at.SetForce(0.);
  at.SetBack(0.);
  at.SetActive(false);
  phys.AddAttractor(at);

  
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
  
  NPCSkeletonWithPhysics s;
  SetupSkeleton(s);
  
  /*
  const PhysObject & phys = s.GetPhysics();     
  cout << "After SetupSkeleton, center is ";
  phys.GetCenter().GetPosition().Print();
  cout << "Print all coordinates:" << endl;
  for (int i=0; i<phys.isize(); i++)
    phys[i].GetPosition().Print();
  */

  /*
  svec<double> ranges;
  for (i=15; i<33; i++) {
    const PhysConnection & c =  s.GetPhysics().GetConnection(i);
    double d = c.GetDistance();
    ranges.push_back(d);
    }*/



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



  MsgSceneNode node2;
  node2.SetName("attract1");
  node2.Material(0).SetTexture("data/Models/green.jpg");
  node2.SetModel("data/Models/ball.ms3d");
  node2.SetPosition(StreamCoordinates(5325, 2061.7, 4894.79));

  // No physics
  node2.SetPhysMode(2);
  node2.SetScale(2.);
  node2.SetRequestLoopBack(true);
  eng.AddSceneNode(node2, &manip2);
  node2.SetName("attract2");
  eng.AddSceneNode(node2, &manip2);


  eng.Run();


  return 0;
}
