#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texcoord_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec4 vertex;
out vec2 texcoord;

void main() {
	mat4 mv = view * model;

	mat3 normal_matrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
	mat4 mvp = projection * mv;

	gl_Position = mvp * vec4(vertex_position, 1.0);

	texcoord = texcoord_in;
}
