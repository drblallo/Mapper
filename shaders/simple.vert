#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec4 vColor;
out float vDepth;
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform float farPlane;

void main()
{
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  vColor = vec4(color, 1);
  vDepth = gl_Position.z;
}
