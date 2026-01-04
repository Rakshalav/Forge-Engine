#include <Forge.hpp>
#include "Applayer.hpp"

int main()
{
	Forge::ApplicationSpecification appSpec;
	appSpec.Name = "LearnOpenGL";
	appSpec.WindowSpec.Width = 1280;
	appSpec.WindowSpec.Height = 720;

	Forge::Application game(appSpec);
	game.PushLayer<GameLayer>();
	game.Run();
}