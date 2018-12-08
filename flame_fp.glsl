#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform float timer;

void main (void)
{

	vec4 color = texture(texture_map, tex_coord);

    gl_FragColor = color;

}
