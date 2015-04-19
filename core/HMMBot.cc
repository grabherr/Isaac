#include "core/HMMBot.h"
#include "base/RandomStuff.h"

void HMMBot::Feed(const svec<Entity> & ent)
{
  int i;
  int xmax = 0;
  int ymax = 0;
  int xmin = 0;
  int ymin = 0;
  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    if (e.GetCoordinates().X() > xmax) {
      xmax = e.GetCoordinates().X();
    }
    if (e.GetCoordinates().X() < xmin) {
      xmin = e.GetCoordinates().X();
    }
    if (e.GetCoordinates().Y() > ymax) {
      ymax = e.GetCoordinates().Y();
    }
    if (e.GetCoordinates().Y() < ymin) {
      ymin = e.GetCoordinates().Y();
    }
  }

  Cerebellum cere;
  cere.Setup(xmax-xmin+1, ymax-ymin+1);
  cout << "x " << xmax-xmin+1 << " y " << ymax-ymin+1 << endl;
  cout << "xmin " << xmin << " ymin " << ymin << endl;

  int tx = -1;
  int ty = -1;
  // Set costs
  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    int x = e.GetCoordinates().X() - xmin;
    int y = e.GetCoordinates().Y() - ymin;
    if (e[0] > 0.5) {
      cere.SetCost(0., x, y);
      tx = x;
      ty = y;
    }
    if (e[1] > 0.5)
      cere.SetCost(0.3, x, y);
    if (e[2] > 0.5)
      cere.SetCost(1., x, y);
    if (e[3] > 0.5)
      cere.SetCost(1., x, y);
  }
  svec<PathItem> path;
  cere.FindPath(path, -xmin, -ymin, 0, 0,
                tx, ty, 0, 0);

  const PathItem & p = path[path.isize()-2];
  cout << "Move from " << -xmin << "," << -ymin << " to " << p.X() << "," << p.Y() << endl;
  m_moveX = p.X() + xmin;
  m_moveY = p.Y() + ymin;
}

void HMMBot::GetAction(IAction & a) 
{
  /* if (m_bRand) {

    double x = RandomFloat(2.) - 1.;
    double y = RandomFloat(2.) - 1.;
    a.Set(0, x);
    a.Set(1, y);    
    return;
    } */

  a.Set(0, m_moveX);
  a.Set(1, m_moveY);
  
  cout << "Set " << a.Get(0) << " " << a.Get(1) << endl;
}

void HMMBot::Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos)
{
  if (outcome.Reward() > 0.5) {
    cout << "YUMM." << endl;    
  }
  if (outcome.Pain() > 0.7) {
    cout << "AAAAARGGGH" << endl;
  } else {
    if (outcome.Pain() > 0.2) {
      cout << "OUCH." << endl;
    }
  }
  if (pos == m_mypos) {
    //cout << "Random." << endl;
    m_bRand = true;
  } else {
    m_bRand = false;
  }
  m_mypos = pos;
}
