//
// Created by fab on 22/06/2020.
//

#ifndef RAYTRACINGONEWEEKEND_CAMERA_HPP
#define RAYTRACINGONEWEEKEND_CAMERA_HPP


#include <glm/glm.hpp>

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 forward;
    glm::vec3 right;

public:
    Camera(glm::vec3 position) : position(position)
    {
        updateVecs();
    }

     glm::vec3 getPosition() const { return position;}

    void updateVecs();
};


#endif //RAYTRACINGONEWEEKEND_CAMERA_HPP
