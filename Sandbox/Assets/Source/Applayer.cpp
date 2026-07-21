#include "Applayer.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>


GameLayer::GameLayer() 
{
    YAML::Emitter out;

    // 2. Start writing data
    out << YAML::BeginMap;           // Starts the { 
    out << YAML::Key << "Message";   // Key
    out << YAML::Value << "Hello World"; // Value
    out << YAML::EndMap;             // Ends the }

    // 3. Dump the emitter content into a file
    std::ofstream fout("hello.yaml");
    fout << out.c_str();
}

void GameLayer::OnAttach()
{

}

void GameLayer::OnEvent(fg::Event& event)
{
   
}

void GameLayer::OnUpdate(float ts)
{
 }

void GameLayer::OnRender()
{

}

void GameLayer::OnDetach()
{
    
}