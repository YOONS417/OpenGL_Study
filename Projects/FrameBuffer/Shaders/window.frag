#version 330 core
out vec4 FragColor;

in vec2 TextureCoord;
uniform sampler2D texture_diffuse;

void main()
{   
    vec4 texColor = texture(texture_diffuse, TextureCoord);
    FragColor = texColor;
}