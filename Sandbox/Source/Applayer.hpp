#include <Forge.hpp>

class GameLayer : public fg::Layer
{
public:
	GameLayer();

private:
	void OnEvent(fg::Event& event) override;
	void OnUpdate(float ts) override;
	void OnRender() override;

	void OnAttach() override;
	void OnDetach() override;

private:
	bool OnKeyBoardPressed(fg::Event::KeyPress& event);

private:
	fg::Ref<fg::Camera> m_Camera;
	fg::Ref<fg::PerspectiveCameraController> m_Controller;

	fg::Ref<fg::Shader> m_lightingShader;
	fg::Ref<fg::Shader> m_ligtCubeShader;

	fg::Ref<fg::VertexArray> m_VertexArrayCube;
	fg::Ref<fg::VertexArray> m_VertexArrayLight;

	glm::vec3 m_lightPos = { 1.2f, 1.0f, 2.0f };
	glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };

	fg::Ref<fg::Texture2D> m_DiffuseMap;
	fg::Ref<fg::Texture2D> m_SpecularMap;

	bool m_ToggleFlashLight = false;
};