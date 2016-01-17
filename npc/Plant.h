#ifndef PLANT_H
#define PLANT_H

#include <math.h>
#include "physics/Coordinates.h"



class Branch 
{
public:
  Branch() {
    m_width = .1;
    m_level = 0;
    m_parent = -1;
    m_where = 1.;
    m_age = 0.;
    m_children = 0;
    m_flow = 0.;
    m_branches = 0;
  }

  Coordinates & Bottom() {return m_bottom;}
  Coordinates & Top() {return m_top;}
  const Coordinates & Bottom() const {return m_bottom;}
  const Coordinates & Top() const {return m_top;}

  void SetWidth(double d) {
    m_width = d;
  }

  double GetWidth() const {return m_width;}

  void BranchOff(const Branch & b, double where, const Coordinates & dir) {
    m_where = where;
    Coordinates root = b.Top() - b.Bottom();
    root *= m_where;    
    m_bottom = b.Bottom() + root;
    m_top = m_bottom + dir;
  }

  void Adjust(const Branch & b) {
    cout << "Adjusting." << endl;
    Coordinates root = b.Top() - b.Bottom();
    root *= m_where;
    Coordinates rel = m_top - m_bottom;
    m_bottom = b.Bottom() + root;
    m_top = m_bottom + rel;
  }

  void SetWhere(double d) {
    m_where = d;
  }
  double GetWhere() const {return m_where;}

  int GetChildren() const {return m_children;}
  void AddChild() {m_children++;}
  int GetBranches() const {return m_branches;}
  void AddBranch() {m_branches++;}

  int GetParent() const {return m_parent;}
  void SetParent(int i) {m_parent = i;}

  int GetLevel() const {return m_level;}
  void SetLevel(int i) {m_level = i;}

  double GetFlow() const {return m_flow;}
  void AddFlow(double f) {m_flow += f;}
  void SetFlow(double f) {m_flow = f;}

  double GetAge() const {return m_age;}
  void SetAge(double age) {m_age = age;}
  void AddAge(double age) {m_age += age;}

  void Grow(double rate) {
    //Coordinates rel = m_top - m_bottom;
    //double l = rel.Length();
    //l += rate;
    //Coordinates e = rel.Einheitsvector()*l;
    //m_top = m_bottom + e;
    cout << "Old size: " << m_width << endl;
    m_width += rate;
    cout << "New size: " << m_width << endl;
    //m_width *= sqrt(rate);
  }
  
  double Length() const {
    return (m_top-m_bottom).Length();
  }
  

private:
  Coordinates m_bottom;
  Coordinates m_top;
  double m_width;
  double m_where;
  double m_age;
  double m_flow;
  int m_level;
  int m_parent;
  int m_children;
  int m_branches;
};



//=====================================================
class Tree
{
public:
  Tree() {
    m_maxlevel = 0;
  }

  int isize() const {return m_branches.isize();}
  const Branch & operator[] (int i) const {return m_branches[i];}
  Branch & operator[] (int i) {return m_branches[i];}


  void AddTrunk(const Coordinates & bottom,
		const Coordinates & top) {
    Branch tmp;
    tmp.Bottom() = bottom;
    tmp.Top() = top;      
    m_branches.push_back(tmp);
  }

  virtual void AddBranches(double p, bool bTips = false) = 0;    
  virtual void Grow();

  void Print() const {
    int i;
    for (i=0; i<m_branches.isize(); i++) {
      cout << "Branch " << i << endl;
      cout << "   parent: " << m_branches[i].GetParent() << endl;
      cout << "   where:  " << m_branches[i].GetWhere() << endl;
      cout << "   ";
      m_branches[i].Bottom().Print();
      m_branches[i].Top().Print();
    }
  }

protected:
  virtual void Update();
  
  svec<Branch> m_branches;
  int m_maxlevel;
};

class SimpleTree : public Tree
{
public:
  SimpleTree() {
  }

  virtual void AddBranches(double p, bool bTips = false);    
  virtual void Grow();

};

#endif //PLANT_H
