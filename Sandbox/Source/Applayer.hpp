#include <Forge.hpp>

class GameLayer : public Forge::Layer
{
public:
	GameLayer();

private:
	void OnEvent(Forge::Event& event) override;
	void OnUpdate(float ts) override;
	void OnRender() override;
};