#include "AIVehicleComponent.hpp"

#include <cmath>

AIVehicleComponent::AIVehicleComponent()
{
    startTime = std::chrono::steady_clock::now();
}

void AIVehicleComponent::tick() {}

bool AIVehicleComponent::IsTimesUp()
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedTime = currentTime - startTime;

    if (elapsedTime.count() > 1.0)
    {
        startTime = currentTime;
        return true;
    }
    else
    {
        return false;
    }
}

void AIVehicleComponent::performControls(
    glm::vec3 &nearestSplineVertex,
    glm::vec3 &perpendicularDirection,
    glm::vec3 &splineSegmentDirection,
    
    glm::vec3 &worldSpaceforward,

    glm::vec3 &objectSpaceForward,
    glm::vec3 &objectSpaceRight,

    float &turnValue

    )
{
    // Direction from the vehicle (forward, is actually) to the nearest spline vertex
    glm::vec3 alpha = nearestSplineVertex - worldSpaceforward;

    // Calculate the dot product of Alpha with the perpendicular direction
    float rightDot = glm::dot(alpha, objectSpaceRight);

    // if(rightDot > 0) left
    // if(rightDot < 0) right

    // Clamp rightdot from 0.03 to 0.0075
    turnValue = 0.04f - std::pow(2, -std::abs(rightDot));

    if(rightDot < 0) {
        turnValue = -turnValue;
    }

    printf(" >>>> TURN VALUE : %f\n", turnValue);
    printf(" >>>> Alpha Length: %f\n", glm::length(alpha));


    printf("xz_orthogonal_dir: %f, %f, %f\n", perpendicularDirection.x, perpendicularDirection.y, perpendicularDirection.z);
    printf("nearestSplineVertex: %f, %f, %f\n", nearestSplineVertex.x, nearestSplineVertex.y, nearestSplineVertex.z);
}