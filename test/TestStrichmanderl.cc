
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "npc/SkeletonPhys.h"
#include "npc/NNet.h"
#include "npc/StickFigure.h"



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
