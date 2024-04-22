#ifndef VEHICLEENTITY_CLASS_H
#define VEHICLEENTITY_CLASS_H

#include "engine_core/bullet_models/VehiclePhysics.h"
#include "engine_core/geometries/RenderableGeometry.h"

class VehicleEntity
{
private:
    RenderableGeometry vehicleGeometry;
    RenderableGeometry vehicleWheelGeometry;

public:
    // These functions load in Verts & Indices into Renderable Geoms to be drawn
    VehicleEntity(VAO *vaoPtr, VBO *vboPtr, EBO *eboPtr, const std::vector<GLfloat> &vehicleBodyVertices, const std::vector<GLuint> &vehicleBodyIndices,
                  VAO *vaoPtr1, VBO *vboPtr1, EBO *eboPtr1, const std::vector<GLfloat> &wheelVertices, const std::vector<GLuint> &wheelIndices);

    // Render Functions to be called every iteration
    void renderWheelGeometries(GLuint modelMatrixLocation, const std::vector<btWheelInfo>& wheelVec);

    // Getter for direct access to VehiclePhysics
    RenderableGeometry &GetGeometry();
};

#endif