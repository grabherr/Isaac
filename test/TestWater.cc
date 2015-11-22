
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"

#include "base/RandomStuff.h"
#include <unistd.h>


class MusicManipulator : public IManipulator
{
public:
  MusicManipulator() {
    m_time = 0.;
  }
  virtual ~MusicManipulator() {
  }
  void SetSound(const string & s) {
    m_sound = s;
  }
  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    m_time += deltatime;

    //if (m_time < 2.)
    //return;

    PhysObject & p = o.GetPhysObject();
    PhysMinimal & m = p.GetCenterDirect();


    const string & name = o.GetName();
    Sound & sound = p.GetSound();
    cout << "NAME: " << name << endl;
    sound.UpdateAdd(name, 
                    m_sound,
                    m.GetPosition());


  
  }
  
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
  }
  
  private:
  string m_sound;
  double m_time;
};




class WaterManipulator : public IManipulator
{
public:
  WaterManipulator() {}
  virtual ~WaterManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {}

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
    PhysObject & p = o.GetPhysObject();
    cout << "Call UPDATE" << endl;
    bool bFirst = false;
    if (m_v.isize() == 0) {
      m_v.resize(p.isize(), 0.);
      bFirst = true;
    }
    int i;
    double scale = 150.;
    deltatime *= 15.;
    
    for (i=0; i<p.isize(); i++) {        
      PhysMinimal & min = p[i];
      Coordinates c = min.GetPosition();

      if (bFirst) {
	c[1] = 25*(RandomFloat(1.)-0.5) / scale;
	min.SetPosition(c);	 
	continue;
      }

      double a = -c[1];
      m_v[i] += a*deltatime;
      double delta = 13*(RandomFloat(1.)-0.5)/scale;
      c[1] += (m_v[i]+delta)*deltatime;
      min.SetPosition(c);
	 
      cout << "Wave " << i << " " << c[1];
      cout << " " << c[0] << " " << c[2] << " ";
      cout << " delta " << delta << endl;
    }


  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
 
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  svec<double> m_v;
};

//===============================================
int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models (water).");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoTriangleCollision(false);
  eng.DoObjectCollision(false);

  int i, j;




  MsgSceneNode triMesh;
  SceneNodeMaterial & material = triMesh.Material(0);
  SceneNodeMeshPhysics & mesh = triMesh.Mesh(0);

  //triMesh.

  WaterManipulator waterManip, waterManip2;

  MTriangleMesh meshMaker;
  meshMaker.SetBoundaries(0, 0, 20, 20, 1.);
  meshMaker.GetMesh(mesh);

  material.SetTexture("data/Textures/water1.jpg");

  //material.SetLighting(true);
  //material.SetTransparent(100.);
  //material.SetShinyness(30.);
 

  //mesh.AbsCoords() = Coordinates(1400, 1400, 1400);
  double off = 2000;
  triMesh.SetPosition(Coordinates(off+1800, 400, off+1800));
  triMesh.SetName("triangles");
  triMesh.SetScale(200);
  mesh.SetPhysMode(1);
  triMesh.SetPhysMode(1);

  eng.AddSceneNode(triMesh, &waterManip);

  //mesh.AbsCoords() = Coordinates(2900, 1400, 1400);
  triMesh.SetPosition(Coordinates(off+3200, 400, off+1800));
  triMesh.SetName("triangles2");
  eng.AddSceneNode(triMesh, &waterManip2);
  
  cout << "Print triangle mesh" << endl;
  for (int i=0; i<mesh.VertexCount(); i++) {
    mesh.GetVertex(i).Print();
    //mesh.GetTextCoord(i).Print();
  }
  cout << "Indices:" << endl;
  for (int i=0; i<mesh.IndexCountTotal(); i++) {
    if (i % 3 == 0)
      cout << endl;
    cout << mesh.GetIndexTotal(i) << endl;
  }

  MsgLightNode light;
  light.SetRadius(2000000);
  light.SetPosition(StreamCoordinates(2000, 1600, 1400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2000, 600, 1400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(1400, 1600, 1400));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2400, 1800, 1400));
  eng.AddLight(light);
  
  AnimatedSceneNode anim;
  anim.SetCoordinates(StreamCoordinates(5800, 30, 6400));
  anim.SetTexture("data/Models/iTrees-02_Bark-WillowOld-02.jpg", 0);
  anim.SetTexture("data/Models/iTrees-02_Bark-WillowOld-02.jpg", 1);
  anim.SetTexture("data/Models/iTrees-02_Bark-WillowOld-02.jpg", 2);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 7);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 6);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 5);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 4);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 8);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 9);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 10);
  anim.SetTexture("data/Models/itrees-02-leaf_002.jpg", 11);
  anim.SetModel("data/Models/Elm5.3ds");
  anim.SetName("tree");
  anim.SetPhysMode(2);
  anim.SetScale(10);
  eng.AddAnimatedModel(anim);

  AnimatedSceneNode anim2;
  MusicManipulator manipS, sWater1, sWater2;
  manipS.SetSound("data/Sounds/wind.wav");
  anim2.SetCoordinates(StreamCoordinates(5800, 30, 6400));
  anim2.SetTexture("data/Models/red.jpg");
  anim2.SetModel("data/Models//ball.ms3d");
  anim2.SetScale(3);
  anim2.SetPhysMode(2);
  eng.AddAnimatedModel(anim2, &manipS);

  sWater1.SetSound("data/Sounds/water1.wav");
  sWater2.SetSound("data/Sounds/water2.wav");
  anim2.SetCoordinates(StreamCoordinates(2800, 300, 2400));
  eng.AddAnimatedModel(anim2, &sWater1);
  
  anim2.SetCoordinates(StreamCoordinates(3800, 300, 3400));
  eng.AddAnimatedModel(anim2, &sWater2);


  //sleep(10);
 
  eng.Run();


  return 0;
}
