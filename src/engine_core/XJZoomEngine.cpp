#include "XJZoomEngine.h"

// Windowing consts
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

ObjModel firstCarModel = ObjModel("../src/ressources/first_car.obj");
ObjModel firstCarWheelModel = ObjModel("../src/ressources/first_car_wheel.obj");

std::vector<GLfloat> Vvertices = firstCarModel.GetVertices();
std::vector<GLuint> Vindices = firstCarModel.GetIndices();

std::vector<GLfloat> VW_vertices = firstCarWheelModel.GetVertices();
std::vector<GLuint> VW_indices = firstCarWheelModel.GetIndices();

// Terrain Scale matrix


glm::mat4 WheelMatrix = glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));

glm::mat4 terrainModelMatrix = glm::scale(glm::vec3(10.0f, 1.0f, 10.0f));
glm::mat4 vehicleModelMatrix = glm::scale(glm::vec3(0.25f));

void XJZoomEngine::Run()
{
  int carTexWidth, carTexHeight, carTexChannels;
  auto carTextureData = stbi_load("../src/ressources/first_car.png", &carTexWidth, &carTexHeight, &carTexChannels, STBI_rgb_alpha);
  assert(carTextureData);

  //* ### Bullet Physics World Singleton Insanciation ###

  PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

  //* ############ PROTOTYPE Collision Plane ############

  btTransform protoPlaneTransform;
  protoPlaneTransform.setIdentity();
  protoPlaneTransform.setOrigin(btVector3(0, 0, 0));
  btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));

  // Create Motion shape:
  btMotionState *motion = new btDefaultMotionState(protoPlaneTransform); //! He put btDefaultMotionShape

  btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
  info.m_friction = 0.6f;
  btRigidBody *planeBody = new btRigidBody(info);
  physicsWorld->dynamicsWorld->addRigidBody(planeBody);

  //* ############ PROTOTYPE Collision Plane ############

  //* test inst of a vehicle

  VehicleEntity vehicle;

  // Terrain Physics
  int width;
  int length;
  std::vector<unsigned short> heightDataVec; // Provide your actual height data here
  btScalar minHeight;                        // Minimum height in your dataset
  btScalar maxHeight;                        // Maximum height in your dataset

  bool loadTerrainFromIMG = loadHeightfieldData("../src/ressources/track1.png", heightDataVec, width, length, minHeight, maxHeight);

  unsigned short *heightData = new unsigned short[heightDataVec.size()];
  std::copy(heightDataVec.begin(), heightDataVec.end(), heightData);

  TerrainPhysics terrain(width, length, heightData, minHeight, maxHeight);

  //* Add terrain to physics world
  // physicsWorld->dynamicsWorld->addRigidBody(terrain.GetRigidBody());

  //* ########## WINDOWING STUFF ############
  uint32_t WindowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *Window = SDL_CreateWindow("XJZoom Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);
  assert(Window);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext Context = SDL_GL_CreateContext(Window);

  gladLoadGLLoader(SDL_GL_GetProcAddress);

  GLuint carTexID;
  glGenTextures(1, &carTexID);
  glBindTexture(GL_TEXTURE_2D, carTexID);

  // Depending on the number of channels, pick the appropriate format
  GLenum carTexFormat;
  if (carTexChannels == 1) carTexFormat = GL_RED;
  else if (carTexChannels == 3) carTexFormat = GL_RGB;
  else if (carTexChannels == 4) carTexFormat = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, carTexFormat, carTexWidth, carTexHeight, 0, carTexFormat, GL_UNSIGNED_BYTE, carTextureData);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

  //*ModelMatrix for GLSL Shader
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

  VBO VBO2(Vvertices.data(), sizeof(GLfloat) * Vvertices.size());
  EBO EBO2(Vindices.data(), sizeof(GLuint) * Vindices.size());

  // // Generates a second Vertex Buffer Object and links it to boxVertices
  // VBO VBO2(boxVertices, sizeof(boxVertices));
  // // Generates a second Element Buffer Object and links it to boxIndices
  // EBO EBO2(boxIndices, sizeof(boxIndices));

  // Links VBO attributes such as coordinates and colors to VAO2
  VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO2.Unbind();
  VBO2.Unbind();
  EBO2.Unbind();

  //* Wheel

  VAO VAO3;
  VAO3.Bind();

  VBO VBO3(VW_vertices.data(), sizeof(GLfloat) * VW_vertices.size());
  EBO EBO3(VW_indices.data(), sizeof(GLuint) * VW_indices.size());

  VAO3.LinkAttrib(VBO3, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO3.LinkAttrib(VBO3, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO3.LinkAttrib(VBO3, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO3.Unbind();
  VBO3.Unbind();
  EBO3.Unbind();


  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  std::string texPath = "../src/ressources/";

  // Texture
  // Texture brickTex((texPath + "brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  // brickTex.texUnit(shaderProgram, "tex0", 0);

  double prevTime = SDL_GetTicks();

  // Enables the Depth Buffer
  glEnable(GL_DEPTH_TEST);

  // Creates camera object
  Camera camera(WinWidth, WinHeight, glm::vec3(0.0f, 0.0f, 2.0f));

  //! NOT REALLY USED (yet)... Scene Culling:
  FrustumCull frustumCuller;

  // #### MAIN GAME LOOP THAT ENGINE IS RUNNING:
  while (Running)
  {

    //*Idle Forces when no keys are applied:
    vehicle.GetPhysics().ApplyEngineForce(0);

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
        case SDLK_UP:
          vehicle.GetPhysics().ApplyEngineForce(2500);
          printf("HIT\n");
          break;
        case SDLK_DOWN:
          vehicle.GetPhysics().ApplyEngineForce(-2500);
          break;
        case SDLK_RIGHT:
          vehicle.GetPhysics().Steer(-0.3);
          break;
        case SDLK_LEFT:
          vehicle.GetPhysics().Steer(0.3);
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

    //! ### IMPORTANT, Allow the Physics Simulation to tick ###
    physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);

    //! PROTOTYPING: VEHICLE RENDERING CODE
    // ### Update the box's model matrix to match the vehicle's transform ###
    btTransform vehicleTransform = vehicle.GetPhysics().GetTransform();

    btVector3 vehiclePosition = vehicleTransform.getOrigin();
    btQuaternion vehicleRotation = vehicleTransform.getRotation();

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z()));
    glm::mat4 rotation = glm::mat4_cast(glm::quat(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z()));
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //* Wheel Model Matrices
    std::vector<glm::mat4> wheelMatrices(4);

    //for (int i = 0; i < 1; i++)
     for (int i = 0; i < vehicle.GetPhysics().vehicle->getNumWheels(); i++)
    {
        btWheelInfo wheelinfo = vehicle.GetPhysics().vehicle->getWheelInfo(i);
      
        //Get Translation (Positioning)
        float wX = wheelinfo.m_worldTransform.getOrigin().getX();

        float wY = wheelinfo.m_worldTransform.getOrigin().getY();

        float wZ = wheelinfo.m_worldTransform.getOrigin().getZ();

        glm::mat4 wheelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(wX,wY,wZ));

      //Get Rotation using GLM
      btQuaternion wheelRotationQuat = wheelinfo.m_worldTransform.getRotation();
      glm::quat glmQuat(wheelRotationQuat.getW(), wheelRotationQuat.getX(), wheelRotationQuat.getY(), wheelRotationQuat.getZ());
      glm::mat4 wheelRotation = glm::toMat4(glmQuat);

      // Full transformation matrix for the wheel
      wheelMatrices.push_back(
      wheelTranslation * wheelRotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.25f))
      );
    }


  //
    vehicleModelMatrix = translation * rotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.4f));
    //WheelMatrix = translation * glm::scale(glm::vec3(0.25f));

    //! PROTOTYPING: VEHICLE RENDERING CODE

    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.Activate();

    // Other rendering logic...

    // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
    // brickTex.Bind();

    // Handles camera inputs

    // naive approach (hard offsets camera, bad for steering)

    //  camera.Position.x = vehiclePosition.x() + 0.5f;
    //  camera.Position.y = vehiclePosition.y() + 2.0f;
    //  camera.Position.z = vehiclePosition.z() - 3.0f;

    //* #### Smooth Camera (For Driving)
    auto targetVec = glm::vec3(vehiclePosition.x() + 0.4f, vehiclePosition.y() + 1.3f, vehiclePosition.z() - 2.4f);
    auto dirVec = targetVec - camera.Position;
    if(glm::distance2(targetVec, camera.Position) > 0.02f)
    camera.Position += dirVec * 0.03f;
    camera.LookAt.x = vehiclePosition.x();
    camera.LookAt.y = vehiclePosition.y();
    camera.LookAt.z = vehiclePosition.z();

    //camera.Inputs(Window);

    //  Updates and exports the camera matrix to the Vertex Shader
    camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

    frustumCuller.Update(camera.viewProjection);

    // if(frustumCuller.IsBoxVisible(boxMin)) {}


    // Bind the VAO so OpenGL knows to use it | Draw Terrain
    VAO1.Bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(terrainModelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);

    VAO2.Bind();
    glBindTexture(GL_TEXTURE_2D, carTexID);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(vehicleModelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * Vindices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);

    VAO3.Bind();
    glBindTexture(GL_TEXTURE_2D, carTexID);

  //Render Same Wheel, but at respective Model Matrix per Wheel 
    for (const glm::mat4& wheelMatrix : wheelMatrices)
    {
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(wheelMatrix));
        glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * VW_indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);
    }

    VAO3.Unbind();

    // Swap the back buffer with the front buffer
    SDL_GL_SwapWindow(Window);

    glm::vec3 cameraPosition = camera.Position;

    //! DEBUG PRINTING STUFF HERE:

    // printf("Camera Position: (%.2f, %.2f, %.2f)\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    //  Get the vehicle's rigid body's velocity (in world coordinates)
    //  vehicle.GetPhysics().printState();
  }

  glDeleteTextures(1, &carTexID);

  // Delete all the objects we've created
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();

  // brickTex.Delete();
  shaderProgram.Delete();

  // TODO: Delete the Physics World Singleton here
}

void XJZoomEngine::Init()
{
  // SDL_ShowSimpleMessageBox(0, "hello", "cppkart", nullptr);
}
