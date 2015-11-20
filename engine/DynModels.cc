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

  m.AddVertex(Coordinates(0,0,0)); //0
  m.AddVertex(Coordinates(1,0,0)); //1
  m.AddVertex(Coordinates(1,1,0)); //2
  m.AddVertex(Coordinates(0,1,0)); //3
  m.AddVertex(Coordinates(1,0,1)); //4
  m.AddVertex(Coordinates(1,1,1)); //5
  m.AddVertex(Coordinates(0,1,1)); //6
  m.AddVertex(Coordinates(0,0,1)); //7
  m.AddVertex(Coordinates(0,1,1)); //6
  m.AddVertex(Coordinates(0,1,0)); //3
  m.AddVertex(Coordinates(1,0,1)); //4
  m.AddVertex(Coordinates(1,0,0)); //1

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

void MLine::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
  int i;
  MBlock block;
  block.GetMesh(m, StreamCoordinates(m_width, m_width, m_width));
  
  StreamCoordinates dir = (m_to - m_from)/2.;
  StreamCoordinates mid = (m_to + m_from)/2.;
  double len = dir.Length();
  m.AbsCoords() = mid;

  for (i=0; i<m.VertexCount(); i++)
    m.GetVertex(i).Rotate(dir);

  cout << "MLine " << len << " -> ";
  dir.Print();
  mid.Print();
  
  cout << "Curr " << m.GetVertex(0)[2] << " sub " << len << endl;

  m.GetVertex(0) -= dir;
  m.GetVertex(1) -= dir;
  m.GetVertex(2) -= dir;
  m.GetVertex(3) -= dir;
  m.GetVertex(4) += dir;
  m.GetVertex(5) += dir;
  m.GetVertex(6) += dir;
  m.GetVertex(7) += dir;

  m.GetVertex(8) += dir;
  m.GetVertex(9) -= dir;
  m.GetVertex(10) += dir;
  m.GetVertex(11) -= dir;

  m.SetPhysMode(2);
  //m.AbsCoords()[2] = 1500;
  
  m.SetScale(1);
  StreamCoordinates rot;
  
  /*
  double phi = 0.;

  if (dir[2] != 0.) {
    phi = atan(dir[0]/dir[2]);
    if (dir[2] < 0.)
      phi += 3.1415;
    if (phi < 0.)
      phi += 2*3.1415;
  }
  cout << "phi= " << phi << endl;
  rot[1] = phi;      
  phi = 0.;

  if (dir[1] != 0.) {
    phi = atan(dir[2]/dir[1]);
    if (dir[1] < 0.)
      phi += 3.1415;
    if (phi < 0.)
      phi += 2*3.1415;
  }

  rot[0] = -phi;      
  phi = 0.;

  if (dir[0] != 0.) {
    phi = atan(dir[1]/dir[0]);
    if (dir[1] < 0.)
      phi += 3.1415;
    if (phi < 0.)
      phi += 2*3.1415;
  }
  rot[2] = phi;      

  cout << "phi= " << phi << endl;
  cout << "Rot ";
  rot.Print();
  m.SetRotation(rot);*/

}

void MLine::SetCoords(const StreamCoordinates & from, const StreamCoordinates & to, double width)
{
  m_from = from;
  m_to = to;
  m_width = width;
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

//======================================================
MTriangleMesh::MTriangleMesh()
{
  m_x0 = m_x1 = m_z0 = m_z1 = 0;
  m_dist = 1.;
}

void MTriangleMesh::GetMesh(MeshModel & m, const StreamCoordinates & size)
{
  double x, z;
  int i, j;
  double d = m_dist*sqrt(0.75);

  int x_s = (m_x1 - m_x0)/m_dist;
  int z_s = (m_z1 - m_z0)/m_dist;
  if (x_s < 0)
    x_s = -x_s;
  if (z_s < 0)
    z_s = -z_s;

  double x_max = x_s + m_dist/2;
  double z_max = z_s;

  for (j=0; j<z_s; j++) {
    z = m_z0 + d * j;
    double xplus = m_dist/2;
    if (j % 2 == 0)
      xplus = 0;

    int from = m.VertexCount();
    for (i=0; i<x_s; i++) {
      x = m_x0 + m_dist * i + xplus;
      m.AddVertex(Coordinates(x,0,z)); 
      m.AddTexCoord(Coordinates(x/x_max,0,z/z_max));
    }
    for (i=0; i<x_s; i++) {
      x = m_x0 + m_dist * i + xplus;
      m.AddVertex(Coordinates(x,0,z)); 
      m.AddTexCoord(Coordinates(x/x_max,0,z/z_max));
    }
    int to = m.VertexCount();   
    
    if (from == 0)
      continue;

    int off = to - from;
    int mm = 0;
    if (j % 2 == 0)
    mm = 1;
    for (i=from; i<from + off/2-1; i++) {
      m.AddIndex(i-off,i-off+1, i+mm);
    }
    //for (i=from+off/2; i<from+off-1; i++) {
    //  m.AddIndex(i,i+1, i-off+1);
    //}
    
  }

  m.RecomputeNormals();
}

