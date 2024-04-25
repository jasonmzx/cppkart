#ifndef SIMULATIONOBJECT_HPP
#define SIMULATIONOBJECT_HPP

#include "core/loaders/ObjModel.hpp"
#include "core/gl/Texture.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include <memory>

class SimulationObject {

    public:
        std::string modelPath;
        std::string texPath;

        glm::vec3 position;
        glm::quat rotation;

        glm::mat4 objModelMatrix; //Object's Model Matrix

    enum SimType {
        Instance,
        Character,
        Vehicle,
        Projectile,
    };

        SimType type;

    SimulationObject(std::string objPath, std::string texturePath);

   // virtual void setPosition(const glm::vec3& pos);

    const glm::vec3& getPosition() const {
        return position;
    }

    const glm::quat& getRotation() const {
        return rotation;
    }

   // virtual void setRotation(const glm::quat& orientation);
};

#endif