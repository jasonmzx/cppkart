#include "XJZoomEngine.h"

// Windowing consts
#define WinWidth 1920
#define WinHeight 1080
namespace {
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
glm::mat4 terrainModelMatrix;
//glm::mat4 terrainModelMatrix = glm::mat4(100.0f);

//*#### Vehicle & Wheel Matrix scaling:
glm::mat4 WheelMatrix = glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));
glm::mat4 vehicleModelMatrix = glm::scale(glm::vec3(1.0f));
}

void XJZoomEngine::Run()
{

  VehicleInputAdapter vehicleInputControl;

  SharedPhysicsResource sharedPhysicsRessource;
  PhysicsThread pT(&sharedPhysicsRessource);
  pT.Start(vehicleInputControl.playerInputQueue);

  //Temp

  //VehiclePhysics vPhy;


  int carTexWidth, carTexHeight, carTexChannels;
  auto carTextureData = stbi_load("../src/ressources/first_car.png", &carTexWidth, &carTexHeight, &carTexChannels, STBI_rgb_alpha);
  assert(carTextureData);

  //* ### Bullet Physics World Singleton Instanciation ###

  //PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

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

  //* Texturing Configuration Setup for OpenGL

  glTexImage2D(GL_TEXTURE_2D, 0, carTexFormat, carTexWidth, carTexHeight, 0, carTexFormat, GL_UNSIGNED_BYTE, carTextureData);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glViewport(0, 0, WinWidth, WinHeight); //(X1, Y1 bottom left) to (X2, Y2, top right)
  glEnable(GL_DEPTH_TEST);




  // ------ Below is the mess i need to clean up eventually ------

  // Load Managers:
  SceneManager sceneManager;
  //! terrainModelMatrix = glm::translate(glm::vec3(-5, -(terrainChunkManager.globalChunkMax - terrainChunkManager.globalChunkMin) / 2, -5)) 
  //! * glm::scale(glm::vec3(1000.0f, 190.0f, 1000.0f));

terrainModelMatrix = glm::translate(glm::vec3(-5, -(100-20) / 2, -5)) 
 * glm::scale(glm::vec3(1000.0f, 190.0f, 1000.0f));


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

  terrainMapLoader(indices, vertices, "../src/ressources/Map_1K.png"); //! EXPERIMENTAL

  VAO VAO4;
  VAO4.Bind();
  VBO VBO4(vertices);
  EBO EBO4(indices);

  //Temporary un-render of terrain Geom
 RenderableGeometry terrainGeom(&VAO4, &VBO4, &EBO4, vertices, indices);
  
  //SolidEntity TERRAIN(&VAO4, &VBO4, &EBO4, vertices, indices, terrainModelMatrix);

  //* #### Player Vehicle Instanciation:

  VehicleEntity vehicle(&VAO2, &VBO2, &EBO2, playerVehicle_verts, playerVehicle_indices,
                        &VAO3, &VBO3, &EBO3, VW_vertices, VW_indices);

  //GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
  std::string texPath = "../src/ressources/";

  // !Texture, TODO ground texture, for terrain
   Texture cobbleTex((texPath + "cobble.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
   cobbleTex.texUnit(shaderProgram, "tex0", 0);
  // Enables the Depth Buffer

  // Creates camera object
  Camera camera(WinWidth, WinHeight, glm::vec3(0.0f, 0.0f, 2.0f));

  //Keyboard Input:
  const uint8_t *state = SDL_GetKeyboardState(NULL);

  //Application Window State
  int32_t Running = 1;
  int32_t FullScreen = 0;

  //physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);
  //SDL_GL_SetSwapInterval(0); // turn vsync off and speed shit up drasitcally, bad design!!

  //* ImGui State

  bool show_debug_draw = true;


  // #### MAIN GAME LOOP THAT ENGINE IS RUNNING:
  while (Running)
  {



    //* POLLING INPUTS for Multiple Keyboard Input and Handle Simultaneously?

  SDL_PumpEvents(); 
  state = SDL_GetKeyboardState(NULL);

  vehicleInputControl.vehicleKeyboardInput(state);

  //vehicle.updateVehicleControls(state, vPhy); //This function handles SDL Inputs for the Vehicle's controls

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

  //* ================= Dear ImGui Setup ================
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
            ImGui::Text("%s", formatted_fps_STR);
            ImGui::Checkbox("Physics Debug Draw (Bullet3)", &show_debug_draw);
            ImGui::EndTabItem();
        }

        // Tab for Vehicle
        if (ImGui::BeginTabItem("Vehicle Debug")) {

            //std::string v_debug_str = vPhy.debugStateSTR(); 
            std::string v_debug_str = "helo";
            ImGui::Text("%s", v_debug_str.c_str());
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
  //* ================= End of ImGUI setup ================

    //physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);


    //btTransform vehicleTransform = vPhy.GetTransform();
    vehiclePhysicsInfo vI = sharedPhysicsRessource.GetVehiclePhyInfo();
    btTransform vehicleTransform = vI.transform;


    btVector3 vehiclePosition = vehicleTransform.getOrigin();

    //* ==== Dynamic World Loading ====
    
    // //player positions (vehicle)
    // btScalar pXpos = vehiclePosition.getX();
    // btScalar pYpos = vehiclePosition.getY();
    // btScalar pZpos = vehiclePosition.getZ();

    // terrainChunkManager.update(pXpos, pZpos);

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
    glUniform1i(useTextureLocation, GL_FALSE); 
    //debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    if(show_debug_draw) {
    //  physicsWorld->dynamicsWorld->debugDrawWorld();
    }
    glUniform1i(useTextureLocation, GL_TRUE); 

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
    } else {
    camera.Inputs(Window);
    }

    camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");
    //camera.Matrix(45.0f, 0.1f, 100.0f, terrainShaderProgram, "camMatrix");

    //*############## OpenGL - Draw Calls ################

    //BOX1.geom.Draw(modelMatrixLocation, terrainModelMatrix);
    
    debugDrawer->flushLines();

//    terrainGeom.Draw(modelMatrixLocation,terrainModelMatrix);
    
    vehicle.GetGeometry().Draw(modelMatrixLocation, vehicleModelMatrix, 0, false);

    //! idk why i'm not binding textures and it's still workign...?
    // glBindTexture(GL_TEXTURE_2D, carTexID);

    //vehicle.renderWheelGeometries(modelMatrixLocation, vPhy.vehicle);

    //! All Draw Calls below use no Texturing, and just Positonal coloring:

    // glUniform1i(useTextureLocation, GL_FALSE); 

    GLint colorUniformLocation = glGetUniformLocation(shaderProgram.ID, "FragColor");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set to wireframe mode
    
    // glBindTexture(GL_TEXTURE_2D, brick)
    cobbleTex.Bind();
    terrainGeom.Draw(modelMatrixLocation, terrainModelMatrix, colorUniformLocation, false);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to default mode


    //* Important: The Bullet Render Debug Drawer uses the Non-Textured Shader Option, therefore we need to re-set the model matrix before making the switch, or else
    //* the Bullet Debug Drawer will be hooked up to the last model Matrix, which will causes wonky debug overlay

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bulletModelMatrix));

    glBindTexture(GL_TEXTURE_2D, carTexID);

    //ImGUI Render calls

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    // Swap the back buffer with the front buffer
    SDL_GL_SwapWindow(Window);
    //glm::vec3 cameraPosition = camera.Position;
  }

  //* ============ Cleanup of Application ===========

  pT.Stop();

  glDeleteTextures(1, &carTexID);
  shaderProgram.Delete();

  // Shutdown for ImGUI
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  //Static Ressource freeing
  stbi_image_free(carTextureData); //TODO: use texture class instead of this
  cobbleTex.Delete();

}

void XJZoomEngine::Init()
{
  // SDL_ShowSimpleMessageBox(0, "hello", "cppkart", nullptr);
}
