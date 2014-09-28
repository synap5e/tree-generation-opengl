#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main() {
	colour = vertex_colour;
	gl_Position = MVP * vec4(vertex_position, 1.0);
}
