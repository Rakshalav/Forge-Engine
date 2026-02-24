#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material 
{
    sampler2D texture_diffuse1;
}; 
uniform Material material;

void main()
{    
    FragColor = texture(material.texture_diffuse1, TexCoords);
    //FragColor = vec4(TexCoords, 0.0, 1.0);
}