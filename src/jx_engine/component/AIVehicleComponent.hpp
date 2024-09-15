#ifndef AIVEHICLECOMPONENT_HPP
#define AIVEHICLECOMPONENT_HPP

#include <chrono>

#include "jx_engine/io/GameInput.hpp"
#include "jx_engine/component/EComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class AIVehicleComponent : public EComponent
{

public:
    AIVehicleComponent();
    void tick() override;

    std::chrono::steady_clock::time_point startTime;
    bool IsTimesUp();

void performControls(
    glm::vec3 &nearestSplineVertex,
    glm::vec3 &perpendicularDirection,
    glm::vec3 &splineSegmentDirection,
    
    glm::vec3 &worldSpaceforward,
    
    glm::vec3 &objectSpaceForward,
    glm::vec3 &objectSpaceRight,

    float &turnValue

    );
};

#endif // AIVEHICLECOMPONENT_HPP