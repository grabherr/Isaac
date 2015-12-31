//#define FORCE_DEBUG

#include "visual/Waves.h"
#include "util/mutil.h"

double SingleWaveForm::Value(const Coords2D & c, double time) const
{
  Coords2D d = c - m_source;

  double radius = d.Length();
  double amp = 0.5/(0.05*radius + 1);

  double x1 = d.Length()-time-m_c1;
  double x2 = d.Length()-time-m_c2;

  double v1 = -amp/(1+m_r1*x1*x1);
  double v2 =  amp/(1+m_r2*x2*x2);
  
  return v1 + v2;

}


  
void WavePainter::Paint(Canvas & c, const IWaveForm & f, double time)
{
  int i, j;
  for (i=0; i<c.X(); i++) {
    for (j=0; j<c.Y(); j++) {
 
      double v = f.Value(Coords2D(i, j), time);
      CanvasPixel & p = c.Pixel(i, j);
      p.V() += v;
    }
  }
}


//=================================================

void WaveRepo::AddWave(const IWaveForm & s, double time)
{
  Canvas c;
  c.resize(m_x, m_y);
  c.PaintAll(0.5, 0.5, 0.5, 0.5);

  WavePainter painter;
  painter.Paint(c, s, time);
  SparseCanvas sparse;
  sparse.FromCanvas(c, 0.49, 0.51);
  m_repo.push_back(sparse);
}

void WaveRepo::Read(const string & fileName)
{
  CMReadFileStream f;
  f.Open(fileName.c_str());
  f.Read(m_x);
  f.Read(m_y);
  int n;
  f.Read(n);
  m_repo.resize(n);
  //cout << m_x << " " << m_y << " " << n << endl;
  for (int i=0; i<m_repo.isize(); i++) {
    m_repo[i].Read(f);
  }
  f.Close();
}

void WaveRepo::Write(const string & fileName)
{
  CMWriteFileStream f;
  f.Open(fileName.c_str());
  f.Write(m_x);
  f.Write(m_y);
  f.Write(m_repo.isize());
  for (int i=0; i<m_repo.isize(); i++) {
    m_repo[i].Write(f);
  }
  f.Close();
}

void WaveRepo::AddToCanvas(Canvas & c, int x_pos, int y_pos, int index)
{  
  const SparseCanvas & s = m_repo[index];
  int x = s.X()/2;
  int y = s.Y()/2;
  
  m_repo[index].AddToCanvas(c, x+x_pos, y+y_pos, 0.5, true);
}


//=======================================
void CalmWaterSurface::BuildRepo()
{
  m_repo.SetSize(m_x, m_y);
  SingleWaveForm s2;

  
  s2.SetSource(Coords2D(m_repo.X()/2, m_repo.Y()/2));
  cout << "Building repo..." << endl;
  for (double time=1; time<256*2.3; time += 1.5) {
    cout << "Add " << time << " of " << 256*3 << endl;
    m_repo.AddWave(s2, time);
  }
  SetQueueSize(m_repo.isize());
}

void CalmWaterSurface::SetQueueSize(int n) 
{
  m_queue.resize(n);
  m_qtime.resize(n, 0);

  for (int i=0; i<n; i++) {
    int x = RandomInt(m_repo.X());
    int y = RandomInt(m_repo.Y());
    m_queue[i].X() = x;
    m_queue[i].Y() = y;
    m_qtime[i] = RandomInt(m_iniSize);
  }
  Sort(m_qtime);
  cout << "New queue size: " << n << endl;
  m_cache.SetSize(n*3);
  m_cache.FillCache();
  cout << "Cache full." << endl;
}

void CalmWaterSurface::MoveTime()
{
  int i;
  int n = 1;
  for (i=m_qtime.isize()-n; i>0; i--) {
    m_qtime[i] = m_qtime[i-n];
    m_queue[i] = m_queue[i-n];
    m_qtime[i]++;
  }
  for (i=0; i<n; i++) {
    m_qtime[i] = m_cache.RandomInt(m_iniSize);
    m_queue[i].X() = m_cache.RandomInt(m_repo.X());
    m_queue[i].Y() = m_cache.RandomInt(m_repo.Y());
  }
}

void CalmWaterSurface::Get(Bitmap & out, double deltatime)
{
  int i;

  Canvas cc;
  cc.resize(m_x, m_y);
  //cout << "Calm size " << m_x << " " << m_y << endl;
  cc.PaintAll(0.5, 0.5, 0.5, 0.5);

  for (i=0; i<m_queue.isize(); i++) {    
    if (m_qtime[i] < m_repo.isize())
      m_repo.AddToCanvas(cc, m_queue[i].X(), m_queue[i].Y(), m_qtime[i]);
  }

  cc.GreyScale();

   
  cc.ToBitmap(out);


  // NOT Realtime yet!!!
  MoveTime();
  m_time += deltatime;
}

//==================================================
double LaplaceWaveForm::Value(const Coords2D & c, double time) const
{
  int i;

  Coords2D rel = c - m_source;

 



  
  //double proj = rel.X()*m_direction.X() + rel.Y()*m_direction.Y();
  double dist = rel.Length();

  if (dist < 0)
    dist = -dist;
 
  // double val = m_height * exp(-dist/m_depth);
  
  double val = m_height / (1. + dist*dist/m_depth/m_depth);

  double decay = 90.;
  val *= sin(time*m_speed)*decay/(time + decay);
  
  //if (c.X() == 128 && c.Y() == 128)
  //cout << time << " -> " << val << endl;

  //cout << proj << " " << dist << " " << val << endl;
  return val;
  

}
