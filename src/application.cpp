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

Application* Application::instance()
{
    static Application __currentapp;
    return &__currentapp;
}

Application::Application() :
    _viewport_size({ 1024, 768 }),
    _current_fractal(nullptr)
{}

Application::~Application()
{
    if (_current_fractal != nullptr) {
        delete _current_fractal;
    }
}

void Application::run(int argc, const char **argv)
{
    for (unsigned i = 0; i < argc; ++i) {
        _arguments.push_back(argv[i]);
    }
    
    // initial setup
    _initialize_glut(argc, const_cast<char**>(argv));
    _initialize_callbacks();
    
    // load a fractal
    _current_fractal = new JuliaFractal();
    
    // start main run loop
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
    _viewport_size = { width, height };
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

#pragma mark - Internal

void Application::_initialize_glut(int argc, char **argv)
{
    glutInit(&argc, const_cast<char**>(argv));
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_viewport_size.first, _viewport_size.second);
    _glut_window_handle = glutCreateWindow("julia");
}

void Application::_initialize_callbacks()
{
    glutDisplayFunc([]{ Application::instance()->display_callback(); });
    glutReshapeFunc([](int width, int height){ Application::instance()->reshape_callback(width, height); });
}

} // namespace julia
