#include "core/MemBot.h"
#include "base/RandomStuff.h"

void MemBot::Feed(const svec<Entity> & raw)
{
  int i, j;
  int xmax = 0;
  int ymax = 0;
  int xmin = 0;
  int ymin = 0;

  if (raw.isize() > 0 && m_assoc.GetSize() == 0) {
    m_assoc.Setup(raw[0].isize(), 4);
  }
  if (raw.isize() == 0) {
    return;
  }

  svec<Entity> ent;
  //ent = raw;

  // WARNING!!!! HARD CODED TARGET!!!!
  Entity food;
  food.resize(raw[0].isize());
  food[0] = 1.;
  m_assoc.Associate(ent, raw, food);
  

  //cout << "Feed entities: " << 
  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    //cout << "Entity " << i << " " << e.GetCoordinates().X() << " " << e.GetCoordinates().Y() << endl;
    if ((int)(e.GetCoordinates().X() + 0.5) > xmax) {
      xmax = (int)(e.GetCoordinates().X() + 0.5);
    }
    if ((int)(e.GetCoordinates().X()) - 0.5 < xmin) {
      xmin = (int)(e.GetCoordinates().X() - 0.5);
    }
    if ((int)(e.GetCoordinates().Y()) + 0.5 > ymax) {
      ymax = (int)(e.GetCoordinates().Y() + 0.5);
    }
    if ((int)(e.GetCoordinates().Y() - 0.5) < ymin) {
      ymin = (int)(e.GetCoordinates().Y() - 0.5);
    }
  }

  if (xmax-xmin+1 < 4)
    xmax = 4 + xmin - 1;
  if (ymax-ymin+1 < 4)
    ymax = 4 + ymin - 1;

  Cerebellum cere;


  //cere.Setup(xmax-xmin+1, ymax-ymin+1);
  cere.Setup(xmax-xmin+1, ymax-ymin+1);
  cout << "x " << xmax-xmin+1 << " y " << ymax-ymin+1 << endl;
  cout << "xmin " << xmin << " ymin " << ymin << endl;

  int tx = -1;
  int ty = -1;
  int tx2 = -1;
  int ty2 = -1;
 
  double maxReward = 0.;
  double maxReward2 = 0.;

  // Set costs from anticipated pain
  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    int x = (int)(e.GetCoordinates().X() - xmin + 0.5);
    int y = (int)(e.GetCoordinates().Y() - ymin + 0.5);

    svec<double> feat;
    feat.resize(e.isize(), 0.);
    for (j=0; j<e.isize(); j++)
      feat[j] = e[j];
    Reaction anticipate;
    m_mem.RetrieveAct(anticipate, feat);
    cere.SetCost(anticipate.Pain()+10*anticipate.Unsuccess(), x, y);
    //cout << "Pain assessment " << x << "," << y << ":" << anticipate.Pain();
    //cout << " failure: " << anticipate.Unsuccess()<< endl;

    if (anticipate.Reward() > maxReward) {     
      if (tx != x && ty != y) {
	maxReward2 = maxReward;
	tx2 = tx;
	ty2 = ty;  	
      }
      maxReward = anticipate.Reward();
      tx = x;
      ty = y;        
    } else {
      if (anticipate.Reward() > maxReward2) {
	if (tx != x && ty != y) {
	  maxReward2 = anticipate.Reward();
	  tx2 = x;
	  ty2 = y;  
	}
      }
    }

  }

  //cout << "MEMBOT!!!" << endl;
  cout << "BEST: " << tx << "," << ty << " SECOND: " << tx2 << "," << ty2 << endl;

  svec<PathItem> path;
  cout << "TRY path finding: " << -xmin << "," << -ymin << " -> " << tx << "," << ty << " expect: " << maxReward << endl;
  if (-xmin == tx && -ymin == ty) {
    tx = tx2;
    ty = ty2;
    cout << "ALT path finding: " << -xmin << "," << -ymin << " -> " << tx << "," << ty << endl;
  }
  bool b = cere.FindPath(path, -xmin, -ymin, 0, 0,
			 tx, ty, 0, 0);
  m_isClose = false;

  // Right at the end...
  if (b && path.isize() == 2)
    m_isClose = true;
  cout << "Path length: " << path.isize() << endl;
  if (b && path.isize() >= 2) {
    const PathItem & p = path[path.isize()-2];
    int three = 3;
    if (path.isize() == 2) {
      three = 2;
    }
    const PathItem & p2 = path[path.isize()-three];
    cout << "Move from (1) " << -xmin << "," << -ymin << " to " << p.X() << "," << p.Y() << endl;
    cout << "Move from (2) " << -xmin << "," << -ymin << " to " << p2.X() << "," << p2.Y() << endl;
    double w = 1.;
    m_moveX = w*p.X() + (1.-w)*(p2.X()) + xmin;
    m_moveY = w*p.Y() + (1.-w)*(p2.Y()) + ymin;
    cout << "Move (interpolated): " << m_moveX << ", " << m_moveY << endl;
  } else {
    cout << "FAILED, no path found." << endl;
    m_moveX = 0.;
    m_moveY = 0.;
  }
}

void MemBot::GetAction(IAction & a) 
{

  a.Set(0, m_moveX);
  a.Set(1, m_moveY);
  
  cout << "Set " << a.Get(0) << " " << a.Get(1) << endl;
}

void MemBot::Feedback(const Reaction & raw, const Entity & e, const Coordinates & pos)
{
  Reaction outcome = raw;
  if (outcome.Reward() > 0.5) {
    cout << "YUMM." << endl;    
  } else {
    if (m_isClose) {
      cout << "OH NO!!! NOT FOUND!!!" << endl;
      outcome.SetUnsuccess(outcome.Unsuccess()+0.6);
    }
      
  }
  if (outcome.Pain() > 0.7) {
    cout << "AAAAARGGGH" << endl;
  } else {
    if (outcome.Pain() > 0.2) {
      cout << "OUCH." << endl;
    }
  }

  int j;

  svec<double> feat;
  feat.resize(e.isize(), 0.);
  for (j=0; j<e.isize(); j++)
    feat[j] = e[j];
 
  //if (outcome.Pain() > 0.2) {    
  for (j=0; j<20; j++)
    m_mem.LearnAct(feat, outcome);
    //}


  m_mypos = pos;
}
