#version 330 core
out vec4 FragColor;
in  vec2 TexCord;            // 纹理坐标
uniform sampler2D texture;  // 纹理采样器
void main()
{
    FragColor = texture2D(texture, TexCord);  // 采样纹理函数
}
