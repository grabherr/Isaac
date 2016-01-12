#include "npc/Plant.h"
#include "base/RandomStuff.h"


void Tree::Update() 
{
  int i, j;
  for (j=0; j<=m_maxlevel; j++) {
    for (i=0; i<m_branches.isize(); i++) {
      if (m_branches[i].GetLevel() != j)
	continue;
      int p = m_branches[i].GetParent();
      
	if (p != -1) {
	  m_branches[i].Adjust(m_branches[p]);
	}
    }
  }
  
}

void Tree::Grow() {
  int i;
  for (i=0; i<m_branches.isize(); i++) {
    m_branches[i].Grow(RandomFloat(1.1));
  }
  Update();
}

//===================================================
void SimpleTree::AddBranches() 
{
    
  int i;
  int n = m_branches.isize();
  for (i=0; i<n; i++) {
    if (m_branches[i].Length() < 10)
      continue;
    double p = .15*(1.-1./(m_branches[i].Length()/10.));
    if (RandomFloat(1.) > p)
      continue;
    
    
    double off = 0.5 + RandomFloat(0.5);
    Branch tmp;
    
    tmp.SetParent(i);
    tmp.SetLevel(m_branches[i].GetLevel()+1);
    
    if (tmp.GetLevel() > m_maxlevel)
      m_maxlevel = tmp.GetLevel();
    
    Coordinates dir;
    dir[0] = -1 + RandomFloat(2);
    dir[1] = RandomFloat(.5);
    dir[2] = -1 + RandomFloat(2);
    //dir[1] = 1.;
    
    Coordinates e = dir.Einheitsvector()*5;
    Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector()*5;
    Coordinates n = (e + pa)*0.5;
    
    tmp.BranchOff(m_branches[i], off, n);
    
    m_branches.push_back(tmp);
    
  }
  
}


void SimpleTree::Grow() {
  int i;
  for (i=0; i<m_branches.isize(); i++) {
    m_branches[i].Grow(RandomFloat(1.1));
  }
  Update();
}
