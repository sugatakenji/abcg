project("flap_bird")
add_executable(${PROJECT_NAME} main.cpp window.cpp bird.cpp)
enable_abcg(${PROJECT_NAME})