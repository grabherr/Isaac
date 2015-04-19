#define FORCE_DEBUG

#include "core/SpatAssocMem.h"
#include "core/Matrix.h"

bool AssociativeMemory::Good(const Entity & one, const Entity & two)
{
  int i;
  
  int maxA = 0;
  int maxB = 0;
  double hiA = 0.;
  double hiB = 0.;
  for (i=0; i<one.isize(); i++) {
    if (one[i] > hiA) {
      hiA = one[i];
      maxA = i;
    }
    if (two[i] > hiB) {
      hiB = two[i];
      maxB = i;
    }    
  }
  if (maxA == maxB && hiA > m_thresh && hiB > m_thresh)
    return true;
  else
    return false;
}

void AssociativeMemory::Associate(svec<Entity> & out, const svec<Entity> & ent, const Entity & which)
{
  int i, j;
  int xmax = 0;
  int ymax = 0;
  int xmin = 0;
  int ymin = 0;


  if (ent.isize() == 0)
    return;
  int worksize = ent[0].isize();

  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    if ((int)(e.GetCoordinates().X()+0.5) > xmax) {
      xmax = (int)(e.GetCoordinates().X()+0.5);
    }
    if ((int)(e.GetCoordinates().X()-0.5) < xmin) {
      xmin = (int)(e.GetCoordinates().X()-0.5);
    }
    if ((int)(e.GetCoordinates().Y()+0.5) > ymax) {
      ymax = (int)(e.GetCoordinates().Y()+0.5);
    }
    if ((int)(e.GetCoordinates().Y()-0.5) < ymin) {
      ymin = (int)(e.GetCoordinates().Y()-0.5);
    }
  }
  
  if (xmax-xmin+1 < m_win) {
    xmax += m_win - (xmax-xmin+1);
  }
  if (ymax-ymin+1 < m_win) {
    ymax += m_win - (ymax-ymin+1);
  }

  cout << "Spat xmin: " << xmin << " ymin " << ymin << endl;
  //cout << "have boundaries." << endl;
  // TODO: Add more levels of abstraction here (rotate, scale, etc.)
  Matrix2D<Entity> mat;
  mat.SetSize(xmax-xmin+1, ymax-ymin+1);
  //cout << "x=" << xmax-xmin << " y=" <<  ymax-ymin << endl;
  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    int xx = (int)(e.GetCoordinates().X()+0.5) - xmin;
    int yy = (int)(e.GetCoordinates().Y()+0.5) - ymin;
    //cout << xx << " " << yy << endl;
    mat.Set(xx, yy, e);
  }

  Matrix2D<Entity> workmat;
  workmat = mat;
  for (i=0; i<workmat.SizeX(); i++) {
    for (j=0; j<workmat.SizeY(); j++) {
      workmat(i, j).SetCoordinates(Coordinates(i+xmin, j+ymin));
      mat(i, j).SetCoordinates(Coordinates(i+xmin, j+ymin));
    }
  }

  //cout << "Matrix set up. " << endl;
  for (i=0; i<=mat.SizeX()-m_win; i++) {
    for (j=0; j<=mat.SizeY()-m_win; j++) {
      svec<double> feat;
      //cout << "Scanning " << i << " " << j << endl;
      bool bGood = false;
      for (int x=i; x<i+m_win; x++) {
	for (int y=j; y<j+m_win; y++) {
	  Entity & e = mat(x, y);
	  if (Good(e, which))
	    bGood = true;

	  Entity & we = workmat(x, y);
	  //cout << x << " " << y << endl;

	  we.resize(0);
	  we.resize(worksize, 0.);
	  //WARNING!!!! HARD CODED
	  if (e.isize() == 0) {
	    e.resize(worksize, 0.);
	    //we.resize(4, 0.);
	  }
	  
	  for (int l=0; l<e.isize(); l++) {
	    feat.push_back(e[l]);
	    //cout << e[l] << endl;
	  }
	}
      }
      if (feat.isize() != m_size) {
	cout << "SIZE ERROR!! " << feat.isize() << " should be " <<  m_size << endl;
      }

      //cout << "Have feature." << endl;
      svec<int> valid;
      valid.resize(feat.isize(), 1);
      
      int index = m_mem.BestMatch(feat, valid);
      SOMNode & node = m_mem.GetNode(index);
      const svec<double> & cc = node.Coords();
      //cout << "Neuron size: " << cc.isize() << endl;
      //cout << "Best node: " << index << endl;
      int k = 0;
      for (int x=i; x<i+m_win; x++) {
	for (int y=j; y<j+m_win; y++) {
	  Entity & we = workmat(x, y);
	  Entity & e = mat(x, y);
	  //cout << "Entity @ " << x << " " << y << " -> " << i << " " << j << endl;
	  for (int l=0; l<e.isize(); l++) {
	    //cout << l << " " << e[l] << " " << we[l] << " + " << cc[k] << endl;
	    we[l] += cc[k]/m_weight;
	    k++;
	  }
	}
      }
      if (bGood) {
	//cout << "ASSOC MEM Train feature!" << endl;
	m_mem.Train(feat, valid);
      } // if (bGood) 
    }
  }
  //cout << "Almost." << endl;
  m_weight *= 0.98;
  if (m_weight < 1.)
    m_weight = 1.;
  
  out = ent;
  for (i=0; i<workmat.SizeX(); i++) {
    for (j=0; j<workmat.SizeY(); j++) {
      if (Good(workmat(i, j), which)) {
	mat(i, j) += workmat(i, j);
	const Entity & nn = mat(i, j);
	
	//cout << "ASSOC MEM Found: " << nn.GetCoordinates().X() << "," << nn.GetCoordinates().Y() << " ";
	//cout << nn[0] << " " << nn[1] << " " << nn[2] << " " << nn[3] << " " << endl;

	// STUPID!!!!!!!!
	bool bExist = false;
	for (int z=0; z<ent.isize(); z++) {
	  if (out[z].GetCoordinates() == nn.GetCoordinates()) {
	    out[z] += nn;
	    bExist = true;
	  }
	} 
	if (!bExist)
	  out.push_back(mat(i, j));
      }
    }
  }
  //cout << "Done." << endl;
}


void AssociativeMemory::Setup(int size, int window)
{
  m_size = size * window * window;
  m_win = window;
  cout << m_size << endl;
  OneDimSOMOrganizer one;
 
  int somSize = 200;
  one.SetDimRange(m_size, 0., 0.05);
  
  cout << "Organizing associative SOM nodes..." << endl;
  m_mem.Organize(one, somSize);
}
