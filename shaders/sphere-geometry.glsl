#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 42) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
//in vec4 color;

out vec3 normal;
out vec3 geom_color;

void main() {
   
    geom_color = vec3(0.9,0.2,0.8);

    vec4 pos = mvp[0] * gl_in[0].gl_Position;

    int num_sides = 20;
    float PI = 3.141592653589793f;

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        vec4 offset = vec4(cos(ang), -sin(ang), 0, 0);
        normal = offset.xyz;
        gl_Position = pos + offset * size[0];
        EmitVertex();

        normal = vec3(0,0,1);
        gl_Position = pos;
        EmitVertex();
    }

    EndPrimitive();
}
