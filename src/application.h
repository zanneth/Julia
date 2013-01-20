/*
 * application.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#pragma once

#include <string>
#include <vector>
#include "window.h"

namespace julia {

class Fractal;

class Application {
public:
    static Application* instance();
    
    void run(int argc, const char **argv);
    void display_callback();
    void reshape_callback(int width, int height);
    
private:
    Application();
    ~Application();
    
    void _initialize_callbacks();

protected:
    std::vector<std::string> _arguments;
    Window _main_window;
    Fractal *_current_fractal;
};

} // namespace julia
