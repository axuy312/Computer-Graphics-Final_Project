#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <fcntl.h> // windows only header that has file control helpers..
#include "bitmap.h"
 
void printHeaderInfo(struct BMPImage* image) {
    // lets print our header info w loaded
    printf("HEADER             Test[%c%c]\n", image->test[0], image->test[1]);
    printf("HEADER             Size[%u]\n", image->size);
    printf("HEADER           Offset[%u]\n", image->offset);
 
    printf("DIBHEADER         Width[%u]\n", image->pixelWidth);
    printf("DIBHEADER        Height[%u]\n", image->pixelHeight);
    printf("DIBHEADER   Color Depth[%u]\n", image->colorDepth);
    printf("DIBHEADER      Row Size[%u]\n", image->rowSize);
}
 
void pixelFrom16(struct BMPPixel* px, unsigned char color) {
    px->r = px->g = px->b = 0;
    px->i16 = color; //save the index color
    switch (color) {
        case  0: break;
        case  1: px->r = 132; break;
        case  2: px->g = 132; break;
        case  3: px->r = 132; px->g = 132; break;
        case  4: px->b = 132; break;
        case  5: px->r = 132; px->b = 132; break;
        case  6: px->g = 132; px->b = 132; break;
        case  7: px->r = 132; px->g = 132; px->b = 132; break;
        case  8: px->r = 198; px->g = 198; px->b = 198; break;
        case  9: px->r = 255; break;
        case 10: px->g = 255; break;
        case 11: px->r = 255; px->g = 255; break;
        case 12: px->b = 255; break;
        case 13: px->r = 255; px->b = 255; break;
        case 14: px->g = 255; px->b = 255; break;
        case 15: px->r = 255; px->g = 255; px->b = 255; break;
    }
}
 
bool readBMPImage(struct BMPImage* image, unsigned char* memory) {
    // We need to read in four main parts:
    // - BitMap header
    // - DIB header
    // - Color Palette (optionally present)
    // - Image data
 
    // Bitmap header
    memcpy(&image->test,        &memory[0],  2);
    memcpy(&image->size,        &memory[2],  4);
    memcpy(&image->reserved,    &memory[6],  4); // shoving both parts into one value cause I dont use it
    memcpy(&image->offset,      &memory[10], 4);
 
    // DIB Header BITMAPINFOHEADER
    memcpy(&image->dibSize,     &memory[14], 4);
    if (image->dibSize != 40) {
        fprintf(stderr, "ERROR: DIB Size must be 40 bytes (got %u) for BITMAPINFOHEADER\n", image->dibSize);
        return false;
    }
    memcpy(&image->pixelWidth,        &memory[18],    4);
    memcpy(&image->pixelHeight,       &memory[22],    4);
    memcpy(&image->numColorPanes,     &memory[26],    2);
    memcpy(&image->colorDepth,        &memory[28],    2);
    memcpy(&image->compressionMethod, &memory[30],    4);
    memcpy(&image->rawImageSize,      &memory[34],    4);
    memcpy(&image->horizRes,          &memory[38],    4);
    memcpy(&image->vertRes,           &memory[42],    4);
    memcpy(&image->numColors,         &memory[46],    4);
    memcpy(&image->numImpColors,      &memory[50],    4);
    if (image->compressionMethod != 0) {
        fprintf(stderr, "ERROR: compression method of %u is unsupported\n", image->compressionMethod);
        return false;
    }
 
    // Read color palette, but we are starting with 24 bit BMP files so we
    // don't care because colors are encoded into pixels directly
    if (image->numColors != 0) {
        fprintf(stderr, "ERROR: color palette detected and unsupported\n");
        return false;
    }
 
    // Lets make sure this is a 24 bit bmp before loading data
    if (image->colorDepth != 24 && image->colorDepth != 4) {
        fprintf(stderr, "ERROR: color depth of %u is unsupported\n", image->colorDepth);
        return false;
    }
 
    // Time to load the pixel data!
    // Size of the row is always a multiple of 4 so there can be padding on the end for
    // images that are not a multiple of 4 pixels wide eg 10x10
    image->rowSize = ( ( (image->colorDepth * image->pixelWidth) + 31 ) / 32 ) * 4; // integer math ceils it!
    image->arraySize = image->pixelWidth * image->pixelHeight; // size of actual pixel image without padding
 
    // forgot to add the pixels to the image
    image->pixels = (struct BMPPixel*)malloc(image->arraySize * sizeof(struct BMPPixel));
 
    // lets load it
    int index = 0;
    for (int i=0; i < image->pixelHeight; i++) {
        // lets load our row data into memory with its full padded size
        unsigned char* rowData = (unsigned char*)malloc(image->rowSize * sizeof(unsigned char));
        // we are adding offset to the number of rows we have looped over
        memcpy(rowData, &memory[image->offset + i * image->rowSize], image->rowSize); // offset should be 54
 
        if (image->colorDepth == 24) {
            // IN 24 bit each pixel is 3 bytes one per color
            // lets load only the pixel data into memory
            for (int j=0; j < image->pixelWidth * 3; j += 3) { // iterate by 3, one for each R G B value
                // colors are stored in order: BLUE GREEN RED    \ ? /
                image->pixels[index].b = rowData[j];
                image->pixels[index].g = rowData[j + 1];
                image->pixels[index].r = rowData[j + 2];
                index++;
            }
        } else if (image->colorDepth == 4) {
            // IN 4 bit each byte has 2 pixels, 4 bits per color
            // colors are just an index of 0 through 15
            for (int j=0; j < image->pixelWidth / 2; j++) {
                // we need to use binary operations to get the data out of it
                unsigned char lmask = 0xf0; // left bits
                unsigned char rmask = 0x0f; // right bits
                unsigned char pixels = rowData[j];
                unsigned char lpixel = pixels & lmask; // AND the bits
                lpixel = lpixel >> 4; // must shift over 4 bits so the color index value is read properly
                unsigned char rpixel = pixels & rmask; // AND the bits
                // we need to convert the colors to actual RGB values
                pixelFrom16(&image->pixels[index], lpixel);
                index++;
                pixelFrom16(&image->pixels[index], rpixel);
                index++;
            }
        }
 
        free(rowData); // release ro data from memor
    }
 
    return true;
}
 
struct BMPImage* loadBMP(const char *file) {
 
    // On windows fopen starts in TEXT mode and will remove data randomly....
    // So lets fix that!
    _set_fmode(_O_BINARY); // fixed
 
    // lets load our file into memory so its safer to use
    FILE* fp;
    fopen_s(&fp, file, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: could not read file\n");
        return NULL;
    }
 
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp); // get filesize then go back to the beginning of the file
    rewind(fp);
    unsigned char* buffer = (unsigned char*)malloc(fsize * sizeof(unsigned char)); // allocate memory on heap
    fread(buffer, 1, fsize, fp); // read into memory
    fclose(fp);
 
    // lets load the data into a bitmap struct
    struct BMPImage* image = (struct BMPImage*)malloc(sizeof(struct BMPImage));
 
    // load image
    if (!readBMPImage(image, buffer)) {
        // if it fails cleanup memory then return
        free(image);
        free(buffer);
        return NULL;
    }
 
    printHeaderInfo(image);
 
    // should not need buffer after we loadin the memory
    free(buffer);
 
    return image;
}
 
void unloadBMP(struct BMPImage* bmp) {
    free(bmp);
}
 