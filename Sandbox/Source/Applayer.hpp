#include <Forge.hpp>
#include <Source/Scene/Model.hpp>

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
	fg::Scope<fg::PerspectiveCameraController> m_Controller;

	fg::Ref<fg::Shader> m_Shader;

	fg::Scope<fg::Model> m_Model;
};