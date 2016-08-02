#ifndef ANTMANIPULATOR_H
#define ANTMANIPULATOR_H

#include "engine/TManipulator.h"

#define ANT_IDLE 0
#define ANT_WALK 1
#define ANT_HEJ 2


class AntInfo
{
 public:
  AntInfo() {
  }

 protected:
  int m_action;
  

};

class AntManipulator : public ThreadedManipulator
//class AntManipulator : public IManipulator
{
 private:

 public:
  AntManipulator();
  AntManipulator(const AntManipulator & a);

  virtual ~AntManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c);

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime);

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
    return; // Do nothing
  }
  
  virtual void Animate(GamePhysObject & o, double deltatime) {}
  double GetDistance() const {return m_dist;}
  
  void SetAnim(bool b) {m_anim = b;}
  bool GetAnim() const {return m_anim;}

  void SetName(const string & s) {
    m_name = s;
  }

  const string & GetName() const {return m_name;}

  void SetModel(const string & s) {
    m_model = s;
  }

  const string & GetModel() const {return m_model;}

 protected:
  virtual void Process();



private:
  Coordinates m_center;
  Coordinates m_lastPos;
  double m_rotY;
  double m_time;
  double m_elapsed;
  bool m_anim;
  int m_counter;
  double m_dist;
  string m_name;
  string m_animname;
  string m_model;

  int m_status;
};


class IndexDist
{
 public:
  double m_dist;
  int m_index;
  bool operator < (const IndexDist & d) const {
    return m_dist < d.m_dist;
  }
};

class AntGlobal : public IGlobal
{
 public:
  AntGlobal() {}
  virtual ~AntGlobal() {}

  virtual void StartFrame(double deltatime) {}
  virtual void EndFrame(double deltatime) {
    //return;
    
    cout << "Call EndFrame" << endl;
    int i, j;
    m_index.resize(m_manip.isize());

    for (i=0; i<m_manip.isize(); i++) {
      m_index[i].m_index = i;
      m_index[i].m_dist = m_manip[i]->GetDistance();
      cout << "Print model " << m_manip[i]->GetModel() << " " << m_manip[i]->GetName() << " " << m_manip[i]->GetAnim() << endl;
    }
    Sort(m_index);
    for (i=0; i<m_index.isize(); i++) {
      int index = m_index[i].m_index;
      if (!m_manip[index]->GetAnim()) {
	int swap = -1;
	for (j=i+1; j<m_index.isize(); j++) {
	  if (m_manip[m_index[j].m_index]->GetAnim())
	    swap = m_index[j].m_index;
	}

	if (swap != -1) {
	  m_manip[index]->SetAnim(true);
	  m_manip[swap]->SetAnim(false);
	  string tmp = m_manip[index]->GetName();
	  m_manip[index]->SetName(m_manip[swap]->GetName());
	  m_manip[swap]->SetName(tmp);

	  tmp = m_manip[index]->GetModel();
	  m_manip[index]->SetModel(m_manip[swap]->GetModel());
	  m_manip[swap]->SetModel(tmp);
	  cout << "Swapping " << index << " <-> " << swap << endl;
	  cout << "Swapping model " << m_manip[index]->GetModel() << " <-> " << m_manip[swap]->GetModel()  << endl;
	}

	break;
      }
    }
  }

  string Register(AntManipulator * p, bool bAnim, const string & model) {
    m_manip.push_back(p);
    string ret = "ant";
    char tmp[256];
    sprintf(tmp, "ant%d", m_manip.isize());
    ret = tmp;
    if (bAnim) {
      ret += "_anim";
    }
    p->SetAnim(bAnim);
    p->SetName(ret);
    p->SetModel(model);
    return ret;
  }

 private:
  svec<AntManipulator*> m_manip;
  svec<IndexDist> m_index;
};

#endif

