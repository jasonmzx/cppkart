#include "XJZoomEngine.h"
#include <SDL_mixer.h>

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

glm::vec3 lerpVec3(const glm::vec3 &start, const glm::vec3 &end, float factor) {
    return start + factor * (end - start);
}




void XJZoomEngine::Run()
{

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO); 

  // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        // Handle error
    }

    // Load the MP3 music
    Mix_Music *backgroundMusic = Mix_LoadMUS("../src/ressources/main-menu-theme.mp3");
    if(!backgroundMusic) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        // Handle error
    } else {
        // Play the music (once)
        Mix_PlayMusic(backgroundMusic, 1);
    }


  VehicleInputAdapter vehicleInputControl;

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

    // Setup Platform/Renderer backends for Imgui
    const char* glsl_version = "#version 330 core";
    ImGui_ImplSDL2_InitForOpenGL(Window, SDL_GL_Context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  glViewport(0, 0, WinWidth, WinHeight); //(X1, Y1 bottom left) to (X2, Y2, top right)
  glEnable(GL_DEPTH_TEST);


  // ------ Below is the mess i need to clean up eventually ------

  // Load Managers:
  //! terrainModelMatrix = glm::translate(glm::vec3(-5, -(terrainChunkManager.globalChunkMax - terrainChunkManager.globalChunkMin) / 2, -5)) 
  //! * glm::scale(glm::vec3(1000.0f, 190.0f, 1000.0f));

// Really bad hard code, todo fix this after jitter is over
terrainModelMatrix = glm::translate(glm::vec3(-5, -(18.1) / 2, -5)) 
 * glm::scale(glm::vec3(1000.0f, 190.0f, 1000.0f));

  // Generates Shader object using shaders default.vert and default.frag
  Shader shaderProgram("../src/rendering/shader/default.vert", "../src/rendering/shader/default.frag");

  //*ModelMatrix for GLSL Shader
  auto modelMatrixLocation = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
  GLint useTextureLocation = glGetUniformLocation(shaderProgram.ID, "useTexture");

  //* Bullet Physics Rendering Debug Tool
  BulletDebugDrawer* debugDrawer = new BulletDebugDrawer(shaderProgram.ID);

  SharedPhysicsResource sharedPhysicsRessource;
  PhysicsThread pT(&sharedPhysicsRessource, debugDrawer);
  pT.Start(vehicleInputControl.playerInputQueue);

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

  terrainMapLoader(indices, vertices, "../src/ressources/loop_map.png"); //! EXPERIMENTAL
  std::vector<GLuint> roadIndices;
  std::vector<GLfloat> roadVertices;
  //roadTerrainMapLoader(roadIndices, roadVertices, "../src/ressources/loop_map.png", "../src/ressources/loop_track.png");

  VAO VAO4;
  VAO4.Bind();
  VBO VBO4(vertices);
  EBO EBO4(indices);

  VAO VAO5;
  VAO5.Bind();
  VBO VBO5(roadVertices);
  EBO EBO5(roadIndices);

  //Temporary un-render of terrain Geom
 RenderableGeometry terrainGeom(&VAO4, &VBO4, &EBO4, vertices, indices);

 RenderableGeometry roadGeom(&VAO5, &VBO5, &EBO5, roadVertices, roadIndices);
  
  //SolidEntity TERRAIN(&VAO4, &VBO4, &EBO4, vertices, indices, terrainModelMatrix);

  //* #### Player Vehicle Instanciation:

  VehicleEntity vehicle(&VAO2, &VBO2, &EBO2, playerVehicle_verts, playerVehicle_indices,
                        &VAO3, &VBO3, &EBO3, VW_vertices, VW_indices);

  //GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
  std::string texPath = "../src/ressources/";

  // !Texture, TODO ground texture, for terrain
   Texture cobbleTex((texPath + "cobble.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
   cobbleTex.texUnit(shaderProgram, "tex0", 0);

 Texture roadCobbleTex((texPath + "road.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
   roadCobbleTex.texUnit(shaderProgram, "tex0", 0);

  Texture pCarTex("../src/ressources/first_car.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  pCarTex.texUnit(shaderProgram, "tex0", 0);  

  // Creates camera object
  Camera camera(WinWidth, WinHeight, glm::vec3(0.0f, 0.0f, 2.0f));
  glm::vec3 initCamLookAt = glm::vec3(1, 1, 1);
  camera.LookAt = initCamLookAt;

  //Inputs:

  const int JOYSTICK_DEAD_ZONE = 4000;

  const uint8_t* state = SDL_GetKeyboardState(nullptr);
  SDL_Joystick* gGameController = nullptr;


        //Check for joysticks
        if( SDL_NumJoysticks() < 1 )
        {
            printf( "Warning: No joysticks connected!\n" );
        }
        else
        {
            //Load joystick
            gGameController = SDL_JoystickOpen( 0 );
            if( gGameController == nullptr )
            {
                printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
            }

            printf(
              "JOYSTICK CONNECTION :-)\n"
            );
        }



  //Application Window State
  int32_t Running = 1;
  int32_t FullScreen = 0;

  btDiscreteDynamicsWorld* physicsWorldPTR = sharedPhysicsRessource.getPhysicsWorld();
  physicsWorldPTR->setDebugDrawer(debugDrawer);
  
  //SDL_GL_SetSwapInterval(0); // turn vsync off and speed shit up drasitcally, bad design!!

  //* ImGui State

  bool show_debug_draw = true;
  bool interpolated_transforms = false;

  //* Physics Interpolation State (for vehicle only rn)

  btTransform lastVehicleTransform;
  bool isFirstUpdate = true; // To handle the first frame case

  // #### MAIN GAME LOOP THAT ENGINE IS RUNNING:
  while (Running)
  {


  //* POLLING INPUTS for Multiple Keyboard Input and Handle Simultaneously?

  SDL_PumpEvents(); 
  state = SDL_GetKeyboardState(nullptr);
  vehicleInputControl.vehicleKeyboardInput(state);

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
            ImGui::Checkbox("Interpolate Vehicle Transforms", &interpolated_transforms);
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

    vehiclePhysicsInfo vI = sharedPhysicsRessource.GetVehiclePhyInfo();
    btTransform vehicleTransform = vI.transform;

    if (isFirstUpdate) {
        lastVehicleTransform = vehicleTransform;
        isFirstUpdate = false;
    }

    btVector3 vehiclePosition = vehicleTransform.getOrigin();
    btVector3 lastVehiclePosition = lastVehicleTransform.getOrigin();

    btQuaternion vehicleRotation = vehicleTransform.getRotation();
    btQuaternion lastVehicleRotation = lastVehicleTransform.getRotation();

    // Interpolation factor
    float interpolationFactor = 0.75f; // Adjust this value as needed

    // Interpolate rotation using slerp
    btQuaternion interpolatedRotation = lastVehicleRotation.slerp(vehicleRotation, interpolationFactor);

    // Interpolated position
    btVector3 interpolatedPosition = lastVehiclePosition.lerp(vehiclePosition, interpolationFactor);

    // Update lastVehicleTransform at the end of the loop
    lastVehicleTransform = vehicleTransform;

    if(interpolated_transforms){
      vehiclePosition = interpolatedPosition;
      vehicleRotation = interpolatedRotation;
    }

    glm::quat glmVehicleRotation = glm::quat(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());
    if(interpolated_transforms){
        glmVehicleRotation = glm::quat(interpolatedRotation.w(), interpolatedRotation.x(), interpolatedRotation.y(), interpolatedRotation.z());
    }

    //TODO: ==== Vehicle Rendering Code, To be Abstracted from this class ====

    //Position Translation
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z()));

    glm::mat4 rotation = glm::mat4_cast(glmVehicleRotation);
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    vehicleModelMatrix = translation * rotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.75f));
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

        physicsWorldPTR->debugDrawWorld();
        debugDrawer->flushLines();


    }
    glUniform1i(useTextureLocation, GL_TRUE); 

  //* ###### CAMERA #######

    camera.DEBUG = false;

    if(!camera.DEBUG) {
    
      auto targetVec = glm::vec3(vehiclePosition.x() + 1.0f, vehiclePosition.y() + 3.0f, vehiclePosition.z() - 5.0f); //* Camera Offset
      auto dirVec = targetVec - camera.Position;
      
      if (glm::distance2(targetVec, camera.Position) > 0.02f)
        camera.Position += dirVec * 0.03f;

      glm::vec3 lookAtPosition = glm::vec3(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z());
      
      camera.LookAt = lookAtPosition; //Naive Approach

      //camera.LookAt = lerpVec3(camera.LookAt, lookAtPosition, 0.35f);
    
    } else { camera.Inputs(Window); }

    camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix"); //! IMPORTANT

  //*############## OpenGL - Draw Calls ################

    pCarTex.Bind();
    
    vehicle.GetGeometry().Draw(modelMatrixLocation, vehicleModelMatrix, 0, false);

    vehicle.renderWheelGeometries(modelMatrixLocation, vI.wheelVec);
    
    pCarTex.Unbind();

    //! All Draw Calls below use no Texturing, and just Positonal coloring:

    // glUniform1i(useTextureLocation, GL_FALSE); 

    GLint colorUniformLocation = glGetUniformLocation(shaderProgram.ID, "FragColor");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set to wireframe mode
    
    // glBindTexture(GL_TEXTURE_2D, brick)
    cobbleTex.Bind();
    terrainGeom.Draw(modelMatrixLocation, terrainModelMatrix, colorUniformLocation, false);
    
    cobbleTex.Unbind();

    roadCobbleTex.Bind();

    // roadGeom.Draw(modelMatrixLocation, terrainModelMatrix, colorUniformLocation, false);

    roadCobbleTex.Unbind();

    

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to default mode


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

  //* ============ Cleanup of Application ===========
  pT.Stop(); //Stopping of Physics Thread

  shaderProgram.Delete();

  // Shutdown for ImGUI
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  //Free Static Ressources (Textures, Models, etc...)

  cobbleTex.Delete();
  pCarTex.Delete();

    // Cleanup
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();


    SDL_JoystickClose( gGameController );
    gGameController = nullptr;

    SDL_DestroyWindow( Window );
    Window = nullptr;
    SDL_Quit();

}

void XJZoomEngine::Init()
{
  // SDL_ShowSimpleMessageBox(0, "hello", "cppkart", nullptrptr);
}
