//
// Created by fab on 22/06/2020.
//

#ifndef RAYTRACINGONEWEEKEND_RAY_HPP
#define RAYTRACINGONEWEEKEND_RAY_HPP


#include <glm/glm.hpp>

class Ray
{
private:
    glm::vec3 a;
    glm::vec3 position;

public:
    Ray(glm::vec3 a, glm::vec3 b) : a(a), position(b)
    {};

    glm::vec3 getA()
    {
        return a;
    }

    glm::vec3 getB()
    {
        return position;
    }

    glm::vec3 getPointAt(float t)
    {
        return a + (position * t);
    }
};


#endif //RAYTRACINGONEWEEKEND_RAY_HPP
