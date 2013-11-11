/*
 * bmp_file.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 02/03/2013
 */
 
#pragma once

#include <cstring>
#include <string>
#include <utility>

namespace julia {

class BitmapFile {
public:
    BitmapFile(unsigned char *data = nullptr, size_t size = 0);
    BitmapFile(const BitmapFile&) = delete;
    ~BitmapFile();
    
    bool write_file(std::string filepath, std::pair<unsigned, unsigned> resolution);
    
protected:
    unsigned char *_rgbdata;
    size_t _buffer_size;
};
    
} // namespace julia
