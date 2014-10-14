#version 400

const int num_sides = 10;
const float radius = 1;
const float PI = 3.141592653589793f;

layout(triangles) in;
layout(line_strip, max_vertices = /* 22 */ 50) out;

in vec3 vertex_color[];
in vec3 vertex_normal[];
in mat4 mvp[];

out vec3 normal;
out vec3 tanjent;
out vec3 bitanjent; 
out vec3 geom_color;


void main() {
    normal = vec3(0,0,1);

    geom_color = vertex_color[0];

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
 


    geom_color = vec3(1,0,0);

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + mvp[0] * vec4(vertex_normal[0] * 3, 0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position + mvp[1] * vec4(vertex_normal[1] * 3, 0);
    EmitVertex();
    EndPrimitive();


    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position + mvp[2] * vec4(vertex_normal[2] * 3, 0);
    EmitVertex();
    EndPrimitive();

}
