#include "engine/EngineCtrl.h"
#include "physics/PhysicsIO.h"


void SceneProp::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
  const Coordinates & center = m_phys.GetCenter().GetPosition();
  // TODO: Get rotation
  Coordinates game;
  game = center * m_scale;
  m_node.SetCoordinates(game);

  Coordinates rot;
  m_phys.GetRotation(rot);
  game = rot;
  m_node.SetDirection(game);
}


void AnimProp::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
  const Coordinates & center = m_phys.GetCenter().GetPosition();
  // TODO: Get rotation
  Coordinates game;
  game = center * m_scale;
  m_anim.SetCoordinates(game);

  Coordinates rot;
  m_phys.GetRotation(rot);
  game = rot;
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
    game = center * m_scale;
    AnimatedSceneNode & anim = m_list[i].GetAnimNode();
    anim.SetCoordinates(game);
  }
}


//====================================================
GameControl::GameControl() 
{
  m_animInComp = 0;
  m_gravity = 9.81;
  //m_gravity = 0.0;
  m_scale = 1.;
 
  // Add bottom
  SolidTriangle t;
  double z = 0;
  t.Set(Coordinates(0, z, 0), 
	Coordinates(0, z, 1000), 
	Coordinates(1000, z, 0));

  t.SetElasticity(0.8);
  AddTriangle(t);

  t.Set(Coordinates(1000, z, 0), 
	Coordinates(1000, z, 1000), 
	Coordinates(0, z, 1000));

  t.SetElasticity(0.8);
  AddTriangle(t);


  // Add cube
  PhysMinimal min;
  min.SetMass(1.);
 
  // 0
  min.SetPosition(Coordinates(-0.5, -0.5, -0.5));
  //min.SetVelocity(Coordinates(1., 1., -2.));
  m_testCube.AddMapped(min);

  min.SetVelocity(Coordinates(0, 0, 0));

  // 1
  min.SetPosition(Coordinates(0.5, -0.5, -0.5));
  m_testCube.AddMapped(min);

  // 2
  min.SetPosition(Coordinates(0.5, 0.5, -0.5));
  m_testCube.AddMapped(min);

  // 3
  min.SetPosition(Coordinates(-0.5, 0.5, -0.5));
  m_testCube.AddMapped(min);

  // 4
  min.SetPosition(Coordinates(0.5, -0.5, 0.5));
  m_testCube.AddMapped(min);

  // 5
  min.SetPosition(Coordinates(0.5, 0.5, 0.5));
  //min.SetVelocity(Coordinates(-1., -1., 2.));
  m_testCube.AddMapped(min);

  min.SetVelocity(Coordinates(0., 0., 0.));

  // 6
  min.SetPosition(Coordinates(-0.5, 0.5, 0.5));
  m_testCube.AddMapped(min);

  // 7
  min.SetPosition(Coordinates(-0.5, -0.5, 0.5));
  m_testCube.AddMapped(min);

  //==========================================================
  // 6
  min.SetPosition(Coordinates(-0.5, 0.5, 0.5));
  m_testCube.AddMapped(min);
  // 3
  min.SetPosition(Coordinates(-0.5, 0.5, -0.5));
  m_testCube.AddMapped(min);
  // 4
  min.SetPosition(Coordinates(0.5, -0.5, 0.5));
  m_testCube.AddMapped(min);
  // 1
  min.SetPosition(Coordinates(0.5, -0.5, -0.5));
  m_testCube.AddMapped(min);


  PhysConnection all;
  m_testCube.ConnectAll(all);
  
  /*m_testCube.Connect(PhysConnection(0, 2));
  m_testCube.Connect(PhysConnection(0, 3));
  m_testCube.Connect(PhysConnection(1, 5));
  m_testCube.Connect(PhysConnection(1, 2));
  m_testCube.Connect(PhysConnection(4, 6));
  m_testCube.Connect(PhysConnection(4, 5));
  m_testCube.Connect(PhysConnection(7, 3));
  m_testCube.Connect(PhysConnection(7, 6));
  m_testCube.Connect(PhysConnection(3, 5));
  m_testCube.Connect(PhysConnection(3, 6));
  m_testCube.Connect(PhysConnection(0, 1));
  m_testCube.Connect(PhysConnection(0, 4));
  m_testCube.Connect(PhysConnection(2, 3));
  m_testCube.Connect(PhysConnection(2, 5));
  m_testCube.Connect(PhysConnection(5, 6));
  
  m_testCube.Connect(PhysConnection(0, 5));
  m_testCube.Connect(PhysConnection(1, 6));
  m_testCube.Connect(PhysConnection(2, 7));
  */

  m_testCube.Fixate();
  m_testCube.MoveTo(Coordinates(350, 170, 350));

  m_testCube.Fixate();
  m_testCube.SetRotImpulse(Coordinates(0, 20000, 10000));
  //0 pos:  -0.5 -0.5 -0.5
  //1 pos:  0.5 -0.5 -0.5
  //2 pos:  0.5 0.5 -0.5
  //3 pos:  -0.5 0.5 -0.5
  //4 pos:  0.5 -0.5 0.5
  //5 pos:  0.5 0.5 0.5
  //6 pos:  -0.5 0.5 0.5
  //7 pos:  -0.5 -0.5 0.5
  //6 8 pos:  -0.5 0.5 0.5
  //3 9 pos:  -0.5 0.5 -0.5
  //4 10 pos:  0.5 -0.5 0.5
  //1 11 pos:  0.5 -0.5 -0.5




  Start();
}

void GameControl::AddMeshModel(const MeshModel & a)
{
  cout << "ADDING MESH MODEL!!!" << endl;
  int i;
  PhysMinimal min;
  min.SetMass(1.);

  PhysObject tmp;

  for (i=0; i<a.VertexCount(); i++) {
    const StreamCoordinates & c = a.GetVertexConst(i);
    min.SetPosition(c);
    tmp.AddMapped(min);
  }

  
  for (i=0; i<a.IndexCount(); i++) {
    int i0 = a.GetIndexConst(i, 0);
    int i1 = a.GetIndexConst(i, 1);
    int i2 = a.GetIndexConst(i, 2);
    cout << i << "\t" << i0 << " " << i1 << " " << i2 << endl;
    tmp.ConnectMapped(PhysConnection(i0, i1));
    tmp.ConnectMapped(PhysConnection(i0, i2));
    tmp.ConnectMapped(PhysConnection(i1, i2));
  }
  
  PhysConnection all;
  // DEBUG!!!!!!!!!!!!!
  tmp.ConnectAll(all);
  m_phys.push_back(tmp);
  

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
    cc = base / m_scale;
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
    cc = base / m_scale;
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
      cc = base / m_scale;
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

  
  //o.Bounce(j, up);
   
  
  for (i=0; i<m_triangles.isize(); i++) {
    if (m_triangles[i].Collide(o)) {
      b = true;
      cout << "COLLISION!" << endl;
    }
  }
  return b;
}

void GameControl::GetCubeModel(MeshModel & m)
{
  m.SetName("cube");
  int i;
  
  const PhysMinimal & centerC = m_testCube.GetCenter();
  Coordinates cc = centerC.GetPosition();
  Coordinates cc1;
  cc1 = cc * m_scale;
  Coordinates center = cc;
  m.AbsCoords() = cc1;

  cout << "Sending cube coordinates. " << endl;
  for (i=0; i<m_testCube.MappedSize(); i++) {
    const PhysMinimal & min = m_testCube.GetMapped(i);
    cc = min.GetPosition(); 
    m.AddVertex(cc);
  }
  /*
  cc = m_testCube[6].GetPosition();
  m.AddVertex(cc);

  cc = m_testCube[3].GetPosition();
  m.AddVertex(cc);

  cc = m_testCube[4].GetPosition();
  m.AddVertex(cc);

  cc = m_testCube[1].GetPosition();
  m.AddVertex(cc);
  */
}
 
void GameControl::Run()
{
  m_clock.WaitUntilNextFrame();
  double deltatime = m_clock.GetSec() - m_lastTime;
  int i;

  CheckCollision(m_testCube);
  m_testCube.Update(deltatime, m_gravity);

  
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
