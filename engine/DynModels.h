#ifndef DYNMODELS_H
#define DYNMODELS_H

#include "graphics/Messages.h"

class MeshModel;

class IDynamicModel
{
 public:
  virtual ~IDynamicModel() {}

  
  // Returns the mesh, i.e. vertices, indices, and texture coordinates. 
  // Note that the texture and position have to be set manually in 
  // the MeshModel object.
  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.)) = 0;

 private:
 
};

// ======================================================
// Basic models & shapes

class MTriangle
{
 public:
  MTriangle() {
    m_bInv = false;
  }

  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
  void SetInv(bool b) {
    m_bInv = b;
  }
 private:
  bool m_bInv;
};

class MRectangle
{
 public:

  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
  
};

class MBlock
{
 public:

  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
  
};


class MSphere
{
 public:
  MSphere() {
    polyCountX = 20;
    polyCountY = 20;    
  }

  void SetPolyCount(int x, int y) {
    polyCountX = x;
    polyCountY = y;
  }
  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
 private:
  int polyCountX;
  int polyCountY;
  
};


class MCylinder
{
 public:

  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
  
};

class MCone
{
 public:

  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
  
};

class MLeaf
{
 public:

  virtual void GetMesh(MeshModel & m, const StreamCoordinates & size = 
		       StreamCoordinates(1., 1., 1.));
  
};

#endif //DYNMODELS_H

