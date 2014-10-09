#version 400

const int num_sides = 10;
const float PI = 3.141592653589793f;

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = /* 22 */ 44) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
in vec4 color[];

out vec3 normal;
out vec3 geom_color;

void main() {
    geom_color = vec3(0.6,0.3,0);

	vec3 prev_bvec = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 prev_nbvec = normalize(prev_bvec);

	vec3 prev_i_unit = normalize(cross(vec3(1,0,0), prev_bvec));
	vec3 prev_k_unit = normalize(cross(prev_i_unit, prev_bvec)); // why isn't this equal to normalize(cross(vec3(0,0,1), prev_bvec)); ?

    vec3 c_bvec = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
    vec3 c_nbvec = normalize(c_bvec);

    vec3 c_i_unit = normalize(cross(vec3(1,0,0), c_bvec));
    vec3 c_k_unit = normalize(cross(c_i_unit, c_bvec)); // why isn't this equal to normalize(cross(vec3(0,0,1), c_bvec)); ?

    vec3 start = gl_in[1].gl_Position.xyz;
    vec3 end = gl_in[2].gl_Position.xyz;

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        float cs = cos(ang);
        float sn = -sin(ang);

        vec3 offset = c_i_unit * cs + c_k_unit * sn;
        normal = normal_matrix[2] * normalize(offset);
        gl_Position = mvp[2] * vec4(end + offset * size[2], 1);
        EmitVertex();

        offset = prev_i_unit * cs + prev_k_unit * sn;
        normal = normal_matrix[1] * normalize(offset);
        gl_Position = mvp[1] * vec4(start + offset * size[1], 1);
        EmitVertex();
    }
    EndPrimitive();

}
