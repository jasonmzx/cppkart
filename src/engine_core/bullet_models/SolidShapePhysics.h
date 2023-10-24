#ifndef SOLIDSHAPEPHYSICS_CLASS_H
#define SOLIDSHAPEPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include<glad/glad.h>


class SolidShapePhysics
{
private:
    btRigidBody *rigidBody;
    btCollisionShape *collisionShape;
    btDefaultMotionState *motionState;

public:
    SolidShapePhysics(
        const std::vector<GLfloat> &vertices,
        const std::vector<GLuint> &indices,
        const glm::mat4 &modelMatrix);
    ~SolidShapePhysics();

    btRigidBody *getRigidBody() const; // Add const here to match the definition
};

#endif
