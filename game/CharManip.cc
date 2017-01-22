#include "game/CharManip.h"
#include <math.h>

void CharManipulator::Update(GamePhysObject & o, double deltatime) {
  int i;

  m_frame++;

  m_time += deltatime;
  m_thinkTime += deltatime;
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
  
  
  //if (m_frame % 100 == 0) {
  // m_skeleton.Write(m_save);
  // cout << "SAVED!!" << endl;
  //}
  
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

  //m_currRot -= deltatime*0.1;
  node.SetRotation(Coordinates(0, 1, 0)*m_currRot*-1);
  //node.SetRotation(rr+Coordinates(0, 1, 0)*deltatime*0.2);
  
  PhysObject & p = o.GetPhysObject();
  PhysMinimal & m = p.GetCenterDirect();
  Coordinates pp = m.GetPosition();
  if (m_frame < 5) {
    m_basePos = pp;
    m_realPos =  m_skeleton.AbsPos();
    m_lastCheck = m_skeleton.AbsPos();
    m_lastRelPos = m_realPos + m_basePos;
  }
  Coordinates deltaPos = m_skeleton.AbsPos() - m_lastCheck;
  double zz = deltaPos[1];
  deltaPos[1] = 0.;
  SphereCoordinates sTmp = deltaPos.AsSphere();
  sTmp.phi() += m_currRot;
  deltaPos.FromSphere(sTmp);
  deltaPos[1] = zz;
  Coordinates oldPos = m_basePos+m_realPos;
  m_realPos += deltaPos;
  Coordinates newPos = m_basePos+m_realPos;
  
  m.SetPosition(m_basePos+m_realPos);
  //m.SetPosition(m_basePos+m_skeleton.AbsPos());

  m_lastCheck = m_skeleton.AbsPos();
    
  //m_headPos = m_skeleton[1].GetCoordsPlusDelta()+m_basePos+m_skeleton.AbsPos();
  m_headPos = m_skeleton[1].GetCoordsPlusDelta()+m_basePos+m_realPos;
  m_headPos[1] += 6.8;
  m_headRot = m_skeleton.RelRot() + node.GetRotation();
  m_headRot[1] += 0.6;
  
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  if (m_thinkTime > 0.2) {
    double relPhi = 0.;
    double score = GetMilkScore(relPhi, m_lastRelPos, newPos)/10;
    double fac = 0.5;
    m_score = fac*m_score + (1-fac)*score;
    IOEntity ent;
    ent.resize(1, 1, 1);
    ent.in(0) = relPhi;
    //ent.score(0) = m_score;
    
    //m_top.Update(ent, m_thinkTime, m_score*2);
    m_top.Update(ent, 0.6, m_score);
    m_currRot += m_thinkTime*ent.out(0);
    
    m_thinkTime = 0.;

    char msg1[1024];
    sprintf(msg1, "Rot: %f; Rel. phi: %f; score=%f;\n", m_currRot, relPhi, m_score);
    strcat(msg, msg1);
    m_lastRelPos = newPos;
    cout << "SCORE " << m_score << endl;
  }
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  
  node.SetMessage(msg);
  
  node.Mesh(0) = phys;
  m_lastKey = m_key;
  m_key = "";
}

double CharManipulator::GetMilkScore(double & relPhi,
				     const Coordinates & oldPos,
				     const Coordinates & realPos)
{
  Coordinates rel = m_itemPos - realPos;
  rel[1] = 0.;
  
  Coordinates relOld = m_itemPos - oldPos;
  relOld[1] = 0.;

  double score = relOld.Length() - rel.Length();
  SphereCoordinates s = rel.AsSphere();
  s.phi() += PI_P/2;

  cout << "MILK " << m_itemPos[0] << " " << m_itemPos[2] << " maxl " << realPos[0] << " " << realPos[2] << " was " << oldPos[0] << " " << oldPos[2] << " phi " << m_currRot << " phi targ " << s.phi() << endl; 
  

  relPhi = (s.phi()-m_currRot)/PI_P;
  while(relPhi > 1) {
    relPhi -= 2;
  }
  while (relPhi < -1) {
    relPhi += 2;
  }

  return score;
  
}
