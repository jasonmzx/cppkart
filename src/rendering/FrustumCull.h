#ifndef FRUSTUMCULL_CLASS_H
#define FRUSTUMCULL_CLASS_H

#include<glm/glm.hpp>


struct Plane
{
float a, b, c, d;
};


enum Halfspace
{
NEGATIVE = -1,
ON_PLANE = 0,
POSITIVE = 1,
};

void NormalizePlane(Plane & plane);
float DistanceToPoint(const Plane & plane, const glm::vec3 & pt);
Halfspace ClassifyPoint(const Plane & plane, const glm::vec3 & pt);
void ExtractPlanesGL(Plane* p_planes, const glm::mat4& comboMatrix, bool normalize);

class FrustumCull {
public:

private:
    Plane planes[6];

public:
    void Update(const glm::mat4& mat); // Extract frustum planes from a view-projection matrix
    bool IsBoxVisible(const glm::vec3& AABB_Point) const; // Check if a bounding box is within the frustum
};

#endif