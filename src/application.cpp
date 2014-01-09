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
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>

namespace julia {

static Application *__current_application = NULL;

Application::Application() :
    _viewport_size({1024, 768}),
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
    __current_application = this;
    
    // initial setup
    _initialize_glut(argc, const_cast<char**>(argv));
    
    // load a fractal
    _current_fractal = new JuliaFractal();
    
    if (argc < 2) {
        // start as application if no CLI args given
        _initialize_callbacks();
        glutMainLoop();
    } else if (argc == 4) {
        int width = atoi(argv[1]);
        int height = atoi(argv[2]);
        const char *output_file = argv[3];
        
        printf("Saving fractal to %s...\n", output_file);
        bool success = _current_fractal->save_jpeg({width, height}, output_file);
        if (success) {
            printf("Successfully saved fractal\n");
        } else {
            printf("Failed to save fractal to file\n");
        }
    } else {
        printf("Usage: %s image_width image_height output_file\n", argv[0]);
    }
}

void Application::display_callback()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (_current_fractal) {
        _current_fractal->draw(_viewport_size);
    }
    
    glutSwapBuffers();
}

void Application::reshape_callback(int width, int height)
{
    _viewport_size = {width, height};
    _current_fractal->set_projection_dirty(true);
}

#pragma mark - Internal

void Application::_initialize_glut(int argc, char **argv)
{
    glutInit(&argc, const_cast<char**>(argv));
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_viewport_size.first, _viewport_size.second);
    _glut_window_handle = glutCreateWindow("Julia");
}

void Application::_initialize_callbacks()
{
    glutDisplayFunc([]{ __current_application->display_callback(); });
    glutReshapeFunc([](int width, int height){ __current_application->reshape_callback(width, height); });
}

} // namespace julia
