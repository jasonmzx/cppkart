#ifndef FRUSTUMCULL_CLASS_H
#define FRUSTUMCULL_CLASS_H

#include<glm/glm.hpp>

class FrustumCull {
public:
    enum Plane {
        TOP = 0, BOTTOM, LEFT, RIGHT, NEARP, FARP
    };

private:
    glm::vec4 planes[6];

public:
    void Update(const glm::mat4& mat); // Extract frustum planes from a view-projection matrix
    bool IsBoxVisible(const glm::vec3& min, const glm::vec3& max) const; // Check if a bounding box is within the frustum
};

#endif