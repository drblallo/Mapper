#version 330 core
in float vDepth;
in vec2 uvPos;
out vec4 fColor;
uniform float farPlane;

uniform sampler2D tex;
void main()
{
   float v = vDepth/farPlane;
   fColor = texture2D(tex, uvPos);
   if (fColor.a < 0.5)
       discard;
   gl_FragDepth = v;
   fColor = vec4(0, 0, 0, 1);
  // fColor = vec4(1, 1, 1, 1);
}
