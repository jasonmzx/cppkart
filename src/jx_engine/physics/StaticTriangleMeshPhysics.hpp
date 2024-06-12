#ifndef STATICTRIANGLEMESHPHYSICS_CLASS_HPP
#define STATICTRIANGLEMESHPHYSICS_CLASS_HPP

#include <bullet/btBulletDynamicsCommon.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <glad/glad.h>


class StaticTriangleMeshPhysics {
private:

public:
    btRigidBody* meshRigidBody;

    StaticTriangleMeshPhysics(const std::vector<glm::vec3> &ordered_verts, const glm::mat4 &modelMatrix, float scaleFactor);
};

#endif