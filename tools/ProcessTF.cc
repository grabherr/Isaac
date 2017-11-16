#include <string>
#include "npc/DLNet.h"
#include "base/RandomStuff.h"
#include "base/CommandLineParser.h"
#include "base/FileParser.h"


void Load(svec<DLIOSingle> & io, svec<string> & labels, svec<string> & samples)
{

  FlatFileParser parser;
  StringParser p;
  
  parser.Open("data/TF/Astmabarn-1.csv");
  parser.ParseLine();

  p.SetLine(parser.Line(), ",");
  int i, j;
  svec<int> good;
  good.resize(p.GetItemCount(), 0);
  for (i=6; i<p.GetItemCount(); i++) {
    if (i <7 || i > 8) {
      if (i == 6 || i == 13 || i == 15 || (i >= 9 && i < 20) || (i>=39 && i<=42) || (i>=62 && i<=63)) {
      //if (i == 6 || (i < 20) || (i>=21 && i<=46)) {
	labels.push_back(p.AsString(i));
	good[i] = 1;
      }
    }
  }

  
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;

    p.SetLine(parser.Line(), ",");
 
    DLIOSingle d;
    //d.Out().resize(4, 0.);
    
    if (p.AsString(8) == "NA")
      continue;

    bool bCont = false;
    if (p.AsInt(7) == 0 && p.AsInt(8) > 0)
      bCont = true;
    if (p.AsInt(7) == 1 && p.AsInt(8) == 0)
      bCont = true;

    if (bCont) {
      cout << "Skipping " << p.AsString(0) << endl;
      continue;
    }
    
    for (i=6; i<p.GetItemCount(); i++) {
      if (i <7 || i > 8) {
	//if (i == 6 || (i < 17) || (i>=21 && i<=46))
	if (good[i] == 1)
	  d.In().push_back(p.AsFloat(i));
      }
      if (i == 8) {
	//d.Out().push_back(p.AsFloat(i)/2);
	d.Out().resize(4, 0.);
	int idx = p.AsInt(i);
	d.Out()[idx] = 1.;
      }
 	
    }
    samples.push_back(p.AsString(0));
    io.push_back(d);
    
  }

  
  for (i=0; i<labels.isize(); i++) {
    double max = 0.001;
    for (j=0; j<io.isize(); j++) {
      if (io[j].In()[i] > max)
	max = io[j].In()[i];
    }
    for (j=0; j<io.isize(); j++) {
      io[j].In()[i] /= max;
    }
  }
  
}

int main( int argc, char** argv )
{
  /*
  commandArg<string> fileCmmd("-i","input file");
  commandArg<int> bCmmd("-from","from column");
  commandArg<int> eCmmd("-to","to column");
  commandArg<int> cCmmd("-classes","class to train by");
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(bCmmd);
  P.registerArg(eCmmd);
  P.registerArg(cCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  int from = P.GetIntValueFor(bCmmd);
  int to = P.GetIntValueFor(eCmmd);
  int c = P.GetIntValueFor(cCmmd);
  */
  
  int i, j;
  
  DLNet nn;

  
  svec<DLIOSingle> io;
  svec<string> labels, samples;
    
  Load(io, labels, samples);

  cout << "Will process: " << endl;
  for (i=0; i<labels.isize(); i++)
    cout << labels[i] << endl;
  
  cout << "First layer: " << labels.isize() << " neurons." << endl;
  nn.AddForwardLayer(labels.isize());
  nn.AddForwardLayer(8);
  nn.AddForwardLayer(4);

  
  nn.SupplyInput(io);

  
  for (i=0; i<10000; i++) {
    double err = nn.TrainOne(0.2);
    cout << "Error: " << err << endl;
    if (i % 200 == 0)
      nn.Evaluate(samples);
  }
  
  return 0;
}
