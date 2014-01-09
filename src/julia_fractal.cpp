/*
 * julia_fractal.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#include "julia_fractal.h"
#include "application.h"
#include <iostream>

namespace julia {

JuliaFractal::JuliaFractal()
{}

JuliaFractal::~JuliaFractal()
{}
    
void JuliaFractal::load_shaders(ShaderProgramRef program)
{
    program->load_shader("shaders/julia_fragment.glsl", GL_FRAGMENT_SHADER);
}

} // namespace julia
