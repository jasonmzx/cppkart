#include "XJZoomEngine.h"

//Bullet Physics Engine Imports:

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>

//Windowing consts

#define WinWidth 1800
#define WinHeight 1000


GLfloat boxVertices[] = {
    // Position (x, y, z)   Color (r, g, b)   Texture coordinates (s, t)
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

// Define the indices for the BOX
GLuint boxIndices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 1, 5, 5, 4, 0,
    2, 3, 7, 7, 6, 2,
    1, 2, 6, 6, 5, 1,
    0, 3, 7, 7, 4, 0};

std::vector<GLfloat> vertices = {};
std::vector<GLuint> indices = {};

//Terrain Scale matrix

glm::mat4 terrainModelMatrix = glm::scale(glm::vec3(10.0f, 1.0f, 10.0f));
glm::mat4 boxModelMatrix = glm::scale(glm::vec3(0.5f));

void XJZoomEngine::Run()
{

// Initialize Bullet Physics
btBroadphaseInterface* broadphase = new btDbvtBroadphase();
btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

// Set gravity for the world
dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

//! ############################################################
//! TODO: PULL ALL OF THIS INTO A VEHICLE PHYSICS ENTITY CLASS #
//! ############################################################

//Definitions of Vehicle physics elements:

btRigidBody* vehicleRigidBody;
btVehicleRaycaster* vehicleRayCaster;
btRaycastVehicle* vehicle;

//Vehicle tuning:
btRaycastVehicle::btVehicleTuning tuning;
tuning.m_suspensionStiffness = 20.0f;
tuning.m_suspensionCompression = 0.83f;
tuning.m_suspensionDamping = 0.88f;
tuning.m_maxSuspensionTravelCm = 500.0f;
tuning.m_frictionSlip = 10.5f;
tuning.m_maxSuspensionForce = 6000.0f;


// Vehicle setup
btCollisionShape* vehicleChassisShape = new btBoxShape(btVector3(1.0f, 0.5f, 2.0f));
btDefaultMotionState* vehicleMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3, 0)));
btScalar vehicleMass = 800;
btVector3 vehicleInertia(0, 0, 0);
vehicleChassisShape->calculateLocalInertia(vehicleMass, vehicleInertia);
btRigidBody::btRigidBodyConstructionInfo vehicleRigidBodyCI(vehicleMass, vehicleMotionState, vehicleChassisShape, vehicleInertia);
vehicleRigidBody = new btRigidBody(vehicleRigidBodyCI);
dynamicsWorld->addRigidBody(vehicleRigidBody);

// Raycaster and the actual vehicle
vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);
vehicle = new btRaycastVehicle(tuning, vehicleRigidBody, vehicleRayCaster);
vehicleRigidBody->setActivationState(DISABLE_DEACTIVATION);
dynamicsWorld->addVehicle(vehicle);

// Wheel configuration
btVector3 wheelDirection = btVector3(0, -1, 0);
btVector3 wheelAxle = btVector3(-1, 0, 0);
btScalar suspensionRestLength = 0.6;
btScalar wheelRadius = 0.5;
btScalar wheelWidth = 0.4;
btScalar suspensionStiffness = 20.0;
btScalar dampingRelaxation = 2.3;
btScalar dampingCompression = 4.4;
btScalar frictionSlip = 1000;
btScalar rollInfluence = 0.1;

// Add wheels to the vehicle
for (int i = 0; i < 4; i++)
{
    bool isFrontWheel = i < 2;
    vehicle->addWheel(
        btVector3(0, 1, i == 0 || i == 3 ? 1.2 : -1.2),
        wheelDirection,
        wheelAxle,
        suspensionRestLength,
        wheelRadius,
        tuning,
        isFrontWheel);
    btWheelInfo& wheel = vehicle->getWheelInfo(i);
    wheel.m_suspensionStiffness = suspensionStiffness;
    wheel.m_wheelsDampingRelaxation = dampingRelaxation;
    wheel.m_wheelsDampingCompression = dampingCompression;
    wheel.m_frictionSlip = frictionSlip;
    wheel.m_rollInfluence = rollInfluence;
}

//Vehicle control:
float engineForce = 0.0;
float vehicleSteering = 0.0;
float steeringIncrement = 0.04;
float steeringClamp = 0.3;
float brakeForce = 0.0;


//! ############################################################
//! TODO: PULL ALL OF THIS INTO A VEHICLE PHYSICS ENTITY CLASS #
//! ############################################################



//* ########## WINDOWING STUFF ############
  uint32_t WindowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *Window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);
  assert(Window);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext Context = SDL_GL_CreateContext(Window);

  gladLoadGLLoader(SDL_GL_GetProcAddress);

  int32_t Running = 1;
  int32_t FullScreen = 0;

  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
  glViewport(0, 0, WinWidth, WinHeight);


  // Load Managers:
  SceneManager sceneManager;

  terrainMapLoader(indices, vertices);

  // Generates Shader object using shaders default.vert and default.frag
  Shader shaderProgram("../src/rendering/shader/default.vert", "../src/rendering/shader/default.frag");

  auto modelMatrixLocation = glGetUniformLocation(shaderProgram.ID, "modelMatrix");

  // Generates Vertex Array Object and binds it
  VAO VAO1;
  VAO1.Bind();

  // Generates Vertex Buffer Object and links it to vertices
  VBO VBO1(vertices.data(), sizeof(GLfloat) * vertices.size());
  // Generates Element Buffer Object and links it to indices
  EBO EBO1(indices.data(), sizeof(GLuint) * indices.size());

  // Links VBO attributes such as coordinates and colors to VAO
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  VAO VAO2;
  VAO2.Bind();

  // Generates a second Vertex Buffer Object and links it to boxVertices
  VBO VBO2(boxVertices, sizeof(boxVertices));
  // Generates a second Element Buffer Object and links it to boxIndices
  EBO EBO2(boxIndices, sizeof(boxIndices));

  // Links VBO attributes such as coordinates and colors to VAO2
  VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO2.Unbind();
  VBO2.Unbind();
  EBO2.Unbind();

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  std::string texPath = "../src/ressources/";

  // Texture
  Texture brickTex((texPath + "brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  brickTex.texUnit(shaderProgram, "tex0", 0);

  double prevTime = SDL_GetTicks();

  // Enables the Depth Buffer
  glEnable(GL_DEPTH_TEST);

  // Creates camera object
  Camera camera(WinWidth, WinHeight, glm::vec3(0.0f, 0.0f, 2.0f));

  // Scene Culling:

  FrustumCull frustumCuller;

  // #### MAIN GAME LOOP THAT ENGINE IS RUNNING:
  while (Running)
  {
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
      if (Event.type == SDL_KEYDOWN)
      {
        switch (Event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          Running = 0;
          break;
        case 'f':
          FullScreen = !FullScreen;
          if (FullScreen)
          {
            SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
          }
          else
          {
            SDL_SetWindowFullscreen(Window, WindowFlags);
          }
          break;
        case SDLK_w:
          engineForce = 1000.0;
          printf("GASS!");
          break;
        default:
          break;
        }
      }
      else if (Event.type == SDL_QUIT)
      {
        Running = 0;
      }
    }
    vehicle->applyEngineForce(engineForce, 2);
    vehicle->applyEngineForce(engineForce, 3);

    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.Activate();

    // Other rendering logic...

    // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
    brickTex.Bind();

    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Handles camera inputs
    camera.Inputs(Window);
    // Updates and exports the camera matrix to the Vertex Shader
    camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

    frustumCuller.Update(camera.viewProjection);

    // if(frustumCuller.IsBoxVisible(boxMin)) {}
    
    //Draw Terrain:
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(terrainModelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);

    VAO2.Bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(boxModelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(boxIndices)) / sizeof(int), GL_UNSIGNED_INT, 0);
    
    // Swap the back buffer with the front buffer
    SDL_GL_SwapWindow(Window);

    glm::vec3 cameraPosition = camera.Position;
    //printf("Camera Position: (%.2f, %.2f, %.2f)\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

    btVector3 velocity = vehicleRigidBody->getLinearVelocity();

  // Get the vehicle's forward direction
  btVector3 vehicleTransform = vehicleRigidBody->getWorldTransform().getBasis().getColumn(2); // Assuming Z-axis is forward. Adjust if different.

  printf("Vehicle Velocity: %.2f, %.2f, %.2f | Direction: %.2f, %.2f, %.2f\n",
       velocity.getX(), velocity.getY(), velocity.getZ(),
       vehicleTransform.getX(), vehicleTransform.getY(), vehicleTransform.getZ());
  }

  // Delete all the objects we've created
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();

  brickTex.Delete();

  shaderProgram.Delete();
}

void XJZoomEngine::Init()
{
  // SDL_ShowSimpleMessageBox(0, "hello", "cppkart", nullptr);
}
