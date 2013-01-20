/*
 * window.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#include "window.h"
#include "gl_includes.h"

namespace julia {

Window::Window(unsigned width, unsigned height) :
    _width(width),
    _height(height)
{}

Window::~Window()
{}

void Window::initialize()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_width, _height);
    _glut_handle = glutCreateWindow(_title.c_str());
}

} // namespace julia
