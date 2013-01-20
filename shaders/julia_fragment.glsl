/*
 * julia_fragment.glsl
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */

uniform vec2 viewport_size;

void main()
{
    float red = gl_FragCoord.x / viewport_size.x;
    gl_FragColor = vec4(red, 0.0, 0.0, 1.0);
}
