#include "Applayer.hpp"
#include <print>

GameLayer::GameLayer()
{
	std::println("Hello World");
}

void GameLayer::onEvent(Forge::Event& event)
{
	std::println("{}", event.ToString());
}
