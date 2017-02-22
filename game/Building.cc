#include "game/Building.h"

SchoolBuilding::SchoolBuilding()
{
  int i, j;
  int n = 50;
  m_data.resize(n);
  for (i=0; i<n; i++) {
    m_data[i].resize(n, 1.);    
  }

  m_xoff = -200;
  m_zoff = 90;
  m_scale = 700/(double)n;

  // Clear main area
  Fill(-200, 460, 88, 520, 0.);
  
  for (i=0; i<n; i++) {
    svec<double> & d = m_data[i];
    for (j=0; j<n; j++) {
      if (j == 0 || i == 0 || j == n-1 || i == n-1)
	d[j] = 1.;
    }
  }
  // Trees
  
  Fill(222, 519, 0.9);
  Fill(-115, 342, 0.9);
  Fill(-115, 153, 0.9);
  Fill(355, 148, 0.9);
  Fill(452, 276, 0.9);
  
  // Areas
  Fill(-30, 170, 470, 540, 1.);
  Fill(-9, 274, 90, 161, 1.);
  Fill(421, 460, 90, 230, 1.);
  
  // Well
  Fill(0, 49, 293, 338, 1.);
  //Fill(0, 49, 293, 338, 1.);
  
  Fill(-110, 140, 235, 270, .1);
  cout << "Print Building!" << endl;
  Print();
}

void SchoolBuilding::Fill(double x1, double x2, double z1, double z2, double val)
{
  int i, j;
  for (i=0; i<m_data.isize(); i++) {
    svec<double> & d = m_data[i];
    for (j=0; j<d.isize(); j++) {
      double x = (double)i*m_scale + m_xoff;
      double z = (double)j*m_scale + m_zoff;
      //cout << "Filling " << x << " " << z << " -> " << x1 << "-" << x2 << " " << z1 << "-" << z2;
      if (x >= x1 && x <= x2 && z >= z1 && z <= z2) {
	d[j] = val;
	//cout << " ## " << val << endl;
      }
      //cout << endl;
    }
  }
}
 
void SchoolBuilding::Print() const
{

  int i, j;
  for (i=0; i<m_data.isize(); i++) {
    const svec<double> & d = m_data[i];
    for (j=0; j<d.isize(); j++) {
      char tmp[256];
      sprintf(tmp, "%1.1f ", d[j]);
      cout << tmp;
    }
    cout << endl;
  }
}

bool SchoolBuilding::Adjust(double & rot, const Coordinates & pos)
{
  double v = Value(pos);
  if (v < 0.01)
    return false;
  
  rot += 0.05;
  return true;
}
