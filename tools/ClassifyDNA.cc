#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "npc/TempNN.h"
#include "applications/DNAVector.h"
#include "base/RandomStuff.h"

double Abs(double a)
{
  if (a < 0)
    return -1;
  return a;
}



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

  int i, j;

  vecDNAVector ecoli, vchol, test;
  
  ecoli.Read("data/applications/E_coli_NC_000913.fasta");
  vchol.Read("data/applications/V.cholerae_ASM674v1.fasta");

  test.Read("data/applications/test4.fasta");

  
  NeuralNetwork nn_e, nn_v;
  int size = 10;
  int neurons = 200;
  int layers = 1;
  nn_v.Setup(neurons, size*2, layers);  
  nn_v.ReSetup(-1, 1);
  nn_v.SetTimeShift(1);

  nn_e.Setup(neurons, size*2, layers);  
  nn_e.ReSetup(-1, 1);
  nn_e.SetTimeShift(1);
  
  NPCIO seq;
  seq.resize(size*2);
  seq.SetAllValid(true);

  //int n = 50;
  int n = ecoli[0].isize()/10;

  cout << "Sizes " << ecoli[0].isize() << " " << vchol[0].isize() << endl;
  
  for (i=0; i<n; i++) {
    if (i % 1000 == 0)
      cout << i << " of " << n << endl;
    int index1 = RandomInt(ecoli[0].isize()-size);
    int index2 = RandomInt(vchol[0].isize()-size);
    seq = 0;
    int k = 0;
    bool b = true;
    for (int j=index1; j<index1+size; j++) {
      if ((ecoli[0])[j] == 'A')
	seq[k] = -1.;
      if ((ecoli[0])[j] == 'T')
	seq[k] = 1.;
      k++;
      if ((ecoli[0])[j] == 'C')
	seq[k] = -1.;
      if ((ecoli[0])[j] == 'G')
	seq[k] = 1.;
      if ((ecoli[0])[j] == 'N') {
	b = false;
	break;
      }
       k++;
    }
    if (b)
      nn_e.Learn(seq);

    k = 0;
    seq = 0;
    b = true;
    for (int j=index2; j<index2+size; j++) {
      if ((vchol[0])[j] == 'A')
	seq[k] = -1.;
      if ((vchol[0])[j] == 'T')
	seq[k] = 1.;
      k++;
      if ((vchol[0])[j] == 'C')
	seq[k] = -1.;
      if ((vchol[0])[j] == 'G')
	seq[k] = 1.;
      k++;
      if ((ecoli[0])[j] == 'N') {
	b = false;
	break;
      }
    }
    if (b)
      nn_v.Learn(seq);
  }

  double score = 0.;
  cout << "Print E. coli" << endl;
  nn_e.Print();
  cout << "Print V. cholerae" << endl;
  nn_v.Print();
  cout << "Testing." << endl;
  double s_e1 = 0.;
  double s_v1 = 0.;
  double s_e2 = 0.;
  double s_v2 = 0.;
  for (i=0; i<400; i++) { 
    int index1 = RandomInt(test[0].isize()-size);
    int index2 = RandomInt(test[1].isize()-size);
    cout << endl << "===================" << endl;
    int k = 0;
    seq = 0;
    for (int j=index1; j<index1+size; j++) {
      if ((ecoli[0])[j] == 'A')
	seq[k] = -1.;
      if ((test[0])[j] == 'T')
	seq[k] = 1.;
      k++;
      if ((test[0])[j] == 'C')
	seq[k] = -1.;
      if ((test[0])[j] == 'G')
	seq[k] = 1.;
      k++;
    }
    nn_e.Retrieve(seq, score);
    cout << "Score (e) " << score << endl;
    s_e1 += score;
    nn_v.Retrieve(seq, score);
    cout << "Score (v) " << score << endl;
    s_v1 += score;
    
    k = 0;
    seq = 0;
    for (int j=index2; j<index2+size; j++) {
      if ((test[1])[j] == 'A')
	seq[k] = -1.;
      if ((test[1])[j] == 'T')
	seq[k] = 1.;
      k++;
      if ((test[1])[j] == 'C')
	seq[k] = -1.;
      if ((test[1])[j] == 'G')
	seq[k] = 1.;
      k++;
    }
    cout << "---------------" << endl;
    nn_e.Retrieve(seq, score);
    s_e2 += score;
    cout << "Score (e) " << score << endl;
    nn_v.Retrieve(seq, score);
    s_v2 += score;
    cout << "Score (v) " << score << endl;
  }

  cout << endl;
  cout << s_e1 << " " << s_v1 << endl;
  cout << s_e2 << " " << s_v2 << endl;
  return 0;
}
