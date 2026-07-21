#include "../Source/Core/EditorLayer.hpp"

int main()
{
	fg::ApplicationSpecification EditorSpecifications;
	EditorSpecifications.Name = "Forge Editor (Asset Manager) 1.1";
	EditorSpecifications.WindowSpec.Width = 1920;
	EditorSpecifications.WindowSpec.Height = 1080;

	fg::Application Editor(EditorSpecifications);
	Editor.PushLayer<Editor::EditorLayer>();
	Editor.Run();
}