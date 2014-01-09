/*
 * utility.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/20/2013
 */
 
#pragma once

#include "gl_includes.h"
#include <array>

namespace julia {

typedef std::array<GLfloat, 4*4> Matrix4f;
typedef std::pair<unsigned, unsigned> Size2u;

class Util {
public:
    static Matrix4f identity_matrix();
    static Matrix4f ortho_matrix(GLfloat left,
                                 GLfloat right,
                                 GLfloat bottom,
                                 GLfloat top,
                                 GLfloat near,
                                 GLfloat far);
};

} // namespace julia
