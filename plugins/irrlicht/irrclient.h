#ifndef IRRCLIENT_H
#define IRRCLIENT_H

#include <irrlicht.h>
#include "driverChoice.h"
#include <math.h>


#include "util/StreamComm.h"
#include "graphics/Messages.h"
#include "base/ThreadHandler.h"
#include "audiothread.h"
#include <X11/Xlib.h>

#define IPI 3.141592653589793

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};

// Class MeshNode
class MeshNode
{
public:
  MeshNode() {
    m_pNode = NULL;
    m_pAnim = NULL;
  }
  MeshNode(const string & s, scene::IMeshSceneNode * pNode) {
    m_name = s;
    m_pNode = pNode;
    m_pAnim = NULL;
  }
  MeshNode(const string & s, scene::IAnimatedMeshSceneNode * pNode) {
    m_name = s;
    m_pAnim = pNode;
    m_pNode = NULL;
  }
  const string & Name() const {return m_name;}

  scene::IMesh * Mesh() {
    if (m_pNode != NULL) {
      return m_pNode->getMesh();
    } else {
      return m_pAnim->getMesh();
    }
  }
  scene::ISceneNode * SceneNode() {
    if (m_pNode != NULL) {
      return m_pNode;
    } else {
      return m_pAnim;
    }
  }

  //  const string & GetTexture() const {return m_texture;}
  bool NeedsTexture(const string & t) {
    if (t == "" || t == m_texture)
      return false;
    m_texture = t;
    return true;
  }
  bool NeedsModel(const string & t) {
    if (t == m_model || t == "")
      return false;
    m_model = t;
    return true;
  }
  bool NeedsAnimation(const string & t) {
    if (t == "" || t == m_animation)
      return false;
    m_animation = t;
    return true;
  }

  void SetTexture(video::ITexture * texture) {
    if (m_pNode != NULL) {
      m_pNode->setMaterialTexture(0, texture);
    } else {
      m_pAnim->setMaterialTexture(0, texture);
    }
  }

  void SetMaterialFlag(video::E_MATERIAL_TYPE f) {
    if (m_pNode != NULL) {
      m_pNode->getMaterial(0).MaterialType = f;
      //m_pNode->setMaterialFlag(f);
    } else {
      //m_pAnim->setMaterialFlag(f);
      m_pAnim->getMaterial(0).MaterialType = f;
    }
  }
  void SetPosition(const core::vector3df & pos) {
    if (m_pNode != NULL) {
      m_pNode->setPosition(pos);
    } else {
      m_pAnim->setPosition(pos);
    }
  }
  core::vector3df GetPosition() const {
    if (m_pNode != NULL) {
      return m_pNode->getPosition();
    } else {
      return m_pAnim->getPosition();
    }
  }
  void SetAnimation(const string & s) {
    if (m_pAnim == NULL || s == "")
      return;
    m_pAnim->setMD2Animation(s.c_str());
  }
 
  void SetDirection(const StreamCoordinates &c) {
    core::vector3df dir;
    SphereCoordinates sp = c.AsSphere();
    dir.Y = 360.*sp.phi()/2./IPI;
    dir.X = dir.Z = 0.;
    cout << "Direction: " << c[0] << " " << c[1] << " " << c[2] << " -> " << dir.Y << endl;
    if (m_pAnim != NULL)
      m_pAnim->setRotation(dir);
    if (m_pNode != NULL)
      m_pNode->setRotation(dir);
  }

  scene::IAnimatedMeshSceneNode * Anim() {return m_pAnim;}

private:
  scene::IMeshSceneNode * m_pNode;
  scene::IAnimatedMeshSceneNode * m_pAnim;
  string m_name;
  string m_texture;
  string m_animation;
  string m_model;
};


//=================================================================
class AnimModel
{
public:
  AnimModel() {
    m_pModel = NULL;
    m_speed = 20.;
  }
  const StreamCoordinates & GetCoords() const {return  m_coords;}
  void SetCoords(const StreamCoordinates &c) {
    m_coords = c;
    m_pModel->setPosition(core::vector3df(m_coords[0], m_coords[1], m_coords[2]));
  }

  void SetDirection(const StreamCoordinates &c) {
    core::vector3df dir;
    SphereCoordinates sp = c.AsSphere();
    dir.Y = 360.*sp.phi()/2./IPI;
    dir.X = dir.Z = 0.;
    cout << "Direction: " << c[0] << " " << c[1] << " " << c[2] << " -> " << dir.Y << endl;
    m_pModel->setRotation(dir);
  }

  const string & GetType() const {return m_type;}
  const string & GetName() const {return m_name;}

  void SetName(const string & n) {m_name = n;}
  void SetType(const string & t) {m_type = t;}

  void SetAnimation(const string & anim, double speed) {
    cout << "SET ANIMATION " << anim << " " << speed << endl;
    if (anim != m_anim || speed != m_speed) {
      m_pModel->setMD2Animation(anim.c_str());
      m_pModel->setAnimationSpeed(speed);      
    }
    m_speed = speed;
    m_anim = anim;
  }

  scene::IAnimatedMeshSceneNode* m_pModel;
 
private:
  
  string m_type;
  string m_name;
  StreamCoordinates m_coords;
  string m_anim;
  double m_speed;
};

// ===================================================================
// Default event receiver - replace with your specific needs
class MyEventReceiver : public IEventReceiver
{
 public:
  
 MyEventReceiver(scene::ISceneNode* terrain, scene::ISceneNode* skybox, scene::ISceneNode* skydome) :
  Terrain(terrain), Skybox(skybox), Skydome(skydome), showBox(true), showDebug(false)
    {
      Skybox->setVisible(showBox);
      Skydome->setVisible(!showBox);
      for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
	KeyIsDown[i] = false;
    }
  // This is used to check whether a key is being held down
  virtual bool IsKeyDown(EKEY_CODE keyCode) const
  {
    return KeyIsDown[keyCode];
  }
  
  bool OnEvent(const SEvent& event)
  {
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
      KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    
    // check if user presses the key 'W' or 'D'
    if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
      {
	switch (event.KeyInput.Key)
	  {
	  case irr::KEY_KEY_Q: // switch wire frame mode
	    Terrain->setMaterialFlag(video::EMF_WIREFRAME,
				     !Terrain->getMaterial(0).Wireframe);
	    Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
	    return true;
	  case irr::KEY_KEY_P: // switch wire frame mode
	    Terrain->setMaterialFlag(video::EMF_POINTCLOUD,
				     !Terrain->getMaterial(0).PointCloud);
	    Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
	    return true;
	  case irr::KEY_KEY_C: // toggle detail map
	    Terrain->setMaterialType(
				     Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
				     video::EMT_DETAIL_MAP : video::EMT_SOLID);
	    return true;
	  case irr::KEY_KEY_V: // toggle skies
	    showBox=!showBox;
	    Skybox->setVisible(showBox);
	    Skydome->setVisible(!showBox);
	    return true;
	  case irr::KEY_KEY_X: // toggle debug information
	    showDebug=!showDebug;
	    Terrain->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
	    return true;
	  case irr::KEY_KEY_Z: // toggle debug information
	    exit(0);
	    return true;
	  default:
	    break;
	  }
      }
    
    return false;
  }
  
 private:
  scene::ISceneNode* Terrain;
  scene::ISceneNode* Skybox;
  scene::ISceneNode* Skydome;
  bool showBox;
  bool showDebug;
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
  
};

//========================================================================
class IrrlichtServer
{
public:
  // Uses a default event receiver if NULL
  IrrlichtServer(int resX, int resY, bool fullScreen, const string & host = "localhost", MyEventReceiver * pRec = NULL);
  ~IrrlichtServer() {
    if (receiver != NULL)
      delete receiver;
    m_pCube = NULL;
  }

  void AddCamera(double x, double y, double z);
  void AddTerrain(const MsgTerrain & t);
  void WaitLoadTerrain();
  void AddCube();
  void AddMeshModel(MeshModel m);

  void Run();
  bool ProcessMessage(const string & type, DataPacket & d);
  
  void SetCameraPosition(const StreamCoordinates & c);
  void SetCameraRotation(const StreamCoordinates & c);
  void SetCameraTilt(double angle);

  void GetCameraPosition(StreamCoordinates & c);
  void GetCameraRotation(StreamCoordinates & c);
  void GetCameraTilt(StreamCoordinates & c);

protected:
  int GetModelIndex(const string & name) {
    int i;
    for (i=0; i<m_anim.isize(); i++) {
      if (m_anim[i].GetName() == name)
	return i;
    }
    return -1;
  }

  void AddSceneNode(const MsgSceneNode & m);
  void UpdateSceneNode(const MsgSceneNode & m);
  void LoopBackSceneNode(const MsgSceneNode & m, scene::IMesh * pMesh, const string & name, core::vector3df posA,
			 const Coordinates & rot = Coordinates(0, 0, 0), int phys = 0, bool sendMesh = false);


  bool SendMeshModel(scene::IMesh * pMesh, const string & name, core::vector3df posA,
		     const Coordinates & rot = Coordinates(0, 0, 0), int phys = 0);
  void UpdateMeshModel(MeshModel & m);

  scene::ICameraSceneNode* camera;
  IrrlichtDevice* device;
  scene::ITerrainSceneNode* terrain;

  video::IVideoDriver* driver;
  scene::ISceneManager* smgr;
  gui::IGUIEnvironment* env;
  scene::IAnimatedMeshSceneNode* fairy;
  //video::SMaterial material;
  scene::IMeshSceneNode * m_pCube;

  svec<AnimModel> m_anim;
  svec<MeshNode> m_meshes;

  MyEventReceiver * receiver;

  double xp;
  double yp;
  double zp;
  
  SharedAudioData audioDat;

  StreamCommTransmitter * m_pTrans;
  StreamCommReceiver * m_pRec; 

};






#endif //IRRCLIENT_H

