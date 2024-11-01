#ifndef BIRD_HPP_
#define BIRD_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

class Bird {
public:
    void create(GLuint program, GameData const &gamedata);
    void paint(GameData const &gamedata);
    void destroy();
    void update(GameData const &gamedata, float deltaTime);

    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{};
    glm::vec2 m_gravity{};
    glm::vec2 m_velocity{};
    glm::vec2 m_sustein{}; //propriedade de planar;
    glm::vec4 m_color{1};

private:
    GLuint m_program{};
    GLuint m_translationLoc{};
    GLuint m_colorLoc{};
    GLuint m_scaleLoc{};
    
    GLuint m_VAO{};
    GLuint m_VBO{};
    GLuint m_EBO{};
};
#endif