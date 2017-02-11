#include "game/Character.h"
#include "base/RandomStuff.h"


Character::Character()
{
  m_size = 0;
  m_myIndex = -1;
  m_desire = -1;
  m_avoid = -1;
  m_act = 0;
  m_score = 0;
}

void Character::Print() const
{
  cout << "Printing contents for " << m_name << endl;
  cout << "Desired:  " << m_desire << endl;
  cout << "Action:   " << m_act << endl;
  cout << "Avoiding: " << m_avoid << endl;
  cout << "Coordinates ";
  m_self.Print();
  cout << endl;
  m_nn.Print();
}

void Character::SetupPeople(int n, int index)
{
  // Reserve 1 for success
  m_nn.Setup(20, n+1, 1);      
  m_map.resize(m_nn.isize(), -1);
  m_nn.ReSetup(-1, 1);
  m_size = m_nn.isize();
  m_myIndex = index;

  m_top.resize(3, 3, 1, 10, 6);
  
  cout << "Set up w/ " << n << endl;
}

void Character::FeedNeutral(const svec<double> & prop, const Coordinates & c, int index)
{
  if (index == m_myIndex)
    return;
  
  int i;
  double w = (m_self - c).Length();
  if (w < 10)
    w = 10;
  w = 10/w;

  NPCIO seq;
  seq.resize(m_size);
  seq.SetAllValid(false);
  for (i=0; i<prop.isize(); i++) {
    seq[i] = prop[i];
    seq.SetValid(i, true);
  }
  m_nn.Learn(seq, w);
  int n = m_nn.Best(seq);
  m_map[n] = index;

  if (m_desire == -1) {
    if (RandomFloat(1.) < 0.01)
      FeedAction(prop, 0);
  }
}

void Character::Select()
{
  throw;
  
  if (m_desire >= 0 || m_avoid >= 0)
    return;
  
 
}

void Character::FeedDone(const svec<double> & prop, double in)
{
  FeedAction(prop, in);
}

void Character::SetScore(double s)
{
  m_score = s;
}

void Character::FeedAction(const svec<double> & prop, double in)
{
  int i;
  
  NPCIO seq;
  seq.resize(m_size);
  seq.SetAllValid(false);
  for (i=0; i<prop.isize(); i++) {
    seq[i] = prop[i];
    seq.SetValid(i, true);
  }
  seq[i] = in;
  seq.SetValid(i, true);
  m_nn.Learn(seq, 1.);

  Coordinates b;
  int n = m_nn.BestCoords(b[0], b[1], b[2], seq);
 
  
  IOEntity ent;
  ent.resize(3, 3, 1);
  ent.in(0) = in; 
  //ent.in(1) = prop[0]; 
  //ent.in(2) = prop[1];
  ent.in(1) = b[0]; 
  ent.in(2) = b[1];
  
  m_top.Update(ent, 0.6, m_score);
  cout << "UPDATE TOP" << endl;
  ent.Print();
  
  int nn = m_nn.BestNeuronForCoords(ent.out(0), ent.out(1), 0);

  m_desire = m_map[nn];
  m_act = ent.out(2);
  while (m_desire == -1 && nn < m_map.isize()-1) {
    nn++;
    m_desire = m_map[nn];
  }
  while (m_desire == -1 && nn > 0) {
    nn--;
    m_desire = m_map[nn];
  }
  
  //m_map[n]
}
