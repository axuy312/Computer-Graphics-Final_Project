#pragma once
#ifndef BITMAP_H
#define BITMAP_H

struct BMPPixel {
  unsigned char r, g, b;
  unsigned char i16;
};

struct BMPImage {
  // Bitmap header parts
  unsigned char test[2];
  unsigned int size;
  unsigned int reserved;
  unsigned int offset;

  // DIB header
  unsigned int dibSize;
  unsigned int pixelWidth;
  unsigned int pixelHeight;
  unsigned short numColorPanes;
  unsigned short colorDepth;
  unsigned int compressionMethod;
  unsigned int rawImageSize;
  int horizRes;
  int vertRes;
  unsigned int numColors;
  unsigned int numImpColors;

  // Pixel info
  unsigned int rowSize;
  unsigned int arraySize;
  struct BMPPixel* pixels;
};

struct BMPImage* loadBMP(const char* file);
void unloadBMP(struct BMPImage* bmp);

#endif