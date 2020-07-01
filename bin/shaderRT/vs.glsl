#version 430
#define SPHERES_MAX 3
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_out;

uniform vec3 camPosition;
uniform vec3 leftScreenCorner;
uniform vec3 horizontal;
uniform vec3 vertical;

struct Sphere
{
    float radius;
    vec3 position;
};

struct Ray
{
    vec3 a;
    vec3 direction;
};

struct Result
{
    vec4 color;
    int hitSomething;
};

uniform Sphere spheres[SPHERES_MAX];

vec3 getPointAt(Ray r, float alpha)
{
   return r.a + (r.direction * alpha);
}

Result sphereIntersection(Ray r, Sphere sphere, float tMin, float tMax)
{
    vec3 AC = r.a - sphere.position;
    float c = dot(AC, AC) - sphere.radius * sphere.radius;
    float a = dot(r.direction, r.direction);
    float b = dot(r.direction, AC);

    float discriminant = b*b - a * c; // aka delta

    float tempAlpha;

    if(discriminant > 0)
    {
        float root = sqrt(discriminant);
        tempAlpha = (-b - root) / a;

        if(tMin < tempAlpha && tMax > tempAlpha)
        {
            vec3 surfaceHit = getPointAt(r, tempAlpha);
            vec3 normal = (surfaceHit - sphere.position) / sphere.radius;
            Result res;
            res.color = vec4(0.5f * (normal + vec3(1)), 1);
            res.hitSomething = 1;
            return res;
        }

        tempAlpha = (-b + root) / a;

        if(tMin < tempAlpha && tMax > tempAlpha)
        {
            vec3 surfaceHit = getPointAt(r, tempAlpha);
            vec3 normal = (surfaceHit - sphere.position) / sphere.radius;
            Result res;
            res.color = vec4(0.5f * (normal + vec3(1)), 1);
            res.hitSomething = 1;
            return res;
        }
    }

    vec3 direction = normalize(r.direction);
    float t = 0.5f * (direction.y + 1.0f);
    Result res;
    res.color = vec4((1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0), 1.0f);
    res.hitSomething = 0;
    return res;
}

void main()
{
    ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(img_out);

    vec2 uvNorm = vec2(uv) / size;

    Ray r;
    r.direction = leftScreenCorner + uvNorm.x * horizontal + uvNorm.y * vertical;
    r.a = camPosition;

    int stop = 0;
    Result res;
    for(int i = 0; i < SPHERES_MAX && stop == 0; ++i)
    {
        res = sphereIntersection(r, spheres[i], 0, 10);

        stop = res.hitSomething;
    }

    imageStore(img_out, uv, res.color);
}