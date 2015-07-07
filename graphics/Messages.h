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


//======================================
class TimeStamp
{
 public:
  TimeStamp() {
    Update();
  }

  void Update();

  void FromPacket(DataPacket & d) {
    d.Read(m_ms);
    d.Read(m_r);
  }

  void ToPacket(DataPacket & d) const {
    d.Write(m_ms);
    d.Write(m_r);
  }

  const string & GetReadable() const {return m_r;}
  int Milliseconds() const {return m_ms;}

 private:
  string m_r;
  int m_ms;
};


class NameType
{
 public:
  NameType() {}

  const string & GetName() const {return m_name;}
  void SetName(const string & s) {m_name = s;}
  const string & GetType() const {return m_type;}
  void SetType(const string & s) {m_type = s;}
  const string & GetPhysics() const {return m_physics;}
  void SetPhysics(const string & s) {m_physics = s;}
  const string & GetControl() const {return m_control;}
  void SetControl(const string & s) {m_control = s;}

 protected:
  string m_name;
  string m_type;
  string m_physics;
  string m_control;
};


//======================================
class StreamCoordinates : public Coordinates
{
 public: 

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

 private:
};


//======================================
class Terrain
{
 public:
  Terrain() {}

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

  void FromPacket(DataPacket & d) {
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

  void ToPacket(DataPacket & d) const {
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


//=============================================
class AnimatedSceneNode : public NameType
{
 public:
  AnimatedSceneNode() {
    m_animspeed = 20.;
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
  
  void SetModel(const string & model) {
    m_model = model;
  }
  void SetTexture(const string & texture) {
    m_texture = texture;
  }

  void SetAnimation(const string & anim) {
    m_animation = anim;
  }
  void SetAnimationSpeed(double s) {
    m_animspeed = s;
  }

  const string & SetTexture() const {return m_animation;}
  const string & GetModel() const {return m_model;}
  const string & GetTexture() const {return m_texture;}
  const string & GetAnimation() const {return m_animation;}
  double GetAnimationSpeed() const {return m_animspeed;}

  virtual void FromPacket(DataPacket & d) {
    m_coords.FromPacket(d);
    m_direction.FromPacket(d);
    d.Read(m_model);
    d.Read(m_texture);
    d.Read(m_type);
    d.Read(m_name);
    d.Read(m_physics);
    d.Read(m_control);
    d.Read(m_animation);
    d.Read(m_animspeed);
  }

  virtual void ToPacket(DataPacket & d) const {
    m_coords.ToPacket(d);
    m_direction.ToPacket(d);
    d.Write(m_model);
    d.Write(m_texture);
    d.Write(m_type);
    d.Write(m_name);
    d.Write(m_physics);
    d.Write(m_control);
    d.Write(m_animation);
    d.Write(m_animspeed);
  }

 private:
  StreamCoordinates m_coords;
  StreamCoordinates m_direction;
  string m_model;
  string m_texture;
  string m_animation;
  double m_animspeed;
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
  }
  const StreamCoordinates & GetAbsCoords() const {return m_abs;}
  StreamCoordinates & AbsCoords() {return m_abs;}

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

  void FromPacket(DataPacket & d) {
    int n;
    int i;
    m_vertices.clear();
    m_indices.clear();
    d.Read(m_type);
    d.Read(m_name);
    m_abs.FromPacket(d);

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
  }

  void ToPacket(DataPacket & d) const {
    int n;
    int i;
    d.Write(m_type);
    d.Write(m_name);
    m_abs.ToPacket(d);

    n = m_vertices.isize();
    d.Write(n);
   
    for (i=0; i<n; i++) {
      m_vertices[i].ToPacket(d);
    }

    n = m_indices.isize();
    d.Write(n);

    for (i=0; i<n; i++) {
      d.Write(m_indices[i]);
    }
   
  }

  int SizeInBytes() const {
    int n = sizeof(StreamCoordinates) * (m_vertices.isize()+1) + 
      sizeof(int) * m_indices.isize();
    return n;
  }

 private:
  StreamCoordinates m_abs;
  svec<StreamCoordinates> m_vertices;
  svec<int> m_indices;
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

  void FromPacket(DataPacket & d) {
    m_coords.FromPacket(d);
    m_direction.FromPacket(d);
    d.Read(m_mesh);
    d.Read(m_texture1);
    d.Read(m_texture2);
    d.Read(m_name);
    d.Read(m_type);
    d.Read(m_physics);
    d.Read(m_control);
  }

  void ToPacket(DataPacket & d) const {
    m_coords.ToPacket(d);
    m_direction.ToPacket(d);
    d.Write(m_mesh);
    d.Write(m_texture1);
    d.Write(m_texture2);
    d.Write(m_name);
    d.Write(m_type);
    d.Write(m_physics);
    d.Write(m_control);
 }

 private:
  StreamCoordinates m_coords;
  StreamCoordinates m_direction;
  string m_mesh;
  string m_texture1;
  string m_texture2;
  
};


class MessageHeader
{
 public:
  MessageHeader() {}
  
  void FromPacket(DataPacket & d) {
    m_ts.FromPacket(d);
    d.Read(m_header);
  }

  void ToPacket(DataPacket & d) {
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


#endif


