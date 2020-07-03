//
// Created by fab on 15/06/2020.
//

#include "Sphere.hpp"

bool Sphere::isIntersecting(Ray &r, Hit& hit, float tMin, float tMax)
{
    glm::vec3 AC = r.getA() - position;
    float c = glm::dot(AC, AC) - radius * radius;
    float a = glm::dot(r.getDirection(), r.getDirection());
    float b = glm::dot(r.getDirection(), AC);

    float discriminant = b*b - a * c; // aka delta

    float tempAlpha;

    if(discriminant > 0)
    {
        float root = glm::sqrt(discriminant);
        tempAlpha = (-b - root) / a;

        if(tMin < tempAlpha && tMax > tempAlpha)
        {
            glm::vec3 surfaceHit = r.getPointAt(tempAlpha);
            hit.alpha = tempAlpha;
            hit.normal = (surfaceHit - position) / radius;
            hit.color = glm::vec4(0.5f * (hit.normal + glm::vec3(1)), 1);

            return true;
        }

        tempAlpha = (-b + root) / a;

        if(tMin < tempAlpha && tMax > tempAlpha)
        {
            glm::vec3 surfaceHit = r.getPointAt(tempAlpha);
            hit.alpha = tempAlpha;
            hit.normal = (surfaceHit - position) / radius;
            hit.color = glm::vec4(0.5f * (hit.normal + glm::vec3(1)), 1);

            return true;
        }
    }

    return IHittable::isIntersecting(r, hit, tMin, tMax);
}
