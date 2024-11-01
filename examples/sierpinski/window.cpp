#include "window.hpp"

void Window::onCreate() {
    auto const *vertexShader{R"gl(#version 300 es
        layout(location = 0) in vec2 inPosition;

        void main() { 
        gl_PointSize = 3.0;
        gl_Position = vec4(inPosition, 0, 1); 
        }
    )gl"};

    auto const *fragmentShader{R"gl(#version 300 es
        precision mediump float;
        out vec4 outColor;

        void main() {
         vec2 coord = gl_FragCoord.xy / vec2(600,600);
         outColor = vec4(sin(coord),cos(coord.y),1.0); 
         }
    )gl"};

     m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

    abcg::glClearColor(0, 0, 0, 1);
    abcg::glClear(GL_COLOR_BUFFER_BIT);

    std::array<GLfloat, 2> sizes{};
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  abcg::glGetFloatv(GL_POINT_SIZE_RANGE, sizes.data());
#else
  abcg::glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes.data());
#endif
  fmt::print("Point size: {:.2f} (min), {:.2f} (max)\n", sizes.at(0),
             sizes.at(1));




    auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
    m_randomEngine.seed(seed);

    std::uniform_real_distribution<float> realDistribution(-1.0f, 1.0f);
    m_P.x = realDistribution(m_randomEngine);
    m_P.y = realDistribution(m_randomEngine);
   
}

void Window::onPaint() {
    setupModel();

    abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

    abcg::glUseProgram(m_program);
    abcg::glBindVertexArray(m_VAO);
    abcg::glDrawArrays(GL_POINTS, 0, 1);
    abcg::glBindVertexArray(0);
    abcg::glUseProgram(0);

    std::uniform_int_distribution<int> intDistribution(0, m_points.size() - 1);
    auto const index{intDistribution(m_randomEngine)};

    m_P = (m_P + m_points.at(index)) / 2.0f;
    glEnable(GL_SCISSOR_TEST);    
    glScissor(0, 0, 300, 300);
    // std::this_thread::sleep_for (std::chrono::milliseconds(16));
}

void Window::setupModel() {
    abcg::glDeleteBuffers(1, &m_VBOVertices);
    abcg::glDeleteVertexArrays(1, &m_VAO);

    abcg::glGenBuffers(1, &m_VBOVertices);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);

    abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_P),&m_P, GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    auto const positionAttribute{
        abcg::glGetAttribLocation(m_program, "inPosition")};
    
    abcg::glGenVertexArrays(1, &m_VAO);
    abcg::glBindVertexArray(m_VAO);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
    abcg::glBindVertexArray(0);
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  // Release shader program, VBO and VAO
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    ImGui::SetNextWindowPos(ImVec2(5, 81));
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    if (ImGui::Button("Clear window", ImVec2(150, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    ImGui::End();
  }
}

