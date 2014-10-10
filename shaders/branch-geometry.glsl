#version 400

const int num_sides = 10;
const float PI = 3.141592653589793f;

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = /* 22 */ 22) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
in vec4 color[];

out vec3 normal;
out vec3 tanjent;
out vec3 bitanjent; 
out vec3 geom_color;

void main() {
    geom_color = vec3(0.6,0.3,0);

    // the axis for the branch is used as its bitanjent even though the radii of each end of the branch are not equal 
	vec3 prev_bitanjent = normalize(gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz);
	vec3 prev_i_unit    = normalize(cross(vec3(1,0,0), prev_bitanjent));
	vec3 prev_k_unit    = normalize(cross(prev_i_unit, prev_bitanjent)); // normalize(cross(vec3(0,0,1), prev_bitanjent)); ?

    vec3 curr_bitanjent = normalize(gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz);
    vec3 curr_i_unit    = normalize(cross(vec3(1,0,0), curr_bitanjent));
    vec3 curr_k_unit    = normalize(cross(curr_i_unit, curr_bitanjent));

    vec3 start = gl_in[1].gl_Position.xyz;
    vec3 end   = gl_in[2].gl_Position.xyz;

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        float cs = cos(ang);
        float sn = -sin(ang);

        vec3 prev_offset = prev_i_unit * cs + prev_k_unit * sn;
        normal      = normal_matrix[1] * normalize(prev_offset);
        bitanjent   = curr_bitanjent;
        tanjent     = cross(normal, bitanjent);
        gl_Position = mvp[1] * vec4(start + prev_offset * size[1], 1);
        EmitVertex();

        vec3 curr_offset = curr_i_unit * cs + curr_k_unit * sn;
        normal      = normal_matrix[2] * normalize(curr_offset);
        bitanjent   = prev_bitanjent;
        tanjent     = cross(normal, bitanjent);
        gl_Position = mvp[2] * vec4(end + curr_offset * size[2], 1);
        EmitVertex();
    }
    EndPrimitive();

}
