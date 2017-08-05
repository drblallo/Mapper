#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in float letter;
out float vDepth;
out vec2 uvPos;
out float targetLetter;
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

uniform sampler2D tex;

void main()
{
  targetLetter = letter;
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  vDepth = gl_Position.z;
  uvPos = uv;
}
