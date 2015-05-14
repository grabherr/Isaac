#include "engine/EngineCtrl.h"
#include "physics/PhysicsIO.h"


void SceneProp::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
}


void AnimProp::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
}

//====================================================
void GameControl::AddProp(const SceneNode & n)
{
  SceneProp p;
  if (n.GetPhysics() != "") {
    PhysicsIO io;
    PhysObject & o = p.GetPhysObject();
    const StreamCoordinates & base = n.GetCoordinates();
    Coordinates cc;
    cc[0] = base[0];
    cc[1] = base[1];
    cc[2] = base[2];
    io.SetCoordsOffset(cc);
    io.Read(o, n.GetPhysics());    
    m_props.push_back(p);
  }
}

void GameControl::AddAnimated(const AnimatedSceneNode & n)
{
  AnimProp p;
  if (n.GetPhysics() != "") {
    PhysicsIO io;
    PhysObject & o = p.GetPhysObject();
    const StreamCoordinates & base = n.GetCoordinates();
    Coordinates cc;
    cc[0] = base[0];
    cc[1] = base[1];
    cc[2] = base[2];
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
