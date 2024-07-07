#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>

class GameWindow {
    
    SDL_Window* window = nullptr;
    SDL_Surface* icon = nullptr;
    SDL_GLContext glContext{nullptr};

public:

    GameWindow() = default;

    void create(const std::string& title, size_t w, size_t h, bool fullscreen);
    void close();

    void showCursor();
    void hideCursor();

    void swapWindow() const {
        SDL_GL_SwapWindow(window);
    }

    bool isOpen() const {
        return !!window;
    }

     //======= Getters ========
    
    SDL_Window* getWindow() {
        return window;
    }

    SDL_GLContext* getSDLContext() {
        return &glContext;
    }

    // glm::ivec2 getSize() const;

    // std::tuple<SDL_Window *, SDL_GLContext> getSDLContext() {
    //     return std::make_tuple(window, glcontext);
    // }
};

#endif
