/*
 * fractal.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
#include "fractal.h"
#include <iostream>

#define COUNT(X) (sizeof(X) / sizeof(X[0]))

static const GLfloat quad_vertex_data[4 * 3] = {
    -1.f,   -1.f,   0.f,
    1.f,    -1.f,   0.f,
    -1.f,   1.f,    0.f,
    1.f,    1.f,    0.f
};

static const GLuint element_vertex_data[4] = {
    0, 1, 2, 3
};

namespace julia {

Fractal::Fractal() :
    _buffers_initialized(false),
    _shaders_initialized(false)
{}

Fractal::~Fractal()
{}

void Fractal::draw()
{
    if (!_buffers_initialized) {
        _setup_buffers();
    }
    
    if (!_shaders_initialized) {
        _setup_shaders();
    }
    
    before_draw();
    
    if (_shader_program.get() != nullptr) {
        _shader_program->use_program();
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_buffers[JULIA_ELEMENT_BUFFER]);
    glDrawElements(GL_TRIANGLE_STRIP, COUNT(element_vertex_data), GL_UNSIGNED_INT, nullptr);
    
    after_draw();
}

#pragma mark - Internal

void Fractal::_setup_buffers()
{
    glGenBuffers(2, _vertex_buffers);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffers[JULIA_VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_data), quad_vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(JULIA_VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(JULIA_VERTEX_ATTRIB_POSITION);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_buffers[JULIA_ELEMENT_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_vertex_data), element_vertex_data, GL_STATIC_DRAW);
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
    }
}

} // namespace julia
