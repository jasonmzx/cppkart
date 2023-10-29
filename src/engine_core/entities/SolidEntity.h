#ifndef SOLIDENTITY_CLASS_H
#define SOLIDENTITY_CLASS_H

#include "engine_core/bullet_models/StaticTriangleMeshPhysics.h"
#include "engine_core/geometries/RenderableGeometry.h"


class SolidEntity {
private:
    StaticTriangleMeshPhysics shapePhysics;

public:
    RenderableGeometry geom;
    SolidEntity(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, glm::mat4& modelMatrix);
};

#endif