#include "FrustumCull.h"
#include <iostream>

//https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf

void NormalizePlane(Plane & plane)
{
    float mag;
    mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;
}

float DistanceToPoint(const Plane & plane, const glm::vec3 & pt)
{
return plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d;
}

Halfspace ClassifyPoint(const Plane & plane, const glm::vec3 & pt)
{
float d;
d = plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d;
if (d < 0) return NEGATIVE;
if (d > 0) return POSITIVE;
return ON_PLANE;
}

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for glm::inverseTranspose

void ExtractPlanesGL(
    Plane* p_planes,
    const glm::mat4& comboMatrix,
    bool normalize)
{
    // Left clipping plane
    p_planes[0].a = comboMatrix[3][0] + comboMatrix[0][0];
    p_planes[0].b = comboMatrix[3][1] + comboMatrix[0][1];
    p_planes[0].c = comboMatrix[3][2] + comboMatrix[0][2];
    p_planes[0].d = comboMatrix[3][3] + comboMatrix[0][3];

    // Right clipping plane
    p_planes[1].a = comboMatrix[3][0] - comboMatrix[0][0];
    p_planes[1].b = comboMatrix[3][1] - comboMatrix[0][1];
    p_planes[1].c = comboMatrix[3][2] - comboMatrix[0][2];
    p_planes[1].d = comboMatrix[3][3] - comboMatrix[0][3];

    // Top clipping plane
    p_planes[2].a = comboMatrix[3][0] - comboMatrix[1][0];
    p_planes[2].b = comboMatrix[3][1] - comboMatrix[1][1];
    p_planes[2].c = comboMatrix[3][2] - comboMatrix[1][2];
    p_planes[2].d = comboMatrix[3][3] - comboMatrix[1][3];

    // Bottom clipping plane
    p_planes[3].a = comboMatrix[3][0] + comboMatrix[1][0];
    p_planes[3].b = comboMatrix[3][1] + comboMatrix[1][1];
    p_planes[3].c = comboMatrix[3][2] + comboMatrix[1][2];
    p_planes[3].d = comboMatrix[3][3] + comboMatrix[1][3];

    // Near clipping plane
    p_planes[4].a = comboMatrix[3][0] + comboMatrix[2][0];
    p_planes[4].b = comboMatrix[3][1] + comboMatrix[2][1];
    p_planes[4].c = comboMatrix[3][2] + comboMatrix[2][2];
    p_planes[4].d = comboMatrix[3][3] + comboMatrix[2][3];

    // Far clipping plane
    p_planes[5].a = comboMatrix[3][0] - comboMatrix[2][0];
    p_planes[5].b = comboMatrix[3][1] - comboMatrix[2][1];
    p_planes[5].c = comboMatrix[3][2] - comboMatrix[2][2];
    p_planes[5].d = comboMatrix[3][3] - comboMatrix[2][3];

    // Normalize the plane equations, if requested
    if (normalize)
    {
        NormalizePlane(p_planes[0]);
        NormalizePlane(p_planes[1]);
        NormalizePlane(p_planes[2]);
        NormalizePlane(p_planes[3]);
        NormalizePlane(p_planes[4]);
        NormalizePlane(p_planes[5]);
    }
}


void FrustumCull::Update(const glm::mat4& viewProjectionMatrix) {
    // Extract the planes from the view-projection matrix
    ExtractPlanesGL(planes, viewProjectionMatrix, true);
}

bool FrustumCull::IsBoxVisible(const glm::vec3& AABB_Point) const {
    // Check if the point is inside or partially inside the frustum

    // Initialize a count for how many planes the point is in front of
    int inFrontCount = 0;

    for (int i = 0; i < 6; i++) {
        Halfspace result = ClassifyPoint(planes[i], AABB_Point);

        // If the point is behind any plane, it is outside the frustum
        if (result == NEGATIVE) {
            return true;
        }
    }

    // If the point is in front of at least one plane, it is partially inside the frustum
    return false;
}
