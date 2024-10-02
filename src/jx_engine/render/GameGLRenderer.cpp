#include "GameGLRenderer.hpp"
#include <chrono>

#define BULLET_DEBUG_DRAW 1

const std::string SHADER_PATH = "../src/game/shader/";

GameGLRenderer::GameGLRenderer(int winWidth, int winHeight, Camera *cam) : camera(cam)
{ 
  physicsWorld = PhysicsWorldSingleton::getInstance();
  glViewport(0, 0, winWidth, winHeight); // Set viewport size

  // Initialize the camera's look-at vector
  glm::vec3 initCamLookAt = glm::vec3(1, 1, 1);
  camera->LookAt = initCamLookAt;

  // Initialize shaders
  std::string vertexShaderPath = SHADER_PATH + "default.vert";
  std::string fragmentShaderPath = SHADER_PATH + "default.frag";
  mainShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // Get locations of uniforms in the shader
  modelMatrixLOC = glGetUniformLocation(mainShader->ID, "modelMatrix");
  useTextureLOC = glGetUniformLocation(mainShader->ID, "useTexture");

  colorUniformLocation = glGetUniformLocation(mainShader->ID, "FragColor");

  if (BULLET_DEBUG_DRAW == 1)
    debugDrawer = new BulletDebugDrawer(mainShader->ID); //TODO: Fix mem leak (dealloc this somewhere)
    printf("Debug draw enabled\n");
    physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);

  //TODO: Find out how physicsWorld is initialized  
  //world->physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);

  std::cout << "modelMatrixLOC: " << modelMatrixLOC << std::endl;
  std::cout << "useTextureLOC: " << useTextureLOC << std::endl;
  std::cout << "colorUniformLocation: " << colorUniformLocation << std::endl;

}

void GameGLRenderer::RenderPrep() {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShader->Activate();

    glUniform1i(useTextureLOC, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

void GameGLRenderer::DebugRender() {
    if (BULLET_DEBUG_DRAW == 1) {
        glUniform1i(useTextureLOC, GL_FALSE);

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(modelMatrixLOC, 1, GL_FALSE, glm::value_ptr(identityMatrix));

        physicsWorld->dynamicsWorld->debugDrawWorld();
        debugDrawer->flushLines();
        glUniform1i(useTextureLOC, GL_TRUE);
    }
}


void GameGLRenderer::DebugDrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    debugDrawer->drawLine(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z), btVector3(color.x, color.y, color.z));
}

void GameGLRenderer::UpdateScreenSize(int winWidth, int winHeight) {
    glViewport(0, 0, winWidth, winHeight);
}

void GameGLRenderer::changeShader(const std::string &shaderName) {
    
  std::string vertexShaderPath = SHADER_PATH + shaderName.c_str() + ".vert";
  std::string fragmentShaderPath = SHADER_PATH + shaderName.c_str() + ".frag";
  auto newShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    
    mainShader = newShader;

    // Reset Debug Drawer too

    if (BULLET_DEBUG_DRAW == 1) {
        debugDrawer = new BulletDebugDrawer(mainShader->ID);
        physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);
    }
}


// Singleton stuff

GameGLRenderer* GameGLRenderer::instance = nullptr;

GameGLRenderer* GameGLRenderer::getInstance(int winWidth, int winHeight, Camera *cam) {
    if (!instance) {
        instance = new GameGLRenderer(winWidth, winHeight, cam);
    }
    return instance;
}

GameGLRenderer* GameGLRenderer::getInstance() {
    if (!instance) {
        throw std::runtime_error("GameGLRenderer instance is not yet initialized. Call getInstance with parameters first.");
    }
    return instance;
}

//* =================================================================================
// Cylinder Geometry                                                            < ===
//* =================================================================================

std::vector<GLfloat> GameGLRenderer::GetCylinderVertices(int n_sides, float height) {
    
    const float PI = 3.14159265359f;
    float sectorStep = 2 * PI / n_sides;

    std::vector<GLfloat> vertices;

    // Generate vertices for the top and bottom circles
    for (int i = 0; i <= 1; ++i) { // i=0: bottom, i=1: top
        float h = -height / 2.0f + i * height; // -h/2 to +h/2 for bottom to top
        for (int j = 0; j < n_sides; ++j) {

            float sectorAngle = j * sectorStep;
            
            // X, and Y are coordinates on a normalized circle (-1 , 1) on x and y axis
            
            float x = cos(sectorAngle);
            float y = sin(sectorAngle);

            // Vertex (X, Y, Z)

            vertices.push_back(x); // x
            vertices.push_back(y); // y
            vertices.push_back(h); // z

            // Color (R, G, B)

            vertices.push_back(0.0f); // r
            vertices.push_back(1.0f); // g
            vertices.push_back(0.0f); // b

            // Texture coordinates (U,V)

            vertices.push_back((float)j / n_sides); // u
            vertices.push_back(1.0f - i); // v

            // Normals
            vertices.push_back(x); // nx
            vertices.push_back(y); // ny
            vertices.push_back(0.0f); // nz
        }
    }

    // Bottom Center Point
    
    vertices.push_back(0.0f);           // X
    vertices.push_back(0.0f);           // Y
    vertices.push_back(-height / 2.0f); // Z

    vertices.push_back(1.0f);        // r
    vertices.push_back(0.0f);        // g
    vertices.push_back(0.0f);        // b

    vertices.push_back(0.0f);        // u
    vertices.push_back(0.0f);        // v

    vertices.push_back(0.0f);        // nx
    vertices.push_back(0.0f);        // ny
    vertices.push_back(-1.0f);       // nz

    // Top Center Point

    vertices.push_back(0.0f);          // X
    vertices.push_back(0.0f);          // Y
    vertices.push_back(height / 2.0f); // Z

    vertices.push_back(0.0f);        // r
    vertices.push_back(0.0f);        // g
    vertices.push_back(1.0f);        // b

    vertices.push_back(0.0f);        // u
    vertices.push_back(1.0f);        // v

    vertices.push_back(0.0f);        // nx
    vertices.push_back(0.0f);        // ny
    vertices.push_back(1.0f);        // nz

    return vertices;
}

std::vector<GLuint> GameGLRenderer::GetCylinderIndices(int n_sides) {
    std::vector<GLuint> indices;

    // Bottom face
    for (int i = 0; i < n_sides; ++i) {
        indices.push_back(n_sides * 2);      // center point of bottom circle
        indices.push_back(i);                // current vertex
        indices.push_back((i + 1) % n_sides);// next vertex
    }

    // Top face
    int topOffset = n_sides;
    for (int i = 0; i < n_sides; ++i) {
        indices.push_back(n_sides * 2 + 1);  // center point of top circle
        indices.push_back(topOffset + i);    // current vertex
        indices.push_back(topOffset + (i + 1) % n_sides); // next vertex
    }

    // Side faces
    for (int i = 0; i < n_sides; ++i) {
        int next = (i + 1) % n_sides;

        // First triangle
        indices.push_back(i);                // bottom current
        indices.push_back(next);             // bottom next
        indices.push_back(n_sides + i);    // top current

        // Second triangle
        indices.push_back(n_sides + i);    // top current
        indices.push_back(next);             // bottom next
        indices.push_back(n_sides + next); // top next
    }

    return indices;
}

//* =================================================================================
// Cone Geometry                                                            < ===
//* =================================================================================

std::vector<GLfloat> GameGLRenderer::GetConeVertices(int n_sides, float height) {
    
    const float PI = 3.14159265359f;
    float sectorStep = 2 * PI / n_sides;

    std::vector<GLfloat> vertices;

    // Generate vertices for the top and bottom circles
    for (int i = 0; i < 1; ++i) { // i= 0: bottom
        float h = -height / 2.0f + i * height; // -h/2 to +h/2 for bottom to top
        for (int j = 0; j < n_sides; ++j) {

            float sectorAngle = j * sectorStep;
            
            // X, and Y are coordinates on a normalized circle (-1 , 1) on x and y axis
            
            float x = cos(sectorAngle);
            float y = sin(sectorAngle);

            // Vertex (X, Y, Z)

            vertices.push_back(x); // x
            vertices.push_back(y); // y
            vertices.push_back(h); // z

            // Color (R, G, B)

            vertices.push_back(0.0f); // r
            vertices.push_back(1.0f); // g
            vertices.push_back(0.0f); // b

            // Texture coordinates (U,V)

            vertices.push_back((float)j / n_sides); // u
            vertices.push_back(1.0f - i); // v

            // Normals
            vertices.push_back(x); // nx
            vertices.push_back(y); // ny
            vertices.push_back(0.0f); // nz
        }
    }

    // Bottom Center Point
    
    vertices.push_back(0.0f);           // X
    vertices.push_back(0.0f);           // Y
    vertices.push_back(-height / 2.0f); // Z

    vertices.push_back(1.0f);        // r
    vertices.push_back(0.0f);        // g
    vertices.push_back(0.0f);        // b

    vertices.push_back(0.0f);        // u
    vertices.push_back(0.0f);        // v

    vertices.push_back(0.0f);        // nx
    vertices.push_back(0.0f);        // ny
    vertices.push_back(-1.0f);       // nz

    // Top Center Point

    vertices.push_back(0.0f);          // X
    vertices.push_back(0.0f);          // Y
    vertices.push_back(height / 2.0f); // Z

    vertices.push_back(0.0f);        // r
    vertices.push_back(0.0f);        // g
    vertices.push_back(1.0f);        // b

    vertices.push_back(0.0f);        // u
    vertices.push_back(1.0f);        // v

    vertices.push_back(0.0f);        // nx
    vertices.push_back(0.0f);        // ny
    vertices.push_back(1.0f);        // nz

    return vertices;
}

std::vector<GLuint> GameGLRenderer::GetConeIndices(int n_sides) {
    std::vector<GLuint> indices;

    // Bottom face
    int bottomCenterIndex = n_sides;  // Index of the bottom center point
    for (int i = 0; i < n_sides; ++i) {
        int next = (i + 1) % n_sides;  // Wrap around to form a closed loop
        indices.push_back(bottomCenterIndex);  // Center point of the bottom circle
        indices.push_back(i);                  // Current vertex
        indices.push_back(next);               // Next vertex
    }

    // Side faces
    int topCenterIndex = n_sides + 1;  // Index of the top center point (apex)
    for (int i = 0; i < n_sides; ++i) {
        int next = (i + 1) % n_sides;  // Wrap around to form a closed loop

        indices.push_back(i);            // Current vertex on the base
        indices.push_back(next);         // Next vertex on the base
        indices.push_back(topCenterIndex); // Apex of the cone
    }

    return indices;
}