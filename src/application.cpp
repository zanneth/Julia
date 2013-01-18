/*
 * application.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#include "application.hpp"
#include "gl_includes.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>

namespace julia {

static Application *__currentapp = nullptr;

Application::Application(int argc, const char **argv)
{
    for (unsigned i = 0; i < argc; ++i) {
        _arguments.push_back(argv[i]);
    }
}

Application::~Application()
{}

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
    glutSwapBuffers();
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
}

} // namespace julia
