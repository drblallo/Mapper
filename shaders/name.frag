#version 330 core
in float vDepth;
in vec2 uvPos;
in vec3 worldPos;
uniform float text[20];
uniform int textLenght;
out vec4 fColor;
uniform float farPlane;
uniform vec3 corners[4];

uniform sampler2D tex;

void main()
{
//    if (textLenght == 0)
  //      discard;

   float v = vDepth/farPlane;
   gl_FragDepth = v;

  /* float dim = 1.0/textLenght;

   int letterNumber = int(floor(uvPos.x/dim));
   if (dim/2.0 < abs(uvPos.y - 0.5))
       discard;

   float posXWithin = uvPos.x - (float(letterNumber)*dim);
   posXWithin = posXWithin/dim;
   float posYWithin = (0.5+(dim/2.0)) - uvPos.y;
   vec2 pos = vec2(posXWithin, posYWithin);

   int targetLetter = int(text[letterNumber]);
*/
   //if (textLenght < 1)
    //   fColor = texture2D(tex, uvPos);
//   else
        //fColor = vec4(0, 0,float(letterNumber)/20.0, 1); //texture2D(tex, uvPos);
   fColor = texture2D(tex, uvPos);

}
