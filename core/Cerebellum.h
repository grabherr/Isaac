// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef CEREBELLUM_H
#define CEREBELLUM_H

#define FORCE_DEBUG

#include "base/MultVector.h"
#include "base/RandomStuff.h"

#define CHIGH 999999999.

class CCell
{
 public:
  CCell() {}

  void SetCost(double c) {
    m_cost = c;
  }
  
  double Cost() const {return m_cost;}

 private:
  double m_cost;
  
};

class CScore
{
 public:
  CScore() {
    m_score = CHIGH;
    m_bx = m_by = m_bz = m_bw = -1;
  }

  double Score() const {return m_score;}
  int BackX() const {return m_bx;}
  int BackY() const {return m_by;}
  int BackZ() const {return m_bz;}
  int BackW() const {return m_bw;}

  int BackSize() const {return 4;}

  void SetScore(double s) {
    m_score = s;
  }

  bool Minimize(double score, int x, int y = 0, int z = 0, int w = 0) {
    if (score < m_score) {
      m_score = score;
      m_bx = x;
      m_by = y;
      m_bz = z;
      m_bw = w;
      //cout << "Min w/ " << x << " " << y << " " << score << endl; 
      return true;
    }
    if (score == m_score && RandomFloat(1.) < 0.5) {
      m_score = score;
      m_bx = x;
      m_by = y;
      m_bz = z;
      m_bw = w;
      return true;

    }
    return false;
  }

  int Back(int i) {
    switch (i) {
    case 0:
      return m_bx;
    case 1:
      return m_by;
    case 2:
      return m_bz;
    case 3:
      return m_bw;
    default:
      return -1;
    }
    return -1;
  }

 private:
  double m_score;
  int m_bx;
  int m_by;
  int m_bz;
  int m_bw;
};


class PathItem
{
 public:
  PathItem() {
    m_x = m_y = m_z = m_w = 0;
  }
  PathItem(int x, int y, int z, int w) {
    Set(x, y, z, w);
  }
  void Set(int x, int y = 0, int z = 0, int w = 0) {
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
  }

  int X() const {return m_x;}
  int Y() const {return m_y;}
  int Z() const {return m_z;}
  int W() const {return m_w;}

  void Print() const {
    cout << "x=" << m_x << " y=" << m_y << " z=" << m_z << " w=" << m_w << endl;
  }

  bool operator == (const PathItem & p) const {
    if (m_x == p.m_x && 
	m_y == p.m_y && 
	m_z == p.m_z &&
	m_w == p.m_w)
      return true;
    return false;
  }

 private:
  int m_x;
  int m_y;
  int m_z;
  int m_w;
};

class Cerebellum
{
 public:
  Cerebellum() {
    m_trans = 0.1;
  }

  void Setup(int x, int y = 1, int z = 1, int w = 1) {
    m_scores.resize(x, y, z, w);
    m_cells.resize(x, y, z, w);

  }

  void SetScore(double score, int x, int y = 0, int z = 0, int w = 0) {
    m_scores(x, y, z, w).SetScore(score);
  }
  void SetCost(double cost, int x, int y = 0, int z = 0, int w = 0) {
    m_cells(x, y, z, w).SetCost(cost);
  }

  bool FindPath(svec<PathItem> & out, int x, int y = 0, int z = 0, int w = 0,
		int tx = -1, int ty = -1, int tz = -1, int tw = -1);

 private:
  void FindBestScore(int & x, int & y, int & z, int & w) const;
  void FindBestCell(int & x, int & y, int & z, int & w) const;
  void Minimize(int x, int y, int z, int w,
	       int x1, int y1, int z1, int w1) {
    if (m_scores.valid(x1, y1, z1, w1)) {
      double score = m_scores(x1, y1, z1, w1).Score();
      double pen = m_cells(x1, y1, z1, w1).Cost();
      
      //cout << "Min " << x1 << " -> " << x << " " << y1 << " -> " << y << endl;
      //cout << "Before: " << m_scores(x, y, z, w).BackX() << endl;
      m_scores(x, y, z, w).Minimize(score + pen + m_trans, x1, y1, z1, w1);
      //cout << "Back: " << m_scores(x, y, z, w).BackX() << endl;
    }
  }
  
  multvec<CScore> m_scores;
  multvec<CCell> m_cells;
  double m_trans;
};


#endif //CEREBELLUM_H

