#version 330 core
in float vDepth;
in vec2 uvPos;
in float targetLetter;
out vec4 fColor;
uniform float farPlane;

uniform sampler2D tex;
uniform sampler2D letters[26];

void main()
{

   float v = vDepth/farPlane;
   gl_FragDepth = v;


   int target = int(floor(targetLetter+0.5));
   if (target == 0)
   {
       fColor = texture2D(letters[0], uvPos);
        return;
   }
   if (target == 1)
   {
       fColor = texture2D(letters[1], uvPos);
        return;
   }
   if (target == 2)
   {
       fColor = texture2D(letters[2], uvPos);
        return;
   }
   if (target == 3)
   {
       fColor = texture2D(letters[3], uvPos);
        return;
   }
   if (target == 4)
   {
       fColor = texture2D(letters[4], uvPos);
        return;
   }
   if (target == 5)
   {
       fColor = texture2D(letters[5], uvPos);
        return;
   }
   if (target == 6)
   {
       fColor = texture2D(letters[6], uvPos);
        return;
   }
   if (target == 7)
   {
       fColor = texture2D(letters[7], uvPos);
        return;
   }
   if (target == 8)
   {
       fColor = texture2D(letters[8], uvPos);
        return;
   }
   if (target == 9)
   {
       fColor = texture2D(letters[9], uvPos);
        return;
   }
   if (target == 10)
   {
       fColor = texture2D(letters[10], uvPos);
        return;
   }
   if (target == 11)
   {
       fColor = texture2D(letters[11], uvPos);
        return;
   }
   if (target == 12)
   {
       fColor = texture2D(letters[12], uvPos);
        return;
   }
   if (target == 13)
   {
       fColor = texture2D(letters[13], uvPos);
        return;
   }
   if (target == 14)
   {
       fColor = texture2D(letters[14], uvPos);
        return;
   }
   if (target == 15)
   {
       fColor = texture2D(letters[15], uvPos);
        return;
   }
   if (target == 16)
   {
       fColor = texture2D(letters[16], uvPos);
        return;
   }
   if (target == 17)
   {
       fColor = texture2D(letters[17], uvPos);
        return;
   }
   if (target == 18)
   {
       fColor = texture2D(letters[18], uvPos);
        return;
   }
   if (target == 19)
   {
       fColor = texture2D(letters[19], uvPos);
        return;
   }
   if (target == 20)
   {
       fColor = texture2D(letters[20], uvPos);
        return;
   }
   if (target == 21)
   {
       fColor = texture2D(letters[21], uvPos);
        return;
   }
   if (target == 22)
   {
       fColor = texture2D(letters[22], uvPos);
        return;
   }
   if (target == 23)
   {
       fColor = texture2D(letters[23], uvPos);
        return;
   }
   if (target == 24)
   {
       fColor = texture2D(letters[24], uvPos);
        return;
   }
   if (target == 25)
   {
       fColor = texture2D(letters[25], uvPos);
        return;
   }


}
