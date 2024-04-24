#ifndef STATICTRIANGLEMESHPHYSICS_CLASS_HPP
#define STATICTRIANGLEMESHPHYSICS_CLASS_HPP


#include <btBulletDynamicsCommon.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <glad/glad.h>

//Local Imports
#include "engine/physics/PhysicsWorldSingleton.h"

class StaticTriangleMeshPhysics {
private:

public:
    btRigidBody* meshRigidBody;

    StaticTriangleMeshPhysics(
        const std::vector<GLfloat> &vertices,
        const std::vector<GLuint> &indices,
        const glm::mat4 &modelMatrix
    );
};

#endif