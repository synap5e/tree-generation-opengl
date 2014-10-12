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
/* 	geom_color = vec3(1,0,0);
	gl_Position = vec4(0,0,0,1);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();

	geom_color = vec3(0,1,0);
	gl_Position = vec4(0,0,0,1);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();

	geom_color = vec3(0,0,1);
	gl_Position = vec4(0,0,0,1);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive(); */
	

    vec4 pos = gl_in[0].gl_Position;
    geom_color = vec3(0,1,0);

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        vec4 offset = vec4(cos(ang), -sin(ang), 0, 0);
        normal = mix(vec3(0,0,1), offset.xyz, 0.8);
        gl_Position = mvp[0] * (pos + offset * radius);
        EmitVertex();

        normal = vec3(0,0,1);
        gl_Position = mvp[0] * pos;
        EmitVertex();
    }

}
