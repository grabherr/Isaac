#include "npc/AntManipulator.h"
#include "base/RandomStuff.h"
#include "physics/Physics.h"
#include "engine/GameEngine.h"

AntManipulator::AntManipulator()
{
  m_rotY = 0;
  m_time = 0.;
  m_anim = true;
  m_elapsed = 0.;
  m_counter = 0;
  m_dist = 0;
  m_animname = "walk";
}

void AntManipulator::Update(GamePhysObject & o, double deltatime) 
{
  
  m_time += deltatime;

  PhysObject & p = o.GetPhysObject();
  double mass = p.GetTotalMass();
  PhysMinimal & m = p.GetCenterDirect();
  MsgSceneNode & msn = o.MessageSceneNode();

  m_elapsed += deltatime;

  msn.SetName(m_name);


  /*if (RandomFloat(1.) < 0.001) {
    m_anim = true;
  }
  if (RandomFloat(1.) < 0.001) {
    m_anim = false;
    }*/

  if (m_elapsed > .1) {
    m_counter++;
    if (m_counter == 4)
      m_counter = 0;
    m_elapsed = 0.;
  }

  if (!m_anim) {  
    if (m_counter == 0) {
      o.MessageSceneNode().SetModel("data/Models/ant_1.ms3d");
    }
    if (m_counter == 1) {
      o.MessageSceneNode().SetModel("data/Models/ant_2.ms3d");
    }
    if (m_counter == 2) {
      o.MessageSceneNode().SetModel("data/Models/ant_3.ms3d");
    }
    if (m_counter == 3) {
      o.MessageSceneNode().SetModel("data/Models/ant_0.ms3d");    
    }
    msn.Animation().SetAnimation("");
  } else {
    
    //msn.Animation().SetAnimation("");
    if (RandomFloat(1.) < 0.01) {
      m_animname = "hej";     
    }
    if (RandomFloat(1.) < 0.01) {
      m_animname = "walk";     
    }
    msn.SetModel(m_model);
    msn.Animation().SetAnimation(m_animname);
    msn.Animation().SetSpeed(60);
    cout << "Set animation " << msn.Animation().GetAnimation () << " for model " << msn.GetModel() << " name " << msn.GetName() << endl;
  }


  if (m_center[0] == 0.)
    m_center = m.GetPosition();
  m_lastPos = m.GetPosition();
  
  cout << "Manipulator y=" << m.GetPosition()[1] << endl;
  double up = 1.;
  if (m.GetPosition()[1] > 100.) {
    up = -1.;
  }
  double v = 5. * mass * deltatime;
  Coordinates l = p.GetLatImpulse();
  l[1] += v * up;
  //p.SetLatImpulse(l);
  
  double phi = m.GetPosition()[1] / 20.;
  double x = 10. * cos(phi);
  double z = 13. * sin(phi);
  Coordinates update = m_center + Coordinates(x, m.GetPosition()[1], z);
  
  //m_rotY += deltatime;
  
  
  cout << "Rot " << m_rotY << endl;
  double plus = 0.;
  if (m_anim)
    plus = PI_P/2.;
  
  msn.SetRotation(StreamCoordinates(0, m_rotY+plus, 0));
 
  //update[1] = m.GetPosition()[1];
  //m.SetPosition(update);
  cout << "Manipulator " << p.GetPhysMode() << endl;
  update.Print();
    
}

void AntManipulator::CamPos(GamePhysObject & self, const Coordinates & c) 
{
  PhysObject & p = self.GetPhysObject();
  PhysMinimal & m = p.GetCenterDirect();
  m_dist = (m.GetPosition() - c).Length();

}
