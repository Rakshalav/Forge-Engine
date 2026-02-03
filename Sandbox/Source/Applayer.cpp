#include "Applayer.hpp"
#include <print>

#include <Platform/OpenGL/OpenGLShader.hpp>

float vertices[] = {
    // Positions          // Normals
    // Face 1: Back (-Z)
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 0: bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 1: bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 2: top-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 3: top-left

    // Face 2: Front (+Z)
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 4: bottom-left
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 5: bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 6: top-right
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 7: top-left

    // Face 3: Left (-X)
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // 8: bottom-left
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // 9: bottom-right
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // 10: top-right
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // 11: top-left

    // Face 4: Right (+X)
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 12: bottom-left
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // 13: bottom-right
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // 14: top-right
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 15: top-left

     // Face 5: Bottom (-Y)
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // 16: top-right (relative to bottom view)
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // 17: top-left
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // 18: bottom-left
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // 19: bottom-right

     // Face 6: Top (+Y)
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 20: bottom-left
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 21: bottom-right
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // 22: top-right
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f  // 23: top-left
};

unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,       // Back
    4, 5, 6, 6, 7, 4,       // Front
    8, 9, 10, 10, 11, 8,    // Left
    12, 13, 14, 14, 15, 12, // Right
    16, 17, 18, 18, 19, 16, // Bottom
    20, 21, 22, 22, 23, 20  // Top
};

GameLayer::GameLayer() 
{
    m_Camera = fg::CreateRef<fg::Camera>(fg::CameraProjection::Perspective);
    m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

    m_Controller = fg::CreateRef<fg::PerspectiveCameraController>(m_Camera.get());
    m_Controller->SetMouseSensitivity(0.08f);

    fg::Application::Get().GetWindow()->ToggleCursor(false);
}

void GameLayer::OnAttach()
{
    m_lightingShader = fg::Shader::Create("C:/Dev/Forge/Sandbox/Shaders/vertex_cube.glsl", "C:/Dev/Forge/Sandbox/Shaders/fragment_cube.glsl");
    m_ligtCubeShader = fg::Shader::Create("C:/Dev/Forge/Sandbox/Shaders/vertex_light.glsl", "C:/Dev/Forge/Sandbox/Shaders/fragment_light.glsl");

    fg::BufferLayout bufferlayout;
    bufferlayout.Push<fg::LayoutType::PosNorm>(fg::ElementType::FLOAT);

    fg::Ref<fg::VertexBuffer> vertexbuffer = fg::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexbuffer->SetLayout(bufferlayout);

    fg::Ref<fg::IndexBuffer> indexbuffer = fg::IndexBuffer::Create(indices, sizeof(indices));

    m_VertexArrayCube = fg::VertexArray::Create();
    m_VertexArrayCube->AddVertexBuffer(vertexbuffer);
    m_VertexArrayCube->SetIndexBuffer(indexbuffer);

    m_VertexArrayLight = fg::VertexArray::Create();
    m_VertexArrayLight->AddVertexBuffer(vertexbuffer);
    m_VertexArrayLight->SetIndexBuffer(indexbuffer);
}

void GameLayer::OnEvent(fg::Event& event)
{
    m_Controller->OnEvent(event);

    FG_TRACE(event.ToString());

    fg::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<fg::Event::KeyPressed>([this](fg::Event::KeyPressed& e) { return OnKeyBoardPressed(e); });
}

void GameLayer::OnUpdate(float ts)
{
    m_Controller->OnUpdate(ts);
}

void GameLayer::OnRender()
{
    glm::mat4 view_projection = m_Camera->GetViewProjectionMatrix();

    float time = fg::Application::Get().GetTime();

    m_lightPos.x = cos(time);
    m_lightPos.z = sin(time);
    
    auto& gl_lightingShader = *std::dynamic_pointer_cast<fg::OpenGLShader>(m_lightingShader);
    gl_lightingShader.Bind();

    gl_lightingShader.Set("light.position", m_lightPos);
    gl_lightingShader.Set("light.ambient", fg::Vec3f(0.2f));
    gl_lightingShader.Set("light.diffuse", fg::Vec3f(0.5f));
    gl_lightingShader.Set("light.specular", fg::Vec3f(1.0f));

    gl_lightingShader.Set("material.ambient", fg::Vec3f(1.0f, 0.5f, 0.31f));
    gl_lightingShader.Set("material.diffuse", fg::Vec3f(1.0f, 0.5f, 0.31f));
    gl_lightingShader.Set("material.specular", fg::Vec3f(0.5f, 0.5f, 0.5f));
    gl_lightingShader.Set("material.shininess", 64.0f);

    gl_lightingShader.Set("viewPos", m_Camera->GetPosition());
    gl_lightingShader.Set("u_view_projection", view_projection);
    gl_lightingShader.Set("u_model", glm::mat4(1.0f));

    fg::Renderer::Draw(m_VertexArrayCube, m_lightingShader);

    auto& gl_lightCubeShader = *std::dynamic_pointer_cast<fg::OpenGLShader>(m_ligtCubeShader);
    gl_lightCubeShader.Bind();

    gl_lightCubeShader.Set("u_view_projection", view_projection);

    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), m_lightPos);
    lightModel = glm::scale(lightModel, fg::Vec3f(0.2f));
    gl_lightCubeShader.Set("u_model", lightModel);

    fg::Renderer::Draw(m_VertexArrayLight, m_ligtCubeShader);
}

void GameLayer::OnDetach()
{

}

bool GameLayer::OnKeyBoardPressed(fg::Event::KeyPressed& event)
{
    if (event.KeyCode == fg::KeyBoard::Q && !event.IsRepeated)
    {
        fg::Application::Get().Close();
        return true;
    }

    return false;
}