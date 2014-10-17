#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;
layout(location = 2) in vec3 vertex_normal;

layout(location = 3) in vec3 model_offset;
layout(location = 4) in mat3 model_rotation;
// 5
// 6
layout(location=7) in float model_scale;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 geom_color;
out vec3 normal;

/* mat3 model_rotation(vec3 axis, float angle){
    //axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
} */

void main() {
	mat4 mv = view * model;

	mat3 normal_matrix = mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) );
	mat4 mvp = projection * mv;
/* 	mat3 rotation = rotation(leaf_axis, leaf_rotation); */

	normal = normal_matrix * model_rotation * vertex_normal;
	geom_color = vertex_colour;

	gl_Position = mvp * vec4(model_rotation * (vertex_position * model_scale) + model_offset, 1.0);
}
