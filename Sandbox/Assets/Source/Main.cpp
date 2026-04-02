#include <Forge.hpp>
#include "Applayer.hpp"

int main()
{
	fg::ApplicationSpecification appSpec;
	appSpec.Name = "Forge Engine";
	appSpec.WindowSpec.Width = 1280;
	appSpec.WindowSpec.Height = 720;

	fg::Application game(appSpec);
	game.PushLayer<GameLayer>();
	game.Run();
}