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
   vec4 col = texture2D(tex, uvPos);
   if (col.rgb == vec3(0,0,0))
       fColor = vec4(col.rgb, 0);
   else
        fColor = texture2D(tex, uvPos) * vColor;
   gl_FragDepth = v;
}
