#version 330 core
in vec4 vColor;
in float vDepth;
in vec2 uvPos;
out vec4 fColor;
uniform float farPlane;
uniform vec3 provinceColors[1000];
uniform float blackSpaceAlpha;
uniform float textureColorImportance;

uniform sampler2D backgroundTexture;
uniform sampler2D tex;
uniform int selected;

vec4 lerpRGB(vec4 a, vec4 b, float t)
{
    return vec4(a+((b-a)*t));
}

void main()
{
   float v = vDepth/farPlane;
   gl_FragDepth = v;


   vec4 col = texture2D(tex, uvPos);

   vec4 texCol = texture2D(backgroundTexture, uvPos);
   if (col.rgb == vec3(0,0,0))
       fColor = texCol * vec4(1, 1, 1, blackSpaceAlpha);
   else
   {
       int index = int(col.r*255.0f*255.0f) + int(floor(col.g*255.0f));
       if (index != selected)
                fColor = vec4(provinceColors[index], 1);
       else
           fColor = vec4(1, 1, 1, 1);

           fColor = lerpRGB(fColor, texCol, textureColorImportance);
   }
}
