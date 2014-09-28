#include "loadShader.hpp"

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

GLuint LoadShader(const char* vertex_shader_path, 
    const char* fragment_shader_path)
{
    // Create GL shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength = 0;

    bool vtxCompiled = false;
    bool fragCompiled = false;

    // Read vtx shader
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::stringstream shaderData;
        shaderData << VertexShaderStream.rdbuf();
        VertexShaderStream.close();
        VertexShaderCode = shaderData.str();

        // Compile Vtx Shader
        TRACE("Compiling shader : " << vertex_shader_path);
        char const * pVtxSrc = VertexShaderCode.c_str();
        GLint iVtxLen = VertexShaderCode.size();
        glShaderSource(VertexShaderID, 1, &pVtxSrc, &iVtxLen);
        glCompileShader(VertexShaderID);
        // Check Vertex shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 1)
        {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            TRACE("Vertex Shader errors: " << std::string(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end()));
        }
        else
        {
            vtxCompiled = true;
        }
    }

    //Read frag shader
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::stringstream shaderData;
        shaderData << FragmentShaderStream.rdbuf();
        FragmentShaderStream.close();
        FragmentShaderCode = shaderData.str();

        // Compile frag shader
        TRACE("Compiling shader : " << fragment_shader_path);
        char const * pFragSrc = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &pFragSrc, NULL);
        glCompileShader(FragmentShaderID);

        // Check frag shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 1)
        {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            TRACE("Fragment shader errors: " << std::string(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end()));
        }
        else
        {
            fragCompiled = true;
        }
    }

    GLuint ProgramID = glCreateProgram();
    if (vtxCompiled && fragCompiled)
    {
        // Link the program
        TRACE("Linking shader program.");

        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 1)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            TRACE("Program errors: " << std::string(ProgramErrorMessage.begin(), ProgramErrorMessage.end()));
        }
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
