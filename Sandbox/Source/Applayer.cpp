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

GameLayer::GameLayer() 
	:	m_CameraController(&m_Camera),
		m_LightingShader("C:/Dev/OpenGL/OpenGL/shaders/vertex.glsl", "C:/Dev/OpenGL/OpenGL/shaders/fragment.glsl"),
		m_LightCubeShader("C:/Dev/OpenGL/OpenGL/shaders/vertex.glsl", "C:/Dev/OpenGL/OpenGL/shaders/Light_source.glsl"),
        m_VertexBuffer(vertices, sizeof(vertices)),
        m_IndexBuffer(indices, sizeof(indices)),
        m_DiffuseMap("C:/Dev/OpenGL/OpenGL/textures/container2.png"),
        m_SpecularMap("C:/Dev/OpenGL/OpenGL/textures/container2_specular.png")
{
	m_Camera.SetPosition({ 0.0f, 0.0f, 3.0f });

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
	
}

void GameLayer::OnUpdate(float ts)
{

}

void GameLayer::OnRender()
{
    glm::mat4 projection = glm::perspective(glm::radians(m_Camera.GetFOV()), 1.778f, 0.1f, 100.0f);
    glm::mat4 view = m_Camera.GetViewMatrix();

}
