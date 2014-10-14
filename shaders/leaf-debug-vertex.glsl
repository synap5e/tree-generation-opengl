#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour_in;
layout(location = 2) in vec3 vertex_normal_in;
layout(location = 3) in vec3 vertex_offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;
out vec3 vertex_normal;

void main() {
	mat4 mv = view * model;

	mat3 normal_matrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
	mat4 mvp = projection * mv;

	//vertex_normal = -normalize(vertex_position);// 
	vertex_normal = /* normal_matrix * */ vertex_normal_in;
	vertex_color = vertex_colour_in;
	
	gl_Position = mvp * vec4(vertex_position + vertex_offset, 1.0);
}
