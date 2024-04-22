#ifndef STATICTRIANGLEMESHPHYSICS_CLASS_H
#define STATICTRIANGLEMESHPHYSICS_CLASS_H


#include <btBulletDynamicsCommon.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <glad/glad.h>

//Local Imports
#include "engine_core/singletons/PhysicsWorldSingleton.h"

class StaticTriangleMeshPhysics {
private:
    btRigidBody* terrainRigidBody;

public:
    StaticTriangleMeshPhysics(
        const std::vector<GLfloat> &vertices,
        const std::vector<GLuint> &indices,
        const glm::mat4 &modelMatrix
    );
};

#endif