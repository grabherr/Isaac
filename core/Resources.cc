#include "core/Resources.h"

ResourcePool::~ResourcePool() 
{
  for (int i=0; i<m_pool.isize(); i++)
    delete m_pool[i];
  m_pool.clear();
}

int ResourcePool::GetIndex(const string & name) const
{
  int i;
  for (i=0; i<m_pool.isize(); i++) {
    if (m_pool[i]->Name() == name)
      return i;
  }
  return -1;
}

IResource * ResourcePool::GetRes(const string & name) const
{
  int i = GetIndex(name);
  if (i < 0)
    return NULL;
  return m_pool[i];      
}

void ResourcePool::GetAll(svec<double> & all) const
{
  all.resize(m_pool.isize());
  int i;
  for (i=0; i<m_pool.isize(); i++) {
    all[i] = m_pool[i]->GetRel();
  }
}

double ResourcePool::Get(const string & name) const
{
  int index = GetIndex(name);
  if (index < 0)
    return 0;
  return m_pool[index]->Get();
}

double ResourcePool::GetRel(const string & name) const
{
  int index = GetIndex(name);
  if (index < 0)
    return 0;
  return m_pool[index]->GetRel();
}
  
double ResourcePool::Add(const string & name, double d)
{
  int index = GetIndex(name);
  if (index < 0)
    return 0;
  return m_pool[index]->Add(d);
}

double ResourcePool::Remove(const string & name, double d)
{
  int index = GetIndex(name);
  if (index < 0)
    return 0;
  return m_pool[index]->Remove(d);
}
