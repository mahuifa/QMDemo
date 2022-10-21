#version 330 core
in  vec2 TexCord;                // 纹理坐标
uniform int      format = -1;    // 像素格式
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;
uniform sampler2D tex_uv;

void main()
{
    vec3 yuv;
    vec3 rgb;


    if(format == 0)           // YUV420P转RGB
    {
        yuv.x = texture2D(tex_y, TexCord).r;
        yuv.y = texture2D(tex_u, TexCord).r-0.5;
        yuv.z = texture2D(tex_v, TexCord).r-0.5;
    }
    else if(format == 23)     // NV12转RGB
    {
        yuv.x = texture2D(tex_y, TexCord.st).r;
        yuv.y = texture2D(tex_uv, TexCord.st).r - 0.5;
        yuv.z = texture2D(tex_uv, TexCord.st).g - 0.5;
    }
    else
    {
    }

    rgb = mat3(1.0, 1.0, 1.0,
               0.0, -0.39465, 2.03211,
               1.13983, -0.58060, 0.0) * yuv;
    gl_FragColor = vec4(rgb, 1.0);
}
