#ifndef AISPLINECOMPONENT_HPP
#define AISPLINECOMPONENT_HPP


#include <vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "EComponent.hpp"


#include "jx_engine/import/PhysChunkedMapLoader.hpp"

class AISplineComponent : public EComponent {
    public:
        AISplineComponent(float scale);
        std::vector<glm::vec3> spline_verts;
         
        void getNearestVertexFromPos(glm::vec3 position, glm::vec3 &nearestVertex, glm::vec3 &nextNearestVertex);
        void tick() override;
};

#endif // AISPLINECOMPONENT_HPP