/*
 * fractal.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#pragma once

#include "gl_includes.h"
#include "shader_program.h"
#include <memory>

namespace julia {

enum {
    JULIA_VERTEX_BUFFER,
    JULIA_ELEMENT_BUFFER,
    JULIA_NUM_BUFFERS
};

class Fractal {
public:
    Fractal();
    virtual ~Fractal();
    
    ShaderProgramRef get_shader_program() const { return _shader_program; }
    
    virtual void before_draw() {}
    virtual void after_draw() {}
    virtual void load_shaders(ShaderProgramRef program) {}
    void draw();

private:
    void _setup_buffers();
    void _setup_shaders();

private:
    GLuint _vertex_buffers[JULIA_NUM_BUFFERS];
    ShaderProgramRef _shader_program;
    bool _buffers_initialized;
    bool _shaders_initialized;
};

} // namespace julia
