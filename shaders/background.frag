#version 330 core
in vec4 vColor;
in float vDepth;
in vec2 uvPos;
out vec4 fColor;
uniform float farPlane;
layout (std140) uniform PCol
{
    vec4 provinceColors[1000];
};
uniform float blackSpaceAlpha;
uniform float textureColorImportance;

uniform sampler2D backgroundTexture;
uniform mediump sampler2D tex;
uniform int selected;

vec4 lerpRGB(vec4 a, vec4 b, float t)
{
    return vec4(a+((b-a)*t));
}

int indexFromColor(vec3 col)
{
    int r = int(round((col.r*255.0)/25.0));
    int g = int(round((col.g*255.0)/25.0))*10;
    int b = int(round((col.b*255.0)/25.0))*100;
    return  r + g + b;
}

void main()
{
   float v = vDepth/farPlane;
   gl_FragDepth = v;


   vec4 col = texture2D(tex, uvPos);
/*   if (col != vColor)
   {
       discard;
   }
   */
  // fColor = col;
  // return;

   vec4 texCol = texture2D(backgroundTexture, uvPos);
   if (col.rgb == vec3(0,0,0))
       fColor = texCol * vec4(1, 1, 1, blackSpaceAlpha);
   else
   {
       int index = indexFromColor(col.rgb);
       if (index != selected)
                fColor = provinceColors[index];
       else
           fColor = vec4(1, 1, 1, 1);

           fColor = lerpRGB(fColor, texCol, textureColorImportance);
   }
}
