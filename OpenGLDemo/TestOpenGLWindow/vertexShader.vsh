attribute highp vec4 qt_Vertex;                      // 顶点坐标数组
attribute highp vec4 qt_MultiTexCoord0;              // 顶点颜色数组
uniform highp mat4 qt_ModelViewProjectionMatrix;     // 模型视图投影矩阵(用于缩放、旋转等设置)
varying highp vec4 qt_TexCoord0;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;   // 顶点位置坐标
    qt_TexCoord0 = qt_MultiTexCoord0;
}
