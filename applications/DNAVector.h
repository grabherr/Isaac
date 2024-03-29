#ifndef _DNAVECTOR_H_
#define _DNAVECTOR_H_



#include "base/SVector.h"
#include <iostream>
#include <map>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <sstream>

typedef svec<char> NumVector;
typedef svec<NumVector> vecNumVector;


double DNA_A(char l);
double DNA_C(char l);
double DNA_G(char l);
double DNA_T(char l);

inline double DNA_N(char l, char nuke) {
  switch(nuke) {
  case 'A':
    return DNA_A(l);
  case 'C':
    return DNA_C(l);
  case 'G':
    return DNA_G(l);
  case 'T':
    return DNA_T(l);

  default:
    cout << "ERROR";
    return 0.;
  }

}

// Returns the ambiguous code given all these bases
char GetAmbiguous(const string & bases);


//RC's a base
char GetRC(const char c);

char ResolveAmbiguous(const string & bases);



// Returns 3 ambiguos bases given the amino acid.
// Note: the character buffer needs to be at least 4 bytes long!
void AminoAcidToBases(char * out, char aa);
char BasesToAminoAcid(char * b);

double DNA_Equal(char letter1, char letter2);
double DNA_EqualAmb(char letter1, char letter2);

double DNA_EqualEmph(char letter1, char letter2);

double DNA_Diff(char letter1, char letter2);
double DNA_DiffAmb(char letter1, char letter2);



const char plain_table[] = 
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0-9
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //10-19
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //20-29
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //30-39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //40-49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //50-59
  0, 0, 0, 0, 0, 1, 0, 2, 0, 0, //60-69
  0, 3, 0, 0, 0, 0, 0, 0, 5, 0, //70-79
  0, 0, 0, 0, 4, 0, 0, 0, 0, 0, //80-89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //90-99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //100-119
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //110-129
  
  

};

const char plain_table_aa[] = 
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0-9
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //10-19
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //20-29
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //30-39
  0, 0, 21, 0, 0, 0, 0, 0, 0, 0, //40-49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //50-59
  0, 0, 0, 0, 0, 1, 0, 2, 3, 4, //60-69
  5, 6, 7, 8, 0, 9, 10, 11, 12, 0, //70-79
  13, 14, 15, 16, 17, 0, 18, 19, 0, 20, //80-89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //90-99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //100-119
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //110-129
    

};

const char plain_table_aa_inverse[] =
{
  'A',  'C',  'D',  'E',
  'F',  'G',  'H',  'I',
  'K',  'L',  'M',  'N',
  'P',  'Q',  'R',  'S',
  'T',  'V',  'W',  'Y',
  '*',  'B',  'Z',  'J', 
  'X'
};


inline int NucIndex(char letter) 
{
  return (plain_table[(int)letter] - 1);
}

inline int AminoAcidIndex(char letter) 
{
  return (plain_table_aa[(int)letter] - 1);
}

inline int AminoAcidLetter(char aa) 
{
  return (plain_table_aa_inverse[(int)aa]);
}



inline char NucLetter(int index) 
{
  switch(index) {
  case 0:
    return 'A';
  case 1:
    return 'C';
  case 2:
    return 'G';
  case 3:
    return 'T';
  default:
    return 'N';
  }
}

inline double DNA_EqualFast(char letter1, char letter2)
{
  if (plain_table[(int)letter1] > 0 && plain_table[(int)letter2] > 0) {
    //cout << letter1 << letter2 << endl;
    if (letter1 != 'N' && letter2 != 'N') {
      if (letter1 == letter2) {
	return 1.;
      } else {
	return 0.;
      }
    } else {
      return 0.;
    }
  }
  return DNA_Equal(letter1, letter2);
}

class DNACodec
{
 public:
  DNACodec();
  double A_Letter(int l) const {
    return m_A[l];
  }
  double C_Letter(int l) const {
    return m_C[l];
  }
  double G_Letter(int l) const {
    return m_G[l];
  }
  double T_Letter(int l) const {
    return m_T[l];
  }

  char GetRC(int l) const {
    return m_rc[l];
  }

 private:

  void Set(int letter, double a, double c, double g, double t, int rc);

  double m_A[256];
  double m_C[256];
  double m_G[256];
  double m_T[256];

  char m_rc[256];

};

//=================================================
class AAAmb
{
public:
  AAAmb() {}


  const string & Base(int i) const {return m_bases[i];}

  void Add(const string & b) {
    const char * p = b.c_str();
    char tmp[16];
    tmp[1] = 0;
    int i;
    for (i=0; i<3; i++) {
      tmp[0] = p[i];
      m_bases[i] += tmp;
    }
  }

private:
  string m_bases[3];
};


class DNAVector;


class AACodons
{
public:
  AACodons();

  const string & GetBases(char aa, int i) {
    int index = 0;
    //cout << "Checking AA: " << aa << endl;
    if (aa < 0) {
      cout << "ERROR(1): Illegal amino acid: " << aa << endl;
    } else {
      index = m_table[aa];
    }
    if (index >= m_bases.isize() || index < 0) {
      cout << "ERROR(2): Illegal amino acid: " << aa << endl;
      index = 0;
    }
    const AAAmb & a = m_bases[index];
    return a.Base(i);
  }

  char GetCodon(const DNAVector & d, int pos);

  char GetCodon(const char * p) {
    char tmp[64];
    tmp[0] = p[0];
    tmp[1] = p[1];
    tmp[2] = p[2];
    tmp[3] = 0;
    int i;

    //cout << "Asking for " << tmp << endl;

    for (i=0; i<m_codonBases.isize(); i++) {
      if (m_codonBases[i] == tmp)
	return m_aminoAcids[i];
    }
    cout << "WEIRD ERROR!" << endl;
    return (char)0xFF;
  }
  
  int GetCodonCount() const {return m_codonBases.isize();}
  const string & GetCodon(int i) const {return m_codonBases[i];}

private:
  svec<int> m_table;
  
  svec<AAAmb> m_bases;

  svec<char> m_aminoAcids;
  svec<string> m_codonBases;

  void Set(const string & aa, const string & codon, int index) {
    char i = (aa.c_str())[0];
    m_table[i] = index;
    m_bases[index].Add(codon);
    
    m_aminoAcids.push_back(i);
    m_codonBases.push_back(codon);

  }
};


//==========================================================

class DNAVector
{
  

 public:
  DNAVector() {}
  ~DNAVector() {}

  void resize(int n) {
    m_data.resize(n, 0);
    if (m_qual.isize() > 0)
      m_qual.resize(n, 0);
  }

  void resize(int n, char c) {
    m_data.resize(n, c);
    if (m_qual.isize() > 0)
      m_qual.resize(n, 0);
  }

  //void Compact();

  void SetToSubOf(const DNAVector & v, int start, int len);

  void ReverseComplement();

  const char & operator [] (int i) const {
    return m_data[i];
  }
  char & operator [] (int i) {
    return m_data[i];
  }

  int Qual(int i) const {
    if (m_qual.isize() == 0)
      return 0;
    return m_qual[i];
  }

  int QualSize() const {return m_qual.isize();}

  void Set(int i, char c) {
    m_data[i] = c;
  }

  void SetFromBases(const string & s);

  void SetFromProteins(const string & s);

  void Proteinize();

  void RemoveGaps(); // Removes the character '-' from the sequences.

  int size() const {return m_data.isize();}
  int isize() const {return m_data.isize();}
  long long lsize() const {return m_data.lsize();}
  
  const string &getName() const;
  /* setName
   * ---------------------
   * Sets the DNAVector's name to the specified name. Note that if
   * this DNAVector is contained within a vecDNAVector, you should
   * instead use vecDNAVector's setName function. Otherwise, the
   * vecDNAVector's name lookup table will become out of date.
   */
  void setName(const string &newName);

  void SetQual(int i, int score) {
    if (m_qual.isize() == 0)
      m_qual.resize(m_data.isize(), 0);
    m_qual[i] = score;
  }


  void Write(FILE * p) const;
  void Write(ostream &s) const;
  void WriteOneLine(ostream &s) const;
  void WriteQual(FILE * p) const;
  void WriteQual(ostream &s) const;

  void ToUpper();

  void operator += (const DNAVector & d) {
    int n = isize();
    resize(n+d.isize());
    for (int i=0; i<d.isize(); i++)
      (*this)[i+n] = d[i];
  }


  bool Append(const DNAVector & d, int min, int max, double ident);

  void clear() {
    m_data.clear();
    m_qual.clear();
  }

/* operator ==
 * ----------------------
 * Note that this comparison starts by comparing the DNAVector's names. Therefore, if
 * you have two DNAVectors with the same sequence but different names, they will not
 * be equal.
 */
  bool operator == (const DNAVector & d) const {
	  if(name != d.getName())
		  return false;
	  int i;
	  if (isize() != d.isize())
		  return false;
	  for (i=0; i<isize(); i++) {
		  if (m_data[i] != d[i])
			  return false;
	  }
	  return true;
  }

  bool operator != (const DNAVector & d) const {
    return !(*this == d);
  }

  bool operator < (const DNAVector & d) const {
	  int i;
	  int n = m_data.isize();
	  if (d.isize() < n)
		  n = d.isize();

	  for (i=0; i<n; i++) {
		  if (m_data[i] == d[i])
			  continue;
		  if (m_data[i] < d[i]) {
			  //cout << "1" << endl;
			  return true;
		  }
		  if (m_data[i] > d[i]) {
			  //cout << "2" << endl;
			  return false;
		  }
	  }

	  if (m_data.isize() < d.isize()) {
		  //cout << "3" << endl;
		  return true;
	  }
	  //cout << "4" << endl;
	  return false;
  }

  void Print(ostream &o) const {
    for(int i=0; i<m_data.isize(); i++) o << m_data[i];
  }

  void Print() const {
    for(int i=0; i<m_data.isize(); i++) cout << m_data[i];
  }


  string AsString() const {
    string s;
    for(unsigned int i=0; i<m_data.size(); i++) s.push_back(m_data[i]);
    return s;
  }
  
  void Substring(string & s, int start, int length) const {
    s.resize(length);
    for(int i=0; i<length; i++) 
      s[i] = m_data[start+i];
  }


  string Substring(int start, int length) const {
    string s;    
    for(int i=0; i<length; i++) s.push_back(m_data[start+i]);
    return s;
  }


  //char * Data() {return &m_data[0]};
  //unsigned char * DataQual() {return &m_qual[0]};

  svec<char> & Data() {return m_data;}
 


 private:
  svec<char> m_data;
  svec<int> m_qual;
  string name;
};




class vecDNAVector
{
 public:
  vecDNAVector();
  vecDNAVector(const vecDNAVector &other);
  ~vecDNAVector();

  vecDNAVector &operator = (const vecDNAVector &other);

  const DNAVector & operator [] (int i) const;
  DNAVector & operator [] (int i);
  const DNAVector &operator () (const string &name) const;
  DNAVector &operator () (const string &name);

  class DNAVectorRef;
  class const_DNAVectorRef;

  /* getDNAVectorRef
   * -------------------
   * Returns a DNAVectorRef object pointing to the DNAVector at index 'i'
   * / with the name 'name'. If there is no such DNAVector, returns an
   * invalid reference.
   */
  DNAVectorRef getDNAVectorRef(int i);
  DNAVectorRef getDNAVectorRef(const string &name);
  const_DNAVectorRef getDNAVectorRef(int i) const;
  const_DNAVectorRef getDNAVectorRef(const string &name) const;

  void RemoveGaps() {
    for (int i=0; i<isize(); i++) {
      m_data[i].RemoveGaps();
    }
  }

  void resize(int n);
  /* reserve
   * -------------------
   * Ensures that the vecDNAVector has at least enough memory allocated to
   * hold newSize elements.
   */
  void reserve(unsigned int newSize);
  void clear();

  int NameIndex(const string & name) const;
  const string & Name(int i) const;
  const char * NameClean(int i) const;

  /* SetName
   * ---------------------
   * Sets the name of theDNAVector at index i to the specified name.
   * Note that you should use this method rather than the DNAVector's
   * own setName method. Otherwise, the vecDNAVector's name lookup table
   * will become out of date.
   */
  void SetName(int i, const string & s);

  void push_back(const DNAVector & v);
  void push_back(const DNAVector & v, const string & name);

  /* erase
   * -------------
   * Erases the DNAVector at the index 'index'.
   *
   * Note that this involves an ~O(n) operation internally.
   * If you do not care about the element order and n is large,
   * fast_erase would be a better choice.
   */
  void erase(int index);

  /* erase
   * -------------
   * Erases the DNAVector with the name 'name', if it exists, and returns true.
   * If there is no DNAVector with that name, returns false.
   *
   * Note that this involves an ~O(n) operation internally.
   * If you do not care about the element order and n is large,
   * fast_erase would be a better choice.
   */
  bool erase(const string &name);

  /* fast_erase
   * -------------
   * Erases the DNAVector at the index 'index'; as a side effect
   * the last element in the vecDNAVector is moved to the vacated index.
   */
  void fast_erase(int index);

  /* fast_erase
   * -------------
   * Erases the DNAVector with the name 'name'; as a side effect
   * the last element in the vecDNAVector is moved to the vacated index.
   */
  bool fast_erase(const string &name);


  int size() const;
  int isize() const;
  long long lsize() const;
  
  long long totalBases() const;

  void ReadV(const string & file);
  void WriteV(const string & file) const;


  void Write(const string & fileName, bool bSkipEmpty = false) const;
  void WriteQuals(const string & fileName) const;
  void Read(const string & fileName, bool bProteins = false, bool shortName = false, bool allUpper = true);
  void Read(const string & fileName, svec<string> & names);
  /* ReadPartial
   * -----------------
   * Reads a section of a fasta file into the vecDNAVector, starting at the sequence at firstToRead and reading
   * numToRead consecutive sequences.
   */
  void ReadPartial(const string & fileName, unsigned int firstToRead, unsigned int numToRead, bool bProteins = false, bool shortName = false, bool allUpper = true);
  /* ReadMultiple
   * --------------
   * Reads multiple files into this vecDNAVector
   */
  void ReadMultiple(const vector<string> &fileNames, bool bProteins = false, bool shortName = false, bool allUpper = true);

  void ReadQuals(const string & fileName);

  void ReverseComplement();

  // The sequence is actually proteins even though given in nucleotide space,
  // so make it ambiguous.
  void DoProteins(bool bKeepProt = false);

  void UniqueSort();

  friend class DNAVectorRef;
  friend class const_DNAVectorRef;

  /* ReferenceListener
   * --------------------
   * Abstract class representing a reference to an internal
   * DNAVector object
   */
  class ReferenceListener {
  public:
	  virtual ~ReferenceListener() = 0;

	  virtual string name() const = 0;

	  friend class vecDNAVector;
  private:
	  virtual void invalidate() = 0;
  };

  /* DNAVectorRef
   * -----------------
   * A reference to a DNAVector object inside a vecDNAVector.
   * It can be dereferenced just like a normal pointer. It will become invalid
   * if the DNAVector object is removed from the vecDNAVector.
   *
   * (Note that if you simply use a normal pointer, if the vecDNAVector
   * changes internally, for example because a new DNAVector is added,
   * your pointer may become invalid)
   */
  class DNAVectorRef : ReferenceListener {
  public:
	  DNAVectorRef(const DNAVectorRef &toCopy);
	  /* Default Constructor
	   * ------------------------
	   * Generates an invalid reference, pointing nowhere.
	   */
	  DNAVectorRef();
	  ~DNAVectorRef();

	  /* operator ==
	   * -------------------
	   * Returns true if the two references point to the same DNAVector, and false otherwise.
	   */
	  bool operator == (const DNAVectorRef & other) const;
	  bool operator == (const const_DNAVectorRef & other) const;
	  bool operator != (const DNAVectorRef & other) const;
	  bool operator != (const const_DNAVectorRef & other) const;
	  DNAVectorRef &operator = (const DNAVectorRef &other);
	  DNAVector &operator *();
	  DNAVector *operator ->();
	  string name() const;

	  /* isInvalid
	   * ------------
	   * Returns true if the reference is no longer valid, i.e. if
	   * it has been removed from the vecDNAVector.
	   */
	  bool isInvalid() const;

	  friend class const_DNAVectorRef;
	  friend class vecDNAVector;

  private:
	  DNAVectorRef(vecDNAVector * owner, string DNAName);
	  /* invalidate
	   * --------------
	   * Causes the DNAVectorRef to become invalid.
	   */
	  void invalidate();

	  vecDNAVector *owner;
	  string DNAName;
  };

  /* const_DNAVectorRef
   * --------------------
   * A const version of DNAVectorRef
   */
  class const_DNAVectorRef : ReferenceListener {
  public:
	  const_DNAVectorRef(const const_DNAVectorRef &toCopy);
	  const_DNAVectorRef(const DNAVectorRef &toCopy);
	  const_DNAVectorRef();
	  ~const_DNAVectorRef();

	  bool operator == (const DNAVectorRef & other) const;
	  bool operator == (const const_DNAVectorRef & other) const;
	  bool operator != (const DNAVectorRef & other) const;
	  bool operator != (const const_DNAVectorRef & other) const;
	  const_DNAVectorRef &operator = (const const_DNAVectorRef &other);
	  const_DNAVectorRef &operator = (const DNAVectorRef &other);
	  const DNAVector &operator *() const;
	  const DNAVector *operator ->() const;
	  string name() const;

	  bool isInvalid() const;

	  friend class DNAVectorRef;
	  friend class vecDNAVector;

  private:
	  const_DNAVectorRef(const vecDNAVector * owner, string DNAName);

	  void invalidate();

	  const vecDNAVector *owner;
	  string DNAName;
  };

 private:
  /* addReference / removeReference
   * --------------------------------
   * Adds / removes the specified ReferenceListener from the list of
   * active references.
   */
  void addReference(ReferenceListener *newReference) const;
  void removeReference(ReferenceListener *toRemove) const;
  /* invalidateReferences
   * ----------------------
   * Invalidates all references to the given DNAVector, if any.
   */
  void invalidateReferences(string toInvalidate);

  void setupMap();

  svec<DNAVector> m_data;

  int default_name_index;
  map<string,int> m_name2index;

  /* references
   * --------------
   * A list of the currently active references to internal DNAVectors.
   */
  mutable map<string, vector<ReferenceListener *> > references;
};


class FlatFileParser;

//================================================
// Streaming version of the vecDNAVector...
class vecDNAVectorStream
{
 public:
  vecDNAVectorStream() {
    m_pParser = NULL;
  }
  ~vecDNAVectorStream();

  void ReadStream(const string & fileName);

  // Returns an empty vector at the end
  const DNAVector & Next();

 private:
  FlatFileParser * m_pParser;
  DNAVector m_seq;
};



int CountNs(const DNAVector & d);

// a single base constitutes a >= 'threshold' fraction of the sequence 
bool IsHomopolymer(const DNAVector &d, double threshold=1);









#endif //_DNAVECTOR_H_


