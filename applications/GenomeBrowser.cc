#define FORCE_DEBUG

#include <string>

#include "base/CommandLineParser.h"
#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include "applications/DNAVector.h"

class GenomeRingBuffer;

class NukeManipulator : public IManipulator
{
public:
  NukeManipulator(int pos, GenomeRingBuffer * p) {
    m_nuke = ' ';
    m_leftright = 0;
    m_pBuffer = p;
    SetPos(pos);
    m_scale = 20.;
    m_dist = 0.;
    m_pos = 0;
    m_chromoSize = -1.;
  }

  void SetChromosomeSize(double d) {
    m_chromoSize = d;
  }

  double GetCamDist() const {return m_dist;}

  virtual void SetPos(int i);
  int GetPos() const {return m_pos;}

  void SetScale(double s ) {m_scale = s;}

  void SetNuke(char c) {
    m_nuke = c;
  }

  virtual ~NukeManipulator() {}

  const Coordinates & GetPosition() {return m_center;}

  virtual void StartFeed(GamePhysObject & self) {}
  virtual void Feed(GamePhysObject & self, GamePhysObject & other) {}
  virtual void DoneFeed(GamePhysObject & self) {}

  virtual void CamPos(GamePhysObject & self, const Coordinates & c) {
    cout << "Call cam pos " << endl;
    PhysObject & p = self.GetPhysObject();
    Coordinates abspos = p.GetCenter().GetPosition();
    Coordinates scaled = m_center / m_scale;
    if (scaled != m_center) {
      p.GetCenterDirect().SetPosition(scaled);
      //cout << "POSITIONS" << endl;
      //abspos.Print();
      //scaled.Print();
    }

    Coordinates dir = c - p.GetCenter().GetPosition();
    m_dist = dir.Length();
    Coordinates dir2 = dir;
    dir2[1] = 0.;
    if (dir2[2] < 0.) {
      dir2[2] = -dir2[2];
      dir2[0] = -dir2[0];
    }
    p.SetDirection(dir2);
    //cout << "Set direction ";
    dir.Print();
    //cout << "My nuke " << m_nuke << endl;

    
    if ((m_nuke < 0 || m_nuke > 'Z') && !(m_nuke == '*' || -m_nuke == '*')) {
      char c = m_nuke;
      char tmp[256];
      if (c < 0) {
	sprintf(tmp, "data/Textures/aa_%c_i.png", -c);
      } else {
 	sprintf(tmp, "data/Textures/aa_%c.png", c);
      }
      cout << "Amino acid texture " << tmp << endl;
      // if (c == 'a')
      p.SetTexture(tmp);
    }
    if (m_nuke == 0)
      p.SetTexture("data/Textures/neutral.png");

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
	case '.':
	  p.SetTexture("data/Textures/neutral.png");
	  break;

	  //case 'a':
	  //p.SetTexture("data/Textures/aa_a.png");
	  //break;
	  //	case '<':
	  //p.SetTexture("data/Textures/backward.png");
	  //break;
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

	  /*
	case 'a':
	  p.SetTexture("data/Textures/aa_a.png");
	  break;
	case '>':
	  p.SetTexture("data/Textures/aa_A.png");
	  break;
	case '<':
	  p.SetTexture("data/Textures/backward.png");
	  break;
	  */
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
  const Coordinates & GetPosition() const {return m_center;}
protected:
  Coordinates m_center;
  Coordinates m_lastPos;
  char m_nuke;
  int m_leftright;
  int m_pos;
  GenomeRingBuffer * m_pBuffer;
  double m_scale;
  double m_dist;
  double m_chromoSize;
};


class AnnotManipulator :public NukeManipulator
{
public:
  AnnotManipulator(int pos, GenomeRingBuffer * p) : NukeManipulator(pos, p) {
    SetPos(pos);
  }
  virtual void SetPos(int i);
 
  
};



//===========================================================
class GenomeRingBuffer : public IGlobal
{
public:
  GenomeRingBuffer() {
    m_circular = true;
    m_index = 0;
    m_bufIndex = 0;
  }

  ~GenomeRingBuffer() {
    for (int i=0; i<m_buffer.isize(); i++) 
      delete m_buffer[i];
  }

  void SetCircular(bool b) {m_circular = b;}
  bool IsCircular() const {return m_circular;}

  NukeManipulator * New(int pos) {
    NukeManipulator * p = new NukeManipulator(pos, this);
    m_buffer.push_back(p);
    return p;
  }

  AnnotManipulator * NewAnnot(int pos) {
    AnnotManipulator * p = new AnnotManipulator(pos, this);
    m_buffer.push_back(p);
    return p;
  }

  const vecDNAVector & GetDNAVector() const {return m_dna;}
  vecDNAVector & DNAVector() {return m_dna;}
  
  char operator [] (int i) const {
    if (m_circular) {
      if (i < 0)
	i += m_dna[m_index].isize();
      if (i >= m_dna[m_index].isize())
	i -= m_dna[m_index].isize();
      return (m_dna[m_index])[i];
    } else {
      if (i < 0 || i >= m_dna[m_index].isize())
	return -1;
      return (m_dna[m_index])[i];
    } 
  }
  int Qual(int i) const {
    if (m_circular) {
      if (i < 0)
	i += m_dna[m_index].isize();
      if (i >= m_dna[m_index].isize())
	i -= m_dna[m_index].isize();
      return (m_dna[m_index]).Qual(i);
    } else {
      if (i < 0 || i >= m_dna[m_index].isize())
	return -1;
      return (m_dna[m_index]).Qual(i);
    } 
  }
  virtual void StartFrame(double deltatime) {

  }
  
  virtual void EndFrame(double deltatime) {
    double min = 99999999.;
    int index = -1;
    int i;
    for (i=0; i<m_buffer.isize(); i++) {
      if (m_buffer[i]->GetCamDist() < min) {
	index = i;
	min = m_buffer[i]->GetCamDist();
      }
    }

    
    cout << "Min index: " << index << endl;
    int half = m_buffer.isize()/2;
    if (index < half) {
      NukeManipulator * tmp = m_buffer[m_buffer.isize()-1];
      for (i=m_buffer.isize()-1; i>0; i--) 
	m_buffer[i] = m_buffer[i-1];
      m_buffer[0] = tmp;
      tmp->SetPos(m_buffer[1]->GetPos()-1);
    }
    if (index > half) {
      NukeManipulator * tmp = m_buffer[0];
      for (i=0; i<m_buffer.isize()-1; i++) 
	m_buffer[i] = m_buffer[i+1];
      m_buffer[m_buffer.isize()-1] = tmp;
      tmp->SetPos(m_buffer[m_buffer.isize()-2]->GetPos()+1);
    }
  }

private:
  int m_index;
  bool m_circular;
  vecDNAVector m_dna;
  svec<NukeManipulator*> m_buffer;
  int m_bufIndex;
};

//=====================================================
void NukeManipulator::SetPos(int i)
{
  m_leftright = 0;
  m_pos = i;
  m_nuke = (*m_pBuffer)[m_pos];

  m_center = Coordinates(2400 + 50*m_pos, 1500, 5300);
  /* if (m_pBuffer->IsCircular()) {
    double chro = m_chromoSize * 50.;
    double radius = chro / 2./3.14159;
    Coordinates center = Coordinates(2400-radius, 1500, 5300);
    double phi = (double)i/m_chromoSize*2.*3.1415;
    m_center = center;
    m_center[0] += radius*cos(phi);
    m_center[2] += radius*sin(phi);
    }*/
}

void AnnotManipulator::SetPos(int i) {
    
  NukeManipulator::SetPos(i);
  
  int annot = m_pBuffer->Qual(m_pos);
  
  char c = (char)annot;
  if (annot < 0)
    c = (char)-annot;
  c = (char)tolower(c);
  m_nuke = c;
  if (annot < 0)
    m_nuke = -m_nuke;

  m_center[0] += 40.;
  cout << "Setting " << annot << " -> " << m_nuke << endl;
  if (annot == 0) {
    m_center[1] -= 50.;
  }
  if (annot > 0) {
    m_center[2] -= 150.;
  }
  if (annot < 0) {
    m_center[2] += 150.;
  }  
  cout << "AnnotManipulator::SetPos nuke: " << m_nuke << endl;; 
}

void Annotate(vecDNAVector & dna, const string & fileName)
{
  FlatFileParser parser;
  
  parser.Open(fileName);

  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    DNAVector & d = dna(parser.AsString(0));
    const string & ori = parser.AsString(6);
    int dir = 1;
    if (ori == "-")
      dir = 2;
    int start = parser.AsInt(3)-1;
    int stop =  parser.AsInt(4)-1;

    DNAVector prot;
    prot.SetToSubOf(d, start, stop-start+1);
    if (dir == 2)
      prot.ReverseComplement();

    //prot.DoProteins(true);
    
    //=====================================================
    static AACodons trans;

    int n = prot.isize()/3;
    char * p = new char[n+1];
    int j;
    for (j=0; j<n; j++) {
      //cout << &prot[j*3] << "  ";
      if (j*3 + 3 >= prot.isize())
	break;

      p[j] = trans.GetCodon(&prot[j*3]);
      //cout << p[j] << endl;
    }
    p[j] = 0;
  
    //prot.SetFromProteins(p);
   
    //prot.SetFromBases(p);
 

    //=====================================================


    for (int x=0; x<n; x++)
      cout << p[x];
    cout << endl;

    int l = 0;
    if (dir == 2) {
      for (int i=stop; i>start; i-=3) {
	int c = p[l];
	l++;
	d.SetQual(i, -c);
      }
    } else {
      for (int i=start; i<stop; i+=3) {
	int c = p[l];
	l++;
	d.SetQual(i, c);
      }
    }
    delete [] p;
  }

}

//=====================================================
int main(int argc,char** argv)
{
  
  commandArg<string> aStringCmmd("-i","input config file");
  commandArg<string> fastaCmmd("-f","genome fasta file", "data/applications/E_coli_NC_000913.fasta");
  commandArg<string> annotCmmd("-a","annotation file", "data/applications/E_coli_NC_000913.gff");
  commandArg<double> sCmmd("-s","internal (physics) scale", 20.);

  commandLineParser P(argc,argv);
  P.SetDescription("Experimenting with genomic sequences.");
  P.registerArg(aStringCmmd);
  P.registerArg(fastaCmmd);
  P.registerArg(annotCmmd);
  P.registerArg(sCmmd);

  P.parse();

  string aString = P.GetStringValueFor(aStringCmmd);
  string genome = P.GetStringValueFor(fastaCmmd);
  string annot = P.GetStringValueFor(annotCmmd);
  double scale = P.GetDoubleValueFor(sCmmd);


  GameEngine eng;
  eng.ReadConfig(aString);
  eng.SetScale(scale);
  eng.SetupMap(0);
  eng.DoObjectCollision(false);

  //m.SetPhysMode(0);


  GenomeRingBuffer buffer;
  GenomeRingBuffer buffer_annot;
  eng.RegisterGlobal(&buffer);
  eng.RegisterGlobal(&buffer_annot);

  buffer.DNAVector().Read(genome);
  buffer_annot.DNAVector().Read(genome);

  Annotate(buffer_annot.DNAVector(), annot);

  int offset = 0;

  
  for (int i=0; i<100; i++) {
    AnimatedSceneNode anim;
    anim.SetInvisible(Coordinates(1, 5, 5));
    char dna = (buffer.GetDNAVector()[0])[i+offset];
    cout << "NUKE " << dna << endl;
    if (dna == 'A')
      anim.SetTexture("data/Textures/a.png");
    if (dna == 'C')
      anim.SetTexture("data/Textures/c.png");
    if (dna == 'G')
      anim.SetTexture("data/Textures/g.png");
    if (dna == 'T')
      anim.SetTexture("data/Textures/t.png");
    if (dna == ' ')
      continue;

    anim.SetModel("data/Models/block4.ms3d");
    anim.SetDirection(Coordinates(0., 0., 1.));
    //anim.SetName("a");
    anim.SetScale(.5);
    anim.SetPhysMode(2);
    NukeManipulator * p = buffer.New(i + offset);
    p->SetScale(scale);
    p->SetChromosomeSize(buffer.GetDNAVector()[0].isize());
    StreamCoordinates coords = p->GetPosition();
    anim.SetCoordinates(coords);
   
    //p->SetNuke(dna);
    cout << "### Add nuke ";
    p->GetPosition().Print();
    eng.AddAnimatedModel(anim, p);
  }
  

  // Add annotations
  for (int i=0; i<100; i++) {
    AnimatedSceneNode anim;
    anim.SetInvisible(Coordinates(1, 5, 5));
    int ann = (buffer_annot.GetDNAVector()[0]).Qual(i+offset);
    cout << "ANNOT " << ann << endl;
    if (ann == 0)
      anim.SetTexture("data/Textures/neutral.png");
    if (ann == 1)
      anim.SetTexture("data/Textures/aa_A.png");
    if (ann == 2)
      anim.SetTexture("data/Textures/backward.png");

    anim.SetModel("data/Models/block5.ms3d");
    anim.SetDirection(Coordinates(0., 0., 1.));
    //anim.SetName("a");
    anim.SetScale(.5);
    anim.SetPhysMode(2);
    AnnotManipulator * p = buffer_annot.NewAnnot(i + offset);
    p->SetScale(scale);
    p->SetChromosomeSize(buffer_annot.GetDNAVector()[0].isize());
    StreamCoordinates coords = p->GetPosition();
    anim.SetCoordinates(coords);
   
    //p->SetNuke(dna);
    cout << "### Add annot ";
    p->GetPosition().Print();
    eng.AddAnimatedModel(anim, p);
  }



  eng.Run();


  return 0;
}
