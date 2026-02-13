#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_model;
uniform mat4 u_view_projection;
uniform mat3 u_normal;

void main()
{

    Normal = u_normal * aNormal;

    vec4 worldPos = u_model * vec4(aPos, 1.0);
    FragPos = vec3(worldPos); 
    
    TexCoords = aTexCoords;
    
    gl_Position = u_view_projection * worldPos;
}