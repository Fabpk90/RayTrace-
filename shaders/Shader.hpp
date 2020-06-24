//
// Created by fab on 23/02/2020.
//

#ifndef DUMBERENGINE_SHADER_HPP
#define DUMBERENGINE_SHADER_HPP

#include <string>
#include <glad/glad.h>
#include <list>
#include "glm/glm.hpp"

class Shader
{
public:
    Shader(const char *path);

    void reload();

    void use();

    void setInt(const std::string &name, int i);

    void setFloat(const std::string &name, float f1);

    void setFloat2(const std::string &name, float f1, float f2);

    void setFloat3(const std::string &name, float f1, float f2, float f3);

    void setFloat4(const std::string &name, float f1, float f2, float f3, float f4);

    void setMatrix4(const std::string &name, glm::mat4x4 &m);

    void setVec3(const char *name, glm::vec3& vec);

    void setVec4(const char *string, glm::vec4 &vec);

    static void reloadShaders();

    void setVec2(const char *name, glm::vec2 val);

    GLuint getID() { return shaderProgram; }

private:
    void load();

    static std::list<Shader*> shadersLoaded;

    const char *path;
    GLuint shaderProgram;
};

#endif //DUMBERENGINE_SHADER_HPP
