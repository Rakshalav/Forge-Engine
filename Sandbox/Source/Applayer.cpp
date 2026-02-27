#include "Applayer.hpp"
#include <Platform/OpenGL/OpenGLShader.hpp>

GameLayer::GameLayer() 
{
    m_Camera = fg::CreateRef<fg::Camera>(fg::CameraProjection::Perspective);
    m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

    m_Controller = fg::CreateScope<fg::PerspectiveCameraController>(m_Camera.get());
    m_Controller->SetMouseSensitivity(0.08f);

    fg::Application::Get().GetWindow()->ToggleCursor(false);
}

void GameLayer::OnAttach()
{
    m_Shader = fg::Shader::Create("C:/Dev/Forge/Sandbox/Shaders/vertex_cube.glsl", "C:/Dev/Forge/Sandbox/Shaders/Model_fragment.glsl");
    m_Model = fg::CreateScope<fg::Model>("C:/Dev/Forge/Sandbox/Textures/Guitar/Guitar.obj");
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
    fg::RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    m_Shader->Bind();
    fg::Renderer::BeginScene(*m_Camera.get());

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
    m_Shader->SetMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));
    m_Shader->SetMat4("u_model", model);
    m_Model->Draw(m_Shader);
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