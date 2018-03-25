#define FORCE_DEBUG

#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "util/mutil.h"
#include "base/RandomStuff.h"

int K=3;

class Kmer
{
public:
  Kmer() {
  }

  bool Add(const char z) {
    kmer += z;
    //cout << kmer << endl;
    if (kmer.size() >= K)
      return true;
    return false;
  }
  
  void clear() {
    string s;
    for (int i=1; i<kmer.size(); i++)
      s += kmer[i];
    kmer = s;
  }
  
  string kmer;
  bool operator < (const Kmer & k) const {
    return kmer < k.kmer;
  }
};

bool Comp(const string & a, const string & b)
{
  int n = a.size();
  if (b.size() < n)
    n = b.size();

  int i;
  for (i=0; i<n; i++) {
    if (a[i] != b[i])
      return false;
  }
  return true;
}

class Model
{
public:
  Model() {
  }

  void Add(const Kmer & k) {
    m_kmers.push_back(k);
    //cout << "Add " << k.kmer << endl;
  }

  string Get(const string & s) {
    int i;
    Kmer k;
    
    for (i=s.size()-K+1; i<s.size(); i++) {
      if (i >= 0)
	k.kmer += s[i];
    }
    string r;
    //cout << "Search for " << k.kmer << endl;
    int index = BinSearchFuzzy(m_kmers, k);
    //cout << "Index " << index << " " << m_kmers.isize() <<endl;
    if (index < 0 || index >= m_kmers.isize()) {
      r = " ";
      return r;
    }
    //cout << "Search for: '" << k.kmer << "' index " << index << endl;
    for (i=index; i<m_kmers.isize(); i++) {
      if (!Comp(m_kmers[i].kmer, k.kmer))
	break;
      //cout << k.kmer << " '" << m_kmers[i].kmer << "' " << endl;
    }
    if (i == index) {
      r = " ";
      return r;
    }

    //cout << i << " " << index << endl;
    int ix = index + RandomInt(i-index);
    //cout << index << " - " << i << " rand " << ix << endl; 
    r += m_kmers[ix].kmer[k.kmer.size()];
    //cout << "Return '" << r << "' " << endl;
    return r;
  }
  
  void Sort() {
    //for (int i=0; i<m_kmers.isize(); i++)
    //cout << m_kmers[i].kmer << endl;
    ::Sort(m_kmers);
    //cout << "-----------------" << endl;
    //for (int i=0; i<m_kmers.isize(); i++)
    //cout << m_kmers[i].kmer << endl;
 }
private:
  svec<Kmer> m_kmers;
  
};


int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-i","input file");
  commandArg<string> fCmmd("-f","filter (case sensitive)", "");
  commandArg<int> kCmmd("-k","k-mer size", 3);
  commandArg<bool> cCmmd("-c","case insensitive", false);
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(fCmmd);
  P.registerArg(kCmmd);
  P.registerArg(cCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string filter = P.GetStringValueFor(fCmmd);
  K = P.GetIntValueFor(kCmmd);
  bool bC = P.GetBoolValueFor(cCmmd);
 
  int i, j;
  FlatFileParser parser;
  
  parser.Open(fileName);

  Model m;

  // RandomFloat(1);

  string word;
  
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    string l;
    //l += parser.AsString(i);
    //l += " ";
    //CMString s = l.c_str();
    //s.toLower();

    
   
    for (i=0; i<parser.GetItemCount(); i++) {
      l += " " + parser.AsString(i);
    }
    l += " ";

    if (bC) {
      CMString ll = l.c_str();
      ll.toLower();
      l = (const char*)ll;
    }
    
    //cout << l << endl;
    //if (l.size() <= 4)
    //continue;
    //cout << "Process " << l << endl;
    Kmer k;
    for (j=0; j<(int)l.size(); j++) {
      if (k.Add(l[j])) {
	m.Add(k);
	k.clear();
      }
    }
  }
  
  m.Sort();
  
  string c = " ";
  string last;
  int k = 0;
  int n = 10000;
  if (filter != "")
    n = 10000000;
  for (i=0; i<n; i++) {
    string name;
    //for (j=0; j<30; j++) {
      //cout << "Get " << c << endl;
    string a = m.Get(c);

    if (a == " ") {
      if (filter != "") {
	if (strstr(word.c_str(), filter.c_str()) != NULL)
	  cout << word << endl;
      }
      word = "";
    }
    
    if (a == " " && last == " ") {
      c = " ";
      if (filter == "")
	cout << endl;
      k = 0;
      //cout << " ";
    } else {
      c += a;
      word += a;
      if (filter == "") {
	cout << a;
	k++;
	if (a == " " && k > 500) {
	  k = 0;
	  cout << endl;
	}
      }
    }
    last = a;
    //if (a == " ")
    //cout << endl;
    
    if (a == " ") {
      //c = " ";
      //break;
    }
      //name += c;
      //}
    //if (name.length() > 0)
    // cout << name << " ";


  }
  cout << endl;

  return 0;
}
