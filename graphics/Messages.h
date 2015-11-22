// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "util/StreamComm.h"
#include "physics/Coordinates.h"

const string MSG_NODE_ADD = "staticmodel";
const string MSG_ANIMNODE_ADD = "animatedmodel";
const string MSG_MESH_ADD = "meshadd";
const string MSG_MESH_UPDATE = "meshupdate";
const string MSG_NODE_UPDATE = "staticmodel_update";
const string MSG_ANIMNODE_UPDATE = "animatedmodel_update";
const string MSG_TERRAIN = "set_terrain";
const string MSG_PHYS_ADD = "physadd";
const string MSG_PHYS_UPDATE = "physupdate";
const string MSG_LIGHT_ADD = "lightadd";
const string MSG_LIGHT_UPDATE = "lightupdate";
const string MSG_SCENENODE_ADD = "sn_add";
const string MSG_SCENENODE_UPDATE = "sn_update";


//======================================
class TimeStamp
{
 public:
  TimeStamp() {
    Update();
  }

  void Update();

  virtual void FromPacket(DataPacket & d) {
    d.Read(m_ms);
    d.Read(m_r);
  }

  virtual void ToPacket(DataPacket & d) const {
    d.Write(m_ms);
    d.Write(m_r);
  }

  const string & GetReadable() const {return m_r;}
  int Milliseconds() const {return m_ms;}

 private:
  string m_r;
  int m_ms;
};



//======================================
class StreamCoordinates : public Coordinates
{
 public: 
  StreamCoordinates() : Coordinates() {}
  StreamCoordinates(const Coordinates & c) : Coordinates(c) {}

  StreamCoordinates(double a, double b, double c) : Coordinates(a, b, c) {
  }

  void FromPacket(DataPacket & d) {
    d.Read((*this)[0]);
    d.Read((*this)[1]);
    d.Read((*this)[2]);
  }

  void ToPacket(DataPacket & d) const {
    d.Write((*this)[0]);
    d.Write((*this)[1]);
    d.Write((*this)[2]);
  }

  /* void SetName(const string & name) {
    m_name = name;
  }

  const string & GetName() const {return m_name;}
  */

  StreamCoordinates & operator = (const Coordinates & c) {
    Coordinates::operator= (c);
    return *this;
  }

  StreamCoordinates operator - (const StreamCoordinates & c) const {
    return Coordinates::operator - (c);
  }
  StreamCoordinates operator + (const StreamCoordinates & c) const {
    return Coordinates::operator + (c);
  }
  StreamCoordinates operator * (double d) const {
    return Coordinates::operator * (d);
  }

  StreamCoordinates operator / (double d) const {
    return Coordinates::operator / (d);
  }

  void operator += (const StreamCoordinates & c) {
    Coordinates::operator += (c);
  }
  void operator -= (const StreamCoordinates & c) {
    Coordinates::operator -= (c);
  }

  void operator /= (double d) {
    Coordinates::operator /= (d);
  }
  void operator *= (double d) {
    Coordinates::operator *= (d);
  }


  bool operator == (const StreamCoordinates & c) const {
    return Coordinates::operator==(c);
  }
  bool operator != (const StreamCoordinates & c) const {
    return Coordinates::operator!=(c);
  }

 private:
};

class NameType
{
 public:
  NameType();

  const string & GetName() const {return m_name;}
  void SetName(const string & s) {m_name = s;}
  const string & GetType() const {return m_type;}
  void SetType(const string & s) {m_type = s;}
  const string & GetPhysics() const {return m_physics;}
  void SetPhysics(const string & s) {m_physics = s;}
  const string & GetControl() const {return m_control;}
  void SetControl(const string & s) {m_control = s;}

  void SetScale(double s) {m_scale = s;}
  double GetScale() const {return m_scale;}

  void SetInvisible(const Coordinates & c) {
    m_invisible = c;
  }
  const StreamCoordinates & GetInvisible() const {return m_invisible;}

  void SetLighting(bool b) {
    if (b) 
      m_bLighting = 1;
    else
      m_bLighting = 0;
  }
 
  bool GetLighting() const {return (m_bLighting != 0);}

  virtual void FromPacket(DataPacket & d) {
    d.Read(m_name);
    d.Read(m_type);
  }

  virtual void ToPacket(DataPacket & d) const {
    d.Write(m_name);
    d.Write(m_type);
  }

 protected:
  string m_name;
  string m_type;
  string m_physics;
  string m_control;
  double m_scale;
  int m_bLighting;
  StreamCoordinates m_invisible;
};


//======================================
class MsgTerrain
{
 public:
  MsgTerrain() {}

  void Set(const string & terrain,
	   const string & texture1,
	   const string & texture2,
	   const string & sky_up,
	   const string & sky_dn,
	   const string & sky_lf,
	   const string & sky_rt,
	   const string & sky_ft,
	   const string & sky_bk,
	   const string & skydome) {
    m_terrain = terrain;
    m_texture1 = texture1;
    m_texture2 = texture2;
    m_sky_up = sky_up;
    m_sky_dn = sky_dn;
    m_sky_lf = sky_lf;
    m_sky_rt = sky_rt;
    m_sky_ft = sky_ft;
    m_sky_bk = sky_bk;
    m_skydome = skydome;
  
  }

  const string & GetTerrain() const {return m_terrain;}
  const string & GetTexture1() const {return m_texture1;}
  const string & GetTexture2() const {return m_texture2;}
  const string & GetSkyUP() const {return m_sky_up;}
  const string & GetSkyDN() const {return m_sky_dn;}
  const string & GetSkyLF() const {return m_sky_lf;}
  const string & GetSkyRT() const {return m_sky_rt;}
  const string & GetSkyFT() const {return m_sky_ft;}
  const string & GetSkyBK() const {return m_sky_bk;}
  const string & GetSkydome() const {return m_skydome;}

  virtual void FromPacket(DataPacket & d) {
    d.Read(m_terrain);
    d.Read(m_texture1);
    d.Read(m_texture2);
    d.Read(m_sky_up);
    d.Read(m_sky_dn);
    d.Read(m_sky_lf);
    d.Read(m_sky_rt);
    d.Read(m_sky_ft);
    d.Read(m_sky_bk);
    d.Read(m_skydome);    
  }

  virtual void ToPacket(DataPacket & d) const {
    d.Write(m_terrain);
    d.Write(m_texture1);
    d.Write(m_texture2);
    d.Write(m_sky_up);
    d.Write(m_sky_dn);
    d.Write(m_sky_lf);
    d.Write(m_sky_rt);
    d.Write(m_sky_ft);
    d.Write(m_sky_bk);
    d.Write(m_skydome);    
  }

 private:
  string m_terrain;
  string m_texture1;
  string m_texture2;
  string m_sky_up;
  string m_sky_dn;
  string m_sky_lf;
  string m_sky_rt;
  string m_sky_ft;
  string m_sky_bk;
  string m_skydome;
};

//==============================================
class MsgLightNode : public NameType
{
 public:
  MsgLightNode() {
    r = g = b = 0.99;
    radius = 8000;
  }

  double R() const {return r;}
  double G() const {return g;}
  double B() const {return b;}
  double Radius() const {return radius;}
  const StreamCoordinates & Position() const {return m_pos;}
  const string & Texture() const {return texture;}

  void SetRGB(double rr, double gg, double bb) {
    r = rr;
    g = gg;
    b = bb;
  }
  void SetRadius(double r) {radius = r;}
  void SetPosition(const StreamCoordinates & p) {m_pos = p;}
  void SetTexture(const string & t)  {texture = t;}

  virtual void FromPacket(DataPacket & d) {
    d.Read(m_type);
    d.Read(m_name);
    m_pos.FromPacket(d);
    d.Read(r);
    d.Read(g);
    d.Read(b);
    d.Read(radius);
    d.Read(texture);
  }
  virtual void ToPacket(DataPacket & d) const {
    d.Write(m_type);
    d.Write(m_name);
    m_pos.ToPacket(d);
    d.Write(r);
    d.Write(g);
    d.Write(b);
    d.Write(radius);
    d.Write(texture);
  }
 private:
  StreamCoordinates m_pos;
  double r, g, b;
  double radius;
  string texture;
};


class UpdatableMessage : public NameType
{
 public: 
  UpdatableMessage() {
    m_dirty = true;
  }

  bool IsDirty() const {return m_dirty;}
  void SetDirty(bool b) {m_dirty = b;}

  virtual void FromPacket(DataPacket & d) {
    NameType::FromPacket(d);  
    d.ReadBool(m_dirty);
  }

  virtual void ToPacket(DataPacket & d) const {
    NameType::ToPacket(d);  
    d.WriteBool(m_dirty);
  }

 protected:
  bool m_dirty;
};



//=======================================================
class MsgSound : public UpdatableMessage
{
 public:
  MsgSound();

  virtual void FromPacket(DataPacket & d) {
    UpdatableMessage::FromPacket(d);
    //if (!IsDirty())
    //return;
    d.Read(m_name);
    d.Read(m_soundFile);
    m_pos.FromPacket(d);
  }

  virtual void ToPacket(DataPacket & d) const {
    UpdatableMessage::ToPacket(d);
    //if (!IsDirty())
    //return;
    d.Write(m_name);
    d.Write(m_soundFile);
    m_pos.ToPacket(d);
  }

  const string & GetWavFile() const {return m_soundFile;}
  void SetWavFile(const string & s) {m_soundFile = s;}

  const string & GetName() const {return m_name;}
  void SetName(const string & s) {m_name = s;}

  const StreamCoordinates & GetPosition() const {return m_pos;}
  void SetPosition(const StreamCoordinates & s) {m_pos = s;}

 private:
  string m_soundFile;
  string m_name;
  StreamCoordinates m_pos;
};



// Classes that make up a scene node
//=========================================================
class SceneNodeMeshPhysics : public UpdatableMessage
{
 public:
  SceneNodeMeshPhysics() {
    m_physMode = 0;  
    m_hasRot = 0;    
  }

  const StreamCoordinates & GetAbsCoords() const {return m_abs;}
  StreamCoordinates & AbsCoords() {return m_abs;}

  const StreamCoordinates & GetDirection() const {return m_direction;}
  void SetDirection(const Coordinates & c) {m_direction = c;}
  void SetRotation(const Coordinates & c) {
    m_rotation = c;
    m_hasRot = 1;
  }
  void SetRotation(const StreamCoordinates & c) {
    m_rotation = c;
    m_hasRot = 1;
  }
  bool HasRotation() const {return (m_hasRot == 1);}

  const StreamCoordinates & GetRotation() const {return m_rotation;}

  void SetRotImp(const Coordinates & c) {
    m_rot = c;
  }
  void SetRotImp(const StreamCoordinates & c) {
    m_rot = c;
  }
  const StreamCoordinates & GetRotImp() const {return m_rot;}
 
  int VertexCount() const {return m_vertices.isize();}
  const StreamCoordinates & GetVertexConst(int i) const {return m_vertices[i];}
  StreamCoordinates & GetVertex(int i) {return m_vertices[i];}

  void AddVertex(const StreamCoordinates & c) {
    m_vertices.push_back(c);
  }

  void AddVertex(const Coordinates & c) {
    StreamCoordinates cc;
    cc = c;
    AddVertex(cc);
  }

  int IndexCount() const {return m_indices.isize()/3;}
  int GetIndexConst(int i, int j) const {return m_indices[i*3+j];}
  int & GetIndex(int i, int j)  {return m_indices[i*3+j];}
  void AddIndex(int i, int j, int k) {
    m_indices.push_back(i);
    m_indices.push_back(j);
    m_indices.push_back(k);
  }

  int IndexCountTotal() const {return m_indices.isize();}
  int GetIndexTotal(int i) const {return m_indices[i];}
  void AddIndexTotal(int i) {
    m_indices.push_back(i);
  }

  
  int GetNormalCount() const {return m_normals.isize();}
  const StreamCoordinates & GetNormalConst(int i) const {return m_normals[i];}
  StreamCoordinates & GetNormal(int i) {return m_normals[i];}

  void RecomputeNormals() {
    int i;
    m_normals.resize(m_vertices.isize());

    for (i=0; i<m_vertices.isize(); i++) {
      StreamCoordinates & n = m_normals[i];
      const StreamCoordinates & v = m_vertices[i];
      n = StreamCoordinates(1, 1, 1);
      if (v[0] <= 0.)
	n[0] = -1;
      if (v[1] <= 0.)
	n[1] = -1;
      if (v[2] <= 0.)
	n[2] = -1;
    }
  }

  void AddNormal(const StreamCoordinates & c) {
    m_normals.push_back(c);
  }

  void AddNormal(const Coordinates & c) {
    StreamCoordinates cc;
    cc = c;
    AddNormal(cc);
  }

  int GetTexCoordCount() const {return m_texCoords.isize();}
  const StreamCoordinates & GetTextCoordConst(int i) const {return m_texCoords[i];}
  StreamCoordinates & GetTextCoord(int i) {return m_texCoords[i];}

  void AddTexCoord(const StreamCoordinates & c) {
    m_texCoords.push_back(c);
  }

  void AddTexCoord(const Coordinates & c) {
    StreamCoordinates cc;
    cc = c;
    AddTexCoord(cc);
  }

  int PhysMode() const {return m_physMode;}
  void SetPhysMode(int n) {m_physMode = n;}


  virtual void FromPacket(DataPacket & d) {
    UpdatableMessage::FromPacket(d);
    //if (!IsDirty())
    //return;
    int n;
    int i;
    m_vertices.clear();
    m_indices.clear();
    m_abs.FromPacket(d);
    m_rot.FromPacket(d);
    m_rotation.FromPacket(d);
    d.Read(m_hasRot);
    m_direction.FromPacket(d);
    d.Read(m_physMode);

    d.Read(n);
    m_vertices.resize(n);
    for (i=0; i<n; i++) {
      m_vertices[i].FromPacket(d);
    }

    d.Read(n);
    m_indices.resize(n);
    for (i=0; i<n; i++) {
      d.Read(m_indices[i]);
    }

    //================
  
    d.Read(n);
    m_normals.resize(n);  
    for (i=0; i<n; i++) {
      m_normals[i].FromPacket(d);
    }
    d.Read(n);
    m_texCoords.resize(n);  
    for (i=0; i<n; i++) {
      m_texCoords[i].FromPacket(d);
    }
  }
  virtual void ToPacket(DataPacket & d) const {
    ToPacket(d, false);
  }

  virtual void ToPacket(DataPacket & d, bool bTrunc) const {
    UpdatableMessage::ToPacket(d);
    //if (!IsDirty())
    //return;

    int n;
    int i;
    m_abs.ToPacket(d);
    m_rot.ToPacket(d);
    m_rotation.ToPacket(d);
    d.Write(m_hasRot);
    m_direction.ToPacket(d);

    d.Write(m_physMode);

    n = m_vertices.isize();

    if (bTrunc)
      n = 1;

    d.Write(n);
   
    for (i=0; i<n; i++) {
      m_vertices[i].ToPacket(d);
    }

    n = m_indices.isize();
    if (bTrunc)
      n = 0;
   
    d.Write(n);

    for (i=0; i<n; i++) {
      d.Write(m_indices[i]);
    }
    // ===============

    n = m_normals.isize();
    if (bTrunc)
      n = 1;
    d.Write(n);
    for (i=0; i<n; i++) {
      m_normals[i].ToPacket(d);
    }
    n = m_texCoords.isize();
    if (bTrunc)
      n = 1;
    d.Write(n);
    for (i=0; i<n; i++) {
      m_texCoords[i].ToPacket(d);
    }
  }

  int SizeInBytes() const {
    int n = sizeof(StreamCoordinates) * (m_vertices.isize()+1) + 
      sizeof(int) * m_indices.isize();
    return n;
  }

 private:
  StreamCoordinates m_abs;
  StreamCoordinates m_rot; 
  StreamCoordinates m_direction;
  StreamCoordinates m_rotation;
  
  int m_hasRot;
  int m_physMode;
  svec<StreamCoordinates> m_vertices;
  svec<int> m_indices;
  
  svec<StreamCoordinates> m_normals;
  svec<StreamCoordinates> m_texCoords;


};

//=========================================================
class SceneNodeMaterial : public UpdatableMessage
{
 public:
  SceneNodeMaterial() {
    m_shiny = 0.;
    m_transparent = -1.;   
    m_bLighting = false;    
    m_isInvis = false;
  }

  virtual void FromPacket(DataPacket & d) {
    UpdatableMessage::FromPacket(d);
    //if (!IsDirty())
    //return;

    d.Read(m_shiny);
    d.Read(m_transparent);
    d.ReadBool(m_bLighting);
    d.ReadBool(m_isInvis);
    d.Read(m_textureName);
    m_invisible.FromPacket(d);
  }

  virtual void ToPacket(DataPacket & d) const {
    UpdatableMessage::ToPacket(d);
    //if (!IsDirty())
    //return;

    d.Write(m_shiny);
    d.Write(m_transparent);
    d.WriteBool(m_bLighting);
    d.WriteBool(m_isInvis);
    d.Write(m_textureName);
    m_invisible.ToPacket(d);
  }

  double GetShinyness() const {return m_shiny;}
  double GetTransparent() const {return m_transparent;}
  const string & GetTexture() const {return m_textureName;}
  bool GetLighting() const {return m_bLighting;}
  const StreamCoordinates & GetInvisibleCoords() const {return m_invisible;}
  bool IsInvisible() const {return m_isInvis;}

  void SetLighting(bool n) {m_bLighting = n;}
  void SetInvisible(bool n) {m_isInvis = n;}
  void SetInvisibleCoords(const StreamCoordinates & c) {m_invisible = c;}

  void SetShinyness(double d) {m_shiny = d;}
  void SetTransparent(double d) {m_transparent = d;}
  void SetTexture(const string & n) {m_textureName = n;}
  

 protected:
  double m_shiny;
  double m_transparent;
  string m_textureName;
  bool m_bLighting;
  StreamCoordinates m_invisible;
  bool m_isInvis;
};

//=========================================================
class SceneNodeAnimation : public UpdatableMessage
{
 public:
  SceneNodeAnimation() {
    m_frame = -1;
    double m_speed = 30.;
  }

  const string & GetAnimation() const {return m_anim;}
  double GetSpeed() const {return m_speed;}
  int GetFrame() const {return m_frame;}

  void SetAnimation(const string & a) {m_anim = a;}
  void SetSpeed(double s) {m_speed = s;}
  void SetFrame(int i) {m_frame = i;}

  virtual void FromPacket(DataPacket & d) {
    UpdatableMessage::FromPacket(d);
    if (!IsDirty())
      return;
  }
  virtual void ToPacket(DataPacket & d) const {
    UpdatableMessage::ToPacket(d);
    if (!IsDirty())
      return;
  }
 protected:
  string m_anim;
  double m_speed;
  int m_frame;
};


//=========================================================
class MsgSceneNode : public UpdatableMessage
{
 public:
  MsgSceneNode() {
    m_physMode = 1;
    m_mat.resize(1);
    m_mesh.resize(1);
    m_requestLoopBack = true;
    m_requestMesh = true;
    m_scale = 1.;
  }

  virtual void FromPacket(DataPacket & d) {
    UpdatableMessage::FromPacket(d);
    //if (!IsDirty())
    //return;
    d.Read(m_model);
    d.Read(m_physMode);
    
    d.ReadBool(m_requestLoopBack);
    d.ReadBool(m_requestMesh);

    
    m_abs.FromPacket(d);
    m_rotation.FromPacket(d);
    
    d.Read(m_scale);
    
    m_anim.FromPacket(d);
    m_sound.FromPacket(d);
    m_light.FromPacket(d);
    
    int i;
    int n;
    d.Read(n);

    m_mat.resize(n);    
    for (i=0; i<n; i++)
      m_mat[i].FromPacket(d);

    d.Read(n);
    m_mesh.resize(n);    
    for (i=0; i<n; i++)
      m_mesh[i].FromPacket(d);
  }

  virtual void ToPacket(DataPacket & d) const {
    UpdatableMessage::ToPacket(d);
    //if (!IsDirty())
    //return;

    d.Write(m_model);
    d.Write(m_physMode);

    d.WriteBool(m_requestLoopBack);
    d.WriteBool(m_requestMesh);


    m_abs.ToPacket(d);
    m_rotation.ToPacket(d);
    
    d.Write(m_scale);
    
    m_anim.ToPacket(d);
    m_sound.ToPacket(d);
    m_light.ToPacket(d);
    
    int i;
    
    int n = m_mat.isize();
    d.Write(n);
    for (i=0; i<n; i++)
      m_mat[i].ToPacket(d);

    n = m_mesh.isize();
    d.Write(n);
    for (i=0; i<n; i++)
      m_mesh[i].ToPacket(d);
  }

  //----------------------------------------------------------
  bool RequestLoopBack() const {return m_requestLoopBack;}
  void SetRequestLoopBack(bool b) {m_requestLoopBack = b;}

  bool RequestMesh() const {return m_requestMesh;}
  void SetRequestMesh(bool b) {m_requestMesh = b;}

  int GetPhysMode() const {return m_physMode;}
  void SetPhysMode(int m) {m_physMode = m;}

  const string & GetModel() const {return m_model;}
  void SetModel(const string & m) {m_model = m;}

  SceneNodeAnimation & Animation() {return m_anim;}
  MsgSound &Sound() {return  m_sound;}
  MsgLightNode & Light() {return m_light;}

  const SceneNodeAnimation & GetAnimation() const {return m_anim;}
  const MsgSound & GetSound() const {return  m_sound;}
  const MsgLightNode & GetLight() const {return m_light;}

  int MaterialCount() const {return m_mat.isize();}
  const SceneNodeMaterial & GetMaterial(int i) const {return m_mat[i];}
  void SetMaterialCount(int i) {m_mat.resize(i);}
  SceneNodeMaterial & Material(int i) {return m_mat[i];}
 
  int MeshCount() const {return m_mesh.isize();}
  const SceneNodeMeshPhysics & GetMesh(int i) const {return m_mesh[i];}
  void SetMeshCount(int i) {m_mesh.resize(i);}
  SceneNodeMeshPhysics & Mesh(int i) {return m_mesh[i];}

  const StreamCoordinates & GetPosition() const {return m_abs;}
  const StreamCoordinates & GetRotation() const {return m_rotation;}

  void SetPosition(const StreamCoordinates & c) {m_abs = c;}
  void SetRotation(const StreamCoordinates & c) {m_rotation = c;}

  double GetScale() const {return m_scale;}
  void SetScale(double s) {m_scale = s;}

 private:
  bool m_requestLoopBack;
  bool m_requestMesh;

  StreamCoordinates m_abs;
  StreamCoordinates m_rotation;
  string m_model;

  double m_scale;
  int m_physMode;

  SceneNodeAnimation m_anim;
  MsgSound m_sound;
  MsgLightNode m_light;

  svec<SceneNodeMaterial> m_mat;
  svec<SceneNodeMeshPhysics> m_mesh;
};


// ****************************************************
// DEPRECATED!!! WILL BE OBSOLETE!!!!!!!
// ****************************************************
//=============================================
class AnimatedSceneNode : public NameType
{
 public:
  AnimatedSceneNode() {
    m_animspeed = 20.;
    m_direction[0] = 1.;
    m_direction[1] = 0.;
    m_direction[2] = 0.;
    m_physMode = 0;
    m_shiny = 12.;
    m_transparent = -1.;
    int m_hasRot;
    m_texture.resize(1);
  }

  void SetCoordinates(const Coordinates & c) {
    m_coords = c;
  }
  void SetCoordinates(const StreamCoordinates & c) {
    m_coords = c;
  }
  const StreamCoordinates & GetCoordinates() const {return m_coords;}

  void SetRotation(const Coordinates & c) {
    m_rotation = c;
    m_hasRot = 1;
  }
  void SetRotation(const StreamCoordinates & c) {
    m_rotation = c;
    m_hasRot = 1;
  }
  bool HasRotation() const {return (m_hasRot == 1);}

  const StreamCoordinates & GetRotation() const {return m_rotation;}

  void SetDirection(const Coordinates & c) {
    m_direction = c;
  }
  void SetDirection(const StreamCoordinates & c) {
    m_direction = c;
  }
  const StreamCoordinates & GetDirection() const {return m_direction;}

  void SetRotImp(const Coordinates & c) {
    m_rot = c;
  }
  void SetRotImp(const StreamCoordinates & c) {
    m_rot = c;
  }
  const StreamCoordinates & GetRotImp() const {return m_rot;}
  
  void SetModel(const string & model) {
    m_model = model;
  }
  void SetTexture(const string & texture) {
    m_texture[0] = texture;
  }

  void SetTexture(const string & texture, int i) {
    if (i >= m_texture.isize())
      m_texture.resize(i+1);
    m_texture[i] = texture;
  }

  int GetTextureCount() const {return m_texture.isize();}

  const string & GetTexture(int i) {
    return m_texture[i];
  }

  void SetAnimation(const string & anim) {
    m_animation = anim;
  }
  void SetAnimationSpeed(double s) {
    m_animspeed = s;
  }

  double GetShinyness() const {return m_shiny;}
  void SetShinyness(double d) {m_shiny = d;}

  double GetTransparent() const {return m_transparent;}
  void SetTransparent(double d) {m_transparent = d;}

  const string & SetTexture() const {return m_animation;}
  const string & GetModel() const {return m_model;}
  const string & GetTexture() const {return m_texture[0];}
  const string & GetAnimation() const {return m_animation;}
  double GetAnimationSpeed() const {return m_animspeed;}

  int PhysMode() const {return m_physMode;}
  void SetPhysMode(int n) {m_physMode = n;}

  MsgSound & Sound() {return m_sound;}
  const MsgSound & GetSound() const {return m_sound;}

  virtual void FromPacket(DataPacket & d) {
    m_coords.FromPacket(d);
    m_direction.FromPacket(d);
    m_rotation.FromPacket(d);
    d.Read(m_hasRot);
    m_rot.FromPacket(d);
    d.Read(m_transparent);
    d.Read(m_scale);
    d.Read(m_model);

    int n = 0;
    d.Read(n);
    m_texture.resize(n);
    for (int i=0; i<n; i++)
      d.Read(m_texture[i]);

    d.Read(m_type);
    d.Read(m_name);
    d.Read(m_physics);
    d.Read(m_control);
    d.Read(m_animation);
    d.Read(m_animspeed);
    d.Read(m_physMode);
    d.Read(m_bLighting);
    d.Read(m_shiny);
    m_invisible.FromPacket(d);
    m_sound.FromPacket(d);
  }

  virtual void ToPacket(DataPacket & d) const {
    m_coords.ToPacket(d);
    m_direction.ToPacket(d);
    m_rotation.ToPacket(d);
    d.Write(m_hasRot);
    m_rot.ToPacket(d);
    d.Write(m_transparent);
    d.Write(m_scale);
    d.Write(m_model);

    d.Write(m_texture.isize());
    for (int i=0; i<m_texture.isize(); i++)
      d.Write(m_texture[i]);

    d.Write(m_type);
    d.Write(m_name);
    d.Write(m_physics);
    d.Write(m_control);
    d.Write(m_animation);
    d.Write(m_animspeed);
    d.Write(m_physMode);
    d.Write(m_bLighting);
    d.Write(m_shiny);
    m_invisible.ToPacket(d);
    m_sound.ToPacket(d);
  }

 private:
  StreamCoordinates m_coords;
  StreamCoordinates m_direction;
  StreamCoordinates m_rotation;
  int m_hasRot;
  StreamCoordinates m_rot; 
  string m_model;
  svec<string> m_texture;

  string m_animation;
  double m_animspeed;
  int m_physMode;
  double m_shiny;
  double m_transparent;
  MsgSound m_sound;
};

//==============================================
class PhysModelNode : public AnimatedSceneNode
{
 public:
  PhysModelNode() {}
 private:
};


//==============================================
class MeshModel : public NameType
{
 public:
  MeshModel() {
    m_physMode = 0;
    m_shiny = 12.;
    m_hasRot = 0;
  }
  const StreamCoordinates & GetAbsCoords() const {return m_abs;}
  StreamCoordinates & AbsCoords() {return m_abs;}

  const StreamCoordinates & GetDirection() const {return m_direction;}
  void SetDirection(const Coordinates & c) {m_direction = c;}
  void SetRotation(const Coordinates & c) {
    m_rotation = c;
    m_hasRot = 1;
  }
  void SetRotation(const StreamCoordinates & c) {
    m_rotation = c;
    m_hasRot = 1;
  }
  bool HasRotation() const {return (m_hasRot == 1);}

  const StreamCoordinates & GetRotation() const {return m_rotation;}

  void SetRotImp(const Coordinates & c) {
    m_rot = c;
  }
  void SetRotImp(const StreamCoordinates & c) {
    m_rot = c;
  }
  const StreamCoordinates & GetRotImp() const {return m_rot;}
  double GetShinyness() const {return m_shiny;}
  void SetShinyness(double d) {m_shiny = d;}

  int VertexCount() const {return m_vertices.isize();}
  const StreamCoordinates & GetVertexConst(int i) const {return m_vertices[i];}
  StreamCoordinates & GetVertex(int i) {return m_vertices[i];}

  void AddVertex(const StreamCoordinates & c) {
    m_vertices.push_back(c);
  }

  void AddVertex(const Coordinates & c) {
    StreamCoordinates cc;
    cc = c;
    AddVertex(cc);
  }

  int IndexCount() const {return m_indices.isize()/3;}
  int GetIndexConst(int i, int j) const {return m_indices[i*3+j];}
  int & GetIndex(int i, int j)  {return m_indices[i*3+j];}
  void AddIndex(int i, int j, int k) {
    m_indices.push_back(i);
    m_indices.push_back(j);
    m_indices.push_back(k);
  }

  int IndexCountTotal() const {return m_indices.isize();}
  int GetIndexTotal(int i) const {return m_indices[i];}
  void AddIndexTotal(int i) {
    m_indices.push_back(i);
  }

  const string & GetTexture() const {return m_texture;}
  void SetTexture(const string & t) {m_texture = t;}
  
  int GetNormalCount() const {return m_normals.isize();}
  const StreamCoordinates & GetNormalConst(int i) const {return m_normals[i];}
  StreamCoordinates & GetNormal(int i) {return m_normals[i];}

  void RecomputeNormals() {
    int i;
    m_normals.resize(m_vertices.isize());

    for (i=0; i<m_vertices.isize(); i++) {
      StreamCoordinates & n = m_normals[i];
      const StreamCoordinates & v = m_vertices[i];
      n = StreamCoordinates(1, 1, 1);
      if (v[0] <= 0.)
	n[0] = -1;
      if (v[1] <= 0.)
	n[1] = -1;
      if (v[2] <= 0.)
	n[2] = -1;
    }
  }

  void AddNormal(const StreamCoordinates & c) {
    m_normals.push_back(c);
  }

  void AddNormal(const Coordinates & c) {
    StreamCoordinates cc;
    cc = c;
    AddNormal(cc);
  }

  int GetTexCoordCount() const {return m_texCoords.isize();}
  const StreamCoordinates & GetTextCoordConst(int i) const {return m_texCoords[i];}
  StreamCoordinates & GetTextCoord(int i) {return m_texCoords[i];}

  void AddTexCoord(const StreamCoordinates & c) {
    m_texCoords.push_back(c);
  }

  void AddTexCoord(const Coordinates & c) {
    StreamCoordinates cc;
    cc = c;
    AddTexCoord(cc);
  }

  int PhysMode() const {return m_physMode;}
  void SetPhysMode(int n) {m_physMode = n;}

  void SetAnimation(const string & s) {
    m_animation = s;
  }
  const string & GetAnimation() const {return m_animation;}

  MsgSound & Sound() {return m_sound;}
  const MsgSound & GetSound() const {return m_sound;}

  virtual void FromPacket(DataPacket & d) {
    int n;
    int i;
    m_vertices.clear();
    m_indices.clear();
    d.Read(m_type);
    d.Read(m_name);
    d.Read(m_scale);
    d.Read(m_bLighting);
    d.Read(m_shiny);
    d.Read(m_animation);
    m_abs.FromPacket(d);
    m_rot.FromPacket(d);
    m_rotation.FromPacket(d);
    d.Read(m_hasRot);
    m_direction.FromPacket(d);
    d.Read(m_physMode);
    m_invisible.FromPacket(d);
    m_sound.FromPacket(d);

    d.Read(n);
    m_vertices.resize(n);
    for (i=0; i<n; i++) {
      m_vertices[i].FromPacket(d);
    }

    d.Read(n);
    m_indices.resize(n);
    for (i=0; i<n; i++) {
      d.Read(m_indices[i]);
    }

    //================
    d.Read(m_texture);
    d.Read(n);
    m_normals.resize(n);  
    for (i=0; i<n; i++) {
      m_normals[i].FromPacket(d);
    }
    d.Read(n);
    m_texCoords.resize(n);  
    for (i=0; i<n; i++) {
      m_texCoords[i].FromPacket(d);
    }
  }
  virtual void ToPacket(DataPacket & d) const {
    ToPacket(d, false);
  }

  virtual void ToPacket(DataPacket & d, bool bTrunc) const {
    int n;
    int i;
    d.Write(m_type);
    d.Write(m_name);
    d.Write(m_scale);
    d.Write(m_bLighting);
    d.Write(m_shiny);
    d.Write(m_animation);
    m_abs.ToPacket(d);
    m_rot.ToPacket(d);
    m_rotation.ToPacket(d);
    d.Write(m_hasRot);
    m_direction.ToPacket(d);

    d.Write(m_physMode);
    m_invisible.ToPacket(d);
    m_sound.ToPacket(d);

    n = m_vertices.isize();

    if (bTrunc)
      n = 1;

    d.Write(n);
   
    for (i=0; i<n; i++) {
      m_vertices[i].ToPacket(d);
    }

    n = m_indices.isize();
    if (bTrunc)
      n = 0;
   
    d.Write(n);

    for (i=0; i<n; i++) {
      d.Write(m_indices[i]);
    }
    // ===============
    d.Write(m_texture);
    n = m_normals.isize();
    if (bTrunc)
      n = 1;
    d.Write(n);
    for (i=0; i<n; i++) {
      m_normals[i].ToPacket(d);
    }
    n = m_texCoords.isize();
    if (bTrunc)
      n = 1;
    d.Write(n);
    for (i=0; i<n; i++) {
      m_texCoords[i].ToPacket(d);
    }
  }

  int SizeInBytes() const {
    int n = sizeof(StreamCoordinates) * (m_vertices.isize()+1) + 
      sizeof(int) * m_indices.isize();
    return n;
  }

 private:
  StreamCoordinates m_abs;
  StreamCoordinates m_rot; 
  StreamCoordinates m_direction;
  StreamCoordinates m_rotation;
  
  int m_hasRot;
  int m_physMode;
  svec<StreamCoordinates> m_vertices;
  svec<int> m_indices;
  
  svec<StreamCoordinates> m_normals;
  string m_texture;
  svec<StreamCoordinates> m_texCoords;
  double m_shiny;
  string m_animation;
  MsgSound m_sound;

};

//========================================
//=============================================
class SceneNode : public NameType
{
 public:
  SceneNode() {
    m_direction[0] = 1.;
    m_direction[1] = 0.;
    m_direction[2] = 0.;
  }

  void SetCoordinates(const Coordinates & c) {
    m_coords = c;
  }
  void SetCoordinates(const StreamCoordinates & c) {
    m_coords = c;
  }
  const StreamCoordinates & GetCoordinates() const {return m_coords;}

  void SetDirection(const Coordinates & c) {
    m_direction = c;
  }
  void SetDirection(const StreamCoordinates & c) {
    m_direction = c;
  }
  const StreamCoordinates & GetDirection() const {return m_direction;}
  
  void SetMesh(const string & mesh) {
    m_mesh = mesh;
  }
  void SetTexture1(const string & texture) {
    m_texture1 = texture;
  }
  void SetTexture2(const string & texture) {
    m_texture2 = texture;
  }

  const string & GetMesh() const {return m_mesh;}
  const string & GetTexture1() const {return m_texture1;}
  const string & GetTexture2() const {return m_texture2;}

  MsgSound & Sound() {return m_sound;}

  virtual void FromPacket(DataPacket & d) {
    m_coords.FromPacket(d);
    d.Read(m_scale);
    m_direction.FromPacket(d);
    d.Read(m_mesh);
    d.Read(m_texture1);
    d.Read(m_texture2);
    d.Read(m_name);
    d.Read(m_type);
    d.Read(m_physics);
    d.Read(m_control);
    d.Read(m_bLighting);
    m_invisible.FromPacket(d);
    m_sound.FromPacket(d);
  }

  virtual void ToPacket(DataPacket & d) const {
    m_coords.ToPacket(d);
    d.Write(m_scale);
    m_direction.ToPacket(d);
    d.Write(m_mesh);
    d.Write(m_texture1);
    d.Write(m_texture2);
    d.Write(m_name);
    d.Write(m_type);
    d.Write(m_physics);
    d.Write(m_control);
    d.Write(m_bLighting );
    m_invisible.ToPacket(d);
    m_sound.ToPacket(d);
 }

 private:
  StreamCoordinates m_coords;
  StreamCoordinates m_direction;
  string m_mesh;
  string m_texture1;
  string m_texture2;
  MsgSound m_sound;

};


class MessageHeader
{
 public:
  MessageHeader() {}
  
  virtual void FromPacket(DataPacket & d) {
    m_ts.FromPacket(d);
    d.Read(m_header);
  }

  virtual void ToPacket(DataPacket & d) {
    m_ts.Update();
    m_ts.ToPacket(d);
    d.Write(m_header);
  }

  const TimeStamp & GetTimeStamp() {return m_ts;}
  const string & GetHeader() {return m_header;}
  void SetHeader(const string & s) {m_header = s;}
   
 private:
  TimeStamp m_ts;
  string m_header;
};

// ****************************************************
// END - DEPRECATED!!! WILL BE OBSOLETE!!!!!!!
// ****************************************************





#endif


