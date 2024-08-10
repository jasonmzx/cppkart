#include "ECManager.hpp"



// Subscribe to an event with a callback
void ECManager::subscribeToEvent(EventType type, Callback callback) {
    int id = nextId++;
    eventCallbacks[type].emplace_back(id, callback);
}

// Unsubscribe from an event using the ID
void ECManager::unsubscribeToEvent(EventType type, int id) {
    auto it = eventCallbacks.find(type);
    if (it != eventCallbacks.end()) {
        auto& subs = it->second;
        subs.erase(std::remove_if(subs.begin(), subs.end(), [id](const auto& pair) {
            return pair.first == id;
        }), subs.end());

        if (subs.empty()) {
            eventCallbacks.erase(it);
        }
    } else {
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "[ECManager : UNSUB ERROR] Event type not found...");
    }
}

// Emit an event to all subscribers of the event type
void ECManager::emitEvent(const Event& event) {
    auto it = eventCallbacks.find(event.type);
    if (it != eventCallbacks.end()) {
        auto& subs = it->second;
        for (const auto& [id, callback] : subs) {
            callback(event);
        }
    } else {
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "[ECManager : EMIT ERROR] Event type not found... " + std::to_string(static_cast<int>(event.type)));
    }
}

//* ------------------- Older Code ------------------- *//

// void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
//     this->sceneManager = sceneManager;
//     this->activeScene = sceneManager.get()->getActiveScene();
// }

void ECManager::tick(std::vector<std::shared_ptr<Entity>>& entities, std::shared_ptr<GameInput> gameInput) 
{
    //TODO: Make a gameinput component and pass it to the player vehicle component
    emitEvent(Event(EventType::PLAYER_VEHICLE_MOVE, std::make_pair(gameInput.get()->currentAcceleration, gameInput.get()->currentTurn)));

    glm::vec3 camPos = m_camera.get()->getCameraPosition();
    
    float pX = camPos.x;
    float pZ = camPos.z;

    emitEvent(Event(EventType::UPDATE_TERRAIN_CHUNKS_XZ, std::make_pair(pX, pZ)));

    float rcX = 0.0f;
    
    float yOffset = 10.0f;
    float rcY = yOffset;
    
    float rcZ = 0.0f;
    

    for(auto entity : entities) {

        for (auto& component : entity.get()->components) {
            
            rcX = gameInput.get()->debugRaycastX;
            rcY = gameInput.get()->debugRaycastY + yOffset;
            rcZ = gameInput.get()->debugRaycastZ;

            if( rcX != 0.0f && rcY != yOffset && rcZ != 0.0f) {
                printf("Raycast: %f, %f, %f\n", rcX, rcY, rcZ);
            }
            component.get()->tick();
        }
    }


    Logger* logger = Logger::getInstance();
    logger->log(Logger::INFO, "Road Barriers: " + std::to_string(NRoadBarriers));
}

void ECManager::renderPass(std::vector<std::shared_ptr<Entity>>& entities) {
    for(auto entity : entities) {
        for (auto& component : entity.get()->components) {

            if (auto vehicleRenderComponent = std::dynamic_pointer_cast<VehicleRenderComponent>(component)) {

                btTransform vehicleTransform = playerVehicleComponent.get()->vehiclePhysics.GetTransform();
                btVector3 vehiclePosition = vehicleTransform.getOrigin();
                btQuaternion vehicleRotation = vehicleTransform.getRotation();

                glm::vec3 glmVehiclePosition(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z());
                glm::quat glmVehicleRotation(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());

                vehicleRenderComponent.get()->UpdateChassisTransform(glmVehiclePosition, glmVehicleRotation);
                vehicleRenderComponent.get()->UpdateWheelTransforms(&playerVehicleComponent.get()->vehiclePhysics);
                vehicleRenderComponent.get()->DrawWheels();
                

                //A Bit Dirty, so change this later

            glm::vec3 forward;

            vehicleRenderComponent.get()->getForwardVector(forward);

            glm::vec3 nearestVertex;
            
            aiSplineComponent.get()->getNearestVertexFromPos(forward, nearestVertex);

            printf("Nearest Vertex: %f, %f, %f\n", nearestVertex.x, nearestVertex.y, nearestVertex.z);

            //TODO: fix this
            auto renderer = GameGLRenderer::getInstance();

            renderer->DebugDrawLine(forward, nearestVertex, glm::vec3(1.0f, 0.0f, 0.0f));


            }

            if( auto movableObjectComponent = std::dynamic_pointer_cast<MovableObjectComponent>(component)) {
                movableObjectComponent.get()->UpdateTransforms();
            }

        }
    }
}

//* =============================== Getters/Setters to the "World" System ===============================

void ECManager::setTerrainChunks(std::shared_ptr<TerrainChunksComponent> terrainChunks) {
    // Check if the input is valid
    if (terrainChunks) {
        terrainChunksComponents = terrainChunks;

        subscribeToEvent(EventType::UPDATE_TERRAIN_CHUNKS_XZ, [this](const Event& event) {
            terrainChunksComponents.get()->handleUpdateChunksEvent(event);
        });

    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid TerrainChunksComponent input");
    }
}

void ECManager::setAISpline(std::shared_ptr<AISplineComponent> aiSpline) {
    // Check if the input is valid
    if (aiSpline) {
        aiSplineComponent = aiSpline;

    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid AISplineComponent input");
    }
}

void ECManager::setPlayerVehicle(std::shared_ptr<PlayerVehicleComponent> playerVehicle) {
    // Check if the input is valid
    if (playerVehicle) {
        playerVehicleComponent = playerVehicle;

        //* Subscribed Events

        subscribeToEvent(EventType::PLAYER_VEHICLE_MOVE, [this](const Event& event) {
            playerVehicleComponent.get()->handlePlayerVehicleMoveEvent(event);
        });

        //* Emitting Events (Set Callbacks)

        playerVehicleComponent->setPlayerPositionCallback([this](float pX, float pY, float pZ, float velocity) {
            //emitEvent(Event(EventType::UPDATE_TERRAIN_CHUNKS_XZ, std::make_pair(pX, pZ)));

            dpX = pX;
            dpY = pY;
            dpZ = pZ;



            emitEvent(Event(EventType::PLAYER_VEHICLE_GET_SPEED, velocity));
        });

        playerVehicleComponent->setPlayerDirectionCallback([this](float dirX, float dirY, float dirZ) {
            emitEvent(Event(EventType::CAMERA_PLAYER_VEHICLE_FOLLOW, std::make_tuple(dpX, dpY, dpZ, dirX, dirY, dirZ)) );
        });

    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid PlayerVehicleComponent Input !");
    }
}

void ECManager::debugSetPlayerVehicleVelocity(float& velocity) {
    subscribeToEvent(EventType::PLAYER_VEHICLE_GET_SPEED, [&velocity](const Event& event) {
        velocity = std::any_cast<float>(event.data);
    });
}

void ECManager::setFreeCameraMode(bool freeCam) {
    emitEvent(Event(EventType::CAMERA_TOGGLE_FREE_CAM, freeCam));
}

void ECManager::setCamera(std::shared_ptr<Camera> camera) {
    // Check if the input is valid
    if (camera) {
        subscribeToEvent(EventType::CAMERA_PLAYER_VEHICLE_FOLLOW, [camera](const Event& event) {
            camera.get()->handleVehicleFollowEvent(event);
        });

        subscribeToEvent(EventType::CAMERA_TOGGLE_FREE_CAM, [camera](const Event& event) {
            camera.get()->handleToggleFreeCamEvent(event);
        });

        m_camera = camera;

    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid Camera Input !");
    }
}

void ECManager::resetPlayerVehicle() {
    playerVehicleComponent.get()->vehiclePhysics.ResetTransform();
}

std::string ECManager::debugStateSTR() {
    std::string debugState = "Player VPOS: " + std::to_string(dpX) + ", " + std::to_string(dpY) + ", " + std::to_string(dpZ);
    return debugState;
}