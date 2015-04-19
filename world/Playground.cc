#include "world/Playground.h"


void Playground::Print()
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
      //if (bBot)
      //cout << '*';
      //else
      cout << m_area.Get(i, j);
    }
    cout << "|" << endl;
  }
  for (j=0; j<m_area.SizeY(); j++) {
    cout << "-";
  }
  cout << "|" << endl;
}

/*int Playground::Register(IAutonomosUnit * pUnit, const Coordinates & pos)
{
  return -1;
  }*/

int Playground::RegisterIsaac(Isaac * pUnit, const Coordinates & pos)
{
  m_reg.push_back(pUnit);
  m_pos.push_back(pos);
  pUnit->SetHandle( m_pos.isize()-1);
  m_area.Set(pos.X(), pos.Y(), ISAAC);
  return m_pos.isize()-1;
}

void Playground::GetEntities(svec<Entity> & ent, int handle)
{
  Coordinates pos = m_pos[handle];

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

void Playground::MakeTemplate()
{

  Properties prop;
  prop.Add("wet", 0.);
  prop.Add("poky", 0.);
  prop.Add("hard", 0.);
  prop.Add("big", 0.);
  prop.Add("delicious", 0.);
  prop.Add("moving", 0.);
  m_template.SetProperties(prop);

  Properties action;
  action.Add("back/forward", 0.);
  action.Add("left/right", 0.);
  action.Add("up/down", 0.);
  action.Add("eat", 0.);
  action.Add("play", 0.);
  
  m_action.SetProperties(action);

  //m_interaction.AddInteraction();
  //PropertyMutator
}

void Playground::MakeEntity(Entity & ent, char c)
{
  
  ent = m_template;
  if (c == FOOD) {
    ent.Set("delicious", 1);
    ent.Set("hard", 0.1);
    ent.Set("wet", 0.1);
    ent.Set("big", 0.1);      
  }
  if (c == WATER) {
    ent.Set("wet", 1.0);   
    ent.Set("big", 0.2);      
  }
  if (c == TREE) {
    ent.Set("hard", 1.0);   
    ent.Set("big", 1.0);      
  }
  if (c == CACTUS) {
    ent.Set("hard", 0.8);   
    ent.Set("big", 0.5);      
    ent.Set("poky", 1.0);      
  }
  if (c == ISAAC) {
    ent.Set("hard", 0.3);   
    ent.Set("big", 0.4);      
    ent.Set("moving", 1.0);      
  }
}

bool Playground::Try(Reaction & outcome, Entity & ent, int handle, const IAction & action)
{
  Coordinates pos = m_pos[handle];

  outcome.Reset();

  int x = (int)(pos.X() + 0.5);
  int y = (int)(pos.Y() + 0.5);

  m_area.Set(x, y, EMPTY);

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
  if (m_area.Get(px, py) == ISAAC) {
    outcome.SetReward(0.5);
    outcome.SetPain(0.1);
    px = x;
    py = y;
  }

  m_area.Set(px, py, ISAAC);

  pos.SetX(px);
  pos.SetY(py);

  m_pos[handle] = pos;

  return true;
}
