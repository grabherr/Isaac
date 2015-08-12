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

  void SetLighting(bool b) {
    if (b) 
      m_bLighting = 1;
    else
      m_bLighting = 0;
  }
 
  bool GetLighting() const {return (m_bLighting != 0);}

 protected:
  string m_name;
  string m_type;
  string m_physics;
  string m_control;
  double m_scale;
  int m_bLighting;
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
    m_physMode = 0;
    m_shiny = 12.;
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
    m_texture = texture;
  }

  void SetAnimation(const string & anim) {
    m_animation = anim;
  }
  void SetAnimationSpeed(double s) {
    m_animspeed = s;
  }

  double GetShinyness() const {return m_shiny;}
  void SetShinyness(double d) {m_shiny = d;}

  const string & SetTexture() const {return m_animation;}
  const string & GetModel() const {return m_model;}
  const string & GetTexture() const {return m_texture;}
  const string & GetAnimation() const {return m_animation;}
  double GetAnimationSpeed() const {return m_animspeed;}

  int PhysMode() const {return m_physMode;}
  void SetPhysMode(int n) {m_physMode = n;}

  virtual void FromPacket(DataPacket & d) {
    m_coords.FromPacket(d);
    m_direction.FromPacket(d);
    m_rot.FromPacket(d);
    d.Read(m_scale);
    d.Read(m_model);
    d.Read(m_texture);
    d.Read(m_type);
    d.Read(m_name);
    d.Read(m_physics);
    d.Read(m_control);
    d.Read(m_animation);
    d.Read(m_animspeed);
    d.Read(m_physMode);
    d.Read(m_bLighting);
    d.Read(m_shiny);
  }

  virtual void ToPacket(DataPacket & d) const {
    m_coords.ToPacket(d);
    m_direction.ToPacket(d);
    m_rot.ToPacket(d);
    d.Write(m_scale);
    d.Write(m_model);
    d.Write(m_texture);
    d.Write(m_type);
    d.Write(m_name);
    d.Write(m_physics);
    d.Write(m_control);
    d.Write(m_animation);
    d.Write(m_animspeed);
    d.Write(m_physMode);
    d.Write(m_bLighting);
    d.Write(m_shiny);
  }

 private:
  StreamCoordinates m_coords;
  StreamCoordinates m_direction;
  StreamCoordinates m_rot; 
  string m_model;
  string m_texture;
  string m_animation;
  double m_animspeed;
  int m_physMode;
  double m_shiny;
};

//==============================================
class PhysModelNode : public AnimatedSceneNode
{
 public:
  PhysModelNode() {}
 private:
};

//==============================================
class LightNode : public NameType
{
 public:
  LightNode() {
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

  void FromPacket(DataPacket & d) {
    d.Read(m_type);
    d.Read(m_name);
    m_pos.FromPacket(d);
    d.Read(r);
    d.Read(g);
    d.Read(b);
    d.Read(radius);
    d.Read(texture);
  }
  void ToPacket(DataPacket & d) const {
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

//==============================================
class MeshModel : public NameType
{
 public:
  MeshModel() {
    m_physMode = 0;
    m_shiny = 12.;
  }
  const StreamCoordinates & GetAbsCoords() const {return m_abs;}
  StreamCoordinates & AbsCoords() {return m_abs;}

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

  void FromPacket(DataPacket & d) {
    int n;
    int i;
    m_vertices.clear();
    m_indices.clear();
    d.Read(m_type);
    d.Read(m_name);
    d.Read(m_scale);
    d.Read(m_bLighting);
    d.Read(m_shiny);
    m_abs.FromPacket(d);
    m_rot.FromPacket(d);
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

  void ToPacket(DataPacket & d) const {
    int n;
    int i;
    d.Write(m_type);
    d.Write(m_name);
    d.Write(m_scale);
    d.Write(m_bLighting);
    d.Write(m_shiny);
    m_abs.ToPacket(d);
    m_rot.ToPacket(d);
    d.Write(m_physMode);
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
    // ===============
    d.Write(m_texture);
    n = m_normals.isize();
    d.Write(n);
    for (i=0; i<n; i++) {
      m_normals[i].ToPacket(d);
    }
    n = m_texCoords.isize();
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
  int m_physMode;
  svec<StreamCoordinates> m_vertices;
  svec<int> m_indices;
  
  svec<StreamCoordinates> m_normals;
  string m_texture;
  svec<StreamCoordinates> m_texCoords;
  double m_shiny;
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
  }

  void ToPacket(DataPacket & d) const {
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


