#include <Forge.hpp>

class GameLayer : public Forge::Layer
{
public:
	GameLayer();

private:
	void OnEvent(Forge::Event& event) override;
	void OnUpdate(float ts) override;
	void OnRender() override;

	void OnAttach() override;
	void OnDetach() override;

private:
	bool OnKeyBoardPressed(Forge::KeyPressedEvent& event);

private:
	Forge::Ref<Forge::Camera> m_Camera;
	Forge::Ref<Forge::PerspectiveCameraController> m_Controller;

	Forge::Ref<Forge::Shader> m_lightingShader;
	Forge::Ref<Forge::Shader> m_ligtCubeShader;

	Forge::Ref<Forge::VertexArray> m_VertexArrayCube;
	Forge::Ref<Forge::VertexArray> m_VertexArrayLight;


	glm::vec3 m_lightPos = { 1.2f, 1.0f, 2.0f };
};