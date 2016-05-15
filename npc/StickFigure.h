#ifndef STICKFIGURE_H
#define STICKFIGURE_H

#include "npc/BodyParts.h"
#include "npc/SkeletonPhys.h"
#include "engine/IManipulator.h"
#include "graphics/Messages.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"

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

 
  void AddAttractor(const AttractInfo & f) {
    m_info.push_back(f);
  }

  int GetAttractorCount() const {return m_info.isize();}
  const AttractInfo & GetAttractor(int i ) const {return m_info[i];}
 
protected:
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


  double m_lastDist;
  double m_lastMove;
  int m_absFrame;
  double m_sign;
  bool m_start;
 
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

void SetupSkeleton(NPCSkeletonWithPhysics & s, StrichManipulator & manip);


#endif

