#version 330 core
in vec4 vColor;
in float vDepth;
in vec2 uvPos;
in float provinceIndex;
in float otherRegionIndex;
out vec4 fColor;
uniform float farPlane;
uniform vec3 provincesColor[4000];

uniform sampler2D tex;
void main()
{
    if (provincesColor[int(provinceIndex)] == provincesColor[int(otherRegionIndex)])
            discard;
   float v = vDepth/farPlane;
   //fColor = texture2D(tex, uvPos) * provincesColor[int(provinceIndex)] * vColor ;
   fColor = texture2D(tex, uvPos)* vec4(provincesColor[int(provinceIndex)].rgb, 1) * vColor;
   gl_FragDepth = v;
}
