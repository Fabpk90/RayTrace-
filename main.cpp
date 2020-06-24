#include <iostream>
#include "libs/glm/glm.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

#include "libs/glad/glad.c"
#include "libs/glfw/glfw3.h"
#include "shaders/Shader.hpp"
#include "Ray.hpp"
#include "Camera.hpp"

#include <vector>

const int image_width = 512;
const int image_height = 512;

const float quad[] = {
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,  1.0f, .0f,   // bottom right
        -1.0f, -1.0f, 0.0f, .0f, .0f,   // bottom left
        -1.0f,  1.0f, 0.0f, .0f, 1.0f    // top left
};

const int ebos[] =
{
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto windowHandle = glfwCreateWindow(image_width, image_height, "RayTracing Test", NULL, NULL);
    glfwMakeContextCurrent(windowHandle);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    Camera cam(glm::vec3(0.0f));
    auto horizontalScreenSize = glm::vec3(image_width, 0, 0);
    auto verticalScreenSize = glm::vec3(0, 2.0f, 0);
    auto focalLength = 1.0f;

    auto leftScreenCorner = cam.getPosition() - (horizontalScreenSize / 2.0f) - (verticalScreenSize / 2.0f)
            - glm::vec3(0, 0, focalLength);

    std::vector<glm::vec4> data(image_height * image_width * 4);

    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            float u = float(i) / (image_width-1);
            float v = float(j) / (image_height-1);

            Ray r(cam.getPosition(), leftScreenCorner + u * horizontalScreenSize + v * verticalScreenSize);

            glm::vec4 pixel(1.0f);
            data[j * image_width + i] = pixel;
        }
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5),
                          (void *) nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5),
                          (void *) (sizeof(float) * 3));

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebos), ebos, GL_STATIC_DRAW);

    GLuint texId;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_FLOAT, data.data());

    Shader shader = Shader("shaders/");
    shader.use();
    shader.setInt("tex", 0);

    while (!glfwWindowShouldClose(windowHandle))
    {
        if(glfwGetKey(windowHandle, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(windowHandle, 1);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(windowHandle);
        glfwPollEvents();
    }

    glfwDestroyWindow(windowHandle);
    glfwTerminate();
}
