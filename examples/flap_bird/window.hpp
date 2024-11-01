#ifndef WINDOW_HPP_
#define WIDNOW_HPP_

#include "abcgOpenGL.hpp"

#include "bird.hpp"
#include "gamedata.hpp"

class Window : public abcg::OpenGLWindow {
protected:
    void onEvent(SDL_Event const &event)override;
    void onCreate()override;
    void onUpdate()override;
    void onPaint()override;
    void onPaintUI()override;
    void onResize(glm::ivec2 const &size)override;
    void onDestroy()override;

private:
    glm::ivec2 m_viewportSize{};
    GLuint m_birdProgram{};
    Bird m_bird;
    GameData m_gameData; 
    abcg::Timer m_restartTimer;

    void restart();
};


#endif