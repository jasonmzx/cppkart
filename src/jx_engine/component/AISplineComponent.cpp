#include "AISplineComponent.hpp"

AISplineComponent::AISplineComponent(float scale) {
    spline_verts = PhysChunkedMapLoader::loadAISpline("../assets/square_island/ai_splines.txt", scale);

    // Build the debug geometry
    GameGLRenderer* renderer = GameGLRenderer::getInstance();

    useTextureLOC = renderer->useTextureLOC;
    modelMatrixLOC = renderer->modelMatrixLOC;
    colorUniformLOC = renderer->colorUniformLocation;

    std::vector<GLfloat> base_vertices = renderer->GetConeVertices(20, 6);
    std::vector<GLuint> base_indices = renderer->GetConeIndices(20);

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    int base_vertex_count = base_vertices.size() / 11; // each vertex has 11 attributes (position, color, texture, normal)

    // Iterate through spline vertices
    for (size_t i = 0; i < spline_verts.size(); ++i) {
        
        glm::vec3 position = spline_verts[i];

        // Compute direction vector
        glm::vec3 direction;
        if (i < spline_verts.size() - 1) {
            // Calculate direction to the next spline vertex
            direction = glm::normalize(spline_verts[i + 1] - position);
           } else {
            // For the last point, use the direction from the previous vertex
            direction = glm::normalize(position - spline_verts[i - 1]);
        }

        // Flip direction
        direction = -direction;

        // Calculate rotation matrix to align the cone with the direction vector (Cone Initially Aligned with +Z axis)
        glm::vec3 init_direction = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::quat rotationQuat = glm::rotation(init_direction, direction); // Calculate quaternion to rotate the up vector to the direction vector
        glm::mat4 rotationMatrix = glm::toMat4(rotationQuat); // quat to mat4

        // Add the first cone (aligned with the direction vector)
        for (int j = 0; j < base_vertex_count; ++j) {
            // Apply rotation to the position
            glm::vec4 rotatedPos = rotationMatrix * glm::vec4(
                base_vertices[j * 11 + 0],
                base_vertices[j * 11 + 1],
                base_vertices[j * 11 + 2],
                1.0f
            );

            // Translate the rotated position
            GLfloat x = rotatedPos.x + position.x;
            GLfloat y = rotatedPos.y + position.y;
            GLfloat z = rotatedPos.z + position.z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Copy the rest of the un-touched attributes (color, texture coordinates, normals)
            for (int k = 3; k < 11; ++k) {
                vertices.push_back(base_vertices[j * 11 + k]);
            }
        }

        // Adjust and add indices
        GLuint index_offset = i * base_vertex_count * 2; // * 2 because we'll have two cones per vertex
        for (size_t j = 0; j < base_indices.size(); ++j) {
            indices.push_back(base_indices[j] + index_offset);
        }

        //* Up direction crossed with the direction vector, gives: perpendicular to the direction vector, for vehicle orientation on the spline
        glm::vec3 plus_y_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 perpendicular_dir = glm::normalize(glm::cross(direction, plus_y_up));  // Perpendicular vector

        // Calculate the rotation matrix for the second cone
        glm::quat perpendicularQuat = glm::rotation(init_direction, perpendicular_dir);
        glm::mat4 perpendicularRotationMatrix = glm::toMat4(perpendicularQuat);

        // Add the second cone (aligned with the perpendicular direction)
        for (int j = 0; j < base_vertex_count; ++j) {
            // Apply rotation to the position
            glm::vec4 rotatedPos = perpendicularRotationMatrix * glm::vec4(
                base_vertices[j * 11 + 0],
                base_vertices[j * 11 + 1],
                base_vertices[j * 11 + 2],
                1.0f
            );

            // Translate the rotated position
            GLfloat x = rotatedPos.x + position.x;
            GLfloat y = rotatedPos.y + position.y;
            GLfloat z = rotatedPos.z + position.z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Copy the rest of the un-touched attributes (color, texture coordinates, normals)
            for (int k = 3; k < 11; ++k) {
                vertices.push_back(base_vertices[j * 11 + k]);
            }
        }

        // Adjust and add indices for the second cone
        index_offset = i * base_vertex_count * 2 + base_vertex_count;
        for (size_t j = 0; j < base_indices.size(); ++j) {
            indices.push_back(base_indices[j] + index_offset);
        }
    }

    std::shared_ptr<Geometry> geom_construct = std::make_shared<Geometry>(vertices, indices);
    this->Geom = geom_construct;

    ObjmodelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

void AISplineComponent::getNearestVertexFromPos(glm::vec3 position, glm::vec3 &nearestVertex, glm::vec3 &nextNearestVertex) {
    
    int nearestIndex = 0;
    float minDist = 1000000.0f;
    
    for (int i = 0; i < spline_verts.size(); i++) {
        float dist = glm::distance(position, spline_verts[i]);
        if (dist < minDist) {
            
            minDist =     dist;
            nearestIndex = i;

            nearestVertex = spline_verts[i];
            nextNearestVertex = spline_verts[(i + 1) % spline_verts.size()];
        }
    }
}

void AISplineComponent::Draw()
{

    glUniform1i(useTextureLOC, GL_FALSE);

    // Save the current culling state
    GLboolean cullingEnabled = glIsEnabled(GL_CULL_FACE);
    
    glGetIntegerv(GL_CULL_FACE_MODE, &currentCullFace);
    glGetIntegerv(GL_FRONT_FACE, &currentFrontFace);

    glDisable(GL_CULL_FACE);

    Geom->Draw(modelMatrixLOC, ObjmodelMatrix, colorUniformLOC, false);

    glEnable(GL_CULL_FACE);

    glUniform1i(useTextureLOC, GL_TRUE);
}


void AISplineComponent::tick(){
    Draw();
}