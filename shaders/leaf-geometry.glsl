#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 64) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
//in vec4 color;

out vec3 normal;
out vec3 geom_color;

void main() {
    normal = vec3(0,0,1);
    geom_color = vec3(0,1,0);
        
    vec4 pos = mvp[0] * gl_in[0].gl_Position;

    gl_Position = pos + vec4(-size[0], 0, 0, 0);
    EmitVertex();       

    gl_Position = pos + vec4(size[0], 0, 0, 0);
    EmitVertex();       

    gl_Position = pos + vec4(0, size[0], 0, 0);
    EmitVertex();
 
   


    EndPrimitive();
}
