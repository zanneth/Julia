/*
 * application.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#include "application.h"
#include "fractal.h"
#include "julia_fractal.h"
#include "gl_includes.h"

#include <cstdlib>
#include <functional>
#include <iostream>

namespace julia {

static Application *__currentapp = nullptr;

Application::Application(int argc, const char **argv) :
    _current_fractal(nullptr)
{
    for (unsigned i = 0; i < argc; ++i) {
        _arguments.push_back(argv[i]);
    }
    
    _main_window.set_title("julia");
    
    _current_fractal = new JuliaFractal();
}

Application::~Application()
{
    if (_current_fractal != nullptr) {
        delete _current_fractal;
    }
}

void Application::run()
{
    _initialize_glut();
    _main_window.initialize();
    _initialize_callbacks();
    
    glutMainLoop();
}

void Application::display_callback()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (_current_fractal) {
        _current_fractal->draw();
    }
    
    glutSwapBuffers();
}

void Application::reshape_callback(int width, int height)
{
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

#pragma mark - Internal

void Application::_initialize_glut()
{
    int argc = (int) _arguments.size();
    const char *argv[argc];
    unsigned i = 0;
    for (std::string arg : _arguments) { argv[i++] = arg.c_str(); }
    glutInit(&argc, const_cast<char**>(argv));
}

void Application::_initialize_callbacks()
{
    __currentapp = this;
    glutDisplayFunc([]{ __currentapp->display_callback(); });
    glutReshapeFunc([](int width, int height){ __currentapp->reshape_callback(width, height); });
}

} // namespace julia
