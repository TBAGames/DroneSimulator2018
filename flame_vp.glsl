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

// Attributes forwarded to the geometry shader
out vec4 particle_color;
out float particle_id;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0); // Up direction
float accel = 8.9; // An acceleration applied to the particles coming from some attraction force
float speed = 8.0; // Control the speed of the motion

// Define some useful constants
const float pi = 3.1415926536;
const float pi_over_two = 1.5707963268;
const float two_pi = 2.0*pi;
//const float cone_angle_max = 30.0;
const vec2 id_bounds = vec2(0.0, 255.0);
//const vec3 const_color = vec3(0.2, 0.1, 0.1);


void main()
{
    // Define particle id
    particle_id = color.r; // Derived from the particle color. We use the id to keep track of particles

	float arc = two_pi * (particle_id / id_bounds.y );

	// Get tradjectory vector
	vec3 direction = vec3(cos(arc), sin(arc), 0.0);

	float phase = two_pi*particle_id; // Start the sin wave later depending on the particle_id
	float param = timer / 10.0 + phase; // The constant that divides "timer" also helps to adjust the "speed" of the fire
	float rem = mod(param, pi_over_two); // Use the remainder of dividing by pi/2 so that we are always in the range [0..pi/2] where sin() gives values in [0..1]
	float circtime = sin(rem); // Get time value in [0..1], according to a sinusoidal wave

	// Set up parameters of the particle motion
	float t = abs(circtime)*(0.3 + abs(normal.y)); // Our time parameter

    // First, work in local model coordinates (do not apply any transformation)
    vec3 position = vertex;
    position += speed*direction*vec3(1.0, 30.0, 20.0)*accel*t*t; // Particle moves up
    
    // Define output position but do not apply the projection matrix yet
    gl_Position = view_mat * world_mat * vec4(position, 1.0);
    
    // Define amount of blending depending on the cyclic time
    float alpha = 1.0 - circtime*circtime;
    particle_color = vec4(color, alpha);
}