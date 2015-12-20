#ifndef WAVES_H
#define WAVES_H

#include "visual/Canvas.h"
#include "base/RandomStuff.h"


class IWaveForm
{
 public:
  IWaveForm() {
    m_speed = 1.;
  }

  virtual ~IWaveForm() {}

  Coords2D & Source() {return m_source;}
  const Coords2D & Source() const {return m_source;}
  void SetSource(const Coords2D & d) {
    m_source = d;
  }

  double Speed() const {return m_speed;}
  void SetSpeed(double v) {m_speed = v;}
 
  virtual double Value(const Coords2D & c, double time) const = 0;

 protected:
  Coords2D m_source;
  double m_speed;

};


class SingleWaveForm : public IWaveForm
{
public:
  SingleWaveForm() {
    m_r1 = 0.2;
    m_r2 = 0.13;
    m_c1 = 7;
    m_c2 = 17;   
  }

  virtual double Value(const Coords2D & c, double time) const;

 
private:
  double m_r1;
  double m_r2;
  double m_c1;
  double m_c2;
};


class LaplaceWaveForm : public IWaveForm
{
 public:
  LaplaceWaveForm() {
    m_height = 0.4;
    m_depth = 25;
    m_speed = 0.1;
  }
  void SetHeight(double d) {m_height = d;}
  void SetDepth(double d) {m_depth = d;}

  
  virtual double Value(const Coords2D & c, double time) const;

  const Coords2D & GetDirection() const {return m_direction;}
  void SetDirection(const Coords2D & d) {
    m_direction = d;
  }

 private:
  Coords2D m_direction;
  double m_width;
  double m_depth;
  double m_height;
};




class WavePainter
{
 public:
  WavePainter() {}
  
  void Paint(Canvas & c, const IWaveForm & f, double time);
};


//===================================================
class WaveRepo
{
 public:
  WaveRepo() {
    m_x = m_y = 0;
  }

  void SetSize(int x, int y) {
    m_x = x;
    m_y = y;
  }

  int X() const {return m_x;}
  int Y() const {return m_y;}

  void AddWave(const IWaveForm & w, double time);

  void Read(const string & fileName);
  void Write(const string & fileName);

  void AddToCanvas(Canvas & c, int x_pos, int y_pos, int index);

  int isize() const {return m_repo.isize();}

 protected:

  svec<SparseCanvas> m_repo;
  int m_x;
  int m_y;
};



// Hard coded size (for now)
class CalmWaterSurface
{
 public:
  CalmWaterSurface() {
    m_x = 512;
    m_y = 256;
    m_iniSize = 200;
    m_time = 0.;
  }

  void SetSize(int x, int y) {
    m_x = x;
    m_y = y;
  }
  int X() const {return m_x;}
  int Y() const {return m_y;}

  void SetInitSize(int d) {
    m_iniSize = d;
  }

  void BuildRepo();

  void Read(const string & fileName) {
    m_repo.Read(fileName);
    SetQueueSize(m_repo.isize());
  }

  void Write(const string & fileName) {
    m_repo.Write(fileName);
  }


  void Get(Bitmap & out, double deltatime);

  // Must be less tham the repo size
  void SetQueueSize(int n);

  int GetQueueSize() const {return m_queue.isize();}

  void FromCache() {
    m_cache.FromCache();
  }

 private:
  void MoveTime();

  int m_x;
  int m_y;
  int m_iniSize;
  WaveRepo m_repo;
  double m_time;
  svec<Coords2D> m_queue;
  svec<int> m_qtime;

  RandomCache m_cache;
};



#endif //WAVES_H
