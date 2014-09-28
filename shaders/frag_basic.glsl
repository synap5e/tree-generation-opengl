#version 400

in vec3 colour;
in vec3 normal;

out vec4 frag_colour;

void main() {
	vec3 light_dir = vec3(0, 0, 1);
	frag_colour = vec4 (colour * max(dot(light_dir,normal), 0.0), 1.0);
}
