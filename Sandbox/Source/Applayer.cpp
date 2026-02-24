#include "Applayer.hpp"
#include <Platform/OpenGL/OpenGLShader.hpp>

float vertices[] = {
    // Positions          // Normals           // UVs (TexCoords)
    // Face 1: Back (-Z)
     0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,

     // Face 2: Front (+Z)
    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,

     // Face 3: Left (-X)
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

     // Face 4: Right (+X)
     0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,

      // Face 5: Bottom (-Y)
    -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,

     // Face 6: Top (+Y)
    -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
};

unsigned int indices[] = {
     0,  1,  2,  2,  3,  0, // Back
     4,  5,  6,  6,  7,  4, // Front
     8,  9, 10, 10, 11,  8, // Left
    12, 13, 14, 14, 15, 12, // Right
    16, 17, 18, 18, 19, 16, // Bottom
    20, 21, 22, 22, 23, 20  // Top
};

fg::Vec3f cubePositions[] = {
    fg::Vec3f(0.0f,  0.0f,  0.0f),
    fg::Vec3f(2.0f,  5.0f, -15.0f),
    fg::Vec3f(-1.5f, -2.2f, -2.5f),
    fg::Vec3f(-3.8f, -2.0f, -12.3f),
    fg::Vec3f(2.4f, -0.4f, -3.5f),
    fg::Vec3f(-1.7f,  3.0f, -7.5f),
    fg::Vec3f(1.3f, -2.0f, -2.5f),
    fg::Vec3f(1.5f,  2.0f, -2.5f),
    fg::Vec3f(1.5f,  0.2f, -1.5f),
    fg::Vec3f(-1.3f,  1.0f, -1.5f)
};

fg::Vec3f pointLightPositions[] = {
    fg::Vec3f(0.7f,  0.2f,  2.0f),
    fg::Vec3f(2.3f, -3.3f, -4.0f),
    fg::Vec3f(-4.0f,  2.0f, -12.0f),
    fg::Vec3f(0.0f,  0.0f, -3.0f)
};

GameLayer::GameLayer() : m_Model("C:/Dev/Forge/Sandbox/Textures/Guitar.obj")
{
    m_Camera = fg::CreateRef<fg::Camera>(fg::CameraProjection::Perspective);
    m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });
    m_Camera->SetPerspective(45.0f, 1.778f, 0.1f, 100.0f);

    m_Controller = fg::CreateRef<fg::PerspectiveCameraController>(m_Camera.get());
    m_Controller->SetMouseSensitivity(0.08f);

    fg::Application::Get().GetWindow()->ToggleCursor(false);

}

void GameLayer::OnAttach()
{
    m_Shader = fg::Shader::Create("C:/Dev/Forge/Sandbox/Shaders/vertex_cube.glsl", "C:/Dev/Forge/Sandbox/Shaders/Model_fragment.glsl");
}

void GameLayer::OnEvent(fg::Event& event)
{
    m_Controller->OnEvent(event);

    fg::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<fg::Event::KeyPress>([this](fg::Event::KeyPress& e) { return OnKeyBoardPressed(e); });
}

void GameLayer::OnUpdate(float ts)
{
    m_Controller->OnUpdate(ts);
}

void GameLayer::OnRender()
{
    fg::Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    m_Shader->Bind();

    glm::mat4 view_projection = m_Camera->GetViewProjectionMatrix();
    m_Shader->SetMat4("u_view_projection", view_projection);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
    m_Shader->SetMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));
    m_Shader->SetMat4("u_model", model);
    m_Model.Draw(m_Shader);
}

void GameLayer::OnDetach()
{
    
}

bool GameLayer::OnKeyBoardPressed(fg::Event::KeyPress& event)
{
    if (event.KeyCode == fg::Key::Q)
    {
        fg::Application::Get().Stop();
        return true;
    }
    return false;
}