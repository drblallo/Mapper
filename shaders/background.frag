#version 330 core
in vec4 vColor;
in float vDepth;
in vec2 uvPos;
out vec4 fColor;
uniform float farPlane;
uniform vec3 provinceColors[4000];

uniform sampler2D backgroundTexture;
uniform sampler2D tex;
uniform int selected;
void main()
{
   float v = vDepth/farPlane;
   vec4 col = texture2D(tex, uvPos);
   vec4 texCol = texture2D(backgroundTexture, uvPos);
   if (col.rgb == vec3(0,0,0))
       fColor = vec4(1, 1, 1, 0.5f);
   else
   {
       int index = int(col.r*255.0f*255.0f) + int(col.g*255.0f);
       if (index != selected)
        fColor = vec4(provinceColors[index], 1);
       else
           fColor = vec4(1, 1, 1, 1);
//        fColor = texCol;
//        fColor = texCol * vColor;
   }
   fColor = fColor * texCol * texCol;
   gl_FragDepth = v;
}
