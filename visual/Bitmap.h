#ifndef BITMAP_H
#define BITMAP_H


#include "base/SVector.h"


class RGBPixel
{
 public:
  RGBPixel() {
    m_r = 0;
    m_g = 0;
    m_b = 0;
    m_extra = 0;
  }

  unsigned char R() const {return m_r;}
  unsigned char G() const {return m_g;}
  unsigned char B() const {return m_b;}
  unsigned char Extra() const {return m_extra;}

  double r() const {return ((double)m_r)/255;}
  double g() const {return ((double)m_g)/255;}
  double b() const {return ((double)m_b)/255;}
  double extra() const {return ((double)m_extra)/255;}


  void Set_R(unsigned char c) {m_r = c;}
  void Set_G(unsigned char c) {m_g = c;}
  void Set_B(unsigned char c) {m_b = c;}
  void Set_Extra(unsigned char c) {m_extra = c;}

  void Set_r(double c) {m_r = (unsigned char)(c*255);}
  void Set_g(double c) {m_g = (unsigned char)(c*255);}
  void Set_b(double c) {m_b = (unsigned char)(c*255);}
  void Set_extra(double c) {m_extra = (unsigned char)(c*255);}

  bool operator == (const RGBPixel & p) const {
    if (m_r == p.m_r &&
	m_g == p.m_g &&
	m_b == p.m_b &&
	m_extra == p.m_extra)
      return true;
    return false;
  }

  bool operator < (const RGBPixel & p) const {
    int a = m_r*256*256 + m_g*256 + m_b;
    int b = p.m_r*256*256 + p.m_g*256 + p.m_b;
    return a < b;
  }

 private:

  unsigned char m_r; 
  unsigned char m_g; 
  unsigned char m_b; 
  unsigned char m_extra; 

};



typedef struct bitmapFileHeader{
	   unsigned short int bfType;              
	   unsigned int bfSize;                   
	   unsigned short int bfReserved1, bfReserved2;
	   unsigned int bfOffBits;                 
	} BITMAPFILEHEADER;
	 
typedef struct bitmapInfoHeader{
	   unsigned int biSize;           
	   int biWidth, biHeight;              
	   unsigned short int biPlanes;   
	   unsigned short int biBitCount;       
	   unsigned int biCompression;     
	   unsigned int biSizeImage;        
	   int biXPelsPerMeter, biYPelsPerMeter;    
	   unsigned int biClrUsed;        
	   unsigned int biClrImportant;  
	} BITMAPINFOHEADER;
	 


//============================================
class Bitmap
{

 public:
  Bitmap();

  RGBPixel & Get(int x, int y) {
    int i = m_x*y + x;
    return m_data[i];
  }

  const RGBPixel & Get(int x, int y) const {
    int i = m_x*y + x;
    return m_data[i];
  }

  void Read(const string & fileName);
  void Write(const string & fileName);


  void SetSize(int x, int y) {
    m_x = x;
    m_y = y;
    m_data.resize(m_x * m_y);
  }

  int X() const {return m_x;}
  int Y() const {return m_y;}

  void Overlay(const Bitmap & b, int xoff = 0, int yoff = 0);

 private:
  svec<RGBPixel> m_data;
  int m_x;
  int m_y;
  BITMAPFILEHEADER m_fileHeader;
  BITMAPINFOHEADER m_infoHeader;
};



#endif //BITMAP_H
