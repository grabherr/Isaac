
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "npc/SkeletonPhys.h"
#include "npc/NNet.h"



class BugLeg
{
public:
  BugLeg() {}

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

class BugBrain
{
public:
  BugBrain() {
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
  svec<BugLeg> m_legs;
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
  }
  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    //return;
  
    PhysObject & p = m_skeleton.Physics();

    if (m_frame % 1800 == 0) {
      NPCSkeletonWithPhysics skel;
      SetupSkeleton(skel);
      m_skeleton = skel;
      cout << "Resetting skeleton!" << endl;
      m_frame++;
      //m_frame = 0;
      m_trials++;
      m_skeletonTemplate = skel;
      m_lastPos = p.GetCenter().GetPosition();
      m_lastMove = 0.;
     return;
    }
    
 
    int i;

    double dist = 0.;
    for (i=0; i<m_skeleton.isize(); i++) {
      Coordinates a = m_skeleton[i].GetCoords();
      Coordinates b = m_skeletonTemplate[i].GetCoords();
      //dist += (a-b).Length();
      dist += (a[1]-b[1])*(a[1]-b[1]);
    }

    //cout << "DIST: " << dist << " last: " << m_lastDist << endl;

    //int dim = 0;
    //if (m_lastX < 0)
    //m_lastX =  p.GetCenter().GetPosition()[dim];
    
    double move = deltatime * (p.GetCenter().GetPosition() - m_lastPos).Length();
    //m_lastPos = p.GetCenter().GetPosition();


    if (m_trials < 10) {
      if (dist < m_lastDist) {
	cout << "LEARN positive!" << endl;
	m_brain.Learn(m_leg);
	//m_brain.Learn();
      }
      if (dist > m_lastDist) {
	//if (m_absFrame < 5000) {
	cout << "LEARN negative!" << endl;
	m_brain.LearnAvoid(m_leg);
	//m_brain.LearnAvoid();
	//}
      }
    } else {
      cout << "Movement: " << move << endl;
      if (move > m_lastMove) {
	cout << "LEARN positive!" << endl;
	m_brain.Learn(m_leg);
      }
      if (move < m_lastMove) {
	cout << "LEARN negative!" << endl;
	m_brain.LearnAvoid(m_leg);
      }
    }

    m_lastMove = move;
    m_absFrame++;
    m_lastDist = dist;
    m_leg++;
    if (m_leg == 6)
      m_leg = 0;

    svec<double> in;
    in.resize(30, 0.);
    
    /*
    for (i=7; i<=12; i++) {
      //in[i-7] = 1.*m_skeleton[i].GetTouch()[1];
      in[i-7] = .1*m_skeleton[i].GetCoords()[1];
      }*/
    for (i=1; i<=6; i++) {
      //in[i-7] = 1.*m_skeleton[i].GetTouch()[1];
      in[i-1] = .1*m_skeleton[i].GetCoords()[1];
      //cout << "FEED(0): " << in[i-1] << endl;
     }
    
    int k = 6;
    for (i=1; i<=12; i++) {
      Coordinates cc = m_skeleton[i].GetCoords() - m_skeleton[0].GetCoords();
      SphereCoordinates s = cc.AsSphere();
      in[k] = s.phi();
      k++;
      in[k] = s.theta();
      k++;
      //cout << "FEED " << s.phi() << " " << s.theta() << endl;
    }
    
    svec<double> out;
    out.resize(24, 0.);
    m_brain.Retrieve(out, in);
 
    
    for (i=15+m_leg*4; i<15+m_leg*4+4; i++) {
      //for (i=15; i<39; i++) {
      PhysConnection & c =  p.Connection(i);
      double d = c.GetDistance();
      double weight = deltatime*12.;
      //weight = .5;
      double dn = out[i-15]*weight;
      //cout << "Dist: " << d << " new: " << d*(1-weight) + dn << endl;

      if (i == 15+m_leg*4+3) {
	dn /= 10.;
	dn = 250*deltatime;
	//cout << "VELOCITY " << i << " " << dn << endl;
	PhysMinimal & mm = p[m_leg+7];
	mm.Velocity()[1] -= dn;
	PhysMinimal & mm1 = p[m_leg+1];
	mm1.Velocity()[1] += dn;

	continue;
      }
      //dn = 1.;

      //if (m_trials >= 0 || 15+3*m_leg == i || 15+3*m_leg+1 == i || 15+3*m_leg+2 == i) {
      c.SetDistance(d*(1-weight) + dn);
    }




    /*
    double extra = 1.;
    NPCIO now = m_io;
 
    double speed = p.GetCenter().GetVelocity().Length()*deltatime;
    int dim = 0;
    if (m_lastX < 0)
      m_lastX =  p.GetCenter().GetPosition()[dim];
    
    speed = p.GetCenter().GetPosition()[dim] - m_lastX;
    m_lastX =  p.GetCenter().GetPosition()[dim];
    speed /= deltatime;
 
    if (m_frame < 1) {
      // Keep still in the beginning
      for (i=15; i<32; i++) {
	PhysConnection & c =  p.Connection(i);
	double d = c.GetDistance();
	m_iol[i-14] = d;
	m_iol[i-14+18] = d;
      }
      for (int j=0; j<1; j++)
	m_nn.Learn(m_iol, 1.);
      m_nn.Print();
    } else {

      for (i=14; i<32; i++) {
	PhysConnection & c =  p.Connection(i);
	double d = c.GetDistance();
	now[i-14] = d;
      }
      
      
      now.SetElement("speed", speed);
      
      
      if (speed > 0) {
	m_nn.Learn(m_iol, .1);
	cout << "LEARN Success, speed = " << speed << " " << p.GetCenter().GetPosition()[dim] << endl;
      } else {
	m_nn.LearnAvoid(m_iol, .1);
	cout << "LEARN Avoid, speed = " << speed << " " << p.GetCenter().GetPosition()[dim] << endl;
      }
      
      
      m_iol = now;
      cout << "+++++++++++++++++++ Retrieve +++++++++++++++++++++++++" << endl;
      m_nn.Print();
      m_nn.Retrieve(now);
      //int k = 0;
      //for (i=14; i<32; i++) {
      
      for (i=14; i<32; i++) {
	PhysConnection & c =  p.Connection(i);
	double d = c.GetDistance();
	double dn = now[18+i-14];
	cout << "Dist: " << d << " new: " << dn << endl;
	if (m_trials >= 0 || 14+3*m_leg == i || 14+3*m_leg+1 == i || 14+3*m_leg+2 == i) {
	  //if (14+3*m_leg == i) {
	  c.SetDistance(now[18+i-14]);
	  //c.SetDistance(c.GetDistance()+now[18+i-14]*deltatime);
	  //c.SetDistance(0);
	}
	m_iol[18+i-14] = now[18+i-14];
      }
    }

 
    m_lastSpeed = speed;
    */

    m_frame++;
 
    m_skeleton.UpdateAndSync(deltatime);

  
    int offset = 15;

    double factor = 20.;
    
    double plus = -deltatime*0.5;
 
   

    m_skeleton.UpdateToPhys(deltatime);
    

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

    /*
    //In
    m_nn.Setup(200, 2*ranges.isize()+1);
    for (int i=0; i<ranges.isize(); i++) {
      m_nn.ReSetup(i, ranges[i]/2, 3*ranges[i]/2);
      char leg[512];
      sprintf(leg, "leg%d", i);
      m_io.AddElement(leg);
      m_io.SetValid(i, true);
      m_iol.AddElement(leg);
      m_iol.SetValid(i, true);
    }

    //Out
    for (int i=0; i<ranges.isize(); i++) {
      m_nn.ReSetup(ranges.isize()+i, ranges[i]/2, 3*ranges[i]/2);
      // m_nn.ReSetup(ranges.isize()+i, -10, +10);
      char leg[512];
      sprintf(leg, "leg%d", i);
      m_io.AddElement(leg);
      m_io.SetValid(i, false);
      m_iol.AddElement(leg);
      m_iol.SetValid(i, true);
    }


    m_io.AddElement("speed");    
    m_iol.AddElement("speed");    
    */

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

  BugBrain m_brain;

  double m_lastDist;
  double m_lastMove;
  int m_absFrame;
};


//=======================================================================
void SetupSkeleton(NPCSkeletonWithPhysics & s)
{
  int i;
  NPCBone aa;
  aa.SetRelCoords(NPCBoneCoords(0, 0, 0, 0)); // Root/butt
  s.AddBone(aa);

  
  aa.SetRelCoords(NPCBoneCoords(2., -0., 0., 0.)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(2., 0., PI_P/4, 0.)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(2., 0, PI_P/2, 0.)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(2., -0., PI_P, 0.)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(2., -0., PI_P+PI_P/4, 0.)); 
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(2., 0, 3*PI_P/2, 0.)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(1.5, 0, 0, -PI_P/2)); 
  s.AddBone(aa);
  s.AddBone(aa);
  s.AddBone(aa);
  s.AddBone(aa);
  s.AddBone(aa);
  s.AddBone(aa);

  // Two extra
  aa.SetRelCoords(NPCBoneCoords(0.9, 0, 0, PI_P/2)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(0.9, 0, -PI_P/4, 0)); 
  s.AddBone(aa);

  aa.SetRelCoords(NPCBoneCoords(0.9, 0, PI_P-PI_P/4, 0)); 
  s.AddBone(aa);

  /*
  aa.SetRelCoords(NPCBoneCoords(2., 0, 0, -PI_P/2-0.1)); 
  s.AddBone(aa);
  s.AddBone(aa);
  aa.SetRelCoords(NPCBoneCoords(2., 0, 0, -PI_P/2+0.1)); 
  s.AddBone(aa);
  s.AddBone(aa);
  */

  

  s.SetParentChild(0, 1);
  s.SetParentChild(0, 2);
  s.SetParentChild(0, 3);
  s.SetParentChild(0, 4);
  s.SetParentChild(0, 5);
  s.SetParentChild(0, 6);

  s.SetParentChild(1, 7);
  s.SetParentChild(2, 8);
  s.SetParentChild(3, 9);
  s.SetParentChild(4, 10);
  s.SetParentChild(5, 11);
  s.SetParentChild(6, 12);
 
  s.SetParentChild(0, 13);
  s.SetParentChild(0, 14);
  s.SetParentChild(0, 15);


  s.Scale(10.);
  s.SetAbsOffset(Coordinates(0., 60., 0));
  s.SetFloorY(0.);

 
 

  s.SetupPhysics();

  
  PhysObject & phys = s.Physics();
  double elast = 0.01;
  /*
  phys.Connect(PhysConnection(1, 2, elast));
  phys.Connect(PhysConnection(2, 3, elast));
  phys.Connect(PhysConnection(3, 4, elast));
  phys.Connect(PhysConnection(4, 5, elast));
  phys.Connect(PhysConnection(5, 6, elast));
  phys.Connect(PhysConnection(6, 1, elast));

  phys.Connect(PhysConnection(7, 8, elast));
  phys.Connect(PhysConnection(8, 9, elast));
  phys.Connect(PhysConnection(9, 10, elast));
  phys.Connect(PhysConnection(10, 11, elast));
  phys.Connect(PhysConnection(11, 12, elast));
  phys.Connect(PhysConnection(12, 8, elast));

  phys.Connect(PhysConnection(7, 0, elast));
  phys.Connect(PhysConnection(8, 0, elast));
  phys.Connect(PhysConnection(9, 0, elast));
  phys.Connect(PhysConnection(10, 0, elast));
  phys.Connect(PhysConnection(11, 0, elast));
  phys.Connect(PhysConnection(12, 0, elast));
  */

  cout << "Hidden connections from " <<  phys.GetConnectCount() << endl;

  phys.Connect(PhysConnection(13, 1, elast));
  phys.Connect(PhysConnection(14, 1, elast));
  phys.Connect(PhysConnection(0, 7, elast));
  phys.Connect(PhysConnection(13, 7, elast));

  phys.Connect(PhysConnection(13, 2, elast));
  phys.Connect(PhysConnection(14, 2, elast));
  phys.Connect(PhysConnection(0, 8, elast));
  phys.Connect(PhysConnection(13, 8, elast));

  phys.Connect(PhysConnection(13, 3, elast));
  phys.Connect(PhysConnection(14, 3, elast));
  phys.Connect(PhysConnection(0, 9, elast));
  phys.Connect(PhysConnection(13, 9, elast));

  phys.Connect(PhysConnection(13, 4, elast));
  phys.Connect(PhysConnection(14, 4, elast));
  phys.Connect(PhysConnection(0, 10, elast));
  phys.Connect(PhysConnection(13, 10, elast));

  phys.Connect(PhysConnection(13, 5, elast));
  phys.Connect(PhysConnection(14, 5, elast));
  phys.Connect(PhysConnection(0, 11, elast));
  phys.Connect(PhysConnection(13, 11, elast));

  phys.Connect(PhysConnection(13, 6, elast));
  phys.Connect(PhysConnection(14, 6, elast));
  phys.Connect(PhysConnection(0, 12, elast));
  phys.Connect(PhysConnection(13, 12, elast));

 
  cout << "Hidden connections to " <<  phys.GetConnectCount() << endl;
  phys.Connect(PhysConnection(13, 14, elast));

  phys.Connect(PhysConnection(13, 15, elast));
  phys.Connect(PhysConnection(14, 15, elast));

  /*
  elast = 0.8;
  phys.Connect(PhysConnection(1, 2, elast));
  phys.Connect(PhysConnection(2, 3, elast));
  phys.Connect(PhysConnection(3, 4, elast));
  phys.Connect(PhysConnection(4, 5, elast));
  phys.Connect(PhysConnection(5, 6, elast));
  phys.Connect(PhysConnection(6, 1, elast));

  phys.Connect(PhysConnection(7, 8, elast));
  phys.Connect(PhysConnection(8, 9, elast));
  phys.Connect(PhysConnection(9, 10, elast));
  phys.Connect(PhysConnection(10, 11, elast));
  phys.Connect(PhysConnection(11, 12, elast));
  phys.Connect(PhysConnection(12, 7, elast));
  */


  phys.Fixate();
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


  svec<double> range;
  for (i=15; i<15+6*4; i++) {
    range.push_back(s.GetPhysics().GetConnection(i).GetDistance());
    cout << "Got " << s.GetPhysics().GetConnection(i).GetDistance() << " " << range[range.isize()-1] << endl;
  }


  manip2.SetupNN(range);

  manip2.SetSkeleton(s);
 
  // Add it!
  eng.AddSceneNode(node, &manip2);


  eng.Run();


  return 0;
}
