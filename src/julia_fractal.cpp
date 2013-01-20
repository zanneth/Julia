/*
 * julia_fractal.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#include "julia_fractal.h"
#include "application.h"

namespace julia {

JuliaFractal::JuliaFractal() :
    _viewport_size_uniform(0)
{}

JuliaFractal::~JuliaFractal()
{}
    
void JuliaFractal::load_shaders(ShaderProgramRef program)
{
    program->load_shader("shaders/julia_fragment.glsl", GL_FRAGMENT_SHADER);
}

void JuliaFractal::before_draw()
{
    ShaderProgramRef program = get_shader_program();
    program->use_program();
    
    std::pair<unsigned, unsigned> viewport_size = Application::instance()->get_viewport_size();
    if (!_viewport_size_uniform) {
        _viewport_size_uniform = glGetUniformLocation(program->get_program(), "viewport_size");
    }
    glUniform2f(_viewport_size_uniform, viewport_size.first, viewport_size.second);
}

} // namespace julia
