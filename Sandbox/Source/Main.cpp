#include <Source/Core/Application.hpp>
#include "Applayer.hpp"

int main()
{
	Forge::ApplicationSpecification appSpec;
	appSpec.Name = "LearnOpenGL";
	appSpec.WindowSpec.Width = 900;
	appSpec.WindowSpec.Height = 900;

	Forge::Application game(appSpec);
	game.PushLayer<GameLayer>();
	game.Run();
}