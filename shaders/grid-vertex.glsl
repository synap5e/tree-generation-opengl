#version 400

layout(location = 0) in vec3 vertex_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 geom_color;
out vec3 normal;

void main() {
	mat4 mv = view * model;

	//normal_matrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
	mat4 mvp = projection * mv;

	gl_Position =  mvp * vec4(vertex_position, 1.0);
	geom_color = vec4(0,0,0,0.2);
	normal = vec3(0,0,1);
}
