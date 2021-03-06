
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"

#include "base/RandomStuff.h"



class MusicManipulator : public IManipulator
{
public:
  MusicManipulator() {}
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
};




class WaterManipulator : public IManipulator
{
public:
  WaterManipulator() {
    m_time = 0.;
  }
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
  double m_time;
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

  double off = 2000;

  MsgLightNode light;
  light.SetRadius(2000000);
  light.SetPosition(StreamCoordinates(2000+off, 1600, 1400+off));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2000+off, 600, 1400+off));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(1400+off, 3600, 2000+off));
  eng.AddLight(light);
  light.SetPosition(StreamCoordinates(2400+off, 3800, 2000+off));
  eng.AddLight(light);


  MsgSceneNode triMesh;
  SceneNodeMaterial & material = triMesh.Material(0);
  SceneNodeMeshPhysics & mesh = triMesh.Mesh(0);

  //triMesh.

  WaterManipulator waterManip, waterManip2;

  MTriangleMesh meshMaker;
  meshMaker.SetBoundaries(0, 0, 20, 20, 1.);
  meshMaker.GetMesh(mesh);

  material.SetTexture("data/Textures/water3.jpg");

  

  //mesh.AbsCoords() = Coordinates(1400, 1400, 1400);
  triMesh.SetPosition(Coordinates(off+3200, 400, off+1800));
  triMesh.SetName("triangles");
  triMesh.SetScale(200);
  mesh.SetPhysMode(1);
  triMesh.SetPhysMode(1);

  //material.SetTransparent(250.);

  eng.AddSceneNode(triMesh, &waterManip);

  //mesh.AbsCoords() = Coordinates(2900, 1400, 1400);
  triMesh.SetPosition(Coordinates(off+3200, 400, off+1800));
  triMesh.SetName("triangles2");

  //material.SetLighting(true);
  //material.SetTransparent(150.);
  //material.SetShinyness(30.);
  material.SetTexture("data/Textures/water4.jpg");

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

 
  eng.Run();


  return 0;
}
