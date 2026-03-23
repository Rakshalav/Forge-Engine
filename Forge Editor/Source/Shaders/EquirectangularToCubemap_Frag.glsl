#version 460 core
in vec3 TexCoords;
out vec4 FragColor;

uniform sampler2D u_EquirectMap;

const vec2 invAtan = vec2(0.1591f, 0.3183f); // (1/2PI, 1/PI)

void main()
{
    vec3 dir = normalize(TexCoords);
    vec2 uv = vec2(atan(dir.z, dir.x), asin(dir.y));
    
    uv *= invAtan;
    uv += 0.5f;
    
    FragColor = texture(u_EquirectMap, uv);
}