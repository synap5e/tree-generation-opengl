#version 400

in vec3 normal;
in vec3 tanjent;
in vec3 bitanjent; 
in vec4 geom_color;
in vec2 uv;
//in vec4 light_dir;

out vec4 frag_colour;

uniform sampler2D normal_texture;

void main() {
	vec3 light_dir = vec3(0, 0, 1);

	mat3 tbn = mat3(tanjent, bitanjent, normal);
	vec3 n_normal = tbn * (texture2D( normal_texture, uv).rgb*2.0 - 1.0);

	vec3 amb = vec3(0.1, 0.1, 0.1);
	vec3 diff = vec3(1,1,1) * max(dot(light_dir,n_normal), 0.0);

	frag_colour = geom_color * vec4(amb + diff, 1);
	//frag_colour = texture2D( normal_texture, uv);
	//frag_colour = vec4(N/2 + vec3(0.5), 1);
	//frag_colour = vec4(diff, 1);
	//frag_colour = vec4(0,0,0,1);
	//frag_colour = geom_color;
}
