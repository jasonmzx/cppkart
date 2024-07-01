#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "jx_engine/core/SceneManager.hpp"
#include "jx_engine/core/Scene.hpp"
#include "jx_engine/entity/ECManager.hpp"

#include "jx_engine/render/GameGLRenderer.hpp"
#include "jx_engine/render/RenderRsrcManager.hpp"
#include "jx_engine/render/gl/Camera.h"

#include "jx_engine/physics/PhysicsWorldSingleton.h"
#include "jx_engine/physics/BulletDebugDrawer.cpp"

#include "jx_engine/physics/manager/PhysicsChunkManager.hpp"

#include "jx_engine/io/GameInput.hpp"

// Component Includes:

#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/component/PlayerVehicleComponent.hpp"
#include "jx_engine/component/TerrainChunksComponent.hpp"
#include "jx_engine/component/VehicleRenderComponent.hpp"

// ImGUI Imports:
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


class GameScene : public Scene {
    public:
        GameScene(int WIN_W, int WIN_H, SDL_Window* window);
        void init() override;
        void initECS(std::shared_ptr<SceneManager> sceneManager);
        
        void update(float dt) override;
        void render() override;

        void procGameInputs();
        void updateImGui();


        // Static instance of self, for bullet collision callback:

        static GameScene* instance;

        // Might put this somewhere else
        static bool bulletCollisionCallback(btManifoldPoint& cp, 
        const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1);

        std::unique_ptr<ECManager> ec;
        std::shared_ptr<Camera> camera;
        
        PhysicsWorldSingleton *physicsWorld;

        SDL_Joystick* gGameController;
        SDL_Joystick* gGameController2;

    private:

        void load_HighRoadHills_Map(std::shared_ptr<Entity> terrainEntity);

        float ecInferenceTimeMS;

        int WIN_WIDTH;
        int WIN_HEIGHT;
        SDL_Window* SDL_window;

        bool isBulletDebugDraw = false;

        std::unique_ptr<ECManager> ecManager;

        std::unique_ptr<PhysicsChunkManager> physicsChunkManager;

        std::shared_ptr<GameGLRenderer> renderer;
        std::shared_ptr<RenderRsrcManager> renderRsrcManager;

        static Logger* logger;

        bool followPlayerVehicle = true;
        bool trackMouse = true;
        bool firstClick = true;

        std::shared_ptr<GameInput> gameInput;

        // Sounds:

        Mix_Chunk* crashSound; 
};

#endif // SCENE_HPP
