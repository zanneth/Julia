/*
 * fractal.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#pragma once

#include "gl_includes.h"
#include "shader_program.h"
#include "utility.h"
#include <algorithm>
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
    void set_projection_dirty(bool dirty) { _projection_dirty = dirty; }
    
    bool save_jpeg(Size2u resolution, std::string path);
   
    virtual void load_shaders(ShaderProgramRef program) {}
    void draw(Size2u viewport_size);

private:
    void _setup_buffers();
    void _setup_shaders();
    
    void _update_viewport(Size2u viewport_size);
    void _update_projection();
    void _update_modelview();

private:
    GLuint _vertex_buffers[JULIA_NUM_BUFFERS];
    ShaderProgramRef _shader_program;
    Matrix4f _projection_matrix;
    Matrix4f _modelview_matrix;
    
    bool _buffers_initialized;
    bool _shaders_initialized;
    
    bool _projection_dirty;
    bool _modelview_dirty;
};

} // namespace julia
