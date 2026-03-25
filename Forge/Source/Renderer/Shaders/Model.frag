#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_ambient1;
    sampler2D texture_emmisive1;
    sampler2D texture_normal1;  
};
uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

void main()
{    
    FragColor = texture(material.texture_diffuse1, TexCoords);
    //FragColor = vec4(TexCoords, 0.0, 1.0);
}