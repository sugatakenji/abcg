#include "window.hpp"
#include <iostream>

void Window::onCreate() {
    auto const assetsPath{abcg::Application::getAssetsPath()};
    std::cout << "Initialize shaders \n";
    m_birdProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "bird.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "bird.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
    std::cout << "Shader ok \n";
    abcg::glClearColor(0, 0, 0, 1);
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif
    restart();
    std::cout << "Create window ok \n";
}

void Window::restart() {
    m_bird.create(m_birdProgram, m_gameData);
}

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up));
  }
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  m_bird.update(m_gameData, deltaTime);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
  m_bird.paint(m_gameData);
}

void Window::onPaintUI() {
    abcg::OpenGLWindow::onPaintUI();
    ImU32 redColor = IM_COL32(255, 0, 0, 125); // RGBA
    // Desenha um retângulo que cobre toda a janela
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    draw_list->AddRectFilled(ImVec2(0, 0), ImVec2(m_viewportSize.x, 50), redColor);
    {
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(m_viewportSize.x,50));
        ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoTitleBar };//| ImGuiWindowFlags_NoInputs};
        ImGui::Begin(" ", nullptr, flags);
        ImGui::PushItemWidth(50);
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::SliderFloat("Sustein", &m_bird.m_sustein.y, -9.8f, 1.0f);  
        ImGui::PopItemWidth(); 
        ImGui::End();
    }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_birdProgram);

  m_bird.destroy();
}