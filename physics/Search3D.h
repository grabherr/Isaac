#ifndef SEARCH3D_H
#define SEARCH3D_H

#include "base/SVector.h"
#include "physics/Coordinates.h"



class SearchStruct
{
public:
  SearchStruct() {
    val = 0;
    index = -1;
  }
  
  SearchStruct(double d) {
    index = -1;
    val = d;
  }

  int index;
  double val;
  bool operator < (const SearchStruct & s) const {
    return val < s.val;
  }

  /*
  SearchStruct operator + (double d) const {
    SearchStruct s = *this;
    s.val += d;
    return s;
  }

  SearchStruct operator - (double d) const {
    SearchStruct s = *this;
    s.val -= d;
    return s;
    }*/
};


template <class T>
class Search3D
{
public:
  void SetTo(const svec<T> & all) {

    int i;

    if (m_x.isize() != all.isize()) {
      m_x.resize(all.isize());
      m_y.resize(all.isize());
      m_z.resize(all.isize());
      m_array.resize(all.isize(), 0);
    }
    for (i=0; i<all.isize(); i++) {
      m_x[i].val = all[i].Pos()[0];
      m_y[i].val = all[i].Pos()[1];
      m_z[i].val = all[i].Pos()[2];
      m_x[i].index = i;
      m_y[i].index = i;
      m_z[i].index = i;
      m_array[i] = 0;
    }

    Sort(m_x);
    Sort(m_y);
    Sort(m_z);
  }

  void Search(svec<int> & result, const T & search, double dist) {

    result.clear();
    int i, j;

    for (i=0; i<m_array.isize(); i++) {
      m_array[i] = 0;
    }

 
    int lo = BinSearchFuzzy(m_x, SearchStruct(search.Pos()[0]-dist));
    int hi = BinSearchFuzzy(m_x, SearchStruct(search.Pos()[0]+dist));
    //cout << "lo=" << lo << " hi=" << hi << endl;
    for (i=lo; i<hi; i++) {
      m_array[m_x[i].index]++;
    } 

    lo = BinSearchFuzzy(m_y, SearchStruct(search.Pos()[1]-dist));
    hi = BinSearchFuzzy(m_y, SearchStruct(search.Pos()[1]+dist));
    //cout << "lo=" << lo << " hi=" << hi << endl;

    for (i=lo; i<hi; i++) {
      m_array[m_y[i].index]++;
    } 

    lo = BinSearchFuzzy(m_z, SearchStruct(search.Pos()[2]-dist));
    hi = BinSearchFuzzy(m_z, SearchStruct(search.Pos()[2]+dist));
    //cout << "lo=" << lo << " hi=" << hi << endl;

    for (i=lo; i<hi; i++) {
      if (m_array[m_z[i].index] == 2) {
	result.push_back(m_z[i].index);
      }
    } 

  }

  void Search(svec<int> & result, const svec<T> & all, const T & search, double dist) {
    svec<int> raw;
    Search(raw, search, dist);
   
    for (int i=0; i<raw.isize(); i++) {
      if ((all[raw[i]].Pos() - search.Pos()).Length() < dist)
	result.push_back(raw[i]);
    }
  }

  void Search(svec<T> & result, const svec<T> & all, const T & search, double dist) {
    svec<int> r;
    Search(r, all, search, dist);
    result.resize(r);
    for (int i=0; i<r.isize(); i++) {
      if ((all[r[i]].Pos() - search.Pos()).Length() < dist)
	result[i] = all[r[i]];
    }
  }

private:
  svec<SearchStruct> m_x;
  svec<SearchStruct> m_y;
  svec<SearchStruct> m_z;
  svec<int> m_array;
};

#endif 
