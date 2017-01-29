#ifndef CHARMANIP_H
#define CHARMANIP_H

#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "npc/Skeleton.h"
#include "npc/TopLevel.h"

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
  }
  virtual ~CharManipulator() {}

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
  CharGlobCtrl(CharManipulator * p, HeadManipulator * pHead, ItemManipulator * pItem) {
    m_pManip = p;
    m_pHead = pHead;
    m_pItem = pItem;
  }
  
  virtual void StartFrame(double deltatime) {
  }
  
  virtual void EndFrame(double deltatime) {
    m_pHead->SetCoords(m_pManip->HeadPos(), m_pManip->HeadRot());
    m_pManip->SetItemPos(m_pItem->GetPos());
  }

  virtual void KeyPressed(const string & s) {
    cout << "Got message Key pressed: " << s << endl;
    if (m_pManip != NULL)
      m_pManip->SetKey(s);
  }
  
private:
  CharManipulator * m_pManip;
  HeadManipulator * m_pHead;
  ItemManipulator * m_pItem;
};



#endif //CHARMANIP_H
