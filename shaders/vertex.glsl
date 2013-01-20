/*
 * vertex.glsl
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */
 
attribute vec4 position;

void main()
{
    // simple orthographic projection
    gl_Position = position;
}
