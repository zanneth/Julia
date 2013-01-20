/*
 * shader_program.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/30/2012
 */
 
#pragma once

#include "gl_includes.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace julia {

enum VertexAttribute {
    JULIA_VERTEX_ATTRIB_POSITION,
    JULIA_VERTEX_ATTRIB_NORMAL,
    JULIA_VERTEX_ATTRIB_COLOR,
    JULIA_VERTEX_ATTRIB_TEXCOORD0,
    JULIA_VERTEX_ATTRIB_TEXCOORD1,
    JULIA_VERTEX_ATTRIB_COUNT
};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    
    GLuint get_program() const { return _program; }
    bool is_linked() const { return _linked; }
    
    bool load_shader(const char *filename, GLenum type);
    bool bind_attribute(VertexAttribute attrib, std::string name);
    bool link_program();
    bool use_program();
    
protected:
    GLuint _program;
    std::vector<GLuint> _shaders;
    std::map<GLuint, std::string> _attrib_map;
    bool _linked;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramRef;

} // namespace julia
