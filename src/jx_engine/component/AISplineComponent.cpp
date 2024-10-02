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

    //TODO: Use SplineVertex Array to do shit properly 
    int base_vertex_count = base_vertices.size() / 11; // each vertex has 11 attributes (position, color, texture, normal)


    for (size_t i = 0; i < spline_verts.size(); ++i) {
        
        SplineVertex sv = spline_verts.at(i);

        // printf("##################");
        // printf("Position: (%f, %f, %f)\n", sv.position.x, sv.position.y, sv.position.z);
        // printf("Direction: (%f, %f, %f)\n", sv.direction.x, sv.direction.y, sv.direction.z);
        // printf("XZ Orthogonal Direction: (%f, %f, %f)\n", sv.xz_orthogonal_dir.x, sv.xz_orthogonal_dir.y, sv.xz_orthogonal_dir.z);

    //* Up direction crossed with the direction vector, gives: perpendicular to the direction vector, for vehicle orientation on the spline

        glm::vec3 init_direction = glm::vec3(0.0f, 0.0f, 1.0f);

        // Calculate the rotation matrix for the second cone
        glm::quat perpendicularQuat = glm::rotation(init_direction, sv.xz_orthogonal_dir);
        glm::mat4 perpendicularRotationMatrix = glm::toMat4(perpendicularQuat);

        for (int j = 0; j < base_vertex_count; ++j) {
            // Apply rotation to the position
            glm::vec4 rotatedPos = perpendicularRotationMatrix * glm::vec4(
                base_vertices[j * 11 + 0],
                base_vertices[j * 11 + 1],
                base_vertices[j * 11 + 2],
                1.0f
            );

            // Translate the rotated position
            GLfloat x = rotatedPos.x + sv.position.x;
            GLfloat y = rotatedPos.y + sv.position.y;
            GLfloat z = rotatedPos.z + sv.position.z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Copy the rest of the un-touched attributes (color, texture coordinates, normals)
            for (int k = 3; k < 11; ++k) {
                vertices.push_back(base_vertices[j * 11 + k]);
            }
        }

        GLuint index_offset = i * base_vertex_count * 2;


        for (size_t j = 0; j < base_indices.size(); ++j) {
            indices.push_back(base_indices[j] + index_offset);
        }
    
    }

    std::shared_ptr<Geometry> geom_construct = std::make_shared<Geometry>(vertices, indices);
    this->Geom = geom_construct;

    printf("SIZEOF VERTS: %ld\n", spline_verts.size());

    ObjmodelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

void AISplineComponent::getNearestVertexFromPos(glm::vec3 position, glm::vec3 &nearestVertexPosition, glm::vec3 &nextNearestVertexPosition, glm::vec3 &nearestPerendicularDir) {
    
    float minDist = 1000000.0f;
    
    for (int i = 0; i < spline_verts.size(); i++) {
        float dist = glm::distance(position, spline_verts[i].position);
        
        if (dist < minDist) {
            minDist =     dist;

            nearestVertexPosition = spline_verts[i].position;
            
            nextNearestVertexPosition = spline_verts[(i+1) % spline_verts.size()].position;

            nearestPerendicularDir = spline_verts[i].xz_orthogonal_dir;
        }
    }
}

void AISplineComponent::toggleShowCyl() {
    showDebugCylinder = !showDebugCylinder;
}

void AISplineComponent::Draw()
{

    if( showDebugCylinder == false) {
        return;
    }

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