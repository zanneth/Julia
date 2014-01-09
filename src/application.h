/*
 * application.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#pragma once

#include <string>
#include <utility>
#include <vector>
#include "utility.h"

namespace julia {

class Fractal;

class Application {
public:
    static Application* instance();
    
    Size2u get_viewport_size() const { return _viewport_size; }
    
    void run(int argc, const char **argv);
    void display_callback();
    void reshape_callback(int width, int height);
    
private:
    Application();
    ~Application();
    
    void _initialize_glut(int argc, char **argv);
    void _initialize_callbacks();

protected:
    std::vector<std::string> _arguments;
    Size2u _viewport_size;
    unsigned _glut_window_handle;
    Fractal *_current_fractal;
};

} // namespace julia
