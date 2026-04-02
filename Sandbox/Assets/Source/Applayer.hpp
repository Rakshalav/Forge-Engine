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
};