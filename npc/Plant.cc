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
void SimpleTree::AddBranches(double prob, bool bTips) 
{
  cout << "Adding branch, new round. Have " << m_branches.isize() << endl;

  int i;
  int n = m_branches.isize();
  for (i=0; i<n; i++) {
    // Add to the top

    m_branches[i].AddAge(1.);

    if (m_branches[i].GetChildren() == 0) {
      Branch tmp1;
      cout << "Adding to " << i << endl;
      tmp1.SetParent(i);
      tmp1.SetLevel(m_branches[i].GetLevel()+1);
      //tmp1.SetLevel(m_branches[i].GetLevel());
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
      tmp1.AddAge(1.);
      m_branches.push_back(tmp1);
         
    }
  }

  cout << "Adding branch: Done w/ tip growth, have " << m_branches.isize() << endl;
  //n = m_branches.isize();
  for (i=0; i<n; i++) {
    // Add to the top

    //if (!bTips && (m_branches[i].GetAge() > 5 || m_branches[i].GetAge() < 3))
    //  continue;
    //if (!bTips && m_branches[i].GetLevel() > 2)
    //  continue;

    //if (bTips && m_branches[i].GetAge() > 3)
    //continue;

    //double p = .08 * 1./(1.+m_branches[i].GetChildren());
    //double p = prob/m_branches[i].GetAge();

    if (m_branches[i].GetBranches() > 0)
      continue;

    double p = 0.;
    double mul = 8.;
    double rest = m_branches[i].GetLevel()/mul - (int)(m_branches[i].GetLevel()/mul);
    if (rest < 0.01 && rest > -0.01)
      p = 0.5;

    double rr = 1.;
    
    if (bTips) {
      p = 0.;
      if (m_branches[i].GetChildren() <= 1 && m_branches[i].GetAge() < 6.) {
	p = 0.4;
	//rr = 1. + RandomFloat(18);      
      }
      cout << "TIPS " << m_branches[i].GetChildren() << endl;
    }
      

    if (RandomFloat(1.) > p)
      continue;
    
    cout << "Adding branch to " << i << " age " << m_branches[i].GetAge() << " level " << m_branches[i].GetLevel();
    if (bTips)
      cout << " as tip!!" << endl;
    cout << endl;
    
    double off = 0.8+RandomFloat(.2);
    Branch tmp;
    
    tmp.SetParent(i);
    tmp.SetLevel(m_branches[i].GetLevel()+1);
    //tmp.SetLevel(0);
    m_branches[i].AddChild();
    m_branches[i].AddBranch();

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

    n *= rr;
    
    tmp.BranchOff(m_branches[i], off, n);
    
    m_branches.push_back(tmp);
    
  }
  
}


void SimpleTree::Grow() {
  int i;
  double rate = 0.8+RandomFloat(0.3);
  rate /= 8.;
  for (i=0; i<m_branches.isize(); i++) {
    if (m_branches[i].GetChildren() == 0)
      m_branches[i].SetFlow(m_branches[i].GetWidth());
    else
      m_branches[i].SetFlow(0);
  }

  for (i=0; i<m_branches.isize(); i++) {
    cout << "Growing " << i << endl;
    m_branches[i].Grow(rate);
  }
  //Update();
}


//=========================================================


//===================================================
void Conifer::AddBranches(double prob, bool bTips) 
{
  cout << "Adding branch, new round. Have " << m_branches.isize() << endl;

  int i;
  int n = m_branches.isize();
  for (i=0; i<n; i++) {
    // Add to the top

    m_branches[i].AddAge(1.);

    if (m_branches[i].GetChildren() == 0) {
      Branch tmp1;
      cout << "Adding to " << i << endl;
      tmp1.SetParent(i);
      tmp1.SetLevel(m_branches[i].GetLevel()+1);
      
      //if (m_branches[i].GetLevel() > 50)
      //continue;

     //tmp1.SetLevel(m_branches[i].GetLevel());
      m_branches[i].AddChild();
      
 
      if (tmp1.GetLevel() > m_maxlevel)
	m_maxlevel = tmp1.GetLevel();
      
      Coordinates dir;
      dir[0] = -.5 + RandomFloat(1);
      dir[1] = RandomFloat(.5);
      dir[2] = -.5 + RandomFloat(1);
      //dir[1] = 1.;
      
      dir[0] /= 5.;
      dir[2] /= 5.;
      
      if (m_branches[i].Top()[1] < 0)
	continue;

      Coordinates e = dir.Einheitsvector();
      Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector();
      Coordinates n = e*0.1;
      n += pa*0.9;

      n *= 8+RandomFloat(4);

      tmp1.BranchOff(m_branches[i], 1., n);
      tmp1.AddAge(1.);
      m_branches.push_back(tmp1);
         
    }
  }

  cout << "Adding branch: Done w/ tip growth, have " << m_branches.isize() << endl;
  //n = m_branches.isize();
  for (i=0; i<n; i++) {
    // Add to the top

    //if (!bTips && (m_branches[i].GetAge() > 5 || m_branches[i].GetAge() < 3))
    //  continue;
    //if (!bTips && m_branches[i].GetLevel() > 2)
    //  continue;

    //if (bTips && m_branches[i].GetAge() > 3)
    //continue;

    //double p = .08 * 1./(1.+m_branches[i].GetChildren());
    //double p = prob/m_branches[i].GetAge();

    if (m_branches[i].GetBranches() > 0)
      continue;

    double p = 0.;
    double mul = 8.;
    double rest = m_branches[i].GetLevel()/mul - (int)(m_branches[i].GetLevel()/mul);
    if (rest < 0.01 && rest > -0.01)
      p = 0.5;

    double rr = 1.;
    
    if (bTips) {
      p = 0.;
      if (m_branches[i].GetChildren() <= 1 && m_branches[i].GetAge() < 6.) {
	p = 0.4;
	//rr = 1. + RandomFloat(18);      
      }
      cout << "TIPS " << m_branches[i].GetChildren() << endl;
    }
      

    if (RandomFloat(1.) > p)
      continue;
    
    cout << "Adding branch to " << i << " age " << m_branches[i].GetAge() << " level " << m_branches[i].GetLevel();
    if (bTips)
      cout << " as tip!!" << endl;
    cout << endl;
    
    double off = 0.8+RandomFloat(.2);
    Branch tmp;
    
    tmp.SetParent(i);
    tmp.SetLevel(m_branches[i].GetLevel()+1);
    //tmp.SetLevel(0);
    m_branches[i].AddChild();
    m_branches[i].AddBranch();

    if (tmp.GetLevel() > m_maxlevel)
      m_maxlevel = tmp.GetLevel();
    
    Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector();
    Coordinates up = Coordinates(0, .15, 0);
    Coordinates dir = pa.CrossProduct(up);
    double du = dir[1];

    if (RandomFloat(1) < 0.5) {
      dir = dir * -1.;
    }
    du = -0.05;
    dir[1] = du;
    
    Coordinates e = dir.Einheitsvector();
    //Coordinates pa = (m_branches[i].Top()-m_branches[i].Bottom()).Einheitsvector()*5;
    Coordinates n = e*0.8;
    n += pa*0.2;

    n *= rr;
    
    tmp.BranchOff(m_branches[i], off, n);
    
    m_branches.push_back(tmp);
    
  }
  
}


void Conifer::Grow() {
  int i;
  double rate = 0.8+RandomFloat(0.3);
  rate /= 8.;
  for (i=0; i<m_branches.isize(); i++) {
    if (m_branches[i].GetChildren() == 0)
      m_branches[i].SetFlow(m_branches[i].GetWidth());
    else
      m_branches[i].SetFlow(0);
  }

  for (i=0; i<m_branches.isize(); i++) {
    cout << "Growing " << i << endl;
    m_branches[i].Grow(rate);
  }
  //Update();
}

