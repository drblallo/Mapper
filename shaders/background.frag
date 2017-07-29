#version 330 core
in vec4 vColor;
in float vDepth;
in vec2 uvPos;
out vec4 fColor;
uniform float farPlane;
uniform vec3 provinceColors[4000];

uniform sampler2D tex;
void main()
{
   float v = vDepth/farPlane;
   vec4 col = texture2D(tex, uvPos);
   if (col.rgb == vec3(0,0,0))
       fColor = vec4(0, 0, 0, 0);
   else
   {
        vec4 texCol = texture2D(tex, uvPos);
        fColor = vec4(provinceColors[int(texCol.r*255.0f*255.0f) + int(texCol.g*255.0f)], 1)* vColor;
//        fColor = texCol * vColor;
   }
   gl_FragDepth = v;
}
