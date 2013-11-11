/*
 * bmp_file.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 02/03/2013
 */

#include "bmp_file.h"
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <stdint.h>

#define BMP_MAGIC_NUMBER    0x424D
#define PIXELS_PER_METER    2835

struct _BMPHeader {
    uint16_t    magic_number;       // "BM"
    uint32_t    file_size;          // size of BMP file
    uint16_t    reserved_1;         // application specific
    uint16_t    reserved_2;         // application specific
    uint32_t    pixeldata_offset;   // offset where pixel data begins
};

// DIB = Device-Independent Bitmap. required structure for file type.
struct _DIBHeader {
    uint32_t    header_size;        // size of this header
    int32_t     num_pixels_width;   // width of the bitmap in pixels
    int32_t     num_pixels_height;  // height of the bitmap in pixels
    uint16_t    num_color_planes;   // number of color planes being used
    uint16_t    bits_per_pixel;     // number of bits per pixel
    uint32_t    compression_method; // pixel array compression method
    uint32_t    pixel_buffer_size;  // size of raw data in pixel buffer
    int32_t     horizontal_res;     // horizontal resolution of the image (pixels per meter)
    int32_t     vertical_res;       // vertical resolution of the image (pixels per meter)
    uint32_t    num_palette_colors; // number of colors in the palette
    uint32_t    important_colors;   // number of important colors used. (0 for all colors) (generally ignored)
};

inline void SwapEndian(uint16_t &val)
{
	val = (val<<8) | (val>>8);
}

inline void SwapEndian(uint32_t &val)
{
	val = (val<<24) | ((val<<8) & 0x00ff0000) |
    ((val>>8) & 0x0000ff00) | (val>>24);
}

namespace julia {

BitmapFile::BitmapFile(unsigned char *data, size_t size)
{
    if (size > 0) {
        _rgbdata = new unsigned char[size];
        memcpy(_rgbdata, data, size);
    }
    
    _buffer_size = size;
}

BitmapFile::~BitmapFile()
{
    if (_rgbdata != nullptr) {
        delete[] _rgbdata;
        _buffer_size = 0;
    }
}

bool BitmapFile::write_file(std::string filepath, std::pair<unsigned int, unsigned int> resolution)
{
    std::ofstream outstream(filepath, std::ios::out | std::ios::binary);
    if (!outstream) {
        std::cerr << "Cannot open file for writing: " << filepath;
        return false;
    }
    
    const size_t header_size = sizeof(struct _BMPHeader) + sizeof(struct _DIBHeader);
    
    // setup BMP header
    _BMPHeader bmp_header;
    bmp_header.magic_number = htons(BMP_MAGIC_NUMBER);
    bmp_header.file_size = htonl(header_size + _buffer_size);
    bmp_header.pixeldata_offset = htonl(0x0 + header_size);
    
    // setup DIB header
    _DIBHeader dib_header;
    dib_header.header_size = htonl(sizeof(struct _DIBHeader));
    dib_header.num_pixels_width = htonl(resolution.first);
    dib_header.num_pixels_height = htonl(resolution.second);
    dib_header.num_color_planes = htonl(1);
    dib_header.bits_per_pixel = htonl(3 * sizeof(unsigned char) * 8);
    dib_header.compression_method = 0x0;
    dib_header.pixel_buffer_size = htonl(_buffer_size);
    dib_header.horizontal_res = htonl(PIXELS_PER_METER);
    dib_header.vertical_res = htonl(PIXELS_PER_METER);
    dib_header.num_palette_colors = 0;
    dib_header.important_colors = 0; // all colors important
    
    // setup pixel data array
    uint8_t *pixbuf = new uint8_t[_buffer_size];
    if (_rgbdata != nullptr) {
        for (ssize_t i = 0; i < _buffer_size; ++i) {
            pixbuf[i] = htonl(_rgbdata[i]);
        }
    }
    
    // write the data to the file
    outstream.write((char *) &bmp_header, sizeof(struct _BMPHeader));
    outstream.write((char *) &dib_header, sizeof(struct _DIBHeader));
    outstream.write((char *) pixbuf, _buffer_size * sizeof(uint8_t));
    outstream.close();
    
    delete[] pixbuf;
    
    return true;
}

} // namespace julia
