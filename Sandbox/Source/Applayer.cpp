#include "Applayer.hpp"
#include <print>

float vertices[] = {
    // Positions             // Normals           // Texture Coords

    // --- Face 1: Back (-Z) ---
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0 (bottom-left)
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 1 (bottom-right)
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // 2 (top-right)
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // 3 (top-left)

    // --- Face 2: Front (+Z) ---
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // 4
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // 5
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // 6
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // 7

    // --- Face 3: Left (-X) ---
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 8
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 9
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 10
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 11

    // --- Face 4: Right (+X) ---
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 12
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 13
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 14
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 15

     // --- Face 5: Bottom (-Y) ---
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // 16
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // 17
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // 18
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // 19

     // --- Face 6: Top (+Y) ---
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // 20
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // 21
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // 22
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // 23
};

unsigned int indices[] = {
    // Face 1: Back 
    0, 1, 2,
    2, 3, 0,

    // Face 2: Front 
    4, 5, 6,
    6, 7, 4,

    // Face 3: Left 
    8, 9, 10,
    10, 11, 8,

    // Face 4: Right 
    12, 13, 14,
    14, 15, 12,

    // Face 5: Bottom 
    16, 17, 18,
    18, 19, 16,

    // Face 6: Top 
    20, 21, 22,
    22, 23, 20
};

auto& renderer = Forge::Renderer::GetInstance();

GameLayer::GameLayer() 
	:   m_CameraController(&m_Camera),
		m_LightingShader("C:/Dev/OpenGL/OpenGL/shaders/vertex.glsl", "C:/Dev/OpenGL/OpenGL/shaders/fragment.glsl"),
		m_LightCubeShader("C:/Dev/OpenGL/OpenGL/shaders/vertex.glsl", "C:/Dev/OpenGL/OpenGL/shaders/Light_source.glsl"),
        m_VertexBuffer(vertices, sizeof(vertices)),
        m_IndexBuffer(indices, sizeof(indices)),
        m_DiffuseMap("C:/Dev/OpenGL/OpenGL/textures/container2.png"),
        m_SpecularMap("C:/Dev/OpenGL/OpenGL/textures/container2_specular.png")
{
	m_Camera.SetPosition({ 0.0f, 0.0f, 3.0f });
    m_CameraController.SetMouseSensitivity(0.09f);

    Forge::BufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.Push(3, GL_FLOAT);
    layout.Push(2, GL_FLOAT);

    m_VertexArrayCube.AddBuffer(m_VertexBuffer, layout);
    m_VertexArrayLightCube.AddBuffer(m_VertexBuffer, layout);

    m_LightingShader.use();
    m_LightingShader.set("material.diffuse", 0);
    m_LightingShader.set("material.specular", 1);
}

void GameLayer::OnEvent(Forge::Event& event)
{
    m_CameraController.OnEvent(event);
}

void GameLayer::OnUpdate(float ts)
{
    m_CameraController.OnUpdate(ts);
}

void GameLayer::OnRender()
{
    glm::mat4 viewProjMatrix = m_Camera.GetViewProjectionMatrix();

    m_LightingShader.set("light.position", m_LightPos);
    m_LightingShader.set("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    m_LightingShader.set("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    m_LightingShader.set("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    m_LightingShader.set("material.shininess", 64.0f);
    m_LightingShader.set("viewPos", m_Camera.GetPosition());
    m_LightingShader.set("view_projection", viewProjMatrix);

    glm::mat4 cubeModel(1.0f);
    m_LightingShader.set("model", cubeModel);

    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
    m_LightingShader.set("normalMatrix", normalMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DiffuseMap.ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_SpecularMap.ID);

    renderer.Draw(m_VertexArrayCube, m_IndexBuffer, m_LightingShader);

    m_LightCubeShader.set("view_projection", viewProjMatrix);

    glm::mat4 lightModel(1.0f);
    lightModel = glm::translate(lightModel, m_LightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    m_LightCubeShader.set("model", lightModel);

    renderer.Draw(m_VertexArrayLightCube, m_IndexBuffer, m_LightCubeShader);
}