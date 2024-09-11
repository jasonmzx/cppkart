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

void ECManager::tick()
{
    glm::vec3 camPos = m_camera.get()->getCameraPosition();
    float pX = camPos.x;
    float pZ = camPos.z;

    for (auto& rc : renderComponents) {
        systemRender.get()->Draw(rc);
    }

}

ECManager::ECManager() {
    std::cout << "ECManager Constructor Called" << std::endl;
    systemRender = std::make_shared<RenderSystem>();
}


void ECManager::buildRenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, float scale, bool cD, bool isTexAlpha) {
    RenderComponent rc = systemRender.get()->buildRenderComponent(modelPath, texPath, meshIndices, scale, cD, isTexAlpha);  
    renderComponents.push_back(rc);
}

//* =============================== Getters/Setters to the "World" System ===============================

void ECManager::setFreeCameraMode(bool freeCam) {
    emitEvent(Event(EventType::CAMERA_TOGGLE_FREE_CAM, freeCam));
}

void ECManager::setCamera(std::shared_ptr<Camera> camera) {
    // Check if the input is valid
    if (camera) {
        m_camera = camera;

    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid Camera Input !");
    }
}