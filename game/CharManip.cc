#include "game/CharManip.h"
#include <math.h>


void CharMovement::MoveSkeleton(NPCSkeleton &skeleton, double deltatime)
{
  m_time += deltatime;
  
  // Walk
  if (m_state == 0) {
    double x = 0.5*sin(m_time*(3+m_speed));
    
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

    
    skeleton.MoveTowards(1, x, deltatime);
    skeleton.MoveTowards(3, left, deltatime/10);
    skeleton.MoveTowards(4, right, deltatime/10);
    m_temp = 0.;
  }
  if (m_state == 1) {
    //skeleton.MoveTowards(5, 1, deltatime/10);
    skeleton.MoveTowards(5, m_temp, deltatime/10);
    m_temp += deltatime;
    if (m_temp >= 2.) {
      //m_temp = 0.;
      m_state = 2;
    }
  }
  if (m_state == 2) {
    skeleton.MoveTowards(5, m_temp, deltatime/10);
    m_temp -= deltatime;
    if (m_temp < 0.) {
      m_temp = 0.;
      m_state = 1;
    }
  }
  if (m_state == 3) {
    //skeleton.MoveTowards(5, 1, deltatime/10);
    skeleton.MoveTowards(6, m_temp, deltatime/10);
    m_temp += deltatime;
    if (m_temp >= 2.) {
      //m_temp = 0.;
      m_state = 4;
    }
  }
  if (m_state == 4) {
    skeleton.MoveTowards(6, m_temp, deltatime/10);
    m_temp -= deltatime;
    if (m_temp < 0.) {
      m_temp = 0.;
      m_state = 0;
    }
  }
}



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

  m_movement.MoveSkeleton(m_skeleton, deltatime);
  
  /*
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
  
  if (m_status == 0) {
    m_skeleton.MoveTowards(1, x, deltatime);
    m_skeleton.MoveTowards(3, left, deltatime/10);
    m_skeleton.MoveTowards(4, right, deltatime/10);
  }
  if (m_status == 1) {
    bool b = m_skeleton.MoveTowards(5, 1, deltatime/10);
    if (!b)
      m_status = 2;
  }
    
  if (m_status == 2) {
    bool b = m_skeleton.MoveTowards(5, 0, deltatime/10);
    // if (!b)
      
   
  }
  */

  
   //m_skeleton.MoveTowards(m_index, x, deltatime);
  m_skeleton.Update(deltatime);
  
  //m_rot = 0.1*deltatime;
  //m_skeleton.RotateAll(Coordinates(1, 0, 1), deltatime);
  
  
  //if (m_frame % 100 == 0) {
  // m_skeleton.Write(m_save);
  // cout << "SAVED!!" << endl;
  //}
  
  //double theMove = m_skeleton.GetNerves()[m_index].GetMove();
  //string name = m_skeleton.GetNerves()[m_index].GetName();
  
  //sprintf(msg, "Nerve: %d (%s); move=%f;\n", m_index, name.c_str(), theMove);
  
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
  m_headPos[1] += m_headPlus;
  m_headRot = m_skeleton.RelRot() + node.GetRotation();
  m_headRot[1] -= PI_P;

  char msg[1024];
  sprintf(msg, "Character: %s; pos=(%f, %f, %f);\nTarget: %s -> %f", m_name.c_str(), m_headPos[0], m_headPos[1], m_headPos[2],
	  m_tName.c_str(), m_tAct);
 

  
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  //if (m_thinkTime > 0.2) {
  double relPhi = 0.;
  double input;
  double score = GetMilkScore(input, m_lastRelPos, newPos, m_itemPos, m_currRot);

  double dist_to_target = (newPos - m_itemPos).Length();
  if (dist_to_target < 30) {
    m_status = 1;
    m_movement.SetState(1);
    char msg1[1024];
    sprintf(msg1, "Pick up item, dist: %f;\n", dist_to_target);
    strcat(msg, msg1);
  }

  double oldRot = m_currRot;
  //double fac = 0.5;
  //m_score = fac*m_score + (1-fac)*score;
  if (m_status == 0) {
    IOEntity ent;
    ent.resize(1, 1, 1);
    ent.in(0) = input;
    //ent.score(0) = m_score;
    
    //m_top.Update(ent, m_thinkTime, m_score*2);
    m_top.Update(ent, 0.6, score);
    double a = ent.out(0)*0.05;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    a = 0.5*(input - m_currRot)/PI_P;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    a *= PI_P;
    
    
    m_currRot += a;
    if (m_currRot < -PI_P)
      m_currRot += 2*PI_P;
    if (m_currRot > PI_P)
      m_currRot -= 2*PI_P;
    
    // m_currRot += m_thinkTime*ent.out(0);
    
    m_thinkTime = 0.;
    
    //char msg1[1024];
    //sprintf(msg1, "Rot: %f; Rel. phi: %f; score=%f dist=%f;\n", m_currRot, relPhi, m_score, dist_to_target);
    //strcat(msg, msg1);
    
    m_lastRelPos = newPos;
    cout << "SCORE " << m_score << endl;
  }
  
  node.ReSetCamPosition();
  if (m_tagMe) {
    // ONLY IF YOU HAVE THE FOCUS!!!
    cout << "MESH SCALE " << p.GetMeshScale() << endl;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // HARD CODED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    m_headPos[1] += 4;
    node.SetCamPosition(m_headPos*20);
    m_headPos[1] += 6;
    StreamCoordinates deltaRot;
    deltaRot[1] = m_currRot - oldRot;
    node.SetCamRotationDelta(deltaRot);
    node.SetMessage(msg);
  }

 
    //}
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------
  
  
  node.Mesh(0) = phys;
  m_lastKey = m_key;
  m_key = "";
}

double CharManipulator::GetMilkScore(double & relPhi,
				     const Coordinates & oldPos,
				     const Coordinates & realPos,
				     const Coordinates & target,
				     double currRot)
{
  /* Coordinates rel = m_itemPos - realPos;
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

  return score;*/
  
  Coordinates rel = target - realPos;
  rel[1] = 0.;
  
  Coordinates relOld = target - oldPos;
  relOld[1] = 0.;

  double score = relOld.Length() - rel.Length();
  SphereCoordinates s = rel.AsSphere();
  //s.phi() += PI_P/2;

  //cout << "MILK " << target[0] << " " << target[2] << " maxl " << realPos[0] << " " << realPos[2] << " was " << oldPos[0] << " " << oldPos[2] << " phi " << currRot << " phi targ " << s.phi() << endl; 
  //cout << "MILK2 " << rel.Length() << " " << relOld.Length() << endl;

  //relPhi = s.phi()/PI_P;

  
  relPhi = (s.phi()-currRot)/PI_P;
  while(relPhi > 1) {
    relPhi -= 2;
  }
  while (relPhi < -1) {
    relPhi += 2;
  }


  
  return score;

}
