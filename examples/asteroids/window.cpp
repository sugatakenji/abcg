#include "window.hpp"

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

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Load a new font
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Create program to render the other objects
  m_objectsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;

  m_ship.create(m_objectsProgram);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_ship.update(m_gameData, deltaTime);
}

void Window::onPaint() {


  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_ship.paint(m_gameData);
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
    ImGui::PushFont(m_font);
    
    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    }
    ImGui::PopFont(); 
    ImGui::PushItemWidth(50);
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::SliderFloat("Planagem", &m_ship.m_planagem, -9.8f, 0.0f);  
    ImGui::PopItemWidth(); 
    ImGui::End();
  }
}


void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_ship.destroy();
}