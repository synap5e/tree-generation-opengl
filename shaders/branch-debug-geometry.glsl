#version 400

layout(lines) in;
layout(line_strip, max_vertices=2) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
in vec4 color[];

out vec3 normal;
out vec3 geom_color;


void main() {
    geom_color = vec3(0.6,0.3,0);

    normal = vec3(0,0,1);
    
    gl_Position = mvp[0] * gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = mvp[1] * gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();

}
