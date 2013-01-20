/*
 * julia_fractal.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#pragma once

#include "fractal.h"
#include "shader_program.h"

namespace julia {

class JuliaFractal : public Fractal {
public:
    JuliaFractal();
    ~JuliaFractal();
    
    void load_shaders(ShaderProgramRef program) override;
    void before_draw() override;
    
private:
    GLuint _viewport_size_uniform;
};

} // namespace julia
