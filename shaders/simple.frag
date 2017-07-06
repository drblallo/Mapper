#version 330 core
in vec4 vColor;
in float vDepth;
out vec4 fColor;
uniform float farPlane;

void main()
{
   float v = vDepth/farPlane;
   fColor = /*vec4(v, v, v, 1);*/ vColor;
    //= vDepth/1000.0f;
   gl_FragDepth = v;

}
