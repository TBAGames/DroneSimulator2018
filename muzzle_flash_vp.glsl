#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;
uniform vec3 color_in;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
vec3 object_color = vec3(1.0, 0.6, 0.6);
float grav = 0.08; // Gravity
float speed = 16.5; // Allows to control the speed of the explosion
float anim_length = 0.45;
float drag = 8.1;


void main()
{

//x(t) = x(0) + v*t + 0.5*g*t*t

    // Let time cycle every four seconds
    float circtime = timer - anim_length * floor(timer / anim_length);
    float t = circtime; // Our time parameter
    
    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 1.0);

    // Move point along normal and down with t*t (acceleration under gravity)
    position.x += norm.x*t*(speed-t*t*drag);
    position.y += norm.y*t*(speed-t*t*drag);
    position.z += norm.z*t*(speed-t*t*drag);
    
    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex
    //vertex_color = color.rgb; // Color defined during the construction of the particles
	vertex_color = object_color;
    //vertex_color = color_in; // Uniform color 
    //vertex_color = vec3(t, 0.0, 1-t);
    //vertex_color = vec3(1.0, 1-t, 0.0);

    // Forward time step to geometry shader
    timestep = t;
}
