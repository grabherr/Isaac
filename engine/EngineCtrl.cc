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
}

//====================================================
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

void GameControl::AddAnimated(const AnimatedSceneNode & n)
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
    m_anim.push_back(p);
  }
}

void GameControl::Start()
{
  m_lastTime = m_clock.GetSec();
}

void GameControl::Run()
{
  m_clock.WaitUntilNextFrame();
  double deltatime = m_clock.GetSec() - m_lastTime;
  int i;
  
  for (i=0; i<m_props.isize(); i++) 
    m_props[i].Update(deltatime, m_gravity);
  for (i=0; i<m_anim.isize(); i++) 
    m_anim[i].Update(deltatime, m_gravity);


  m_lastTime = m_clock.GetSec();
}
