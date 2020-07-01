//
// Created by fab on 15/06/2020.
//

#ifndef RAYTRACINGONEWEEKEND_SPHERE_HPP
#define RAYTRACINGONEWEEKEND_SPHERE_HPP


#include "IHittable.hpp"
#include "Vbo.hpp"

class Sphere : public IHittable
{
private:
    float radius;
    glm::vec3 position;
public:
    Sphere(float radius, glm::vec3 position) : radius(radius), position(position) {};
    bool isIntersecting(Ray &r, Hit &hit, float tMin, float tMax) override;

    void move(glm::vec3 delta) {  position += delta; }
    glm::vec3 getPosition() { return position; }
    float getRadius() { return radius;}
};


#endif //RAYTRACINGONEWEEKEND_SPHERE_HPP
