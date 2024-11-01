#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "asteroids.hpp"
#include "bullets.hpp"
#include "ship.hpp"
#include "starlayer.hpp"

class Window : public abcg::OpenGLWindow {
protected:
    void onEvent(SDL_Event const &event) override;
    void onCreate()override;
    void onUpdate()override;
    void onPaint()override;
    void onPaintUI()override;
    void onResize(glm::ivec2 const &size)override;
    void onDestroy()override;

private:
    glm::ivec2 m_viewportSize{};

    GLuint m_objectsProgram{};
    GameData m_gameData;
    Ship m_ship;
    abcg::Timer m_restartWaitTimer;
    ImFont *m_font{};

    std::default_random_engine m_randomEngine;
    void restart();

};
#endif