#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "jx_engine/core/SceneManager.hpp"
#include "jx_engine/core/Scene.hpp"
#include "jx_engine/entity/ECManager.hpp"

#include "jx_engine/render/GameGLRenderer.hpp"
#include "jx_engine/render/gl/Camera.h"

#include "jx_engine/physics/PhysicsWorldSingleton.h"
#include "jx_engine/physics/BulletDebugDrawer.cpp"


//! DEBUG IMPORTS:
#include "jx_engine/physics/manager/PhysicsChunkManager.hpp"
#include "jx_engine/physics/DynamicConvexHullMeshPhysics.hpp"

#include "jx_engine/io/GameInput.hpp"

// Component Includes:

#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/component/VehicleControlComponent.hpp"
#include "jx_engine/component/AIVehicleComponent.hpp"
#include "jx_engine/component/TerrainChunksComponent.hpp"
#include "jx_engine/component/VehicleRenderComponent.hpp"
#include "jx_engine/component/MovableObjectComponent.hpp"
#include "jx_engine/component/AISplineComponent.hpp"
#include "jx_engine/component/RenderCylinderComponent.hpp"



// ImGUI Imports:
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


class GameScene : public Scene {
    public:

        GameScene(int WIN_W, int WIN_H, SDL_Window* window);
        void initECS(std::shared_ptr<SceneManager> sceneManager);

        // Scene Functions:
        void init() override;
        void tickScene() override;
        void updateScreenSize(int w, int h) override;

        //Game Specific Functions:
        void update(float dt);
        void render();

        void makeBarrier(float x, float y, float z);

        void updateImGui();

        glm::vec3 BulletRaycast();

        // I/O Functions:
        void procGameInputs();
        SDL_Joystick* gGameController;

        // Static instance of self, for bullet collision callback as it's static:
        static GameScene* instance;

        // Might put this somewhere else
        static bool bulletCollisionCallback(btManifoldPoint& cp, 
        const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1);

        std::unique_ptr<ECManager> ec;
        std::shared_ptr<Camera> camera;
        
        PhysicsWorldSingleton *physicsWorld;


    private:

        //OGL Render: TODO: Make a Renderer Frontend
        GameGLRenderer* renderer;

        void load_HighRoadHills_Map(std::shared_ptr<Entity> terrainEntity);
        void load_SquareIsland_Map(std::shared_ptr<Entity> terrainEntity);

        float ecInferenceTimeMS;
        float vSpeed;

        int WIN_WIDTH;
        int WIN_HEIGHT;
        SDL_Window* SDL_window;

        bool isBulletDebugDraw = false;

        std::unique_ptr<ECManager> ecManager;

        std::unique_ptr<PhysicsChunkManager> physicsChunkManager;


        static Logger* logger;

        bool followPlayerVehicle = true;
        bool trackMouse = true;
        bool firstClick = true;

        std::shared_ptr<GameInput> gameInput;

        // Sounds:

        Mix_Chunk* crashSound; 
};

#endif // SCENE_HPP
