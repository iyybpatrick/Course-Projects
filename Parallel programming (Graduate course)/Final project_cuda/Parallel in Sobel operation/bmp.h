#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

typedef unsigned short int WORD;
typedef unsigned int DWORD;
typedef int LONG;
typedef unsigned char BYTE;
#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{ // bmfh
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
}BITMAPFILEHEADER;
#pragma pack()
typedef struct tagBITMAPINFOHEADER
{ // bmih
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{ // rgbq
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFO
{
BITMAPINFOHEADER bmiHeader;
RGBQUAD bmiColors[1];
}BITMAPINFO;

#endif//  BMP_H_INCLUDED







