#version 330 core
in  vec2 TexCord;            // 纹理坐标
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main()
{
    vec3 yuv;
    vec3 rgb;

    // YUV转RGB
    yuv.x = texture2D(tex_y, TexCord).r;
    yuv.y = texture2D(tex_u, TexCord).r-0.5;
    yuv.z = texture2D(tex_v, TexCord).r-0.5;

    rgb = mat3(1.0, 1.0, 1.0,
               0.0, -0.39465, 2.03211,
               1.13983, -0.58060, 0.0) * yuv;
    gl_FragColor = vec4(rgb, 1.0);
}
