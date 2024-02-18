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
        std::shared_ptr<ObjModel> model; // Shared geometry
        std::shared_ptr<Texture> tex0;

        glm::vec3 position;
        glm::quat rotation;

        glm::mat4 objModelMatrix; //Object's Model Matrix

    enum SimType {
        Instance,
        Character,
        Vehicle,
        Projectile,
    };

    SimulationObject(std::shared_ptr<ObjModel> modelPtr, std::shared_ptr<Texture> texPtr);

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