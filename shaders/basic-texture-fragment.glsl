#version 400

/* in vec3 geom_color;
in vec3 normal; */
//in vec4 light_dir;
in vec2 texcoord;

uniform sampler2D textureID;

out vec4 frag_colour;

void main() {
/* 	vec3 light_dir = vec3(0, 0, 1);

	vec3 amb = vec3(0.1, 0.1, 0.1);
	vec3 diff = vec3(1,1,1) * max(dot(light_dir,normal), 0.0); */

	frag_colour = /* vec4(1,0,0,1);// */texture(textureID, texcoord);
	//frag_colour = vec4(texcoord, 0, 1);
}
