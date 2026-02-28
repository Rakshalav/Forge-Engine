#include "../Source/Core/EditorLayer.hpp"

int main()
{
	fg::ApplicationSpecification EditorSpecifications;
	EditorSpecifications.Name = "Forge Editor";
	EditorSpecifications.WindowSpec.Width = 1920;
	EditorSpecifications.WindowSpec.Height = 1080;

	fg::Application Editor(EditorSpecifications);
	Editor.PushLayer<fg::Editor::EditorLayer>();
	Editor.Run();
}