#version 330 core
layout(location = 0) in highp vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in float num;
layout(location = 3) in highp vec3 otherPos;

out vec4 vColor;
out float vDepth;
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform highp float farPlane;
uniform highp float timeLeft;
uniform highp float vertexCount;

void main()
{
    highp vec3 vec = position;
    highp float val = ((float(timeLeft)-float(num))/float(vertexCount));
    if (num < 1.0)
    {
        val = val * float(4.0);
        if (val < float(0.0))
            val = float(0.0);
        if (val > float(1.0))
            val = float(1.0);
        //vec = mix(otherPos, position, val);
        highp float other = float(1.0f) - val;
        vec.r = (otherPos.r*(float(other))) + (position.r*float(val));
        vec.g = (otherPos.g*(float(other))) + (position.g*float(val));
        vec.b = (otherPos.b*(float(other))) + (position.b*float(val));

    }
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(vec, 1.0);

  vColor = vec4(color, 1);
  vColor.a = val;
  vDepth = gl_Position.z;
}
