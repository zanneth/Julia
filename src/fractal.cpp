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
#include <jpeglib.h>

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

void Fractal::draw(Size2u viewport_size)
{
    if (!_buffers_initialized) {
        _setup_buffers();
    }
    
    if (!_shaders_initialized) {
        _setup_shaders();
    }
    
    if (_shader_program.get() != nullptr) {
        _shader_program->use_program();
    }
    
    _update_viewport(viewport_size);
    _update_projection();
    _update_modelview();
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_buffers[JULIA_ELEMENT_BUFFER]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
}

bool Fractal::save_jpeg(Size2u resolution, std::string path)
{
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    GLuint rendered_texture;
    glGenTextures(1, &rendered_texture);
    glBindTexture(GL_TEXTURE_2D, rendered_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.first, resolution.second, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.first, resolution.second);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    glFramebufferTextureEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rendered_texture, 0);

    draw(resolution);
    
    const unsigned num_components = 3;
    size_t data_length = num_components * resolution.first * resolution.second;
    unsigned char *image_data = new unsigned char[data_length];
    glReadPixels(0, 0, resolution.first, resolution.second, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbuffer);
    glDeleteTextures(1, &rendered_texture);
    
    FILE *output_file = fopen(path.c_str(), "w+");
    if (output_file) {
        struct jpeg_compress_struct jpeginfo;
        struct jpeg_error_mgr jpegerr;
        
        jpeg_create_compress(&jpeginfo);
        jpeginfo.err = jpeg_std_error(&jpegerr);
        jpeginfo.image_width = resolution.first;
        jpeginfo.image_height = resolution.second;
        jpeginfo.input_components = num_components;
        jpeginfo.in_color_space = JCS_RGB;
        
        jpeg_stdio_dest(&jpeginfo, output_file);
        jpeg_set_defaults(&jpeginfo);
        jpeg_set_quality(&jpeginfo, 100, true);
        
        jpeg_start_compress(&jpeginfo, true);
        JSAMPROW row_pointer = 0;
        while (jpeginfo.next_scanline < jpeginfo.image_height) {
            row_pointer = (JSAMPROW)&image_data[jpeginfo.next_scanline * num_components * jpeginfo.image_width];
            jpeg_write_scanlines(&jpeginfo, &row_pointer, 1);
        }
        jpeg_finish_compress(&jpeginfo);
        
        fclose(output_file);
    }
    delete[] image_data;
    
    return (output_file != NULL);
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

void Fractal::_update_viewport(Size2u viewport_size)
{
    ShaderProgramRef program = get_shader_program();
    GLint viewportsz_uniform = program->get_uniform("viewport_size");
    glUniform2f(viewportsz_uniform, viewport_size.first, viewport_size.second);
    glViewport(0, 0, viewport_size.first, viewport_size.second);
}

void Fractal::_update_projection()
{
    if (_projection_dirty) {
        _projection_matrix = Util::ortho_matrix(-1.f, 1.f, -1.f, 1.f, 0.001f, 1000.f);
        
        GLint proj_uniform = _shader_program->get_uniform("projection");
        glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, _projection_matrix.data());
        
        _projection_dirty = false;
    }
}

void Fractal::_update_modelview()
{
    if (_modelview_dirty) {
        _modelview_matrix = Util::identity_matrix();
        
        GLint modelview_uniform = _shader_program->get_uniform("modelview");
        glUniformMatrix4fv(modelview_uniform, 1, GL_FALSE, _modelview_matrix.data());
        
        _modelview_dirty = false;
    }
}

} // namespace julia
