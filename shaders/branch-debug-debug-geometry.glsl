#version 400

const int num_sides = 40;
const float PI = 3.141592653589793f;

layout(lines_adjacency) in;
//layout(line_strip, max_vertices = 64) out;
layout(triangle_strip, max_vertices = 146) out;

in mat4 mvp[];
in mat3 normal_matrix[];
in float size[];
in vec4 color[];

out vec3 normal;
out vec3 geom_color;
out vec4 light_dir;

void main() {
    light_dir = vec4(normal_matrix[0] * vec3(0,0,1), 0);

	vec3 prev_bvec = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
	vec3 prev_nbvec = normalize(prev_bvec);

	vec3 prev_i_unit = normalize(cross(vec3(1,0,0), prev_nbvec));
	vec3 prev_k_unit = normalize(cross(prev_i_unit, prev_nbvec));

    /* if (size[2] < 0.15){
        geom_color = vec3(0.2,0.9,0.4);

        vec4 pos = mvp[0] * gl_in[2].gl_Position;
        float r = 20;

        for (int i = 0; i <= 10; i++) {
            float ang = PI * 2.0 / 10.0 * i;

            vec4 offset = vec4(cos(ang), -sin(ang), 0, 0);
            normal = mix(vec3(0,0,1), offset.xyz, 0.8);
            gl_Position = pos + offset * 1;
            EmitVertex();

            normal = vec3(0,0,1);
            gl_Position = pos;
            EmitVertex();
        }

        EndPrimitive();
    } */

    geom_color = vec3(0.6,0.3,0);

    vec3 c_bvec = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
    vec3 c_nbvec = normalize(c_bvec);

    vec3 c_i_unit = normalize(cross(vec3(1,0,0), c_nbvec));
    vec3 c_k_unit = normalize(cross(vec3(0,0,1), c_nbvec));

    vec3 start = gl_in[1].gl_Position.xyz;
    vec3 end = gl_in[2].gl_Position.xyz;
    

  /*   if (end.x > 0 || end.y < 100 || size[2] > 0.52 || end.z < -100 || size[2] < 0.50){
        return;
    } 
 */

/*     for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        float cs = cos(ang);
        float sn = -sin(ang);

        vec3 offset = c_i_unit * cs + c_k_unit * sn;
        //normal = -(mvp[2] * vec4(normalize(offset), 0)).xyz;
        normal = normal_matrix[2] * normalize(offset);
        gl_Position = mvp[2] * vec4(end + offset * size[2], 1);
        EmitVertex();

    } */
    //EndPrimitive();
    /* normal = vec3(1);

    geom_color = vec3(1,0,0);
    gl_Position = mvp[1] * vec4(start, 1) + mvp[1] * vec4(prev_i_unit * size[1], 0);
    EmitVertex();
    gl_Position = mvp[1] * vec4(start, 1);
    EmitVertex();
    EndPrimitive(); 

    geom_color = vec3(0,0,1);
    gl_Position = mvp[1] * vec4(start, 1) + mvp[1] * vec4(prev_k_unit * size[1], 0);
    EmitVertex();
    gl_Position = mvp[1] * vec4(start, 1);
    EmitVertex();
    EndPrimitive(); 


    geom_color = vec3(0,1,0);
    gl_Position = mvp[1] * vec4(start, 1) + mvp[1] * vec4(prev_k_unit * size[1], 0) + mvp[1] * vec4(prev_i_unit * size[1], 0);
    EmitVertex();
    gl_Position = mvp[1] * vec4(start, 1);
    EmitVertex();
    EndPrimitive(); 


  //  return;
   // return;
   geom_color = vec3(0,0,0);
    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        float cs = cos(ang);
        float sn = -sin(ang);
        vec3 offset = prev_i_unit * cs + prev_k_unit * sn;
        //normal = -(mvp[1] * vec4(normalize(offset), 0)).xyz;
        normal = normal_matrix[1] * normalize(offset);
        gl_Position = mvp[1] * vec4(start + offset * size[1], 1);
        EmitVertex();
    }
    EndPrimitive(); 
    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        float cs = cos(ang);
        float sn = -sin(ang);
        vec3 offset = prev_i_unit * cs + prev_k_unit * sn;
//        gl_Position = mvp[1] * vec4(start + offset * size[1], 1);
        gl_Position = mvp[1] * vec4(start, 1) + mvp[1] * vec4(offset * size[1], 0);
        EmitVertex();

        gl_Position = mvp[1] * vec4(start, 1);
        EmitVertex();
        EndPrimitive(); 
    }
    

    return;
     */
    for (int i = 0; i <= num_sides; i++) {
        float ang = PI * 2.0 / num_sides * i;

        float cs = cos(ang);
        float sn = sin(ang);

        vec3 offset = c_i_unit * cs + c_k_unit * sn;
        //normal = -(mvp[2] * vec4(normalize(offset), 0)).xyz;
        normal = normal_matrix[2] * normalize(offset);
        gl_Position = mvp[2] * vec4(end, 1) + mvp[2] * vec4(offset * size[2], 0);
        EmitVertex();

        vec4 oof = vec4(offset * size[2], 0);
       /*  if (length(oof) < 0.3 && length(oof) < 0.35){
            return;
        } */

        offset = prev_i_unit * cs + prev_k_unit * sn;
        //normal = -(mvp[1] * vec4(normalize(offset), 0)).xyz;
        normal = normal_matrix[1] * normalize(offset);
        gl_Position = mvp[1] * vec4(start + offset * size[2], 1);
        EmitVertex();

        vec4 nof = vec4(offset * size[2], 0);

       /*  if (length(oof) != length(nof)){
            geom_color = vec3(1,0,0);
        } */
    }
    EndPrimitive();
 
}
