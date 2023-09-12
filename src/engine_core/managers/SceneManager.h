#ifndef SCENEMANAGER_CLASS_H
#define SCENEMANAGER_CLASS_H

#include <SDL2/SDL.h>
#include <glad/glad.h>

class SceneManager {

public:
    void Render(GLuint* indices);
private:
    void Delete();
};

#endif