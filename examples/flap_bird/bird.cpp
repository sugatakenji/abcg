#include "bird.hpp"
#include <iostream>

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Bird::create(GLuint program, GameData const &gamedata){
    destroy();

    m_program = program;
    m_gravity = gamedata.m_gravity;
    
    m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

    m_translation = glm::vec2(0);
    m_velocity = glm::vec2(0);

    std::array positions{
      // Ship body
      glm::vec2{-02.5f, +12.5f}, glm::vec2{-15.5f, +02.5f},
      glm::vec2{-15.5f, -12.5f}, glm::vec2{-09.5f, -07.5f},
      glm::vec2{-03.5f, -12.5f}, glm::vec2{+03.5f, -12.5f},
      glm::vec2{+09.5f, -07.5f}, glm::vec2{+15.5f, -12.5f},
      glm::vec2{+15.5f, +02.5f}, glm::vec2{+02.5f, +12.5f},

      // Cannon (left)
      glm::vec2{-12.5f, +10.5f}, glm::vec2{-12.5f, +04.0f},
      glm::vec2{-09.5f, +04.0f}, glm::vec2{-09.5f, +10.5f},

      // Cannon (right)
      glm::vec2{+09.5f, +10.5f}, glm::vec2{+09.5f, +04.0f},
      glm::vec2{+12.5f, +04.0f}, glm::vec2{+12.5f, +10.5f},
      
      // Thruster trail (left)
      glm::vec2{-12.0f, -07.5f}, 
      glm::vec2{-09.5f, -18.0f}, 
      glm::vec2{-07.0f, -07.5f},

      // Thruster trail (right)
      glm::vec2{+07.0f, -07.5f}, 
      glm::vec2{+09.5f, -18.0f}, 
      glm::vec2{+12.0f, -07.5f},
      };

        // Normalize
    for (auto &position : positions) {
        position /= glm::vec2{15.5f, 15.5f};
    }

    std::array const indices{0, 1, 3,
                           1, 2, 3,
                           0, 3, 4,
                           0, 4, 5,
                           9, 0, 5,
                           9, 5, 6,
                           9, 6, 8,
                           8, 6, 7,
                           // Cannons
                           10, 11, 12,
                           10, 12, 13,
                           14, 15, 16,
                           14, 16, 17,
                           // Thruster trails
                           18, 19, 20,
                           21, 22, 23};

    abcg::glGenBuffers(1, &m_VBO);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glGenBuffers(1, &m_EBO);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

    abcg::glGenVertexArrays(1, &m_VAO);
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    abcg::glBindVertexArray(0);

}

void Bird::paint(const GameData &gamedata) {
    abcg::glUseProgram(m_program);

    abcg::glBindVertexArray(m_VAO);
    abcg::glUniform1f(m_scaleLoc, m_scale);
    abcg::glUniform2fv(m_translationLoc, 1,  &m_translation[0]);

    abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
    abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);
    abcg::glBindVertexArray(0);

    abcg::glUseProgram(0);
}

void Bird::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Bird::update(GameData const &gamedata, float deltaTime){
    // Apply thrust
  if (gamedata.m_input[gsl::narrow<size_t>(Input::Up)]) {
    std::cout << "up is pressed" << "\n";
    m_velocity.y -= .005f;
  }
   std::cout << "no" << "\n";
  //Atualiza a altura
  if (m_translation.y > -1.0) {
    m_velocity += (m_gravity + m_sustein) * deltaTime;
  }
  if (m_translation.y < -1.00000f) {
    m_velocity = glm::vec2(0.f, 0.f);
  }
  m_translation -= m_velocity * deltaTime;

}