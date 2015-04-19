#ifndef MATCHSOMS_H
#define MATCHSOMS_H


#include "physics/CrossCorr.h"
#include "core/SOM.h"


class MatchSOMs
{
public:
  MatchSOMs() {}

  double GetMatchScore(const SelfOrgFeatMap & a, 
		       const SelfOrgFeatMap & b);

private:
  CrossCorrelation m_xc;
};

template <class T>
class TMatchMemory
{
 public:
  TMatchMemory() {}

  double GetMatchScore(const T & a, 
		       const T & b) {
    const SelfOrgFeatMap & one = a.GetSOM();
    const SelfOrgFeatMap & two = b.GetSOM();
    return m_match.GetMatchScore(one, two);
  }

  // Returns the best score
  double GetMatchScore(svec<double> & scores,
		       const svec<T> & target, 
		       const svec<T> & query) {
    int i, j;
    int n = query.isize();
    scores.clear();
    scores.resize(target.isize(), 0.);

    double max = 0.;

    for (i=0; i<target.isize(); i++) {
      double sum = 0.;
      for (j=i; j<i+n; j++) {       
	//cout << j << " " << i << " "  << j % target.isize() << endl;
	sum += GetMatchScore(target[j % target.isize()], query[j-i]);
      }
      //cout << "..." << endl;
      sum += (double)n;
      scores[i] = sum;
      if (sum > max)
	max = sum;
    }
    cout << "Done here. " << endl;
    return max;
  }

 private:
  MatchSOMs m_match;
};

#endif //MATCHSOMS_H

