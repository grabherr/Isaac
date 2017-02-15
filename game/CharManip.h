#ifndef CHARMANIP_H
#define CHARMANIP_H

#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/Skeleton.h"
#include "npc/TopLevel.h"
#include "base/SVector.h"
#include "game/SchoolLogic.h"
#include "game/Character.h"
#include "util/mutil.h"

class CharMovement
{
 public:
  CharMovement() {
    m_state = 3;
    m_time = 0;
    m_temp = 0;
    m_lastVal = 0.;
    m_speed = RandomFloat(1.0);

  }

  void MoveSkeleton(NPCSkeleton &s, double deltatime);
  
  void SetState(int s) {
    if (s != m_lastSet)
      m_state = s;
    m_lastSet = s;
  }
  
 private:
  int m_state;
  int m_lastSet;
  double m_time;
  double m_temp;
  double m_lastVal;
  double m_speed;

};



class CharManipulator : public IManipulator
{
public:
  CharManipulator() {
    m_index = 0;
    m_frame = 0;
    m_rot = 0;
    m_time = 0;
    m_lastVal = 0.;
    m_currRot = 0.;
    m_top.resize(1, 1, 1, 30);
    m_score = 0.;
    m_status = 0;
    m_headPlus = 6.8;
    m_tagMe = false;
    m_tAct = 0.;
  }
  virtual ~CharManipulator() {}

  void SetHeadPlus(double d) {
    m_headPlus = d;
  }

  bool AmITagged() const {return m_tagMe;}
  void SetTagged(bool b) {m_tagMe = b;}
  
  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  void SetKey(const string & key) {
    m_key = key;
  }
  
  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime);
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

  void SetItemPos(const Coordinates & c) {
    m_itemPos = c;
  }

  const string & Name() const {return m_name;}
  string & Name()  {return m_name;}

  svec<double> & Properties() {return m_properties;}
  const svec<double> & Properties() const {return m_properties;}
  void SetTargetInfo(const string & name, double act) {
    m_tName = name;
    m_tAct = act;
  }
private:
  double GetMilkScore(double & input,
		      const Coordinates & oldPos,
		      const Coordinates & realPos,
		      const Coordinates & target,
		      double currRot);
  
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
  double m_time;
  svec<double> m_moves;
  double m_lastVal;
  double m_currRot;
  Coordinates m_lastCheck;
  Coordinates m_realPos;
  TopLevel m_top;
  Coordinates m_itemPos;
  double m_score;
  Coordinates m_lastRelPos;
  double m_thinkTime;
  int m_status;
  CharMovement m_movement;
  double m_headPlus;
  bool m_tagMe;
  string m_name;
  svec<double> m_properties;
  string m_tName;
  double m_tAct;

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


class ItemManipulator : public IManipulator
{
public:
  ItemManipulator() {}
  virtual ~ItemManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    //MsgSceneNode & n = o.MessageSceneNode();
    m_pos = m.GetPosition();
 
  
  }
  const Coordinates & GetPos() const {return m_pos;}
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }

private:
  Coordinates m_pos;
};

class CharGlobCtrl : public IGlobal
{
public:
  CharGlobCtrl() {
    m_focus = 0;
  }

  void AddFigure(CharManipulator * p, HeadManipulator * pHead) {
    m_pManip.push_back(p);
    if (m_pManip.isize() == m_focus+1)
      p->SetTagged(true);
    m_pHead.push_back(pHead);
    m_pos.push_back(Coordinates());
    m_rot.push_back(Coordinates());
    m_map.push_back(-1);

    SchoolCharacter s;

    s.SetName(p->Name());
    s.SetStrength(0.5);
    s.SetAttract(0.5);
    s.SetGender(0.8);
    s.Properties() = p->Properties();
    m_logic.push_back(s);
    
    svec<double> test;
    m_logic[0].AsVec(test);

    Character cc;
    cc.SetupPeople(test.isize(), m_logic.isize()-1);
    cc.SetName(p->Name());
    cc.SetCoords(p->HeadPos());
    m_characters.push_back(cc);

  }

  void AddItem(ItemManipulator * pItem) {   
    m_pItem.push_back(pItem);
  }
  
  virtual void StartFrame(double deltatime) {
    m_logic.BeginRound(); 
  }
  
  virtual void EndFrame(double deltatime) {
    int i, j;
    m_time += deltatime;
    for (int i=0; i<m_pManip.isize(); i++) {
      m_pHead[i]->SetCoords(m_pManip[i]->HeadPos(), m_pManip[i]->HeadRot());
      m_pos[i] = m_pManip[i]->HeadPos();
      m_rot[i] = m_pManip[i]->HeadRot();
      m_characters[i].SetCoords(m_pManip[i]->HeadPos());

      /*
      if (m_map[i] < 0) {
	for (j=0; j<m_pItem.isize(); j++) {
	  m_pManip[i]->SetItemPos(m_pItem[j]->GetPos());
	}
      } else {
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_pManip[i]->SetItemPos(m_pManip[m_map[i]]->HeadPos());
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
      if (RandomFloat(1.) < 0.005) {
	int nn = RandomInt(m_map.isize());
	if (nn != i)
	  m_map[i] = nn;
      }*/
   
    }
    //if (m_time < 30.)
    //return;
    
    for (i=0; i<m_logic.isize(); i++) {
      svec<double> input;
      m_logic[i].AsVec(input);
      for (j=0; j<m_logic.isize(); j++) {
	m_characters[j].FeedNeutral(input, m_characters[i].GetCoords(), i);
      }
      //m_logic[i].Print();
    }

      
    // Move them
    for (i=0; i<m_logic.isize(); i++) {
      int des = m_characters[i].GetDesire();
      int avoid = m_characters[i].GetAvoid();
      double act = m_characters[i].GetAct();
      svec<double> input;
      m_logic[i].AsVec(input);
      Coordinates self = m_characters[i].GetCoords();
      
      cout << "Process " << m_logic[i].GetName() << endl;
      cout << "Desire " << des << " Avoid " << avoid << " Act " << act << " Coords ";
      self.Print();

      string tName = "<none>";
      if (des >= 0)
	tName = m_pManip[des]->Name();
      
      m_pManip[i]->SetTargetInfo(tName, act);
      
      if (des >= 0) {
	//const Coordinates & other = m_characters[des].GetCoords();
	//self += (other - self).Einheitsvector()*0.5;
	//m_characters[i].SetCoords(self);
	
	//==============================================
	//==============================================
	//==============================================
	m_pManip[i]->SetItemPos(m_pManip[des]->HeadPos());
	
	// Interact here!!!!!
	if ((m_pManip[i]->HeadPos() - m_pManip[des]->HeadPos()).Length() < 25.) {
	  cout << "INTERACTION " << i << " <-> " << des << " act " << act << endl;
	  svec<double> input_other;
	  m_logic[des].AsVec(input_other);
	  
	  m_logic[i].SetTarget(des);
	  m_logic[i].SetInteract(act);
	  m_characters[des].FeedAction(input, act);
	  m_characters[i].FeedDone(input, act);
	}
      }
      //if (avoid >= 0) {
      //const Coordinates & other = m_characters[avoid].GetCoords();
      //self -= (other - self).Einheitsvector()*0.3;
      //m_characters[i].SetCoords(self);	
      //}      
    }
      
    m_logic.EndRound();
    
    for (i=0; i<m_logic.isize(); i++) {
      m_characters[i].SetScore(m_logic[i].GetStrength());
      m_characters[i].Print();
    }
    
    
    
        
  
  }

  virtual void KeyPressed(const string & s) {
    //cout << "Got message Key pressed: " << s << endl;
    //for (int i=0; i<m_pManip.isize(); i++)
    //m_pManip[i]->SetKey(s);
    if (s == m_lastKey)
      return;
    if (s == "TAB") {
      m_focus++;
      if (m_focus >= m_pManip.isize())
	m_focus = 0;
    }
    if (s == "BACK") {
      m_focus--;
      if (m_focus < 0)
	m_focus = m_pManip.isize()-1;
    }
    m_lastKey = s;
    for (int i=0; i<m_pManip.isize(); i++) {
      m_pManip[i]->SetTagged(false);
      if (i == m_focus)
	m_pManip[i]->SetTagged(true);
    }
    
  }
  
  void Read(CMReadFileStream & s) {
    int n;
    s.Read(n);
    m_characters.resize(n);
    for (int i=0; i<m_characters.isize(); i++)
      m_characters[i].Read(s);
  }
  void Write(CMWriteFileStream & s) {
    s.Write(m_characters.isize());
    for (int i=0; i<m_characters.isize(); i++)
      m_characters[i].Write(s);
  }

  int isize() const {return m_characters.isize();}
  Character & operator[] (int i) {return m_characters[i];}
  const Character & operator[] (int i) const {return m_characters[i];}
private:
  svec<CharManipulator*> m_pManip;
  svec<HeadManipulator*> m_pHead;
  svec<ItemManipulator*> m_pItem;
  svec<Coordinates> m_pos;
  svec<Coordinates> m_rot;
  svec<int> m_map;
  int m_focus;
  string m_lastKey;
  SchoolLogic m_logic;
  svec<Character> m_characters;

  double m_time;
  
};



#endif //CHARMANIP_H
