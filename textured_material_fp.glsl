#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos1;
in vec3 light_pos2;
//in int toon_on;

// Uniform (global) buffer
uniform sampler2D texture_map;
uniform int toon_enabled;

// Material attributes (constants)
vec4 ambient_color = vec4(-0.2, -0.1, -0.1, 1.0);
vec4 diffuse_color1 = vec4(0.9, 0.0, 0.9, 1.0);
vec4 diffuse_color2 = vec4(0.0, 0.9, 0.0, 1.0);
vec4 specular_color = vec4(0.2, 0.5, 0.1, 1.0);
float phong_exponent = 128.0;

// Toon Coloring
vec4  toon_r = vec4(0.0, 0.0, 0.0, 0.0);
vec4  toon_g = vec4(0.0, 0.0, 0.0, 0.0);
vec4  toon_b = vec4(0.3, 0.4, 0.6, 1.0);

void main() 
{
	vec4 pixel = vec4(0.0, 0.0, 0.0, 0.0);
	if (toon_enabled != 1) {
		 // Retrieve texture value-
		pixel = texture(texture_map, uv_interp);

		// Use texture in determining fragment colour
		// gl_FragColor = pixel;
	}

	// Blinn-Phong shading

    vec3 N, // Interpolated normal for fragment
         L1, L2, // Light-source direction
         V, // View direction
         H1, H2; // Half-way vector

    // Compute Lambertian lighting Id
    N = normalize(normal_interp);

    L1 = (light_pos1 - position_interp);
    L1 = normalize(L1);

	L2 = (light_pos2 - position_interp);
    L2 = normalize(L2);

    float Id1 = max(dot(N, L1), 0.0);
	float Id2 = max(dot(N, L2), 0.0);
    
    // Compute specular term for Blinn-Phong shading
    // V = (eye_position - position_interp);
    V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

    //H1 = 0.5*(V + L1); // Halfway vector
    H1 = (V + L1); // Halfway vector
    H1 = normalize(H1);

	//H2 = 0.5*(V + L2); // Halfway vector
    H2 = (V + L2); // Halfway vector
    H2 = normalize(H2);

    float spec_angle_cos1 = max(dot(N, H1), 0.0);
    float Is1 = pow(spec_angle_cos1, phong_exponent);

	float spec_angle_cos2 = max(dot(N, H2), 0.0);
    float Is2 = pow(spec_angle_cos2, phong_exponent);

    // Compile light terms
    vec4 illum = ambient_color + Id1*diffuse_color1 + Is1*specular_color + Id2*diffuse_color2 + Is2*specular_color + pixel;
	vec4 color;

	// snap values within given threshold
	if (toon_enabled == 1) {
	
		// Assign fragment color based on toon shading
		float light_intensity = (illum.x + illum.y + illum.z) / 3.0;
		int i;

		if (light_intensity < 0.15) {
			i = 0;
		} else if (light_intensity < 0.4) {
			i = 1;
		} else if (light_intensity < 7.0){
			i = 2;
		} else {
			i = 3;
		}
		color = vec4(toon_r[i], toon_g[i], toon_b[i], 1.0);
	} else {
		color = illum;
	}
	
	gl_FragColor = color;
                    
    // For debug, we can display the different values
    //gl_FragColor = ambient_color;
    //gl_FragColor = diffuse_color;
    //gl_FragColor = specular_color;
    //gl_FragColor = color_interp;
    //gl_FragColor = vec4(N.xyz, 1.0);
    //gl_FragColor = vec4(L1.xyz, 1.0);
    //gl_FragColor = vec4(V.xyz, 1.0);
}
