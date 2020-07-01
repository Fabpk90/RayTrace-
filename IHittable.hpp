//
// Created by fab on 15/06/2020.
//

#include "libs/glm/glm.hpp"
#include "Ray.hpp"

#ifndef RAYTRACINGONEWEEKEND_IHITTABLE_HPP
#define RAYTRACINGONEWEEKEND_IHITTABLE_HPP

struct Hit
{
    glm::vec4 color;
    glm::vec3 normal;
    float alpha;
    glm::vec3 point;
};

class IHittable
{
public:
    virtual bool isIntersecting(Ray& r, Hit& hit, float tMin, float tMax)
    {
        glm::vec3 direction = glm::normalize(r.getDirection());
        float t = 0.5f * (direction.y + 1.0f);
        hit.color = glm::vec4((1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0)
                , 1.0f);
        return false;
    }

    virtual ~IHittable() = default;
};

#endif //RAYTRACINGONEWEEKEND_IHITTABLE_HPP
