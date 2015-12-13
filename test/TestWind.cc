#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Bitmap.h"
#include "visual/Canvas.h"
#include "physics/Wind.h"
#include "base/RandomStuff.h"

int main( int argc, char** argv )
{

  /* commandArg<string> fileCmmd("-i","input file");
  commandArg<int> bCmmd("-from","from column");
  commandArg<int> eCmmd("-to","to column");
  commandArg<bool> nCmmd("-newline","add newline", false);
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(bCmmd);
  P.registerArg(eCmmd);
  P.registerArg(nCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  int from = P.GetIntValueFor(bCmmd);
  int to = P.GetIntValueFor(eCmmd);
  bool bN = P.GetBoolValueFor(nCmmd);*/
  

  /*Coordinates a = Coordinates(1., 5, 0);
  cout << a[0] << " " << a[1] << " " << a[2] << endl;
  SphereCoordinates h = a.AsSphere();
  h.SetTheta(1.22);
  h.SetPhi(1.5708);
  a.FromSphere(h);
  cout << a[0] << " " << a[1] << " " << a[2] << endl;
  return 0;*/
  

  int i, j;
  
  Wind wind;

  wind.SetSize(256, 1, 256);
  ParticleMovement move;
  move.Setup(30000, Coordinates(256, 1, 256));

  svec<LinearWindShape> lin;
  lin.resize(25);

  svec<CircleWindShape> circle;
  circle.resize(25);
  for (i=0; i<circle.isize(); i++) {
    circle[i].SetCenter(Coordinates(80+RandomFloat(140), 0, RandomFloat(240)), 5+RandomFloat(10), 1-RandomFloat(2));
  }


  UpDownWindShape updown;
  updown.SetCenter(Coordinates(128., 0., 0.));
  updown.SetWidth(8.);
  
  for (i=0; i<lin.isize(); i++) {
    lin[i].SetSpeed(0.1 + RandomFloat(1.));
    double x = 64+RandomFloat(128);
    double y = 1-RandomFloat(2);
    cout << "Adding " << x << " " << y << endl;
    double z = 0.1;
    if (RandomFloat(1.) < 0.5)
      z = -z;
    lin[i].Set(Coordinates(x, 0., 128), 
	       Coordinates(z, 0., y));
  }

  int k = 1000;
  int reset = 0;
  for (i=0; i<800; i++) {
    wind.Clear();
 
    
    if (true || i % 10 == 0) {
      int index = RandomInt(lin.isize());
      LinearWindShape tmp;

      lin[index] = tmp;
      lin[index].SetSpeed(0.1 + RandomFloat(1.));
      double z = 0.1;
      if (RandomFloat(1.) < 0.5)
	z = -z;
      lin[index].Set(Coordinates(64+RandomFloat(128), 0., 80), 
		     Coordinates(z, 0., 1-RandomFloat(2)));
    }


    for (j=0; j<lin.isize(); j++) {
      lin[j].Update(7.);
      wind.Process(lin[j]);
    }

    int ic = RandomInt(circle.isize());
    circle[ic].SetCenter(Coordinates(100+RandomFloat(140), 0, RandomFloat(240)), 5+RandomFloat(20), 1-RandomFloat(2));
    
    for (j=0; j<circle.isize(); j++) {    
      wind.Process(circle[j]);
    }



    //updown.Update(0.1);
    wind.Process(updown);
    move.Add(20+RandomInt(140), Coordinates(128., 0, 5.), 8.);
    move.AddRandomNoise(0.1);
    
    CircleWindShape circle;
    circle.SetCenter(Coordinates(64, 0, 64), 20, 8);
    wind.Process(circle);


    Canvas c1;
    c1.resize(256, 256);
    
    for (int x=0; x<wind.X(); x++) {
      for (int z=0; z<wind.Z(); z++) {     
	const Coordinates & v = wind.Get(x, 0, z).Direction();
	
	CanvasPixel & p = c1.Pixel(x, z);
	p.R() = 0.5 + v[0]/4;
	p.G() = 0.5 + v[1]/4;
	p.B() = 0.5 + v[2]/4;
      }
    }
    
    Bitmap bmp1;
    c1.ToBitmap(bmp1);
    char name2[256];
    sprintf(name2, "data/line%d.bmp", k);
    bmp1.Write(name2);
    

    cout << "Iteration: " << i << endl;
 
   
    Canvas c;
    c.resize(256, 256);

    move.Move(wind, 0.2);

    
    for (j=0; j<move.used(); j++) {
      WindParticle & p = move[j];
      CanvasPixel & pix = c.Pixel(p.Position()[0], p.Position()[2]);
      pix.V() += .2;
      //pix.V() += 1;
      //cout << "Particle " << j << " " << p.Position()[0] << " " << p.Position()[2];
      //cout <<  " . " << p.Velocity()[0] << " " << p.Velocity()[2] << endl;
      //p.Velocity() += Coordinates(0.1 - RandomFloat(0.2), 0, 0.1 - RandomFloat(0.2));
    }

    c.GreyScale();

    for (j=0; j<10; j++)
      c.Smooth();
    
    Bitmap bmp;
    c.ToBitmap(bmp);
    char name[256];
    sprintf(name, "data/wind%d.bmp", k);
    bmp.Write(name);
    k++;
  }


 
  return 0;
}
