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
    fg::Renderer::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glm::mat4 view_projection = m_Camera->GetViewProjectionMatrix();

    auto& gl_lightingShader = *std::dynamic_pointer_cast<fg::OpenGLShader>(m_lightingShader);
    gl_lightingShader.Bind();

    gl_lightingShader.Set("viewPos", m_Camera->GetPosition());
    gl_lightingShader.Set("material.diffuse", 0);
    m_DiffuseMap->Bind(0);
    gl_lightingShader.Set("material.specular", 1);
    m_SpecularMap->Bind(1);
    gl_lightingShader.Set("material.shininess", 32.0f);

    gl_lightingShader.Set("dirLight.direction", fg::Vec3f(-0.2f, -1.0f, -0.3f));
    gl_lightingShader.Set("dirLight.ambient", fg::Vec3f(0.05f, 0.05f, 0.05f));
    gl_lightingShader.Set("dirLight.diffuse", fg::Vec3f(0.4f, 0.4f, 0.4f));
    gl_lightingShader.Set("dirLight.specular", fg::Vec3f(0.5f, 0.5f, 0.5f));

    for (uint32_t i = 0; i < 4; i++)
    {
        std::string base = "pointLights[" + std::to_string(i) + "].";
        gl_lightingShader.Set(base + "position", pointLightPositions[i]);
        gl_lightingShader.Set(base + "ambient", fg::Vec3f(0.05f));
        gl_lightingShader.Set(base + "diffuse", fg::Vec3f(0.8f));
        gl_lightingShader.Set(base + "specular", fg::Vec3f(1.0f));
        gl_lightingShader.Set(base + "constant", 1.0f);
        gl_lightingShader.Set(base + "linear", 0.09f);
        gl_lightingShader.Set(base + "quadratic", 0.032f);
    }

    gl_lightingShader.Set("spotLight.toggleSpotLight", m_ToggleFlashLight);

    if (m_ToggleFlashLight)
    {
        gl_lightingShader.Set("spotLight.position", m_Camera->GetPosition());
        gl_lightingShader.Set("spotLight.direction", m_Camera->GetFront());
        gl_lightingShader.Set("spotLight.ambient", fg::Vec3f(0.0f));
        gl_lightingShader.Set("spotLight.diffuse", fg::Vec3f(1.0f));
        gl_lightingShader.Set("spotLight.specular", fg::Vec3f(1.0f));
        gl_lightingShader.Set("spotLight.constant", 1.0f);
        gl_lightingShader.Set("spotLight.linear", 0.09f);
        gl_lightingShader.Set("spotLight.quadratic", 0.032f);
        gl_lightingShader.Set("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        gl_lightingShader.Set("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    }

    gl_lightingShader.Set("u_view_projection", view_projection);
    for (uint32_t i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), fg::Vec3f(1.0f, 0.3f, 0.5f));

        gl_lightingShader.Set("u_model", model);
        gl_lightingShader.Set("u_normal", glm::mat3(glm::transpose(glm::inverse(model))));

        fg::Renderer::Draw(m_VertexArrayCube, m_lightingShader);
    }

    auto& gl_lightCubeShader = *std::dynamic_pointer_cast<fg::OpenGLShader>(m_ligtCubeShader);
    gl_lightCubeShader.Bind();
    gl_lightCubeShader.Set("u_view_projection", view_projection);

    for (uint32_t i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
        model = glm::scale(model, fg::Vec3f(0.2f));
        gl_lightCubeShader.Set("u_model", model);
        gl_lightCubeShader.Set("color", fg::Vec3f(1.0f));

        fg::Renderer::Draw(m_VertexArrayLight, m_ligtCubeShader);
    }
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

    if (event.KeyCode == fg::Key::F && !event.IsRepeated)
    {
        if (m_ToggleFlashLight)
        {
            m_ToggleFlashLight = false;
            FG_INFO("Flashlight OFF");
            return true;
        }
        else
        {
            m_ToggleFlashLight = true;
            FG_INFO("Flashlight ON");
            return true;
        }
    }

    return false;
}