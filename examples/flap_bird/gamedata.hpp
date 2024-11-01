#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>
#include "abcgOpenGL.hpp"

enum class Input { Up };

struct GameData {
    std::bitset<1> m_input;
    glm::vec2 m_gravity{glm::vec2(0.f, 9.8f)};
    
};

#endif