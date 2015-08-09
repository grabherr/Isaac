#include "engine/DynModels.h"

#include <math.h>

void MTriangle::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
  int count = m.VertexCount();
  Coordinates e = Coordinates(0, 0, 0);
  if (m_bInv) 
    e = Coordinates(1, 0, 1);
 
  m.AddVertex(e); 
  m.AddVertex(Coordinates(1,0,0)); 
  m.AddVertex(Coordinates(0,0,1)); 
  m.AddVertex(e); 
  m.AddVertex(Coordinates(1,0,0)); 
  m.AddVertex(Coordinates(0,0,1)); 
 
  for (int i=count; i<m.VertexCount(); i++) {
    StreamCoordinates & c = m.GetVertex(i);    
    c[0] *= size[0]; 
    c[1] *= size[1]; 
    c[2] *= size[2]; 
  }

  m.RecomputeNormals();
  //m.AddNormal(Coordinates(0,1,0));
  //m.AddNormal(Coordinates(0,1,0));
  //m.AddNormal(Coordinates(0,1,0));
  //m.AddNormal(Coordinates(0,-1,0));
  //m.AddNormal(Coordinates(0,-1,0));
  //m.AddNormal(Coordinates(0,-1,0));

  if (m_bInv) 
    m.AddTexCoord(Coordinates(1,1,0));
  else
    m.AddTexCoord(Coordinates(0,0,0));

  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(0,1,0));

  if (m_bInv) 
    m.AddTexCoord(Coordinates(0,0,0));
  else
    m.AddTexCoord(Coordinates(1,1,0));

  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(0,1,0));


  m.AddIndex(count+0,count+2,count+1);
  m.AddIndex(count+3,count+4,count+5);

}

void MRectangle::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
  MTriangle t;
  t.GetMesh(m, size);
  t.SetInv(true);
  t.GetMesh(m, size);
  
}
  
void MBlock::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
  StreamCoordinates r = size / 2.;

  m.AddVertex(Coordinates(0,0,0)); 
  m.AddVertex(Coordinates(1,0,0)); 
  m.AddVertex(Coordinates(1,1,0)); 
  m.AddVertex(Coordinates(0,1,0)); 
  m.AddVertex(Coordinates(1,0,1)); 
  m.AddVertex(Coordinates(1,1,1)); 
  m.AddVertex(Coordinates(0,1,1)); 
  m.AddVertex(Coordinates(0,0,1)); 
  m.AddVertex(Coordinates(0,1,1)); 
  m.AddVertex(Coordinates(0,1,0)); 
  m.AddVertex(Coordinates(1,0,1)); 
  m.AddVertex(Coordinates(1,0,0)); 

  int i;
  StreamCoordinates minus(0.5, 0.5, 0.5);

  for (i=0; i<m.VertexCount(); i++) {
    StreamCoordinates & c = m.GetVertex(i);
    c -= minus;
    c[0] *= r[0]; 
    c[1] *= r[1]; 
    c[2] *= r[2]; 
  }


  m.AddNormal(Coordinates(-1,-1,-1));
  m.AddNormal(Coordinates( 1,-1,-1));
  m.AddNormal(Coordinates( 1, 1,-1));
  m.AddNormal(Coordinates(-1, 1,-1));
  m.AddNormal(Coordinates( 1,-1, 1));
  m.AddNormal(Coordinates( 1, 1, 1));
  m.AddNormal(Coordinates(-1, 1, 1));
  m.AddNormal(Coordinates(-1,-1, 1));
  m.AddNormal(Coordinates(-1, 1, 1));
  m.AddNormal(Coordinates(-1, 1,-1));
  m.AddNormal(Coordinates( 1,-1, 1));
  m.AddNormal(Coordinates( 1,-1,-1));
 
  m.AddTexCoord(Coordinates(0,1,0));
  m.AddTexCoord(Coordinates(1,1,0));
  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(0,0,0));
  m.AddTexCoord(Coordinates(0,1,0));
  m.AddTexCoord(Coordinates(0,0,0));
  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(1,1,0));
  m.AddTexCoord(Coordinates(0,1,0));
  m.AddTexCoord(Coordinates(1,1,0));
  m.AddTexCoord(Coordinates(1,0,0));
  m.AddTexCoord(Coordinates(0,0,0));

  m.AddIndex(0,2,1);
  m.AddIndex(0,3,2);
  m.AddIndex(1,5,4);
  m.AddIndex(1,2,5);
  m.AddIndex(4,6,7);
  m.AddIndex(4,5,6);
  m.AddIndex(7,3,0);
  m.AddIndex(7,6,3);
  m.AddIndex(9,5,2);
  m.AddIndex(9,8,5);
  m.AddIndex(0,11,10);
  m.AddIndex(0,10,7);

  m.SetPhysMode(0);
}


void MSphere::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
}
  
void MCylinder::GetMesh(MeshModel & m, const StreamCoordinates & size)
{

}
  
void MCone::GetMesh(MeshModel & m, const StreamCoordinates & size)
{

}

void MLeaf::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
  StreamCoordinates r = size / 2.;

  // Center
  m.AddVertex(Coordinates(0.5,0,0)); 

  m.AddVertex(Coordinates(0,0,0)); 
  m.AddVertex(Coordinates(0.2,0,0.5)); 
  m.AddVertex(Coordinates(0.8,0,0.5)); 
  m.AddVertex(Coordinates(1,0,0)); 
  m.AddVertex(Coordinates(0.8,0,-0.5)); 
  m.AddVertex(Coordinates(0.2,0,-0.5)); 

  
  m.AddIndex(0,1,2);
  m.AddIndex(0,2,3);
  m.AddIndex(0,3,4);
  m.AddIndex(0,4,5);
  m.AddIndex(0,5,6);
  m.AddIndex(0,6,1);
 



  int i;
  StreamCoordinates minus(0.5, 0, 0);
  StreamCoordinates plus(0, 0, 0.5);

  for (i=0; i<m.VertexCount(); i++) {
    StreamCoordinates & c = m.GetVertex(i);
    StreamCoordinates tex = c + plus;
    tex[1] = tex[2];
    m.AddTexCoord(tex);
    cout << "Tex coords: ";
    tex.Print();
    c -= minus;
    c[0] *= r[0]; 
    c[1] *= r[1]; 
    c[2] *= r[2]; 
  }
 
  m.RecomputeNormals();
}
