/*
 * application.hpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#pragma once

#include <string>
#include <vector>
#include "window.hpp"

namespace julia {

class Application {
public:
    Application(int argc = 0, const char **argv = nullptr);
    ~Application();
    
    void run();
    void display_callback();
    
private:
    void _initialize_glut();
    void _initialize_callbacks();

protected:
    std::vector<std::string> _arguments;
    Window _main_window;
};

} // namespace julia
