#include <iostream>
#include "libs/glm/glm.hpp"

#include "glad/glad.c"
#include "libs/glfw/glfw3.h"
#include "shaders/Shader.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"

#include <vector>
#include <thread>

#include <fstream>
#include <sstream>
#include <iostream>


const int image_width = 1280;
const int image_height = 720;

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

std::vector<Sphere*> spheres;
glm::vec4* data;

void pathTracing(int x, int y, int amountX, int amountY, glm::vec3& leftScreenCorner, glm::vec3& horizontal, glm::vec3& vertical,
        Camera& cam)
{

    for (int j = y; j <  y + amountY; ++j)
    {
        for (int i = x; i <  x + amountX; ++i)
        {
            float u = float(i) / (image_width-1);
            float v = float(j) / (image_height-1);

            Ray r(cam.getPosition(), leftScreenCorner + u * horizontal + v * vertical);
            Hit hit;

            bool found = false;
            for (int k = 0; k < spheres.size() && !found; ++k)
            {
                found = spheres[k]->isIntersecting(r, hit, 0, 10);
            }

            data[j * image_width + i] = hit.color;
        }
    }
}

int main() {

    data = new glm::vec4[image_height * image_width];
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto windowHandle = glfwCreateWindow(image_width, image_height, "RayTracing Test", nullptr, nullptr);
    glfwMakeContextCurrent(windowHandle);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    float aspectRatio = 16.0f / 9.0f;
    float viewportHeight = 2.0f;
    float viewportWidth = aspectRatio * viewportHeight;

    Camera cam(glm::vec3(0.0f));
    auto focalLength = 1.0f;
    auto horizontal = glm::vec3(viewportWidth, 0, 0);
    auto vertical = glm::vec3(0, viewportHeight, 0);

    auto leftScreenCorner = cam.getPosition() - (horizontal / 2.0f) - (vertical / 2.0f)
            - glm::vec3(0, 0, focalLength);


    spheres.push_back(new Sphere(1.0f, glm::vec3(1, 0, -5.0f)));
    spheres.push_back(new Sphere(1.0f, glm::vec3(-1, 0, -5.0f)));
    spheres.push_back(new Sphere(1.5f, glm::vec3(0, 0, -2.5f)));


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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image_width, image_height, 0, GL_RGBA, GL_FLOAT, data);

    glBindImageTexture(0, texId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    printf("max global (total) work group counts x:%i y:%i z:%i\n",
           work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
           work_grp_size[0], work_grp_size[1], work_grp_size[2]);


    Shader shaderQuad = Shader("shaders/");
    shaderQuad.use();
    shaderQuad.setInt("tex", 0);

    double previousTime = glfwGetTime();
    int frameCount = 0;
    char* fpsCount = new char[64];

    auto* threads = new std::thread[4];

    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

    std::stringstream buf;
    std::ifstream istream("shaderRT/vs.glsl");

    buf << istream.rdbuf();
    auto str = buf.str();
    const char* computeCode = str.c_str();
    glShaderSource(shader, 1, &computeCode, nullptr);
    glCompileShader(shader);

    int compiledShader = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledShader);

    if (!compiledShader)
    {
        char infoLog[256];
        glGetShaderInfoLog(shader, 256, NULL, infoLog);
        std::cout << infoLog << compiledShader << std::endl;
    }

    GLuint ray_program = glCreateProgram();
    glAttachShader(ray_program, shader);
    glLinkProgram(ray_program);

    glUseProgram(ray_program);

    while (!glfwWindowShouldClose(windowHandle))
    {
        if(glfwGetKey(windowHandle, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(windowHandle, 1);

        if(glfwGetKey(windowHandle, GLFW_KEY_LEFT))
        {
            spheres[0]->move(glm::vec3(-1, 0, 0) * 1.0f/ 60.0f);
        }
        else if(glfwGetKey(windowHandle, GLFW_KEY_RIGHT))
        {
            spheres[0]->move(glm::vec3(1, 0, 0) * 1.0f/ 60.0f);
        }

        int t = 0;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                threads[t++] = std::thread(pathTracing,
                                         i * (image_width / 2), j * (image_height / 2), (image_width / 2), (image_height / 2),
                                         leftScreenCorner, horizontal, vertical, cam);
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            threads[i].join();
        }

       /* glUseProgram(ray_program);

        GLint location = glGetUniformLocation(ray_program, "camPosition");
        glUniform3f(location, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);

        location = glGetUniformLocation(ray_program, "leftScreenCorner");
        glUniform3f(location, leftScreenCorner.x, leftScreenCorner.y, leftScreenCorner.z);

        location = glGetUniformLocation(ray_program, "horizontal");
        glUniform3f(location, horizontal.x, horizontal.y, horizontal.z);

        location = glGetUniformLocation(ray_program, "vertical");
        glUniform3f(location, vertical.x, vertical.y, vertical.z);

        for (int j = 0; j < spheres.size(); ++j)
        {
            //this is garbage cache this
            std::string s = "spheres[";
            s += std::to_string(j);
            s += "].radius";

            location = glGetUniformLocation(ray_program, s.c_str());
            glUniform1f(location, spheres[j]->getRadius());

            s = "spheres[";
            s += std::to_string(j);
            s += "].position";

            location = glGetUniformLocation(ray_program, s.c_str());
            glUniform3f(location, spheres[j]->getPosition().x, spheres[j]->getPosition().y, spheres[j]->getPosition().z);
        }



        glDispatchCompute((GLuint)image_width, (GLuint)image_height, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);*/


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image_width, image_height, 0, GL_RGBA, GL_FLOAT, data);

        glClear(GL_COLOR_BUFFER_BIT);

        shaderQuad.use();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(windowHandle);
        glfwPollEvents();

        double currentTime = glfwGetTime();
        frameCount++;
        // If a second has passed.
        if ( currentTime - previousTime >= 1.0 )
        {
            sprintf(fpsCount, "%d", frameCount);
            glfwSetWindowTitle(windowHandle, fpsCount);

            frameCount = 0;
            previousTime = currentTime;
        }

    }

    glfwDestroyWindow(windowHandle);
    glfwTerminate();
}
