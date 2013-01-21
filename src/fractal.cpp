/*
 * fractal.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#include "fractal.h"
#include "application.h"

#include <algorithm>
#include <iostream>

static const GLfloat quad_vertex_data[4 * 3] = {
    -1.0f,   -1.0f,   0.f,
    1.0f,    -1.0f,   0.f,
    -1.0f,   1.0f,    0.f,
    1.0f,    1.0f,    0.f
};

static const GLuint element_vertex_data[4] = {
    0, 1, 2, 3
};

namespace julia {

Fractal::Fractal() :
    _buffers_initialized(false),
    _shaders_initialized(false),
    _projection_dirty(true),
    _modelview_dirty(true)
{}

Fractal::~Fractal()
{
    glDeleteBuffers(JULIA_NUM_BUFFERS, _vertex_buffers);
}

void Fractal::draw()
{
    if (!_buffers_initialized) {
        _setup_buffers();
    }
    
    if (!_shaders_initialized) {
        _setup_shaders();
    }
    
    _update_projection();
    _update_modelview();
    
    before_draw();
    
    if (_shader_program.get() != nullptr) {
        _shader_program->use_program();
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_buffers[JULIA_ELEMENT_BUFFER]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
    
    after_draw();
}

#pragma mark - Internal

void Fractal::_setup_buffers()
{
    glGenBuffers(JULIA_NUM_BUFFERS, _vertex_buffers);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffers[JULIA_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), quad_vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(JULIA_VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(JULIA_VERTEX_ATTRIB_POSITION);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_buffers[JULIA_ELEMENT_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), element_vertex_data, GL_STATIC_DRAW);
    
    _buffers_initialized = true;
}

void Fractal::_setup_shaders()
{
    bool success = true;
    ShaderProgramRef program(new ShaderProgram);
    
    // load a simple orthographic vertex shader
    success &= program->load_shader("shaders/vertex.glsl", GL_VERTEX_SHADER);
    
    // bind attributes
    success &= program->bind_attribute(JULIA_VERTEX_ATTRIB_POSITION, "position");
    
    if (success) {
        load_shaders(program);
        program->link_program();
        _shader_program = program;
        
        _shaders_initialized = true;
    }
}

void Fractal::_update_projection()
{
    if (_projection_dirty) {
        _projection_matrix = Util::ortho_matrix(-1.f, 1.f, -1.f, 1.f, 0.001f, 1000.f);
        
        _shader_program->use_program();
        GLint proj_uniform = _shader_program->get_uniform("projection");
        glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, _projection_matrix.data());
        
        _projection_dirty = false;
    }
}

void Fractal::_update_modelview()
{
    if (_modelview_dirty) {
        _modelview_matrix = Util::identity_matrix();
        
        _shader_program->use_program();
        GLint modelview_uniform = _shader_program->get_uniform("modelview");
        glUniformMatrix4fv(modelview_uniform, 1, GL_FALSE, _modelview_matrix.data());
        
        _modelview_dirty = false;
    }
}

} // namespace julia
