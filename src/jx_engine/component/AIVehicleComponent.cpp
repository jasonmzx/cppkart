#include "AIVehicleComponent.hpp"

AIVehicleComponent::AIVehicleComponent() {
    startTime = std::chrono::steady_clock::now();
}

void AIVehicleComponent::tick() {}

bool AIVehicleComponent::IsTimesUp() {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedTime = currentTime - startTime;

    if (elapsedTime.count() > 1.0) {
        startTime = currentTime;
        return true;
    } else {
        return false;
    }

}

void AIVehicleComponent::performControls(
    glm::vec3 &nearestSplineVertex, 
    glm::vec3 &perpendicularDirection, 
    glm::vec3 &forward
) {
    // Direction from the vehicle (forward, is actually) to the nearest spline vertex
                glm::vec3 Alpha = glm::normalize(nearestSplineVertex - forward);

                // Calculate the dot product of Alpha with the perpendicular direction
                float dotProduct = glm::dot(Alpha, perpendicularDirection);

                if (dotProduct > 0) {
                    printf("\nTurn Right\n");
                } else if (dotProduct < 0) {
                    printf("\nTurn Left\n");
                } else {
                    printf("Move Forward\n");
                }

                printf("xz_orthogonal_dir: %f, %f, %f\n", perpendicularDirection.x, perpendicularDirection.y, perpendicularDirection.z);
                printf("nearestSplineVertex: %f, %f, %f\n", nearestSplineVertex.x, nearestSplineVertex.y, nearestSplineVertex.z);
}