#version 400

layout(lines) in;
layout(triangle_strip, max_vertices = 64) out;

in mat4 mvp[];
in mat3 normal_matrix[];
//in vec4 color;

out vec3 normal;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main() {
	vec3 bvec = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;

	float height = length(bvec);
	float pf = pow(25, height/100);
	float radius = (pf-1)/pf + 0.05;

	vec3 nbvec = normalize(bvec);
	vec3 i_unit = cross(vec3(1,0,0), nbvec) * radius;
	vec3 k_unit = cross(vec3(0,0,1), nbvec) * radius;

	int num_sides = 10;
    float PI = 3.141592653589793f;

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        vec3 offset = i_unit * cos(ang) + k_unit * -sin(ang);
        vec4 toffset = mvp[0] * vec4(offset, 0);
        normal =  normal_matrix[0] *  normalize(offset);
        gl_Position = gl_in[0].gl_Position + toffset;
        EmitVertex();

        gl_Position = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.5) + toffset;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        vec3 offset = i_unit * cos(ang) + k_unit * -sin(ang);
        vec4 toffset = mvp[0] * vec4(offset, 0);
        normal =  normal_matrix[0] *  normalize(offset);
        gl_Position = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.5) + toffset;
        EmitVertex();

        gl_Position = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.75) + toffset/2;
        EmitVertex();
    }
    EndPrimitive();

/*     normal = vec3(0,0,1);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex(); 
 
    EndPrimitive(); */
}
