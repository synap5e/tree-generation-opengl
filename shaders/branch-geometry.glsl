#version 400

layout(lines) in;
layout(triangle_strip, max_vertices = 42) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
in vec4 color[];

out vec3 normal;
out vec3 geom_color;


void main() {
    geom_color = vec3(0.6,0.3,0);

	vec3 bvec = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;


	vec3 nbvec = normalize(bvec);
	vec3 i_unit = cross(vec3(1,0,0), nbvec);
	vec3 k_unit = cross(vec3(0,0,1), nbvec);

	int num_sides = 20;
    float PI = 3.141592653589793f;

    vec3 start = gl_in[0].gl_Position.xyz + bvec/4;
    vec3 end = gl_in[1].gl_Position.xyz - bvec/4;

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        vec3 offset = i_unit * cos(ang) + k_unit * -sin(ang);
        normal =  normal_matrix[0] *  normalize(offset);

        //f_color = vec4(1,1,1,1);//color[0];
        gl_Position = mvp[0] * vec4(start + offset * size[0], 1);
        EmitVertex();

       // f_color = vec4(1,1,1,1);//color[1];
        gl_Position = mvp[1] * vec4(end + offset * size[1], 1);
        EmitVertex();
    }
    EndPrimitive();

}
