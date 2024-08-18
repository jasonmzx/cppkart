#include "AISplineComponent.hpp"

AISplineComponent::AISplineComponent(float scale) {
    spline_verts = PhysChunkedMapLoader::loadAISpline("../assets/square_island/ai_splines.txt", scale);
}

void AISplineComponent::getNearestVertexFromPos(glm::vec3 position, glm::vec3 &nearestVertex, glm::vec3 &nextNearestVertex) {
    
    int nearestIndex = 0;
    float minDist = 1000000.0f;
    
    for (int i = 0; i < spline_verts.size(); i++) {
        float dist = glm::distance(position, spline_verts[i]);
        if (dist < minDist) {
            
            minDist =     dist;
            nearestIndex = i;

            nearestVertex = spline_verts[i];
            nextNearestVertex = spline_verts[(i + 1) % spline_verts.size()];
        }
    }
}

void AISplineComponent::tick(){
    //DO NOTHING
}