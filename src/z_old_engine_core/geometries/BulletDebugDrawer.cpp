#ifndef BULLET_DEBUG_DRAWER_H
#define BULLET_DEBUG_DRAWER_H

#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <vector>

class BulletDebugDrawer : public btIDebugDraw {
private:
    int m_debugMode;
    std::vector<GLfloat> lineVertices;
    GLuint VAO, VBO;
    GLuint lineShaderProgram; // Assume you have a shader program for lines

public:
    BulletDebugDrawer(GLuint shaderProgram) 
    : m_debugMode(btIDebugDraw::DBG_DrawWireframe), lineShaderProgram(shaderProgram) {
        // Setup the VAO and VBO for debug drawing
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW); // Initial buffer, no data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~BulletDebugDrawer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        // Each line has two vertices and each vertex has position (3 floats) and color (3 floats)
        GLfloat line[] = {
            from.x(), from.y(), from.z(), color.x(), color.y(), color.z(),
            to.x(), to.y(), to.z(), color.x(), color.y(), color.z()
        };

        // Add lines to the buffer for batch drawing
        lineVertices.insert(lineVertices.end(), line, line + 12);
    }

    void flushLines() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Update the buffer with all lines
        glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(GLfloat), lineVertices.data(), GL_DYNAMIC_DRAW);
        
        // Use shader program to draw lines
        glUseProgram(lineShaderProgram);
        // Set uniforms as necessary for the shader program
        glDrawArrays(GL_LINES, 0, lineVertices.size() / 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        // Clear the line vertices for the next update
        lineVertices.clear();
    }

    void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
    int getDebugMode() const override { return m_debugMode; }

    void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override {}
    void reportErrorWarning(const char*) override {}
    void draw3dText(const btVector3&, const char*) override {}
};

#endif // BULLET_DEBUG_DRAWER_H
