#include "world/Simple2D.h"


void WorldSimple2D::Print()
{
  cout << "WORLD: " << endl;
  int i, j;
  for (j=0; j<m_area.SizeY(); j++) {
    cout << "-";
  }
  cout << "|" << endl;
  for (i=0; i<m_area.SizeX(); i++) {
    for (j=0; j<m_area.SizeY(); j++) {
      bool bBot = false;
      for (int k =0; k<m_pos.isize(); k++) {
	if (m_pos[k].X() == i && m_pos[k].Y() == j)
	  bBot = true;
      }
      if (bBot)
	cout << '*';
      else
	cout << m_area.Get(i, j);
    }
    cout << "|" << endl;
  }
  for (j=0; j<m_area.SizeY(); j++) {
    cout << "-";
  }
  cout << "|" << endl;
}

int WorldSimple2D::Register(IAutonomosUnit * pUnit, const Coordinates & pos)
{
  m_reg.push_back(pUnit);
  m_pos.push_back(pos);
  pUnit->SetHandle( m_pos.isize()-1);
  return m_pos.isize()-1;
}

void WorldSimple2D::GetEntities(svec<Entity> & ent, const Coordinates & pos)
{
  ent.clear();
  int i, j;
  int k = 0;
  for (i=0; i<m_area.SizeX(); i++) {
    for (j=0; j<m_area.SizeY(); j++) {
      char c = m_area.Get(i, j);
      if (c == EMPTY)
	continue;
      Entity tmp;
      MakeEntity(tmp, c);

      Coordinates p;
      p.SetX(i);
      p.SetY(j);
      p -= pos;
      //cout << "Internal entity " << k << " " << p.X() << " " << p.Y() << endl;
      k++;
      tmp.SetCoordinates(p);
      ent.push_back(tmp);
    }
  }
}

void WorldSimple2D::MakeEntity(Entity & ent, char c)
{
  // Feedback what we hit
  ent.resize(4);
  if (c == FOOD) {
    ent[0] = 1.;
  }
  if (c == WATER) {
    ent[1] = 1.;
  }
  if (c == TREE) {
    ent[2] = 1.;
  }
  if (c == CACTUS) {
    ent[3] = 1.;
  }
}

bool WorldSimple2D::Try(Reaction & outcome, Coordinates & pos, Entity & ent, int handle, const IAction & action)
{
  outcome.Reset();

  int x = (int)(pos.X() + 0.5);
  int y = (int)(pos.Y() + 0.5);

  double dx = action.Get(0);
  double dy = action.Get(1);

  cout << "Try move: " << dx << "\t" << dy << endl;

  int px = x;
  int py = y;
  if (dx > 0.2)
    px++;
  if (dx < -0.2)
    px--;
  if (dy > 0.2)
    py++;
  if (dy < -0.2)
    py--;

  if (px < 0) {
    px = 0;
    outcome.SetUnsuccess(1.);
    outcome.SetPain(0.2);
  }
  if (py < 0) {
    py = 0;
    outcome.SetUnsuccess(1.);
    outcome.SetPain(0.2);
  }
  if (px >= SizeX()) {
    px = SizeX()-1;
    outcome.SetUnsuccess(1.);
    outcome.SetPain(0.2);
  }
  if (py >= SizeY()) {
    py = SizeY()-1;
    outcome.SetUnsuccess(1.);
    outcome.SetPain(0.2);
  }

  MakeEntity(ent, m_area.Get(px, py));
 

  if (m_area.Get(px, py) == EMPTY) {
    outcome.SetPain(0.1);    
  }
  if (m_area.Get(px, py) == FOOD) {
    outcome.SetReward(1.);
    m_area.Set(px, py, EMPTY);
    m_food--;
  }
  if (m_area.Get(px, py) == WATER) {
    outcome.SetPain(0.2);    
  }
  if (m_area.Get(px, py) == TREE) {
    outcome.SetPain(0.7);
    outcome.SetUnsuccess(1.0);
    px = x;
    py = y;
  }
  if (m_area.Get(px, py) == CACTUS) {
    outcome.SetUnsuccess(1.0);
    outcome.SetPain(0.9);
    px = x;
    py = y;
  }

  pos.SetX(px);
  pos.SetY(py);

  m_pos[handle] = pos;

  return true;
}
