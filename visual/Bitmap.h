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

 private:
  svec<RGBPixel> m_data;
  int m_x;
  int m_y;
  BITMAPFILEHEADER m_fileHeader;
  BITMAPINFOHEADER m_infoHeader;
};



#endif //BITMAP_H
