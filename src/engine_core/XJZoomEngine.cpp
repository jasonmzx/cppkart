#include "XJZoomEngine.h"

#define WinWidth 1800
#define WinHeight 1000

GLfloat vertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
        0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f};

// Indices for vertices order
GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4};

void XJZoomEngine::Run()
{

  uint32_t WindowFlags = SDL_WINDOW_OPENGL;
  SDL_Window *Window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);
  assert(Window);
  SDL_GLContext Context = SDL_GL_CreateContext(Window);

  gladLoadGLLoader(SDL_GL_GetProcAddress);

  int32_t Running = 1;
  int32_t FullScreen = 0;

  // Introduce the window into the current context
  // Load GLAD so it configures OpenGL
  gladLoadGL();
  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
  glViewport(0, 0, WinWidth, WinHeight);

  //Load Managers:
  SceneManager sceneManager;

  // Generates Shader object using shaders default.vert and default.frag
  Shader shaderProgram("../src/rendering/shader/default.vert", "../src/rendering/shader/default.frag");

  // Generates Vertex Array Object and binds it
  VAO VAO1;
  VAO1.Bind();

  // Generates Vertex Buffer Object and links it to vertices
  VBO VBO1(vertices, sizeof(vertices));
  // Generates Element Buffer Object and links it to indices
  EBO EBO1(indices, sizeof(indices));

  // Links VBO attributes such as coordinates and colors to VAO
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  // Unbind all to prevent accidentally modifying them
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

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

  //Scene Culling:

  FrustumCull frustumCuller;

  //#### MAIN GAME LOOP THAT ENGINE IS RUNNING:

//load terrain beofre game loop:
  //#### MAIN GAME LOOP THAT ENGINE IS RUNNING: (end)
  terrainMapLoader(indices, vertices);



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
        default:
          break;
        }
      }
      else if (Event.type == SDL_QUIT)
      {
        Running = 0;
      }
    }
    
    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Tell OpenGL which Shader Program we want to use
    shaderProgram.Activate();

    //Other rendering logic...

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
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    
    // Swap the back buffer with the front buffer
    SDL_GL_SwapWindow(Window);



  glm::vec3 cameraPosition = camera.Position;
  printf("Camera Position: (%.2f, %.2f, %.2f)\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

  }

  // Delete all the objects we've created
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();

  brickTex.Delete();

  shaderProgram.Delete();
}