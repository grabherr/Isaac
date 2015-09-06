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

void GamePhysObject::Update(double deltatime, double gravity)
{
  m_phys.Update(deltatime, gravity);
  if (m_pManip != NULL) {
    m_pManip->Update(*this, deltatime);
  }
  cout << "Rot imp for update: ";
  m_phys.GetRotImpulse().Print();
}

//====================================================
GameControl::GameControl() 
{
  m_animInComp = 0;
  m_gravity = 9.81;
  //m_gravity = 0.0;
  m_scale = 1.;
  m_bDoTriangleCollision = true;
  m_bDoObjectCollision = true;
 
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
  //m_testCube.SetRotImpulse(Coordinates(300, 2, 700));
  m_testCube.SetRotImpulse(Coordinates(0.5, 4.9, 20.));
  m_testCube.SetLatImpulse(Coordinates(0., 0., 0.));
 

  //m_testCube.SetRotImpulse(Coordinates(0, 20000, 10000));
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

void GameControl::RegisterCompound(IManipulator * p)
{
  m_custom.push_back(p);
}

void GameControl::AddMeshModel(const MeshModel & a, IManipulator * pManip)
{
  cout << "ADDING/UPDATING MESH MODEL!!!" << endl;
  int i;
  PhysMinimal min;
  min.SetMass(1.);

  PhysObject tmp;

  tmp.SetMeshScale(a.GetScale());
  
  cout << "AddMeshModel Print coordinates: " << endl;
  for (i=0; i<a.VertexCount(); i++) {
    Coordinates c = a.GetVertexConst(i) * a.GetScale() / m_scale;  // TEST
    c.Print();
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

    tmp.AddTriangleMapped(i0, i1, i2);
  }

  tmp.Center();

  GamePhysObject obj;
  obj.SetName(a.GetName());
  obj.SetManipulator(pManip);

  tmp.MoveTo(a.GetAbsCoords()/m_scale);
  cout << "AddMesh move to ";
  a.GetAbsCoords().Print();
  cout << "At scale " << m_scale << endl;
  tmp.Fixate();
  //a.GetAbsCoords().Print();

  PhysConnection all;
  // DEBUG!!!!!!!!!!!!!
  //tmp.ConnectToCenter(all);
  //tmp.ConnectWithin(all, 1.1);

  //tmp.Print();
  
  if (a.HasRotation())
    tmp.SetEngRotation(a.GetRotation());

  if (obj.GetName() == "cube") {
    cout << "ERROR, not adding " << obj.GetName() << endl;
    return;
  }

  tmp.SetRotImpulse(a.GetRotImp());
  cout << "Got rot impulse ";
  a.GetRotImp().Print();

  int mode = a.PhysMode();
  tmp.SetPhysMode(mode);
  if (mode == 1)
    tmp.SetElast(1);
  
  obj.GetPhysObject() = tmp;
  cout << "Adding mesh " << obj.GetName() << endl;
  int index = PhysIndex(a.GetName());
  if (index == -1) {
    m_phys.push_back(obj);
  } else {
    m_phys[index] = obj;
  }
  

}

void GameControl::AddProp(const SceneNode & n)
{
  SceneProp p;
  p.GetSceneNode() = n;
  p.SetScale(n.GetScale());
  if (n.GetPhysics() != "") {
    PhysicsIO io;
    PhysObject & o = p.GetPhysObject();
    o.SetMeshScale(n.GetScale());
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
  p.SetScale(n.GetScale());
  cout << "ADDING OBJECT!" << endl;

  if (n.GetPhysics() != "") {
    PhysicsIO io;
    PhysObject & o = p.GetPhysObject();
    o.SetMeshScale(n.GetScale());
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

  if (!m_bDoTriangleCollision)
    return false;

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

void GameControl::GetObjectModel(int index, MeshModel & m)
{
  const PhysObject & p = m_phys[index].GetPhysObject();
  m.SetName(m_phys[index].GetName());
  int i;
  
  const PhysMinimal & centerC = p.GetCenter();
  Coordinates cc = centerC.GetPosition();
  Coordinates cc1;
  cc1 = cc * m_scale; // TEST
  cout << "Send abs pos ";
  cc1.Print();

  Coordinates center = cc;
  m.AbsCoords() = cc1;
  m.SetPhysMode(p.GetPhysMode());
  m.SetAnimation(p.GetAnimation());
  m.SetTexture(p.GetTexture());
  m.SetInvisible(p.GetInvisible());
  cout << "INVISIBLE " << p.GetInvisible()[0] << endl;
  m.SetDirection(p.GetDirection());
  if (p.HasEngRot())
    m.SetRotation(p.GetEngRotation());

  // Do not send coordinates if rot imp is 0
  double rot = p.GetRotImpulse().Length();

  if (rot > 0.00001 && p.GetPhysMode() != 2) {
    cout << "Sending object coordinates. " << endl;
    for (i=0; i<p.MappedSize(); i++) {
      const PhysMinimal & min = p.GetMapped(i);
      cc = min.GetPosition() * m_scale / p.GetMeshScale(); //TEST 
      m.AddVertex(cc);
    }
  } else {
    cout << "NOT sending vertices." << endl;
  }
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
 
void GameControl::Run(const Coordinates & camPos)
{
  m_clock.WaitUntilNextFrame();
  double deltatime = m_clock.GetSec() - m_lastTime;
  int i, j;

  //CheckCollision(m_testCube);
  //m_testCube.Update(deltatime, m_gravity);

  
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

   
  //-----------------------------------------------
  // Tell them about each other (be smarter about this!!!)
  for (i=0; i<m_phys.isize(); i++) {
    m_phys[i].StartFeed();
    for (j=0; j<m_phys.isize(); j++) {
      if (i == j)
	continue;

      // TODO: Check whether in sight
      m_phys[i].Feed(m_phys[j]);
    }
    m_phys[i].DoneFeed();
    m_phys[i].CamPos(camPos / m_scale);
  }

  for (i=0; i<m_phys.isize(); i++) {
    PhysObject & o = m_phys[i].GetPhysObject();
    cout << "Updating model " << m_phys[i].GetName() << endl;
    cout << "Rot ";
    o.GetRotImpulse().Print();
    CheckCollision(o);
    
    cout << "Num objects: " << m_phys.isize() << endl;
    for (j=0; j<m_phys.isize(); j++) {
      if (i == j)
	continue;

      //================================
      m_phys[i].Interact(m_phys[j]);
      //================================

      if (m_bDoObjectCollision && m_phys[j].GetPhysObject().DoesCollide(o)) {
	cout << "Objects " << i << " and " << j << " collide." << endl;
	o.GetCenter().GetPosition().Print();
 	m_phys[j].GetPhysObject().GetCenter().GetPosition().Print();
      }
    }

    m_phys[i].Update(deltatime, m_gravity);
  }


  m_lastTime = m_clock.GetSec();
}
