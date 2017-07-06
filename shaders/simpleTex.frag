#version 330 core
in vec4 vColor;
in float vDepth;
in vec2 uvPos;
out vec4 fColor;
uniform float farPlane;

uniform sampler2D tex;
void main()
{
   float v = vDepth/farPlane;
   fColor = texture2D(tex, uvPos) * vColor;
   gl_FragDepth = v;
}
