#include "leafmodel.hpp"

LeafModel::LeafModel(){
	std::vector<vec3> triangle_points;
	std::vector<vec3> triangle_normals;

	std::vector<vec3> colors;

/*	triangle_points.push_back(vec3(0,	0.5,	0));
	triangle_points.push_back(vec3(-0.5,	-0.5,	0));
	triangle_points.push_back(vec3(0.5,	-0.5,	0));
	
	colors.push_back(vec3(1.0f, 0.0f,  0.0f));
	colors.push_back(vec3(0.0f, 1.0f,  0.0f));
	colors.push_back(vec3(0.0f, 0.0f,  1.0f));*/

	float PI = 3.141592653589793f;

	float ystep = 0.1;

	int i = 75;
	std::map<int, std::map<int, vec3>> points;

	int x_steps = 20;
	int z_steps = 2;

	for (int x=0; x<=x_steps; ++x){
		float u = ((PI * 2.f) / float(x_steps)) * x;

		for (int z=0; z<=z_steps; ++z){
			float v = (PI / float(z_steps)) * z;

			points[x][z] = vec3(/* x = */ (1.f-cosf(u)) * sinf(u) * cosf(v),
							   	/* y = */ 2.5f * (1.f - cosf(u)),
								/* z = */ 0.15f * sinf(u) * sinf(v));
		}
	}

	for (int x=0; x<x_steps; ++x){
		for (int z=0; z<z_steps; ++z){

			if (points[x][z].z >= 0){
				// wind one way
				triangle_points.push_back(points[x][z]);
				triangle_normals.push_back(normalize(points[x][z]));

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(normalize(points[x+1][z]));

				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(normalize(points[x][z+1]));


				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(normalize(points[x][z+1]));

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(normalize(points[x+1][z]));

				triangle_points.push_back(points[x+1][z+1]);
				triangle_normals.push_back(normalize(points[x+1][z+1]));


				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));

				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));

			}

			if (points[x][z].z <= 0){
				// wind the other

				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(normalize(points[x][z+1]));

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(normalize(points[x+1][z]));

				triangle_points.push_back(points[x][z]);
				triangle_normals.push_back(normalize(points[x][z]));


				triangle_points.push_back(points[x+1][z+1]);
				triangle_normals.push_back(normalize(points[x+1][z+1]));

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(normalize(points[x+1][z]));

				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(normalize(points[x][z+1]));



				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));

				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));
				colors.push_back(vec3(0.f, 0.8f, 0.f));
			}
		}
	}

	/*float yu = RandomGen::get(0, 1);
        
    float theta = acosf(1-2*yu);
    float phi = RandomGen::get(0, PI);

    float xrad = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi);
    float zrad = 0.5 * (1-cosf(theta)) * sinf(theta) * sinf(phi);*/





	
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, triangle_points.size() * sizeof(vec3), &triangle_points[0], GL_STATIC_DRAW);
	
	glGenBuffers (1, &colours_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glBufferData (GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), &colors[0], GL_STATIC_DRAW);

	glGenBuffers (1, &normals_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glBufferData (GL_ARRAY_BUFFER, triangle_normals.size() * sizeof(vec3), &triangle_normals[0], GL_STATIC_DRAW);
	
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	size = triangle_points.size();
}

void LeafModel::bind(){
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    glEnableVertexAttribArray (2);
}
