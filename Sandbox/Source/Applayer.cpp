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

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
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
    bufferlayout.Push<fg::LayoutType::PosNormTex>(fg::ElementType::FLOAT);

    fg::Ref<fg::VertexBuffer> vertexbuffer = fg::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexbuffer->SetLayout(bufferlayout);

    fg::Ref<fg::IndexBuffer> indexbuffer = fg::IndexBuffer::Create(indices, sizeof(indices));

    m_VertexArrayCube = fg::VertexArray::Create();
    m_VertexArrayCube->AddVertexBuffer(vertexbuffer);
    m_VertexArrayCube->SetIndexBuffer(indexbuffer);

    m_VertexArrayLight = fg::VertexArray::Create();
    m_VertexArrayLight->AddVertexBuffer(vertexbuffer);
    m_VertexArrayLight->SetIndexBuffer(indexbuffer);

    m_DiffuseMap = fg::Texture2D::Create("C:/Dev/Forge/Sandbox/Textures/container2.png");
    m_SpecularMap = fg::Texture2D::Create("C:/Dev/Forge/Sandbox/Textures/container2_specular.png");
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
    glm::mat4 view_projection = m_Camera->GetViewProjectionMatrix();
    
    auto& gl_lightingShader = *std::dynamic_pointer_cast<fg::OpenGLShader>(m_lightingShader);
    gl_lightingShader.Bind();
        
    gl_lightingShader.Set("light.position", m_Camera->GetPosition());
    gl_lightingShader.Set("light.direction", m_Camera->GetFront());
    gl_lightingShader.Set("light.cutOff", glm::cos(glm::radians(12.5f)));
    gl_lightingShader.Set("light.outerCutOff", glm::cos(glm::radians(17.5f)));

    gl_lightingShader.Set("light.ambient", lightColor * 0.2f);
    gl_lightingShader.Set("light.diffuse", lightColor * 0.5f);
    gl_lightingShader.Set("light.specular", fg::Vec3f(1.0f));

    gl_lightingShader.Set("light.constant", 1.0f);
    gl_lightingShader.Set("light.linear", 0.09f);
    gl_lightingShader.Set("light.quadratic", 0.032f);

    gl_lightingShader.Set("material.ambient", fg::Vec3f(1.0f, 0.5f, 0.31f));

    gl_lightingShader.Set("material.diffuse", 0);
    m_DiffuseMap->Bind(0);

    gl_lightingShader.Set("material.specular", 1);
    m_SpecularMap->Bind(1);

    gl_lightingShader.Set("material.shininess", 64.0f);

    gl_lightingShader.Set("viewPos", m_Camera->GetPosition());

    gl_lightingShader.Set("u_view_projection", view_projection);

    for (uint32_t i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), fg::Vec3f(1.0f, 0.3f, 0.5f));
        gl_lightingShader.Set("u_model", model);

        auto normal = glm::mat3(glm::transpose(glm::inverse(model)));
        gl_lightingShader.Set("u_normal", normal);

        fg::Renderer::Draw(m_VertexArrayCube, m_lightingShader);
    }

    auto& gl_lightCubeShader = *std::dynamic_pointer_cast<fg::OpenGLShader>(m_ligtCubeShader);
    gl_lightCubeShader.Bind();

    gl_lightCubeShader.Set("color", lightColor);

    gl_lightCubeShader.Set("u_view_projection", view_projection);

    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), m_lightPos);
    lightModel = glm::scale(lightModel, fg::Vec3f(0.2f));
    gl_lightCubeShader.Set("u_model", lightModel);

    fg::Renderer::Draw(m_VertexArrayLight, m_ligtCubeShader);
}

void GameLayer::OnDetach()
{
    
}

bool GameLayer::OnKeyBoardPressed(fg::Event::KeyPress& event)
{
    if (event.KeyCode == fg::Key::Q)
    {
        fg::Application::Get().Close();
        return true;
    }

    return false;
}