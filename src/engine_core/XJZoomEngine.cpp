#include "XJZoomEngine.h"

// Windowing consts
#define WinWidth 1800
#define WinHeight 900

std::vector<GLfloat> vertices = {};
std::vector<GLuint> indices = {};



ObjModel firstCarModel = ObjModel("../src/ressources/first_car.obj");
ObjModel firstCarWheelModel = ObjModel("../src/ressources/first_car_wheel.obj");

ObjModel firstBuilding = ObjModel("../src/ressources/first_building.obj");

std::vector<GLfloat> buildingVertices = firstBuilding.GetVertices();
std::vector<GLuint> buildingIndices = firstBuilding.GetIndices();


std::vector<GLfloat> playerVehicle_verts = firstCarModel.GetVertices();
std::vector<GLuint> playerVehicle_indices = firstCarModel.GetIndices();

std::vector<GLfloat> VW_vertices = firstCarWheelModel.GetVertices();
std::vector<GLuint> VW_indices = firstCarWheelModel.GetIndices();

//*#### Bullet Debugger Scale Matrix:
glm::mat4 bulletModelMatrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

//*#### Terrain Scale Matrix:
glm::mat4 terrainModelMatrix = glm::scale(glm::vec3(100.0f, 100.0f, 100.0f));
glm::mat4 boxMtrxTest = glm::scale(glm::vec3(100.0f, 4.0f, 100.0f));

//*#### Vehicle & Wheel Matrix scaling:
glm::mat4 WheelMatrix = glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));
glm::mat4 vehicleModelMatrix = glm::scale(glm::vec3(1.0f));

void XJZoomEngine::Run()
{
  int carTexWidth, carTexHeight, carTexChannels;
  auto carTextureData = stbi_load("../src/ressources/first_car.png", &carTexWidth, &carTexHeight, &carTexChannels, STBI_rgb_alpha);
  assert(carTextureData);

  //* ### Bullet Physics World Singleton Instanciation ###

  PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

  //* Proto Plane Wuz Here (check the gist for this)

// Terrain Physics
int width;
int length;
std::vector<float> heightDataVec; 
btScalar minHeight;               
btScalar maxHeight;               

// ----- Height Field -----

bool loadTerrainFromIMG = loadHeightfieldData("../src/ressources/small_map.png", heightDataVec, width, length, minHeight, maxHeight);

float *heightData = new float[heightDataVec.size()];
std::copy(heightDataVec.begin(), heightDataVec.end(), heightData);

    // Instantiate TerrainPhysics
    TerrainPhysics terrain(width, length, heightData, minHeight, maxHeight, 0, 0);

    // Add the terrain to the dynamics world
    //physicsWorld->dynamicsWorld->addRigidBody(terrain.terrainRigidBody);

    //Second heightfield test, works fine, can do LOD heightfield

    // TerrainPhysics terrain2(width, length, heightData, 0, 100, -100, 0);
    // physicsWorld->dynamicsWorld->addRigidBody(terrain2.terrainRigidBody);


// chunking stuff

// int chunk_size = 50; //Represents Width & Length (X & Z)
// std::vector<std::vector<float>> chunkVecs;
// btScalar globalChunkMin; 
// btScalar globalChunkMax;
// int N_chunks_x;
// int N_chunks_y;

// bool chunkHeightMap = chunkHeightDataFromIMG("../src/ressources/Map_B.png", &chunk_size, chunkVecs, N_chunks_x, N_chunks_y, globalChunkMin, globalChunkMax);

PhysicsChunkManager terrainChunkManager("../src/ressources/Map_1K.png");

  //* ########## WINDOWING STUFF ############
  uint32_t WindowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *Window = SDL_CreateWindow("XJZoom Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);
  assert(Window);

  //* ########## OpenGL & SDL Context Setup (Versioning, Settings) ############
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext SDL_GL_Context = SDL_GL_CreateContext(Window);
  double prevTime = SDL_GetTicks(); // Window Tick Rate (SDL thing)

  gladLoadGLLoader(SDL_GL_GetProcAddress);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 330 core";
    ImGui_ImplSDL2_InitForOpenGL(Window, SDL_GL_Context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  GLuint carTexID;
  glGenTextures(1, &carTexID);
  glBindTexture(GL_TEXTURE_2D, carTexID);

  // Depending on the number of channels, pick the appropriate format
  GLenum carTexFormat;
  if (carTexChannels == 1)
    carTexFormat = GL_RED;
  else if (carTexChannels == 3)
    carTexFormat = GL_RGB;
  else if (carTexChannels == 4)
    carTexFormat = GL_RGBA;

  //* Texting Configuration Setup for OpenGL

  glTexImage2D(GL_TEXTURE_2D, 0, carTexFormat, carTexWidth, carTexHeight, 0, carTexFormat, GL_UNSIGNED_BYTE, carTextureData);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glViewport(0, 0, WinWidth, WinHeight); //(X1, Y1 bottom left) to (X2, Y2, top right)
  glEnable(GL_DEPTH_TEST);




  // ------ Below is the mess i need to clean up eventually ------

  // Load Managers:
  SceneManager sceneManager;

  terrainMapLoader(indices, vertices);

  // Generates Shader object using shaders default.vert and default.frag
  Shader shaderProgram("../src/rendering/shader/default.vert", "../src/rendering/shader/default.frag");

  //*ModelMatrix for GLSL Shader
  auto modelMatrixLocation = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
  GLint useTextureLocation = glGetUniformLocation(shaderProgram.ID, "useTexture");

  //* Bullet Physics Rendering Debug Tool
  BulletDebugDrawer* debugDrawer = new BulletDebugDrawer(shaderProgram.ID);

  //* ####### Terrain Geometry Instance
  VAO VAO1;
  VAO1.Bind();
  VBO VBO1(buildingVertices); // Vertex Buffer Object ; links it to vertices
  EBO EBO1(buildingIndices);  // Element Buffer Object ; links it to indices
  //RenderableGeometry terrainGeom(&VAO1, &VBO1, &EBO1, buildingVertices, buildingIndices, terrainModelMatrix);

  //* ####### Player Vehicle Geometry Instance
  VAO VAO2;
  VAO2.Bind();
  VBO VBO2(playerVehicle_verts);
  EBO EBO2(playerVehicle_indices);

  //* ####### Wheel Geom Instance
  VAO VAO3;
  VAO3.Bind();
  VBO VBO3(VW_vertices);
  EBO EBO3(VW_indices);

  //* ####### 


  VAO VAO4;
  VAO4.Bind();
  VBO VBO4(vertices);
  EBO EBO4(indices);

  //Temporary un-render of terrain Geom
  //RenderableGeometry terrainGeom(&VAO4, &VBO4, &EBO4, vertices, indices);
  
  
  //SolidEntity TERRAIN(&VAO4, &VBO4, &EBO4, vertices, indices, terrainModelMatrix);

  //* #### Player Vehicle Instanciation:

  VehicleEntity vehicle(&VAO2, &VBO2, &EBO2, playerVehicle_verts, playerVehicle_indices,
                        &VAO3, &VBO3, &EBO3, VW_vertices, VW_indices);

  //GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
  //std::string texPath = "../src/ressources/";

  // !Texture, TODO ground texture, for terrain
  // Texture brickTex((texPath + "brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  // brickTex.texUnit(shaderProgram, "tex0", 0);
  // Enables the Depth Buffer

  // Creates camera object
  Camera camera(WinWidth, WinHeight, glm::vec3(0.0f, 0.0f, 2.0f));

  //Keyboard Input:
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  //Application Window State
  int32_t Running = 1;
  int32_t FullScreen = 0;


  physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);


  //* ImGui State

  bool show_debug_draw = true;


  // #### MAIN GAME LOOP THAT ENGINE IS RUNNING:
  while (Running)
  {

    //* POLLING INPUTS for Multiple Keyboard Input and Handle Simultaneously

  SDL_PumpEvents(); // Make sure we have the latest state
  state = SDL_GetKeyboardState(NULL);

  // Handling continuous key states rather than events
  if (state[SDL_SCANCODE_UP]) {
    vehicle.GetPhysics().ApplyEngineForce(2000);
  } else if (state[SDL_SCANCODE_DOWN]) {
    vehicle.GetPhysics().ApplyEngineForce(-2500);
  } else {
    vehicle.GetPhysics().ApplyEngineForce(0);
  }

  if (state[SDL_SCANCODE_LEFT]) {
    vehicle.GetPhysics().ApplySteer(0.13);
  } else if (state[SDL_SCANCODE_RIGHT]) {
    vehicle.GetPhysics().ApplySteer(-0.13);
  } else {
    vehicle.GetPhysics().ApplySteer(0);
  }

    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {

      ImGui_ImplSDL2_ProcessEvent(&Event);

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
        default:
          break;
        }
      }
      else if (Event.type == SDL_QUIT)
      {
        Running = 0;
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    float framerate = ImGui::GetIO().Framerate;
    char formatted_fps_STR[50]; 
    // Use sprintf to format the float as a string
    sprintf(formatted_fps_STR, "FPS: %f", framerate);

ImGui::Begin("Debug Menu");

if (ImGui::BeginTabBar("DebugTabBar")) {
    // Tab for FPS & Physics Debug
    if (ImGui::BeginTabItem("General")) {
        ImGui::Text(formatted_fps_STR);
        ImGui::Checkbox("Physics Debug Draw (Bullet3)", &show_debug_draw);
        ImGui::EndTabItem();
    }

    // Tab for Vehicle
    if (ImGui::BeginTabItem("Vehicle Debug")) {

        std::string v_debug_str = vehicle.GetPhysics().debugStateSTR(); 
        ImGui::Text("%s", v_debug_str.c_str());
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
}

ImGui::End();

    //! ### IMPORTANT, Allow the Physics Simulation to tick ###
    physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);

    //! PROTOTYPING: VEHICLE RENDERING CODE
    // ### Update the box's model matrix to match the vehicle's transform ###

    btTransform vehicleTransform = vehicle.GetPhysics().GetTransform();
    btVector3 vehiclePosition = vehicleTransform.getOrigin();

    //* ==== Dynamic World Loading ====
    
    //player positions (vehicle)
    btScalar pXpos = vehiclePosition.getX();
    btScalar pYpos = vehiclePosition.getY();
    btScalar pZpos = vehiclePosition.getZ();

    terrainChunkManager.update(pXpos, pZpos);


    btQuaternion vehicleRotation = vehicleTransform.getRotation();

    //Position Translation
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z()));

    glm::mat4 rotation = glm::mat4_cast(glm::quat(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z()));
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    vehicleModelMatrix = translation * rotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.4f));
    // WheelMatrix = translation * glm::scale(glm::vec3(0.25f));

    //! PROTOTYPING: VEHICLE RENDERING CODE

    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.Activate();

    // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
    // brickTex.Bind();

    //debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    if(show_debug_draw) {
      physicsWorld->dynamicsWorld->debugDrawWorld();
    }

    //* ###### CAMERA #######

    camera.DEBUG = false;

    //* naive approach (hard offsets camera, bad for steering)
    //  camera.Position.x = vehiclePosition.x() + 0.5f;
    //  camera.Position.y = vehiclePosition.y() + 2.0f;
    //  camera.Position.z = vehiclePosition.z() - 3.0f;

    //* #### Smooth Camera (For Driving)


    if(!camera.DEBUG) {
    auto targetVec = glm::vec3(vehiclePosition.x() + 1.0f, vehiclePosition.y() + 3.0f, vehiclePosition.z() - 5.0f);
    auto dirVec = targetVec - camera.Position;
    if (glm::distance2(targetVec, camera.Position) > 0.02f)
      camera.Position += dirVec * 0.03f;


    //Look at Vehicle:

    camera.LookAt.x = vehiclePosition.x();
    camera.LookAt.y = vehiclePosition.y();
    camera.LookAt.z = vehiclePosition.z();
    }


    //camera.Inputs(Window);

    //  Updates and exports the camera matrix to the Vertex Shader
    camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
    //camera.Matrix(45.0f, 0.1f, 100.0f, terrainShaderProgram, "camMatrix");

    //*############## OpenGL - Draw Calls ################
    glUniform1i(useTextureLocation, GL_TRUE); 
    //BOX1.geom.Draw(modelMatrixLocation, terrainModelMatrix);
    
    debugDrawer->flushLines();


//    terrainGeom.Draw(modelMatrixLocation,terrainModelMatrix);
    
    vehicle.GetGeometry().Draw(modelMatrixLocation, vehicleModelMatrix, NULL, false);
 

    //! idk why i'm not binding textures and it's still workign...?
    // glBindTexture(GL_TEXTURE_2D, carTexID);

    vehicle.renderWheelGeometries(modelMatrixLocation);


    //! All Draw Calls below use no Texturing, and just Positonal coloring:

    glUniform1i(useTextureLocation, GL_FALSE); 

    GLint colorUniformLocation = glGetUniformLocation(shaderProgram.ID, "FragColor");

    //terrainGeom.Draw(modelMatrixLocation,terrainModelMatrix, colorUniformLocation, false);


    //* Important: The Bullet Render Debug Drawer uses the Non-Textured Shader Option, therefore we need to re-set the model matrix before making the switch, or else
    //* the Bullet Debug Drawer will be hooked up to the last model Matrix, which will causes wonky debug overlay

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bulletModelMatrix));



    //ImGUI Render calls

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    

    // Swap the back buffer with the front buffer
    SDL_GL_SwapWindow(Window);
    //glm::vec3 cameraPosition = camera.Position;
  }

  glDeleteTextures(1, &carTexID);
  // brickTex.Delete();
  shaderProgram.Delete();

  // Shutdown for ImGUI
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  // TODO: Delete the Physics World Singleton here
}

void XJZoomEngine::Init()
{
  // SDL_ShowSimpleMessageBox(0, "hello", "cppkart", nullptr);
}
