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

void JuliaFractal::before_draw()
{
    std::pair<unsigned, unsigned> viewport_size = Application::instance()->get_viewport_size();
    
    ShaderProgramRef program = get_shader_program();
    program->use_program();
    GLint viewportsz_uniform = program->get_uniform("viewport_size");
    glUniform2f(viewportsz_uniform, viewport_size.first, viewport_size.second);
}

} // namespace julia
