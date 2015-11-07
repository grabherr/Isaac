
#include <string>

#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "core/Matrix.h"
#include <unistd.h>
#include <iostream>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}



class Coords2D
{
public:
  Coords2D() {
    m_x = m_y = 1;
  }

  const int & X() const {return m_x;}
  const int & Y() const {return m_y;}
  int & X()  {return m_x;}
  int & Y()  {return m_y;}

private:
  int m_x;
  int m_y;
};

class QuickMap
{
public:
  QuickMap(int x, int y) {
    m_map.SetSize(x, y);
    //cout << x << " " << y << endl;
  }

  void Set(int y, const string & s) {
    int i;
    //cout << "y=" << y << endl;
    for (i=0; i<(int)s.size(); i++) {
      m_map.Set(i, y, s[i]);
    }   
  }

  int SizeX() const {return m_map.SizeX();}
  int SizeY() const {return m_map.SizeY();}

  void Print() const {
    int i, j, k;
    for (j=0; j<m_map.SizeY(); j++) {
      for (i=0; i<m_map.SizeX(); i++) {
	bool b = false;
	if (i == m_player.X() && j == m_player.Y()) {
	  cout << "#";
	  b = true;
	}
	for (k=0; k<m_iaus.isize(); k++) {
	  if (i == m_iaus[k].X() && j == m_iaus[k].Y()) {
	    cout << "%";
	    b = true;
	  }
	}

	if (!b)
	  cout << m_map(i, j);
      }
      cout << endl;
    }
  }
  const Coords2D & Player() const {return m_player;}
  Coords2D & Player() {return m_player;}
  const Coords2D & Iau(int i) const {return m_iaus[i];}
  Coords2D & Iau(int i) {return m_iaus[i];}

  bool PlayerX(int i) {
    if (m_map(m_player.X() + i, m_player.Y()) == ' ') {
      m_player.X() += i;
      return true;
    }
    return false;
  }
  bool PlayerY(int i) {
    if (m_map(m_player.X(), m_player.Y() + i) == ' ') {
      m_player.Y() += i;
      return true;
    }
    return false;
  }

  bool IauX(int k, int i) {
    if (m_map(m_iaus[k].X() + i, m_iaus[k].Y()) == ' ') {
      m_iaus[k].X() += i;
      return true;
    }
    return false;
  }
  bool IauY(int k, int i) {
    if (m_map(m_iaus[k].X(), m_iaus[k].Y() + i) == ' ') {
      m_iaus[k].Y() += i;
      return true;
    }
    return false;
  }


private:
  Matrix2D<char> m_map;
  Coords2D m_player;
  svec<Coords2D> m_iaus;
};


int main(int argc,char** argv)
{

  commandArg<string> aStringCmmd("-i","input file (map)");

  commandLineParser P(argc,argv);
  P.SetDescription("Quick walk-around game.");
  P.registerArg(aStringCmmd);

  P.parse();

  string fileName = P.GetStringValueFor(aStringCmmd);
  
  svec<string> tmp;
  
  FlatFileParser parser;

  int i;
  parser.Open(fileName);
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    tmp.push_back(parser.Line());
  }
  
  QuickMap qm((int)tmp[0].size(), tmp.isize());
  for (i=0; i<tmp.isize(); i++) {
    qm.Set(i, tmp[i]);
  }

  qm.Player().X() = 5;
  qm.Player().Y() = 7;
  
  string cmd;

  qm.Print();

  while (cmd != "x") {
    while (!kbhit()) {
      usleep(100);
    }
    cmd = getchar();


    if (cmd == "a") {
      qm.PlayerX(-1);	
    }
    if (cmd == "d") {
       qm.PlayerX(1);	
    }
    if (cmd == "w") {
      qm.PlayerY(-1);	
    }
    if (cmd == "s") {
      qm.PlayerY(1);	       
    }
    // Do something
    if (cmd == "e") {
      
    }
    qm.Print();
  }

  return 0;
}
