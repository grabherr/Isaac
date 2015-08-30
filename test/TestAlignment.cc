
#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "applications/DNAVector.h"

class MyManipulator : public IManipulator
{
public:
  MyManipulator() {
    m_nuke = ' ';
    m_leftright = 0;
  }

  void SetNuke(char c) {
    m_nuke = c;
  }

  virtual ~MyManipulator() {}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}
  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    cout << "Call cam pos " << endl;
    PhysObject & p = self.GetPhysObject();
    Coordinates dir = c - p.GetCenter().GetPosition();
    p.SetDirection(dir);
    cout << "Set direction ";
    dir.Print();
    cout << "My nuke " << m_nuke << endl;
    if (dir[2] > 0.) {
      //cout << "SWAP TEXTURE!" << endl;
      //p.SetTexture("data/Textures/a.png");
      //dir = p.GetCenter().GetPosition() - c;
      //p.SetDirection(dir);
      if (m_leftright != 1) {
	p.SetInvisible(Coordinates(1, 5, 5));
	switch(m_nuke) {
	case 'A':
	  p.SetTexture("data/Textures/t.png");
	  break;
	case 'C':
	  p.SetTexture("data/Textures/g_i.png");
	  break;
	case 'G':
	  p.SetTexture("data/Textures/c_i.png");
	  break;
	case 'T':
	  p.SetTexture("data/Textures/a.png");
	  break;
	}
      } else {
	p.SetTexture("");
      }
      m_leftright == 1;
    } else {
      if (m_leftright != -1) {
	p.SetInvisible(Coordinates(1, 5, 5));
	switch(m_nuke) {
	case 'A':
	  p.SetTexture("data/Textures/a.png");
	  break;
	case 'C':
	  p.SetTexture("data/Textures/c.png");
	  break;
	case 'G':
	  p.SetTexture("data/Textures/g.png");
	  break;
	case 'T':
	  p.SetTexture("data/Textures/t.png");
	  break;
	  
	}
      } else {
	p.SetTexture("");
      }
      m_leftright == -1;
    }
     
  }

  // Note: you can dynamically switch out the manipulator if you wish
  virtual void Update(GamePhysObject & o, double deltatime) {
  
  }

  virtual void Interact(GamePhysObject & self, GamePhysObject & other) {
 
  }

private:
  Coordinates m_center;
  Coordinates m_lastPos;
  char m_nuke;
  int m_leftright;
};


int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input file");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Testing dynamic models.");
  P.registerArg(aStringCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);
  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);


  //m.SetPhysMode(0);

 
  string dna  = "GTTACCCCTAGTAGGTACTTAA CCTA";
  //string dna  = "A";
  string dna2 = "GTTGCCCCTAGCAGGTACCCAAACTTA";
  

  //dna = "TCAAAATATCACAATATCATTATTCTTCTCTAGGTGAGGCTCGTTTCCCTTGTT";
  dna = "TCAAAATATCACAATATCATTATTCTTCTCTAGGTGAGGCTCGTTTCCCTTGTTATTTACTAAAAAACCCAAAAACTTTCCTATGATGGTAAAGACATATTTAGAAGGATTAAGCTTCATTTGATGATGATCAAGCATATAAAAGGACTTCTTCTTGGTCTAGGAAAT";

//GTTGCTCAAAAGTCTTGTTTT--TTATTAACATGTCATTAACATATCCCTTCATAACTCCCAAGTTAATTCTTGAATATT--ATTTTTACCATCCACTGGTAAGTGGCCTCTACATTTTTAAGCTTGAAAGACATAGCTTTGTAACAAAATGTTCCCCCTTCAATTATGAAGCATAGGAATTTGGTGATAGCTAGAGTTGACATCCAAGAATCTAAGGTATTCAAAACTCGTGGTGAACTCAACCAATCAGTCAATCTTAAGGAGATGATGGTTTTTTTATGTGAAAACTTTGTTTAAATATGTGAAGTTCACACAC";

  dna2 = "TCAAAATATCGCAATATCATTATTCTTCTCTAGGTTAGGCTCGATTCCCTTGTTACTTACTAGAAAACCCAAAAACGTTCCTTATTTGATGGCAAAGACATATTTAAAAGGATTAAGTTTAATCTTATTATGGTTAAGTAGAGGAAAGGACTTCTTTTTTGTCTAGGAAAT";

//GTTGCTCCAAAGTCTTGCTTTCACTAGCATATTGTCATCAACATATCCCCTTATAACTCTCAAGTTGGTTCTTGAATATTTTTTTTTTACCATCCACTAGTAAGTGACCGTTGCATTTTTACGCCTAAAAAGCATAACTTTATAACAAAATGTTCCCACTTTAATTATGGAGCATGGTAATTTGATGATAGCTAGAGTTGGCATCCAAGAAGCTGAGGCATTCAAAACTCGCAGTGGAGTTAACCAATCAGTTAATCTTAGAGAGAGGATGGTTATCTTCTG-GGAAACTTTGTTTAAATCTGGGAAGTCCACAAAC";

  for (int i=0; i<dna.size(); i++) {
    AnimatedSceneNode anim;
    anim.SetCoordinates(StreamCoordinates(2400 + 50*i, 1500, 5300));
    anim.SetInvisible(Coordinates(1, 5, 5));
    //anim.SetRotImp(StreamCoordinates(0, 5000, 0));

    if (dna[i] == 'A')
      anim.SetTexture("data/Textures/a.png");
    if (dna[i] == 'C')
      anim.SetTexture("data/Textures/c.png");
    if (dna[i] == 'G')
      anim.SetTexture("data/Textures/g.png");
    if (dna[i] == 'T')
      anim.SetTexture("data/Textures/t.png");
    if (dna[i] == ' ')
      continue;

    anim.SetModel("data/Models/block4.ms3d");
    anim.SetDirection(Coordinates(0., 0., 1.));
    //anim.SetName("a");
    anim.SetScale(.5);
    anim.SetPhysMode(2);
    MyManipulator * p = new MyManipulator;
    
    p->SetNuke(dna[i]);
    eng.AddAnimatedModel(anim, p);
  }

  /*
  for (int i=0; i<dna2.size(); i++) {
    //SceneNode anim;
    AnimatedSceneNode anim;
    anim.SetCoordinates(StreamCoordinates(2400 + 50*i, 1600, 5300));
    anim.SetInvisible(Coordinates(1, 2, 1));
    //anim.SetRotImp(StreamCoordinates(0, 5000, 0));
    if (dna2[i] == 'A')
      anim.SetTexture("data/Textures/a.png");
    if (dna2[i] == 'C')
      anim.SetTexture("data/Textures/c.png");
    if (dna2[i] == 'G')
      anim.SetTexture("data/Textures/g.png");
    if (dna2[i] == 'T')
      anim.SetTexture("data/Textures/t.png");
    if (dna2[i] == ' ')
      continue;
    anim.SetModel("data/Models/block4.ms3d");
    anim.SetDirection(Coordinates(0., 0., 1.));
    //anim.SetName("a");
    anim.SetScale(.5);
    anim.SetPhysMode(2);
    MyManipulator * p = new MyManipulator;
    
    p->SetNuke(dna[i]);
    eng.AddAnimatedModel(anim, p);

   }*/
  eng.Run();


  return 0;
}
