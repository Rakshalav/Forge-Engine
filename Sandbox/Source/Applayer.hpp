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
	bool OnKeyBoardPressed(Forge::KeyPressedEvent& event);

private:
	Forge::Ref<Forge::Camera> m_Camera;
	Forge::Ref<Forge::PerspectiveCameraController> m_Controller;

	Forge::Ref<Forge::Shader> m_lightingShader;
};