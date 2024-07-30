#ifndef AISPLINECOMPONENT_HPP
#define AISPLINECOMPONENT_HPP

#include <vector>
#include <glm/glm.hpp>
#include "EComponent.hpp"

class AISplineComponent : public EComponent {
private:
    std::vector<glm::vec3> m_vertices; // Ordered Spline Vertices
public:
    AISplineComponent(std::vector<glm::vec3> vertices);
    void tick() override;
};

#endif // AISPLINECOMPONENT_HPP
