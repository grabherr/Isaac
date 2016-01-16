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
    // Add to the top
    if (m_branches[i].GetChildren() == 0) {
      Branch tmp1;
      cout << "Adding to " << i << endl;
      tmp1.SetParent(i);
      tmp1.SetLevel(m_branches[i].GetLevel()+1);
      m_branches[i].AddChild();
      
      if (tmp1.GetLevel() > m_maxlevel)
	m_maxlevel = tmp1.GetLevel();
      
      Coordinates dir;
      dir[0] = -1 + RandomFloat(2);
      dir[1] = RandomFloat(.5);
      dir[2] = -1 + RandomFloat(2);
      //dir[1] = 1.;
    
      Coordinates e = dir.Einheitsvector();
      Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector();
      Coordinates n = e*0.1;
      n += pa*0.9;

      n *= 8+RandomFloat(4);

      tmp1.BranchOff(m_branches[i], 1., n);
    
      m_branches.push_back(tmp1);
       
      continue;
    }


    double p = .03 * 1./(1.+m_branches[i].GetChildren());
    if (RandomFloat(1.) > p)
      continue;
    
    
    double off = RandomFloat(1.);
    Branch tmp;
    
    tmp.SetParent(i);
    tmp.SetLevel(m_branches[i].GetLevel()+1);
    m_branches[i].AddChild();

    if (tmp.GetLevel() > m_maxlevel)
      m_maxlevel = tmp.GetLevel();
    
    Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector();
    Coordinates up = Coordinates(0, 1, 0);
    Coordinates dir = pa.CrossProduct(up);

    if (RandomFloat(1) < 0.5) {
      dir = dir * -1.;
    }

    
    Coordinates e = dir.Einheitsvector();
    //Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector()*5;
    Coordinates n = e*0.8;
    n += pa*0.2;
    
    tmp.BranchOff(m_branches[i], off, n);
    
    m_branches.push_back(tmp);
    
  }
  
}


void SimpleTree::Grow() {
  int i;
  double rate = 0.8+RandomFloat(0.3);
  rate /= 2.;
  for (i=0; i<m_branches.isize(); i++) {
    cout << "Growing " << i << endl;
    m_branches[i].Grow(rate);
  }
  //Update();
}
