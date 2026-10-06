#version 330 core
out vec4 FragColor;

in vec2 TextureCoord;
uniform sampler2D texture_diffuse;

void main()
{   
    vec4 texColor = texture(texture_diffuse, TextureCoord);
    // 알파 값이 일정 기준 미만이면 알파 블렌딩 대신 해당 픽셀을 완전히 버림 (Alpha Cutout)
    if (texColor.a < 0.1)
        discard;
        
    FragColor = texColor;
}