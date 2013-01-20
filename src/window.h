/*
 * window.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#pragma once

#include <string>

namespace julia {

class Window {
public:
    Window(unsigned width = 1024, unsigned height = 768);
    ~Window();
    
    std::string get_title() const { return _title; }
    void set_title(const std::string &title) { _title = title; }
    
    void initialize();

protected:
    unsigned _width, _height;
    unsigned _glut_handle;
    std::string _title;
};

} // namespace julia
