#define FORCE_DEBUG

#include "core/Cerebellum.h"


void Cerebellum::FindBestScore(int & x, int & y, int & z, int & w) const
{
  int i, j, k, l;
  double min = CHIGH;
  for (i=0; i<m_scores.isize_x(); i++) {
    for (j=0; j<m_scores.isize_y(); j++) {
      for (k=0; k<m_scores.isize_z(); k++) {
	for (l=0; l<m_scores.isize_w(); l++) {
	  if (m_scores(i, j, k, l).Score() < min) {
	    min = m_scores(i, j, k, l).Score();
	    x = i;
	    y = j;
	    z = k;
	    w = l;
	  }
	}
      }
    }
  }
}

void Cerebellum::FindBestCell(int & x, int & y, int & z, int & w) const
{
  int i, j, k, l;
  double min = CHIGH;
  
  for (i=0; i<m_cells.isize_x(); i++) {
    for (j=0; j<m_cells.isize_y(); j++) {
      for (k=0; k<m_cells.isize_z(); k++) {
	for (l=0; l<m_cells.isize_w(); l++) {
	  if (m_cells(i, j, k, l).Cost() < min) {
	    min = m_cells(i, j, k, l).Cost();
	    x = i;
	    y = j;
	    z = k;
	    w = l;
	  }
	}
      }
    }
  }
}

bool Cerebellum::FindPath(svec<PathItem> & out, int x, int y, int z, int w,
			  int tx, int ty, int tz, int tw)
{
  //cout <<  m_scores(x, y, z, w).Score() << endl;
  m_scores(x, y, z, w).SetScore(0.);
  // cout <<  m_scores(x, y, z, w).Score() << endl;
  PathItem start;
  start.Set(x, y, z, w);
  
  if (tx == -1) {
    cout << "Finding best cell" << endl;
    FindBestCell(tx, ty, tz, tw);
  } else {
    cout << "Skip cell finding." << endl;
  }

  PathItem target;
  target.Set(tx, ty, tz, tw);
  
  cout << "Start at ";
  start.Print();
  cout << "Target at ";
  target.Print();

  if (target == start)
    return false;
  
  int i, j, k, l;
  /*cout << "Total: " << m_scores.isize() << endl;
  for (i=0; i<m_scores.isize_x(); i++) {
    for (j=0; j<m_scores.isize_y(); j++) {
      cout << i << " " << j << " - " << m_scores(i, j).BackX() << " " << m_scores(i, j).BackY() << endl;
      cout << "Index: " <<  m_scores.index(i, j) << endl;
    }
  }
  exit(0);*/

  bool bTarget = false;
  /* int extra = 0;
  int extraMax = 20;
  int total = 0;
  int totalMax = 300;
  while (total < totalMax && extra < extraMax) {
    total++;
    if (bTarget)
    extra++;*/

  for (int m=0; m<m_scores.isize(); m++) {
    //cout << "Update: " << m << endl;
    for (i=0; i<m_scores.isize_x(); i++) {
      for (j=0; j<m_scores.isize_y(); j++) {
	for (k=0; k<m_scores.isize_z(); k++) {
	  for (l=0; l<m_scores.isize_w(); l++) {
	    // Two dimensions only (for now)
	    double score = m_scores(i, j, k, l).Score();
	    //cout << "Before: " << score << endl;
	    Minimize(i, j, k, l, i-1, j, k, l);
	    Minimize(i, j, k, l, i+1, j, k, l);

	    Minimize(i, j, k, l, i-1, j-1, k, l);
	    Minimize(i, j, k, l, i+1, j-1, k, l);

	    Minimize(i, j, k, l, i-1, j+1, k, l);
	    Minimize(i, j, k, l, i+1, j+1, k, l);

	    Minimize(i, j, k, l, i, j-1, k, l);
	    Minimize(i, j, k, l, i, j+1, k, l);
	    score = m_scores(i, j, k, l).Score();
	    if (score < 0.) {
	      cout << "ERROR, score=" << score << " " << i << " " << j << endl;  
	    }
	    //cout << "After: " << score << endl;
	    //cout << "Back: " << m_scores(i, j, k, l).BackX() << endl;
	    if (score < 0. && x == tx && y == ty && z == tz && w == tw) {
	      bTarget = true;
	    }
	  }
	}
      }
    }
  }

  // Traceback now
  //cout << "Traceback." << endl;
  //cout << "Start with " << tx << " " << ty << " " << tz << " " << tw << " score: " << m_scores(tx, ty, tz, tw).Score() << endl;  
  //const  CScore & s1 = m_scores(tx, ty, tz, tw);
  //cout << "bx= " << s1.BackX() << " " << "by= " << s1.BackY() << endl;

  bool bDone = false;
  bool bSuccess = true;
  out.clear();
  out.push_back(PathItem(tx, ty, tz, tw));
  while (!bDone) {
    //cout << "Check: " << tx << " " << ty << " " << tz << " " << tw << " score: " << m_scores(tx, ty, tz, tw).Score() << endl;
   const  CScore & s = m_scores(tx, ty, tz, tw);

    tx = s.BackX();
    ty = s.BackY();
    tz = s.BackZ();
    tw = s.BackW();
    //cout << "TRACEBACK " << tx << " " << ty << " " << tz << " " << tw << " score: " << m_scores(tx, ty, tz, tw).Score() << endl;
    out.push_back(PathItem(tx, ty, tz, tw));
    if (tx == -1) {
      cout << "Failed!!!" << endl;
      bSuccess = true;
      break;
    }
    if (tx == x && ty == y && tz == z && tw == w) {
      cout << "Success!!!" << endl;
      cout << x << " " << y << endl;
      bSuccess = true;
      break;
    }
  }

  //cout << "RETURN" << endl;
  return bSuccess;
}
