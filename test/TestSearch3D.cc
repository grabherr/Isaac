#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "physics/Search3D.h"
#include "base/RandomStuff.h"


class Dummy
{
public:
  const Coordinates & Pos() const {return m_coords;}
  int Index() const {return index;}

  Coordinates m_coords;
  int index;
};

int main( int argc, char** argv )
{

  svec<Dummy> dummy;

  dummy.resize(15000);

  int i, j;

  for (i=0; i<dummy.isize(); i++) {
    dummy[i].m_coords[0] = RandomFloat(100.);
    dummy[i].m_coords[1] = RandomFloat(100.);
    dummy[i].m_coords[2] = RandomFloat(100.);
    dummy[i].index = i;
  }
  double thresh = 10.;
  int n = 0;

  
  cout << "Start" << endl;
  for (i=0; i<dummy.isize(); i++) {
    //cout << i << endl;
    for (j=0; j<dummy.isize(); j++) {
      if ((dummy[i].Pos()-dummy[j].Pos()).Length() < thresh) {
	n++;
	//cout << dummy[j].index << endl;
      }
    }
  } 
  cout << "End, found " << n << endl;
  


  
  cout << "Start" << endl;
  Search3D<Dummy> ss;
  ss.SetTo(dummy);
  n = 0;
  for (i=0; i<dummy.isize(); i++) {    
    //cout << i << endl;
    svec<int> out;
    ss.Search(out, dummy, dummy[i], thresh);
    n += out.isize();
    /*for (j=0; j<out.isize(); j++) {      
      if ((dummy[i].Pos()-dummy[out[j]].Pos()).Length() < thresh) {
	n++;
      }
      }*/
  }
  

  cout << "End, found " << n << endl;
  

  return 0;
}
