#include <Forge.hpp>

class GameLayer : public Forge::Layer
{
public:
	GameLayer();

private:
	void onEvent(Forge::Event& event) override;
};