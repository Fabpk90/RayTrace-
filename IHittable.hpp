//
// Created by fab on 15/06/2020.
//

#include "libs/glm/glm.hpp"

#ifndef RAYTRACINGONEWEEKEND_IHITTABLE_HPP
class IHittable
{
public:
    bool virtual isIntersecting(glm::vec3 worldPos, glm::vec4& outColor) = 0;
    virtual ~IHittable() = default;
};
#define RAYTRACINGONEWEEKEND_IHITTABLE_HPP

#endif //RAYTRACINGONEWEEKEND_IHITTABLE_HPP
