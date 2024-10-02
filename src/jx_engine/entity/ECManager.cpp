#include "ECManager.hpp"

// Subscribe to an event with a callback
void ECManager::subscribeToEvent(EventType type, Callback callback)
{
    int id = nextId++;
    eventCallbacks[type].emplace_back(id, callback);
}

// Unsubscribe from an event using the ID
void ECManager::unsubscribeToEvent(EventType type, int id)
{
    auto it = eventCallbacks.find(type);
    if (it != eventCallbacks.end())
    {
        auto &subs = it->second;
        subs.erase(std::remove_if(subs.begin(), subs.end(), [id](const auto &pair)
                                  { return pair.first == id; }),
                   subs.end());

        if (subs.empty())
        {
            eventCallbacks.erase(it);
        }
    }
    else
    {
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "[ECManager : UNSUB ERROR] Event type not found...");
    }
}

// Emit an event to all subscribers of the event type
void ECManager::emitEvent(const Event &event)
{
    auto it = eventCallbacks.find(event.type);
    if (it != eventCallbacks.end())
    {
        auto &subs = it->second;
        for (const auto &[id, callback] : subs)
        {
            callback(event);
        }
    }
    else
    {
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "[ECManager : EMIT ERROR] Event type not found... " + std::to_string(static_cast<int>(event.type)));
    }
}

//* ------------------- Older Code ------------------- *//

// void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
//     this->sceneManager = sceneManager;
//     this->activeScene = sceneManager->getActiveScene();
// }

void ECManager::tick(std::vector<std::shared_ptr<Entity>> &entities)
{
    glm::vec3 camPos = m_camera->getCameraPosition();

    float pX = camPos.x;
    float pZ = camPos.z;

    skyboxRenderComponent->CentreArroundXYZ(camPos);
    skyboxRenderComponent->tick();

    emitEvent(Event(EventType::UPDATE_TERRAIN_CHUNKS_XZ, std::make_pair(pX, pZ)));

    for (auto entity : entities)
    {
        for (auto &component : entity->components)
        {
            component->tick();

        }
    }
}

void ECManager::componentSpecificPass(std::vector<std::shared_ptr<Entity>> &entities, std::shared_ptr<GameInput> gameInput)
{




    // --- Player Vehicle Movement ---
    if (!isAIVehicleControl)
    {
        emitEvent(Event(EventType::PLAYER_VEHICLE_MOVE, std::make_pair(gameInput->currentAcceleration, gameInput->currentTurn)));
    }

    float rcX = 0.0f;

    float yOffset = 10.0f;
    float rcY = yOffset;

    float rcZ = 0.0f;

    for (auto entity : entities)
    {
        for (auto &component : entity->components)
        {

            // Get RayTest:

            rcX = gameInput->debugRaycastX;
            rcY = gameInput->debugRaycastY + yOffset;
            rcZ = gameInput->debugRaycastZ;

            if (rcX != 0.0f && rcY != yOffset && rcZ != 0.0f)
            {
                printf("Raycast: %f, %f, %f\n", rcX, rcY, rcZ);
            }

            // Use Dynamic Dispatch to call the specific component's update function

            if (auto vehicleRenderComponent = std::dynamic_pointer_cast<VehicleRenderComponent>(component))
            {

                btTransform vehicleTransform = playerVehicleComponent->vehiclePhysics.GetTransform();
                btVector3 vehiclePosition = vehicleTransform.getOrigin();
                btQuaternion vehicleRotation = vehicleTransform.getRotation();

                glm::vec3 glmVehiclePosition(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z());
                glm::quat glmVehicleRotation(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());

                vehicleRenderComponent->UpdateChassisTransform(glmVehiclePosition, glmVehicleRotation);
                vehicleRenderComponent->UpdateWheelTransforms(&playerVehicleComponent->vehiclePhysics);
                vehicleRenderComponent->DrawWheels();

                glm::vec3 forward;
                vehicleRenderComponent->getForwardVector(forward);

                glm::vec3 objectForward;
                glm::vec3 objectRight;

                vehicleRenderComponent->getObjectVectors(objectForward, objectRight);

                glm::vec3 nearestSplineVertex;
                glm::vec3 nextNearestSplineVertex;

                glm::vec3 perpendicularDirection; // Perpendicular Direction (XZ Orthogonal to Direction of Spline)

                aiSplineComponent->getNearestVertexFromPos(forward, nearestSplineVertex, nextNearestSplineVertex, perpendicularDirection);

                glm::vec3 splineSegmentDirection = nextNearestSplineVertex - nearestSplineVertex;

                float turnIndicator = 0.0f;

                aiVehicleComponent->performControls(
                    nextNearestSplineVertex, perpendicularDirection, splineSegmentDirection, forward, objectForward, objectRight, turnIndicator);

                GameInput::Control currentAISteer = GameInput::Idle;


                if (isAIVehicleControl)
                {
                    emitEvent(Event(EventType::PLAYER_VEHICLE_MOVE, std::make_pair(gameInput->currentAcceleration, currentAISteer)));
                    playerVehicleComponent->vehiclePhysics.ApplySteer(turnIndicator);
                }
                auto renderer = GameGLRenderer::getInstance();
                // Draw line from Vehicle Forward to the nearest spline vertex
                renderer->DebugDrawLine(forward, nearestSplineVertex, glm::vec3(1.0f, 0.0f, 0.0f));     // Red line
                renderer->DebugDrawLine(forward, nextNearestSplineVertex, glm::vec3(1.0f, 1.0f, 0.0f)); // Red line
            }

            if (auto movableObjectComponent = std::dynamic_pointer_cast<MovableObjectComponent>(component))
            {
                movableObjectComponent->UpdateTransforms();
            }
        }
    }
}

//* Debug Toggles

void ECManager::toggleAIVehicleControl()
{
    isAIVehicleControl = !isAIVehicleControl;
}

void ECManager::toggleNormalsShader() {
    GameGLRenderer* renderer = GameGLRenderer::getInstance();

    isNormalsShader = !isNormalsShader;

    if(isNormalsShader) {
        renderer->changeShader("default_normal");
    } else {
        renderer->changeShader("default");
    }
}

void ECManager::toggleAICylinders() {
    aiSplineComponent->toggleShowCyl();
}

//* =============================== Getters/Setters to the "World" System ===============================

void ECManager::setTerrainChunks(std::shared_ptr<TerrainChunksComponent> terrainChunks)
{
    // Check if the input is valid
    if (terrainChunks)
    {
        terrainChunksComponents = terrainChunks;

        subscribeToEvent(EventType::UPDATE_TERRAIN_CHUNKS_XZ, [this](const Event &event)
                         { terrainChunksComponents->handleUpdateChunksEvent(event); });
    }
    else
    {
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid TerrainChunksComponent input");
    }
}

void ECManager::setAISpline(std::shared_ptr<AISplineComponent> aiSpline)
{
    // Check if the input is valid
    if (aiSpline)
    {
        aiSplineComponent = aiSpline;
    }
    else
    {
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid AISplineComponent input");
    }
}

void ECManager::setAIVehicle(std::shared_ptr<AIVehicleComponent> aiVehicle)
{

    if (aiVehicle)
    {
        aiVehicleComponent = aiVehicle;
    }
    else
    {
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid AI Vehicle Component input");
    }
}

void ECManager::setPlayerVehicle(std::shared_ptr<VehicleControlComponent> playerVehicle)
{
    // Check if the input is valid
    if (playerVehicle)
    {
        playerVehicleComponent = playerVehicle;

        //* Subscribed Events

        subscribeToEvent(EventType::PLAYER_VEHICLE_MOVE, [this](const Event &event)
                         { playerVehicleComponent->handlePlayerVehicleMoveEvent(event); });

        //* Emitting Events (Set Callbacks)

        playerVehicleComponent->setPlayerPositionCallback([this](float pX, float pY, float pZ, float velocity)
                                                          {
            //emitEvent(Event(EventType::UPDATE_TERRAIN_CHUNKS_XZ, std::make_pair(pX, pZ)));

            dpX = pX;
            dpY = pY;
            dpZ = pZ;

            playerPosition = glm::vec3(pX, pY, pZ);


            
            emitEvent(Event(EventType::PLAYER_VEHICLE_GET_SPEED, velocity)); });

        playerVehicleComponent->setPlayerDirectionCallback([this](float dirX, float dirY, float dirZ)
                                                           { emitEvent(Event(EventType::CAMERA_PLAYER_VEHICLE_FOLLOW, std::make_tuple(dpX, dpY, dpZ, dirX, dirY, dirZ))); });
    }
    else
    {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid VehicleControlComponent Input !");
    }
}

void ECManager::debugSetPlayerVehicleVelocity(float &velocity)
{
    subscribeToEvent(EventType::PLAYER_VEHICLE_GET_SPEED, [&velocity](const Event &event)
                     { velocity = std::any_cast<float>(event.data); });
}

void ECManager::setFreeCameraMode(bool freeCam)
{
    emitEvent(Event(EventType::CAMERA_TOGGLE_FREE_CAM, freeCam));
}

void ECManager::setCamera(std::shared_ptr<Camera> camera)
{
    // Check if the input is valid
    if (camera)
    {
        subscribeToEvent(EventType::CAMERA_PLAYER_VEHICLE_FOLLOW, [camera](const Event &event)
                         { camera->handleVehicleFollowEvent(event); });

        subscribeToEvent(EventType::CAMERA_TOGGLE_FREE_CAM, [camera](const Event &event)
                         { camera->handleToggleFreeCamEvent(event); });

        m_camera = camera;
    }
    else
    {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger *logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid Camera Input !");
    }
}

void ECManager::resetPlayerVehicle()
{
    playerVehicleComponent->vehiclePhysics.ResetTransform();
}

std::string ECManager::debugStateSTR()
{
    std::string debugState = "Player VPOS: " + std::to_string(dpX) + ", " + std::to_string(dpY) + ", " + std::to_string(dpZ);
    return debugState;
}

void ECManager::setSkybox(std::shared_ptr<RenderComponent> skybox)
{
    skyboxRenderComponent = skybox;
}

glm::vec3 ECManager::getLastPlayerPos() {
    return playerPosition;
}