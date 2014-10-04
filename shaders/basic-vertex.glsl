#version 400

layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in mat4 model;
/*layout(location = 2) in vec3 vertex_normal; */

/* out vec3 colour;
out vec3 normal; */

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec4 vertex;
out mat4 mvp;
out mat3 normal_matrix;
out vec4 color;

void main() {
	mat4 mv = view * model;

	normal_matrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
	mvp = projection * mv;

	gl_Position = mvp * vec4(vertex_position, 1.0);

	color = vec4(1,1,1,1);

/* 	colour = vertex_colour;
	normal = NormalMatrix * vertex_normal;
	gl_Position = MVP * vec4(vertex_position, 1.0); */

	

}
