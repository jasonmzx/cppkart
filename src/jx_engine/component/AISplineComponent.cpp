#include "AISplineComponent.hpp"

AISplineComponent::AISplineComponent(float scale) {
    spline_verts = PhysChunkedMapLoader::loadAISpline("../assets/square_island/ai_splines.txt", scale);
}

void AISplineComponent::getNearestVertexFromPos(glm::vec3 position, glm::vec3 &nearestVertex) {
    float minDist = 1000000.0f;
    for (int i = 0; i < spline_verts.size(); i++) {
        float dist = glm::distance(position, spline_verts[i]);
        if (dist < minDist) {
            minDist =     dist;
            nearestVertex = spline_verts[i];
        }
    }
}

void AISplineComponent::tick(){
    //DO NOTHING
}