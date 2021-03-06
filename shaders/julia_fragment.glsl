/*
 * julia_fragment.glsl
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/19/2013
 */

/* configuration */
#ifdef GL_ES
precision highp float;
#endif

/* inputs */
uniform vec2 viewport_size;

/* structures */
struct complex {
    float real;
    float imaginary;
};
struct color_palette {
    vec3 begin_color;
    vec3 end_color;
};

/* constants */
const float ESCAPE_RADIUS = 2.0;
const int NUM_CYCLES = 100;

/* these are arbitrary values that comprise the equation used to generate the julia set. */
const int EQUATION_EXPONENT = 3;
const complex EQUATION_CONSTANT = complex(-0.47, 0.56);

complex add_complex(complex c1, complex c2)
{
    return complex(c1.real + c2.real, c1.imaginary + c2.imaginary);
}

complex multiply_complex(complex c1, complex c2)
{
    float r1 = c1.real;
    float i1 = c1.imaginary;
    float r2 = c2.real;
    float i2 = c2.imaginary;
    
    /* (r1 + i1) * (r2 + i2) = ((r1 * r2) - (i1 * i2)) + ((r1 * i2) + (i1 * r2))i */
    float real = (r1 * r2) - (i1 * i2);
    float imaginary = (r1 * i2) + (i1 * r2);
    return complex(real, imaginary);
}

complex exponent_complex(complex c1, int exp)
{
    if (exp == 0) {
        return complex(1.0, 0.0);
    } else if (exp == 1) {
        return c1;
    }
    
    /* GLSL doesn't allow recursion, which stinks */
    complex result = complex(1.0, 0.0);
    while (exp > 0) {
        result = multiply_complex(c1, result);
        exp--;
    }
    return result;
}

float distance_complex(complex c1)
{
    return sqrt(c1.real * c1.real + c1.imaginary * c1.imaginary);
}

void apply_function(inout complex num)
{
    /* f(z) = z^3 + C */
    complex result = add_complex(exponent_complex(num, EQUATION_EXPONENT), EQUATION_CONSTANT);
    num = result;
}

vec3 color_for_iterations(int num_iterations)
{
    vec3 color;
    
    if (num_iterations == 1) {
        color = vec3(1.0, 0.0, 0.0);
    } else if (num_iterations == 2) {
        color = vec3(0.0, 1.0, 0.0);
    } else if (num_iterations == 3) {
        color = vec3(0.0, 0.0, 1.0);
    } else if (num_iterations == 4) {
        color = vec3(1.0, 1.0, 0.0);
    } else if (num_iterations == 5) {
        color = vec3(0.0, 1.0, 1.0);
    } else if (num_iterations == 6) {
        color = vec3(1.0, 0.0, 1.0);
    } else {
        color = vec3(0.0, 0.0, 0.0);
    }
    
    return color;
}

vec3 compute_color_alt(complex pt, color_palette palette)
{
    vec3 final_color;
    
    /* apply the function above multiple times until the value "escapes". when it
       does, calculate how quickly it escaped based on its distance after stopping. */
    complex result = pt;
    int iteration;
    for (iteration = 0; iteration < NUM_CYCLES; ++iteration) {
        apply_function(result);
        
        float distance = distance_complex(result);
        if (distance >= ESCAPE_RADIUS) {
            break;
        }
    }
    
    if (iteration >= NUM_CYCLES) {
        final_color = vec3(0.0, 0.0, 0.0);
    } else {
        float interp_weight = float(iteration) / (float(NUM_CYCLES) / 2.0);
        final_color = mix(palette.begin_color, palette.end_color, interp_weight);
    }
    
    return final_color;
}

/* get the point on the complex plane */
complex get_point(vec2 screen_coord)
{
    vec2 complex_pt;
    complex_pt.x = (screen_coord.x / viewport_size.x) - 0.5;
    complex_pt.y = (screen_coord.y / viewport_size.y) - 0.5;
    
    complex result = complex(complex_pt.x, complex_pt.y);
    return result;
}

void main()
{
    complex pt = get_point(vec2(gl_FragCoord));
    color_palette palette = color_palette(vec3(0.0, 0.0, 0.0), vec3(1.0, 0.2, 0.0));
    gl_FragColor = vec4(compute_color_alt(pt, palette), 1.0);
}
