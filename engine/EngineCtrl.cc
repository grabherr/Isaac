#include "engine/EngineCtrl.h"
#include "physics/PhysicsIO.h"


void SceneProp::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
  const Coordinates & center = m_phys.GetCenter().GetPosition();
  // TODO: Get rotation
  Coordinates game;
  game[0] = center[0]*m_scale;
  game[1] = center[2]*m_scale;
  game[2] = center[1]*m_scale;
  m_node.SetCoordinates(game);

  Coordinates rot;
  m_phys.GetRotation(rot);
  game[0] = rot[0];
  game[1] = rot[2];
  game[2] = rot[1];
  m_node.SetDirection(game);
}


void AnimProp::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
  const Coordinates & center = m_phys.GetCenter().GetPosition();
  // TODO: Get rotation
  Coordinates game;
  game[0] = center[0]*m_scale;
  game[1] = center[2]*m_scale;
  game[2] = center[1]*m_scale;
  m_anim.SetCoordinates(game);

  Coordinates rot;
  m_phys.GetRotation(rot);
  game[0] = rot[0];
  game[1] = rot[2];
  game[2] = rot[1];
  m_anim.SetDirection(game);
}


void Compound::Update(double deltatime, double gravity)
{
  m_master.Update(deltatime, gravity);
  const Coordinates & center = m_master.GetCenter().GetPosition();

  for (int i=0; i<m_master.isize(); i++) {
    const PhysMinimal & min = m_master[i];
    const Coordinates & cc = min.GetPosition();
    Coordinates game;
    game[0] = center[0]*m_scale;
    game[1] = center[2]*m_scale;
    game[2] = center[1]*m_scale;
    AnimatedSceneNode & anim = m_list[i].GetAnimNode();
    anim.SetCoordinates(game);
  }
}


//====================================================
GameControl::GameControl() 
{
  m_animInComp = 0;
  m_gravity = 9.81;
  m_scale = 1.;
 
  // Add bottom
  SolidTriangle t;
  double z = 20;
  t.Set(Coordinates(0, 0, z), 
	Coordinates(0, 1000, z), 
	Coordinates(1000, 0, z));

  t.SetElasticity(0.8);
  AddTriangle(t);

  t.Set(Coordinates(1000, 0, z), 
	Coordinates(1000, 1000, z), 
	Coordinates(0, 1000, z));

  t.SetElasticity(0.8);
  AddTriangle(t);

  Start();
}

void GameControl::AddProp(const SceneNode & n)
{
  SceneProp p;
  p.GetSceneNode() = n;
  p.SetScale(m_scale);
  if (n.GetPhysics() != "") {
    PhysicsIO io;
    PhysObject & o = p.GetPhysObject();
    const StreamCoordinates & base = n.GetCoordinates();
    Coordinates cc;
    cc[0] = base[0]/m_scale;
    cc[1] = base[1]/m_scale;
    cc[2] = base[2]/m_scale;
    io.SetCoordsOffset(cc);
    io.Read(o, n.GetPhysics());    
  
    m_props.push_back(p);
  }
}

void GameControl::AddObject(const AnimatedSceneNode & n)
{
  AnimProp p;
  p.GetAnimNode() = n;
  p.SetScale(m_scale);

  if (n.GetPhysics() != "") {
    PhysicsIO io;
    PhysObject & o = p.GetPhysObject();
    const StreamCoordinates & base = n.GetCoordinates();
    Coordinates cc;
    cc[0] = base[0]/m_scale;
    cc[1] = base[1]/m_scale;
    cc[2] = base[2]/m_scale;
    io.SetCoordsOffset(cc);
    io.Read(o, n.GetPhysics());    
 
    m_objects.push_back(p);
  }
}

void GameControl::AddCompound(const svec<AnimatedSceneNode> & all)
{
  int i;
  
  Compound comp;

  for (i=0; i<all.isize(); i++) {
    const AnimatedSceneNode & n = all[i];
    AnimProp p;
    p.GetAnimNode() = n;
    p.SetScale(m_scale);

    if (n.GetPhysics() != "") {
      PhysicsIO io;
      PhysObject & o = comp.GetPhysObject();
      const StreamCoordinates & base = n.GetCoordinates();
      Coordinates cc;
      cc[0] = base[0]/m_scale;
      cc[1] = base[1]/m_scale;
      cc[2] = base[2]/m_scale;
      io.SetCoordsOffset(cc);
      io.Read(o, n.GetPhysics());    
    }
    comp.push_back(p);
    m_animInComp++;
  }

  m_compounds.push_back(comp);
}

void GameControl::Start()
{
  m_lastTime = m_clock.GetSec();
}

bool GameControl::CheckCollision(PhysObject & o)
{
  int i;
  bool b = false;
  for (i=0; i<m_triangles.isize(); i++) {
    if (m_triangles[i].Collide(o)) {
      b = true;
      cout << "COLLISION!" << endl;
    }
  }
  return b;
}


void GameControl::Run()
{
  m_clock.WaitUntilNextFrame();
  double deltatime = m_clock.GetSec() - m_lastTime;
  int i;
  
  for (i=0; i<m_props.isize(); i++) {
    PhysObject & o = m_props[i].GetPhysObject();
    CheckCollision(o);
    m_props[i].Update(deltatime, m_gravity);
  }
  for (i=0; i<m_objects.isize(); i++) {
    PhysObject & o = m_objects[i].GetPhysObject();
    CheckCollision(o);
    m_objects[i].Update(deltatime, m_gravity);
  }
  for (i=0; i<m_compounds.isize(); i++) {
    PhysObject & o = m_compounds[i].GetPhysObject();
    CheckCollision(o);
    m_compounds[i].Update(deltatime, m_gravity);
  }


  m_lastTime = m_clock.GetSec();
}
