
#include "visual/Whiteboard.h"

#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "base/SVector.h"
#include "visual/Color.h"
#include "physics/Coordinates.h"
#include "base/RandomStuff.h"
#include "npc/NNet.h"

class Pendulum
{
public:
  Pendulum() {
    m_top = Coordinates(0.5, 1., 0.5);
    m_bottom = Coordinates(0.5, 0.5, 0.5);
    //m_rot = 0.1-RandomFloat(0.2);
    m_rot = 0.1;
  }


  const Coordinates & Top() const {return m_top;}
  const Coordinates & Bottom() const {return m_bottom;}
  
  double Rot() const {return m_rot;}

  void SetAccelerate(const Coordinates & a) {
    m_v += a;
    cout << "v=" << m_v[2] << " a=" << a[2] << endl;
  }
  void SetVelocity(const Coordinates & v) {
    m_v = v;
    //cout << "v=" << m_v[2] << " a=" << a[2] << endl;
  }

  void Update(double deltatime) {
    Coordinates rel = m_top - m_bottom;
    rel.Flip();
    double ll = rel.Length();
    SphereCoordinates s = rel.AsSphere();
    s.NonNegative();
    cout << "Theta raw " << s.theta() << " phi " << s.phi() <<  endl;
    double theta = s.theta();
    double ag = 9.81*sin(theta);

    
    //if (s.phi() > 0) {
    //  ag = -ag;
    //}
    m_rot += deltatime*ag;
    

    //cout << m_rot << " " << theta << " " << "ag " << ag << " " << sin(theta) << endl;
    theta += m_rot*deltatime;
    s.SetTheta(theta);    
    double lat = sin(theta);
    double r = cos(theta);
    m_top.FromSphere(s);
    m_top.Flip();

    
    m_top += m_bottom;
    cout << "DEBUG before " << m_bottom[2] << " -> " << m_top[2] << " (" << m_v[2]*deltatime << ")" << endl; 
     m_bottom += m_v*deltatime;
    //m_top[1] += m_v[2]*deltatime*lat;
    m_top[2] += m_v[2]*deltatime*lat;
    //cout << "lat=" << lat << " tang=" << r << endl;
    cout << "DEBUG after  " << m_bottom[2] << " -> " << m_top[2] << endl; 
  
    rel = (m_top - m_bottom).Einheitsvector()*ll;
    m_top = m_bottom + rel;
    //m_top[2] += m_v[2]*deltatime*lat;
    //m_top[1] += m_v[2]*deltatime*r;
    
  }

private:
  Coordinates m_top;
  Coordinates m_bottom;
  Coordinates m_v;
  double m_rot;
  
};


void OneFrame(const string & o, const Pendulum & t)
{
  double x_offset = 20;
  double y_offset = 20;


  int i, j, k;
  ns_whiteboard::whiteboard board;

 
  double x_max = 500.;
  double y_max = 500.;
  board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(0, 0), 
				      ns_whiteboard::xy_coords(2*x_offset+x_max, 2*y_offset+y_max), 
				      color(0.9,0.9,0.9)));

  //cout << "top    " << t.Top()[0] << " " << t.Top()[1] << " " << t.Top()[2] << endl;
  //cout << "bottom " << t.Bottom()[0] << " " << t.Bottom()[1] << " " << t.Bottom()[2] << endl;

  //double scale = 200.;
  //double plus = 150.;
  double scale = 400.;
  double plus = 50.;
  board.Add( new ns_whiteboard::line( ns_whiteboard::xy_coords(x_offset+t.Top()[2]*scale+plus, y_offset+t.Top()[1]*scale), 
				      ns_whiteboard::xy_coords(x_offset+t.Bottom()[2]*scale+plus, y_offset+t.Bottom()[1]*scale), 
				      2.,
				      color(0.,0.,0.)));
  ofstream out(o.c_str());
  
  ns_whiteboard::ps_display display(out, x_max + 2 * x_offset, y_max + 2 * y_offset);
  board.DisplayOn(&display);
 
}


int main( int argc, char** argv )
{
  /*
  commandArg<string> iStringO("-i","input");
  commandArg<string> aStringO("-o","outfile (post-script)");
 
  
  commandLineParser P(argc,argv);
  P.SetDescription("Animation example");

  P.registerArg(iStringO);
  P.registerArg(aStringO);

  P.parse();
  */
  NeuralNetwork nn;
  nn.Setup(20, 2);
  nn.ReSetup(1, -6, 6);
  nn.ReSetup(0, -0.5, 0.5);

  int i, j;
  Pendulum p;
  double deltatime = 0.02;
  int k = 1000;
  double sum = 0.;
  double lastZ = p.Top()[2]-p.Bottom()[2];


  NPCIO io;
  io.AddElement("top");
  io.AddElement("move");

  double lastTop = p.Top()[1];
 
  double lastMove = 0.;
  bool bDo = false;
  char name[256];
  
  //int train = 10000;
  int train = 0;
  int off = train;


  double stupid_fac = 1.;
  for (i=0; i<off+300; i++) {
    double z = p.Top()[2]-p.Bottom()[2];
    double a;

    // STUPID WAY!!!
    double a_stupid = (z - lastZ)*55*stupid_fac;
    //cout << "DEBUG DUMMY: " << a << endl;

    double top = p.Top()[1];
    double diff = top - lastTop;
    

    if (p.Top()[1] < 0.1 || p.Bottom()[2] < 0. || p.Bottom()[2]>1.) {
      cout << "DEBUG: Reset" << endl;
      io.SetElement("top", lastTop);
      io.SetElement("move", lastMove);
      io.SetValid(0, true);
      io.SetValid(1, true);
      double weight = 1.;    
      //nn.LearnAvoid(io, weight);
      stupid_fac += 0.2;

      Pendulum np;
      p = np;
      top = lastTop = p.Top()[1];
      lastZ = z = p.Top()[2]-p.Bottom()[2];
      bDo = false;
 
      if (train <= 0) {
	sprintf(name, "tmp%d.ps", k);
	k++;
	OneFrame(name, p);
	sprintf(name, "tmp%d.ps", k);
	k++;
	OneFrame(name, p);
	sprintf(name, "tmp%d.ps", k);
	k++;
	OneFrame(name, p);
	sprintf(name, "tmp%d.ps", k);
	k++;
	OneFrame(name, p);
      }
    }

    cout << "DEBUG Coordinates: " << k << ": " << p.Bottom()[2] << "," << p.Bottom()[1] << " <-> " << p.Top()[2] << "," << p.Top()[1] << endl;

  
    if (bDo) {
      io.SetElement("top", lastZ);
      io.SetElement("move", lastMove);
      io.SetValid(0, true);
      io.SetValid(1, false);
      double weight = 1;
      cout << "TOP " << top << " last=" << lastTop << endl;
      cout << "++++++++++++++ Before ++++++++++++++++++" << endl;
      nn.Print();
      if (top < lastTop) {
	cout << "DEBUG: Learn avoid!" << endl;
	//io.SetValid(1, false);
	nn.LearnAvoid(io, 1. - p.Top()[1]);
      } else {
	io.SetElement("move", lastMove);
	cout << "DEBUG: Learn positive! last move: " << lastMove << endl;
	//int many = (int)(20*lastTop);
	for (j=0; j<1; j++)
	  nn.Learn(io, 1.);
	
      }
      cout << "++++++++++++++ After +++++++++++++++++++" << endl;
      nn.Print();


    } else {
      bDo = true;
    }

    lastTop = top;
   
    io.SetElement("top", z);
    io.SetValid(0, true);
    io.SetValid(1, false);
    cout << "Retrieve for " << io[0] << " " << io[1] << endl;
    //cout << "++++++++++++++ Retrieve? +++++++++++++++++" << endl;
    //nn.Print();
    nn.Retrieve(io);
    //cout << "++++++++++++++ Retrieved +++++++++++++++++" << endl;
    //nn.Print();
    a = io[1];
    lastMove = a;

    cout << "DEBUG action, k=" << k << "  diff=" << z - lastZ << " z=" << z << " last=" << lastZ << " a=" << a << endl;
    cout << "DEBUG lastTop " << lastTop << endl;
    lastZ = z;
    //if (p.Rot() > 0)
    //a = -a;

    a = a_stupid;

    p.SetVelocity(Coordinates(0., 0., a));
    p.Update(deltatime);
    if (train <= 0) {
      sprintf(name, "tmp%d.ps", k);
      k++;
      OneFrame(name, p);
      cout << "Printing frame!" << endl;
    }
    train--;
  }
  
  
  

  //OneFrame("tree.ps", tree);

  return 0;
}
