#include "physics/Physics.h"

void PhysObject::Fixate()
{
  m_center.Clear();
  int i, j;
  double weight = 0;
  for (i=0; i<m_objects.isize(); i++) {
    weight += m_objects[i].GetMass();
  }
  m_center.SetMass(weight);
  for (i=0; i<m_objects.isize(); i++) {
    const PhysMinimal & p = m_objects[i];
    for (j=0; j<p.GetPosition().isize(); j++) {
      (m_center.Position())[j] += (p.GetPosition())[j]*p.GetMass()/weight;
      (m_center.Velocity())[j] += (p.GetVelocity())[j]*p.GetMass()/weight;
    }
   
  }
}

void PhysObject::Impulse(int index, const Coordinates & direction)
{
  int i;

  PhysMinimal & p = m_objects[index];
  Coordinates rel = p.GetPosition();
  rel -= m_center.GetPosition();
  
  Coordinates r = direction.Rotate(rel);
  cout << "Rot " << r[0] << " " << r[1] << " " << r[2] << endl;
  cout << "centrifugal: " << r[0] << endl;
  double centrifugal = r[0];
  r[0] = 0.;

  Coordinates rr = r.RotateBack(rel);
  cout << "lateral: " << rr[0] << " " << rr[1] << " " << rr[2] << endl;

  Coordinates newVel = m_center.GetVelocity();
  for (i=0; i<newVel.isize(); i++) {
    newVel[i] += direction[i]*centrifugal/m_center.GetMass();
  }
  m_center.SetVelocity(newVel);

  newVel = p.GetVelocity();
  for (i=0; i<newVel.isize(); i++) {
    newVel[i] += rr[i]/p.GetMass();
  }

  double rad = sqrt(rel.Scalar(rel));
  Coordinates diff = newVel;
  diff -= p.GetVelocity();
  p.SetVelocity(newVel);


  // WRONG!!!!
  //m_rotspeed[0] += sqrt(diff[1]*diff[1]+diff[2]*diff[2])/rad*2*3.1415;
  //m_rotspeed[1] += sqrt(diff[0]*diff[0]+diff[2]*diff[2])/rad*2*3.1415;
  //m_rotspeed[2] += sqrt(diff[1]*diff[1]+diff[0]*diff[0])/rad*2*3.1415;

  m_rotspeed = diff.CrossProduct(rel);
  cout << "Diff  " << diff[0] << " " << diff[1] << " " << diff[2] << endl;
  cout << "Rel   " << rel[0] << " " << rel[1] << " " << rel[2] << endl;
  cout << "Cross " << m_rotspeed[0] << " " << m_rotspeed[1] << " " << m_rotspeed[2] << endl;

  //for (i=0; i<newVel.isize(); i++) {
  //  m_rotspeed[i] += diff[i]/rad*2*3.1415;
  //}

  /*  int i;
  PhysMinimal & p = m_objects[index];
  Coordinates rel = p.GetPosition();
  rel -= m_center.GetPosition();
  double r = sqrt(rel.Scalar(rel));
  double sc = rel.Scalar(direction);
  double sl = p.GetVelocity().Scalar(direction);
 

  Coordinates newVel = m_center.GetVelocity();
  
  //cout << r << " " << sc << " " << sl << endl;

  // Movement of the whole object
  for (i=0; i<newVel.isize(); i++) {
    newVel[i] += direction[i]*sc/m_center.GetMass();
    //cout << "Whole: " << direction[i]*sc/m_center.GetMass() << endl;
  }
  m_center.SetVelocity(newVel);
 
  newVel = p.GetVelocity();
  for (i=0; i<newVel.isize(); i++) {
    newVel[i] += direction[i]*sl/p.GetMass();
  }
  Coordinates diff = newVel;
  diff -= p.GetVelocity();
  p.SetVelocity(newVel);
  for (i=0; i<newVel.isize(); i++) {
    m_rotspeed[i] += diff[i]/r*2*3.1415;
    //cout << "Rot: " <<  diff[i]/r*2*3.1415 << endl;
    }*/
}

void PhysObject::Print() const
{
  int i;
  cout << "Center" << endl;
  m_center.Print();
  cout << "Objects" << endl;
  for (i=0; i<m_objects.isize(); i++)
    m_objects[i].Print();
  cout << endl;
}

void PhysObject::Update(double deltatime, double gravity)
{
  int i, j;
  Coordinates xp = m_center.GetPosition();

  double before = (m_center.Position())[2];
  for (j=0; j<xp.isize(); j++) {
    (m_center.Position())[j] += deltatime*(m_center.Velocity())[j];
  }
  // Gravity
  (m_center.Velocity())[2] -= gravity*deltatime;
  cout << "Gravity speed " << (m_center.Velocity())[2];
  cout << " pos " << (m_center.Position())[2];
  cout << " diff " << before - (m_center.Position())[2] << endl;

  
  // Move all objects according to the center
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    for (j=0; j<p.GetPosition().isize(); j++) {
      (p.Position())[j] += deltatime*(m_center.Velocity())[j];
      (p.Velocity())[j] += (m_center.Velocity())[j];
    }   
    //p.Print();
  }
  
  cout << "Rotation speed " <<  m_rotspeed[0] << " " << m_rotspeed[1] << " " << m_rotspeed[2] << endl;
  // Compute rotation
  for (i=0; i<m_objects.isize(); i++) {
    PhysMinimal & p = m_objects[i];
    //p.Print();
    //cout << "ABS POS" << endl;
    Coordinates relPos = p.GetPosition();
    //relPos.Print();
    relPos -= m_center.GetPosition();   
    //cout << "REL POS" << endl;
    //relPos.Print();

    double r;
    
    // Rotate around x
    r = sqrt(relPos[1]*relPos[1] + relPos[2]*relPos[2]);
    if (r > 0.) {
      double phi = acos(relPos[1]/r)*Sign(relPos[2]);
      phi += m_rotspeed[0]*deltatime;
      relPos[1] = r*cos(phi);
      relPos[2] = r*sin(phi);
    }
    //cout << "ROT-X" << endl;
    //relPos.Print();

    // Rotate around y
    r = sqrt(relPos[0]*relPos[0] + relPos[2]*relPos[2]);
    if (r > 0.) {
      double phi = acos(relPos[0]/r)*Sign(relPos[2]);
      phi += m_rotspeed[1]*deltatime;
      relPos[0] = r*cos(phi);
      relPos[2] = r*sin(phi);
    }
    //cout << "ROT-Y" << endl;
    //relPos.Print();
    

    // Rotate around z
    r = sqrt(relPos[0]*relPos[0] + relPos[1]*relPos[1]);
    //cout << "DEBUG: " << i << " r=" << r << " rot=" << m_rotspeed[2];
    if (r > 0.) {
      double phi = acos(relPos[0]/r)*Sign(relPos[1]);
      //cout << " phi=" << phi << " +/- " << m_rotspeed[2]*deltatime << " " << relPos[0] << "," << relPos[1] << "," << relPos[2] << " -> ";
      phi += m_rotspeed[2]*deltatime;
      
      if (phi < 0)
	phi += 2*PI_P;
      if (phi > 2*PI_P)
	phi -= 2*PI_P;
      

      relPos[0] = r*cos(phi);
      relPos[1] = r*sin(phi);
      //cout  << relPos[0] << "," << relPos[1] << "," << relPos[2];
      //cout << "DEBUG: phi=" << phi << " acos=" << acos(relPos[0]/r) << endl;
      
    }
    //cout << endl;

    //cout << "ROT-Z" << endl;
    //relPos.Print();

    relPos += m_center.GetPosition();
    Coordinates vel = relPos;
    // cout << "Velocity" << endl;
    //vel.Print();
    for (j=0; j<vel.isize(); j++) {
      vel[j] = (vel[j] - (p.GetPosition()[j]))/deltatime;
      //cout << "  i=" << j << vel[j] << " "<< (p.GetPosition()[j])/deltatime << endl;
    }
    //p.GetPosition().Print();
    //vel.Print();
    
    p.SetPosition(relPos);
    p.SetVelocity(vel);
  }
  
}
