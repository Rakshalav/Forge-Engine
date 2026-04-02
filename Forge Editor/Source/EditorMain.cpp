#include "../Source/Core/EditorLayer.hpp"

int main()
{
	fg::ApplicationSpecification EditorSpecifications;
	EditorSpecifications.Name = "Forge Editor 1.0.0 Alpha";
	EditorSpecifications.WindowSpec.Width = 1920;
	EditorSpecifications.WindowSpec.Height = 1080;

	fg::Application Editor(EditorSpecifications);
	Editor.PushLayer<Editor::EditorLayer>();
	Editor.Run();
}