#ifndef CHARMANIP_H
#define CHARMANIP_H

#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/Skeleton.h"
#include "npc/TopLevel.h"
#include "base/SVector.h"
#include "game/SchoolLogic.h"
#include "game/Character.h"
#include "game/Building.h"
#include "util/mutil.h"


inline string TextArtPlusMinus(double d) {
  int i;
  int n = 20;
  string s;
  d = (-d+1)/2;
  for (i=0; i<n; i++) {
    double t = (double)i/(double)(n-1);
    if (t >= d)
      s += "+";
    else
      s += "-";
  }
  return s;
}

inline string TextArtPlus(double d) {
  int i;
  int n = 20;
  string s;
  for (i=0; i<n; i++) {
    double t = (double)i/(double)(n-1);
    if (t <= d)
      s += "*";
    else
      s += " ";
  }
  return s;
}


class CharMovement
{
 public:
  CharMovement() {
    m_state = 0;
    m_time = 0;
    m_temp = 0;
    m_lastVal = 0.;
    m_speed = RandomFloat(1.0);

  }

  void MoveSkeleton(NPCSkeleton &s, double deltatime);
  
  void SetState(int s) {
    if (m_state == 0 || m_state == 11) {
      if (s != m_lastSet)
	m_state = s;
      m_lastSet = s;
    }
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
    m_tagTarget = false;
    m_tAct = 0.;
    m_targetID = -1;
    m_gameScore = 0.;
    m_toggle = 1;
    m_pBuilding = NULL;
    m_scream = 0.;
    m_bSound = false;
    m_taggedLast = false;
    m_interimTime = 0.;
    m_bOnOff = 1;
  }
  virtual ~CharManipulator() {}

  void SetHeadPlus(double d) {
    m_headPlus = d;
  }

  void Scream(double d) {
    m_scream = d;
  }
  
  void SetBuilding(SchoolBuilding * p) {
    m_pBuilding = p;
  }
  
  void ToggleCamera() {
    m_toggle *= -1;
  }

  int GetTargetID() const {return m_targetID;}
  
  bool AmITagged() const {return m_tagMe;}
  void SetTagged(bool b) {m_tagMe = b;}
  bool AmITaggedTarget() const {return m_tagTarget;}
  void SetTaggedTarget(bool b) {m_tagTarget = b;}
  
  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    m_camPos = c;
  }

  void SetKey(const string & key) {
    m_key = key;
  }

  void ToggleOnOff() {
    m_bOnOff *= -1;
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
  void SetTargetInfo(const string & name, double act, int target) {
    m_tName = name;
    m_tAct = act;
    m_targetID = target;
  }
  void SetScore(double d) {
    m_gameScore = d;
  }

  void Avoid(const Coordinates & c);
  void SetSummary(const string & s) {
    m_summary = s;
  }
  void SetState(int s) {
    m_movement.SetState(s);
  }

  void SetInterimTarget(const Coordinates & c, double time) {
    m_interimPos = c;
    m_interimTime = time;
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
  bool m_taggedLast;
  bool m_tagTarget;
  string m_name;
  svec<double> m_properties;
  string m_tName;
  double m_tAct;
  int m_targetID;
  double m_gameScore;
  int m_toggle;
  Coordinates m_camPos;
  SchoolBuilding * m_pBuilding;
  Coordinates m_fromGrabPos;

  double m_scream;
  bool m_bSound;
  string m_summary;

  Coordinates m_interimPos;
  double m_interimTime;

  int m_bOnOff;

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
  ItemManipulator() {
    m_counter = 0;
  }
  virtual ~ItemManipulator() {
  }

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    double mass = p.GetTotalMass();
    PhysMinimal & m = p.GetCenterDirect();

    //MsgSceneNode & n = o.MessageSceneNode();
    m_counter++;
    //cout << "Milchcounter: " << m_counter << endl;
    m_pos = m.GetPosition();
    Sound & sound = p.GetSound();
    if (m_sound != "") {
      if (m_counter % 120 == 0) {
	cout << "Adding sound to MilchPackerl!" << endl;
	sound.UpdateAdd(m_name, 
			m_sound,
			m_pos);
      } else {
	sound.UpdateAdd(m_name, 
			"",
			m_pos);
	
      }
    }

  
  }
  const Coordinates & GetPos() const {return m_pos;}
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing

  
  }

  void SetName(const string & name) {
    m_name = name;
  }
  void SetSound(const string & name) {
    m_sound = name;
  }
  
private:
  Coordinates m_pos;
  int m_counter;
  string m_name;
  string m_sound;
};

class CharGlobCtrl : public IGlobal
{
public:
  CharGlobCtrl() {
    m_focus = 0;
    m_arrow = -1;
    m_pTarget = NULL;
  }

  void SetTarget(HeadManipulator * pHead) {
    m_pTarget = pHead;
  }
  
  void AddFigure(CharManipulator * p, HeadManipulator * pHead) {
    p->SetBuilding(&m_building);
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

    m_social.push_back(0);
    m_inter.push_back(0);
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

      m_social[i] = m_logic[i].GetSocialStatus();
      svec<double> input;
      m_logic[i].AsVec(input);
      Coordinates self = m_characters[i].GetCoords();
      
      //cout << "Process " << m_logic[i].GetName() << endl;
      //cout << "Desire " << des << " Avoid " << avoid << " Act " << act << " Coords ";
      self.Print();

      string tName = "<none>";
      if (des >= 0)
	tName = m_pManip[des]->Name();
      
      m_pManip[i]->SetTargetInfo(tName, act, des);

      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if (i < 7) {
	//m_pManip[i]->Avoid(m_pManip[7]->HeadPos());
      }
      
      if (des >= 0) {
	//const Coordinates & other = m_characters[des].GetCoords();
	//self += (other - self).Einheitsvector()*0.5;
	//m_characters[i].SetCoords(self);
	
	//==============================================
	//==============================================
	//==============================================
	m_pManip[i]->SetItemPos(m_pManip[des]->HeadPos());
	
	// Interact here!!!!!
	if ((m_pManip[i]->HeadPos() - m_pManip[des]->HeadPos()).Length() < 20.) {
	  cout << "INTERACTION " << i << " <-> " << des << " act " << act << endl;
	  svec<double> input_other;
	  m_logic[des].AsVec(input_other);
	  
	  m_logic[i].SetTarget(des);
	  m_logic[i].SetInteract(act);
	  m_characters[des].FeedAction(input, act);

	  if (act < 0) {
	    m_pManip[i]->SetState(9);
	    m_pManip[des]->SetState(7);
	    m_pManip[des]->SetInterimTarget(Coordinates(RandomFloat(6000), 0., RandomFloat(6000)), -15*act);
	  } else {
 	    m_pManip[i]->SetState(1);
	    m_pManip[des]->SetState(5);
	  }
 
	  //???????????????????????????????????????????
	  m_pManip[des]->Scream(act);
	  
	  m_characters[i].FeedDone(input, act);
	  m_inter[i] = 0.9*m_inter[i] + 0.1*act;
	}
      }
      //if (avoid >= 0) {
      //const Coordinates & other = m_characters[avoid].GetCoords();
      //self -= (other - self).Einheitsvector()*0.3;
      //m_characters[i].SetCoords(self);	
      //}      
    }
      
    m_logic.EndRound();
    string summary;
    for (i=0; i<m_logic.isize(); i++) {
      //m_characters[i].SetScore(m_logic[i].GetStrength());
      //m_pManip[i]->SetScore(m_logic[i].GetStrength());
      m_characters[i].SetScore(m_logic[i].GetSocialStatus());
      m_pManip[i]->SetScore(m_logic[i].GetSocialStatus());
      //m_characters[i].Print();
      char tmp[256];
      //sprintf(tmp, "%s: %1.3f *%1.3f*\n", m_logic[i].GetName().c_str(), m_inter[i], m_social[i]);
      sprintf(tmp, "%s  %s  %s\n", TextArtPlusMinus(m_inter[i]).c_str(), TextArtPlus(m_social[i]).c_str(), m_logic[i].GetName().c_str());
      summary += tmp;
    }
    for (i=0; i<m_logic.isize(); i++) {
      m_pManip[i]->SetSummary(summary);
    }
    
    if (m_pTarget != NULL) {
      if (m_arrow >= 0) {
	Coordinates ac = m_pManip[m_arrow]->HeadPos();
	ac += Coordinates(0, 10, 0);
	m_pTarget->SetCoords(ac, Coordinates(0, 0, 0));	
      } else {
	m_pTarget->SetCoords(Coordinates(0, 10000, 0), Coordinates(0, 0, 0));
      }
    
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
    if (m_focus != -1) {
      int currTar = m_characters[m_focus].GetDesire();
      double currAct = m_characters[m_focus].GetAct();
      if (s == "SPACE") {
	m_pManip[m_focus]->ToggleCamera();
      }
      if (s == "RETURN") {
 	m_pManip[m_focus]->ToggleOnOff();
      }
      if (s == "B") {
	currTar--;
	if (currTar < 0)
	  currTar = m_characters.isize()-1;
	m_characters[m_focus].OverrideDesire(currTar);
      }
      if (s == "G") {
	currTar++;
	if (currTar >= m_characters.isize())
	  currTar = 0;
	m_characters[m_focus].OverrideDesire(currTar);
      }
      if (s == "N") {
	currAct -= 0.1;
	if (currAct < -1.)
	  currAct = -1.;
	m_characters[m_focus].OverrideAct(currAct); 	
      }
      if (s == "H") {
 	currAct += 0.1;
	if (currAct > 1.)
	  currAct = 1.;
	m_characters[m_focus].OverrideAct(currAct); 	
      }
    }

    
    m_lastKey = s;
    for (int i=0; i<m_pManip.isize(); i++) {
      m_pManip[i]->SetTagged(false);
      m_pManip[i]->SetTaggedTarget(false);
      //m_pManip[i]->SetKey(s);
      if (i == m_focus)
	m_pManip[i]->SetTagged(true);
	
    }
    if (m_focus >= 0 && m_pManip[m_focus]->GetTargetID() >= 0) {
      m_pManip[m_pManip[m_focus]->GetTargetID()]->SetTaggedTarget(true);
      m_arrow = m_pManip[m_focus]->GetTargetID();
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
  HeadManipulator * m_pTarget;
  svec<Coordinates> m_pos;
  svec<Coordinates> m_rot;
  svec<int> m_map;
  int m_focus;
  string m_lastKey;
  SchoolLogic m_logic;
  svec<Character> m_characters;

  double m_time;
  int m_arrow;

  SchoolBuilding m_building;

  svec<double> m_social;
  svec<double> m_inter;
  
};



#endif //CHARMANIP_H
