#include "leafmodel.hpp"
#include "random.hpp"

typedef struct Triangle{
	vec3 v1, v2, v3;
} Triangle;

vec3 get_normal(std::vector<Triangle> &tris){
	vec3 total;
	for (Triangle &tri : tris){
		vec3 side1 = normalize(tri.v2 - tri.v1);
		vec3 side2 = normalize(tri.v3 - tri.v1);

		vec3 normal = normalize(cross(side1, side2));

		total += normal;
	}
	return normalize(total / float(tris.size()))	;
}

LeafModel::LeafModel(){
	std::vector<vec3> triangle_points;
	std::vector<vec3> triangle_normals;

	std::vector<vec3> colors;

	std::map<int, std::map<int, vec3>> points;

	for (int x=0; x<=x_steps; ++x){
		float u = ((PI * 2.f) / float(x_steps)) * x;

		for (int z=0; z<=z_steps; ++z){
			float v = (PI / float(z_steps)) * z;

			points[x][z] = vec3(/* x = */ (1.f-cosf(u)) * sinf(u) * cosf(v),
							   	/* y = */ 2.5f * (1.f - cosf(u)),
								/* z = */ 0.15f * sinf(u) * sinf(v)) * scale;
		}
	}

	Triangle tri;
	std::map<int, std::map<int, std::vector<Triangle>>> tris;

	for (int x=0; x<x_steps; ++x){
		for (int z=0; z<z_steps; ++z){
			if (points[x][z].z >= 0){
				// wind one way
				tri.v1 = points[x][z];
				tri.v2 = points[x+1][z];
				tri.v3 = points[x][z+1];
				tris[x][z].push_back(tri);
				tris[x+1][z].push_back(tri);
				tris[x][z+1].push_back(tri);

				tri.v1 = points[x][z+1];
				tri.v2 = points[x+1][z];
				tri.v3 = points[x+1][z+1];
				tris[x][z+1].push_back(tri);
				tris[x+1][z].push_back(tri);
				tris[x+1][z+1].push_back(tri);
				
			}

			if (points[x][z].z <= 0){
				// wind the other

				tri.v1 = points[x][z+1];
				tri.v2 = points[x+1][z];
				tri.v3 = points[x][z];
				tris[x][z+1].push_back(tri);
				tris[x+1][z].push_back(tri);
				tris[x][z].push_back(tri);
								
				tri.v1 = points[x+1][z+1];
				tri.v2 = points[x+1][z];
				tri.v3 = points[x][z+1];
				tris[x+1][z+1].push_back(tri);
				tris[x+1][z].push_back(tri);
				tris[x][z+1].push_back(tri);
				
			}
		}
	}

	vec3 leaf_color = vec3(0.f, 0.6f, 0.f);
	leaf_color.x = RandomGen::get(0, 0.3);
	leaf_color.y += RandomGen::get(-0.1, 0.1);
	leaf_color.z = RandomGen::get(0, 0.2);


	for (int x=0; x<x_steps; ++x){
		for (int z=0; z<z_steps; ++z){
			if (points[x][z].z >= 0){
				triangle_points.push_back(points[x][z]);
				triangle_normals.push_back(get_normal(tris[x][z]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(get_normal(tris[x+1][z]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(get_normal(tris[x][z+1]));
				colors.push_back(leaf_color);

				

				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(get_normal(tris[x][z+1]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(get_normal(tris[x+1][z]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x+1][z+1]);
				triangle_normals.push_back(get_normal(tris[x+1][z+1]));
				colors.push_back(leaf_color);

			}
			if (points[x][z].z <= 0){
				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(get_normal(tris[x][z+1]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(get_normal(tris[x+1][z]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x][z]);
				triangle_normals.push_back(get_normal(tris[x][z]));
				colors.push_back(leaf_color);


								
				triangle_points.push_back(points[x+1][z+1]);
				triangle_normals.push_back(get_normal(tris[x+1][z+1]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x+1][z]);
				triangle_normals.push_back(get_normal(tris[x+1][z]));
				colors.push_back(leaf_color);

				triangle_points.push_back(points[x][z+1]);
				triangle_normals.push_back(get_normal(tris[x][z+1]));
				colors.push_back(leaf_color);

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
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glEnableVertexAttribArray (2);
	glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
