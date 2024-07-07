#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <memory>

#include "Scene.hpp"

class SceneManager {
    
    public:
        SceneManager();

        std::shared_ptr<Scene> getActiveScene();
        void setActiveScene(std::shared_ptr<Scene> scene);

    private:
        std::shared_ptr<Scene> activeScene;

};

#endif // SCENEMANAGER_HPP