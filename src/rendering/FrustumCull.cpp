#include "FrustumCull.h"
#include <iostream>

void FrustumCull::Update(const glm::mat4& mat) {

    // Extract the planes from the matrix
    planes[NEARP] = glm::vec4(mat[3] + mat[2]);
    planes[FARP]  = glm::vec4(mat[3] - mat[2]);
    planes[BOTTOM]= glm::vec4(mat[3] + mat[1]);
    planes[TOP]   = glm::vec4(mat[3] - mat[1]);
    planes[LEFT]  = glm::vec4(mat[3] + mat[0]);
    planes[RIGHT] = glm::vec4(mat[3] - mat[0]);

    // Normalize the plane equations
    for(int i = 0; i < 6; i++) {
        float length = glm::length(glm::vec3(planes[i]));
        planes[i] /= length;
    }
}

bool FrustumCull::IsBoxVisible(const glm::vec3& min, const glm::vec3& max) const {
const float epsilon = 0.001f; // Small epsilon value

for(int i = 0; i < 6; i++) {
    glm::vec3 positiveVertex = min;

    if(planes[i].x >= 0)
        positiveVertex.x = max.x;
    if(planes[i].y >= 0)
        positiveVertex.y = max.y;
    if(planes[i].z >= 0)
        positiveVertex.z = max.z;

    if(glm::dot(glm::vec3(planes[i]), positiveVertex) + planes[i].w < -epsilon) {
        return false; // Outside the frustum (with epsilon)!
    }
}
return true; // Box is inside the frustum

}