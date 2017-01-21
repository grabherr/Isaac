#include "game/CharManip.h"
#include <math.h>

void CharManipulator::Update(GamePhysObject & o, double deltatime) {
  int i;

  m_frame++;

  m_time += deltatime;
  
  int offset = 15;

  if (m_moves.isize() == 0)
    m_moves.resize(m_skeleton.GetNerves().isize(), 0.);
    
  //double angle = 0.02;
  double angle = 0.04;
  double x = 0.;
  if (m_key == "B") {
    x = angle;
  }
  if (m_key == "G") {
    x = -angle;
  }
  if (m_key == "TAB" && m_key != m_lastKey) {
    m_index++;
    if (m_index >= m_skeleton.GetNerves().isize())
      m_index = 0;
  }
  if (m_key == "BACK" && m_key != m_lastKey) {
    m_index--;
    if (m_index < 0)
      m_index = m_skeleton.GetNerves().isize()-1;
    
  }
  cout << "Nerves: " << m_skeleton.GetNerves().isize() << endl;

  //for (i=0; i<m_moves.isize(); i++) {
  //m_skeleton.Move(i, m_moves[i]);
  //}

  x = 0.5*sin(m_time*3);

  double left = 0.2*(1+sin(m_time*5));
  double right = 0.2*(1-sin(m_time*5));

  if (x < m_lastVal) {
    right = 0.;
    left = 0.6;
  } else {
    right = 0.6;
    left = 0.;
  }
  m_lastVal = x;
  
  /* if (m_moves[1] > 0.)
    m_moves[1] = 0.02;
  else
    m_moves[1] = -0.02;
  */
  //m_skeleton.MoveTowards(0, 20.3, deltatime);
  m_skeleton.MoveTowards(1, x, deltatime);
  m_skeleton.MoveTowards(3, left, deltatime/10);
  m_skeleton.MoveTowards(4, right, deltatime/10);
 
   //m_skeleton.MoveTowards(m_index, x, deltatime);
  m_skeleton.Update(deltatime);
  
  //m_rot = 0.1*deltatime;
  //m_skeleton.RotateAll(Coordinates(1, 0, 1), deltatime);
  
  
  if (m_frame % 100 == 0) {
    m_skeleton.Write(m_save);
    cout << "SAVED!!" << endl;
  }
  
  char msg[1024];
  double theMove = m_skeleton.GetNerves()[m_index].GetMove();
  string name = m_skeleton.GetNerves()[m_index].GetName();
  
  sprintf(msg, "Nerve: %d (%s); move=%f;\n", m_index, name.c_str(), theMove);
  
  SceneNodeMeshPhysics phys;
  MSkeleton makeSkeleton;
  makeSkeleton.MakeSkeleton(phys, m_skeleton);
  MsgSceneNode & node = o.MessageSceneNode();
  
  StreamCoordinates rr = node.GetRotation();
  node.SetRotation(rr+m_skeleton.RelRot());
  //node.SetRotation(rr+Coordinates(0, 1, 0)*deltatime*0.2);
  
  PhysObject & p = o.GetPhysObject();
  PhysMinimal & m = p.GetCenterDirect();
  Coordinates pp = m.GetPosition();
  if (m_frame < 5) {
    m_basePos = pp;
  }
  m.SetPosition(m_basePos+m_skeleton.AbsPos());
  
  m_headPos = m_skeleton[1].GetCoordsPlusDelta()+m_basePos+m_skeleton.AbsPos();
  m_headPos[1] += 6.8;
  m_headRot = m_skeleton.RelRot() + node.GetRotation();
  m_headRot[1] += 0.6;
  
  
  node.SetMessage(msg);
  
  node.Mesh(0) = phys;
  m_lastKey = m_key;
  m_key = "";
}

