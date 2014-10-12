#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour_in;
layout(location = 2) in vec3 vertex_offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 geom_color;
out vec3 normal;

void main() {
	mat4 mv = view * model;

	//normal_matrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
	mat4 mvp = projection * mv;

	normal = vec3(0,0,1);
	geom_color = vertex_colour_in;//vec3(1,0,0);//vertex_colour;

/* 	if (gl_InstanceID == 0){
		vp2 = vec3(0, 0, 0);
	} else if (gl_InstanceID == 1){
		vp2 = vec3(0, 1, 0);
	} else {
		vp2 = vec3(1, 0, 0);
	} */
/* 	vp2.x = gl_InstanceID;
	vp2.y = (gl_InstanceID+1); */
	gl_Position = mvp * vec4(vertex_position + vertex_offset, 1.0);
}
