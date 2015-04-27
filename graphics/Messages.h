#ifndef MESSAGES_H
#define MESSAGES_H

#include "util/StreamComm.h"
#include "physics/Coordinates.h"

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

//======================================
class StreamCoordinates : public Coordinates
{
 public:

  void FromPacket(DataPacket & d) {
    d.Read((*this)[0]);
    d.Read((*this)[1]);
    d.Read((*this)[2]);
  }

  void ToPacket(DataPacket & d) {
    d.Write((*this)[0]);
    d.Write((*this)[1]);
    d.Write((*this)[2]);
  }
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

  void ToPacket(DataPacket & d) {
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

#endif


