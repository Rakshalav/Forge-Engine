#include <Forge.hpp>

class GameLayer : public Forge::Layer
{
public:
	GameLayer();

private:
	void OnEvent(Forge::Event& event) override;
	void OnUpdate(float ts) override;
	void OnRender() override;

private:
	Forge::PerspectiveCamera m_Camera;
	Forge::PerspectiveCameraController m_CameraController;

	Forge::Shader m_LightingShader;
	Forge::Shader m_LightCubeShader;

	Forge::VertexBuffer m_VertexBuffer;
	Forge::IndexBuffer m_IndexBuffer;

	Forge::VertexArray m_VertexArrayCube;
	Forge::VertexArray m_VertexArrayLightCube;

	Forge::Texture m_DiffuseMap;
	Forge::Texture m_SpecularMap;


};