#pragma once

#include "glHeaders.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint load_shaders(const char* vertex_shader_path, const char* fragment_shader_path);
GLuint create_shader(GLenum type, const char* filename);

class Shader {
private:
    GLuint model_location;
    GLuint view_location;
    GLuint projection_location;

  //  GLuint use_

protected:
    void load_locations();
    GLuint shader_id;

public:
    Shader(){};
    void virtual load() = 0;
    void activate();

    void set_projection(glm::mat4& mat);
	void set_model(glm::mat4& mat);
	void set_view(glm::mat4& mat);

};

class BranchShader : public Shader {

public:
    void load();
};

class AttractionPointShader : public Shader {

public:
    void load();
};
