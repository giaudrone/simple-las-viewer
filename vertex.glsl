#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in float aIntensity;

// uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float maxIntensity;
uniform float minIntensity;
uniform float maxX;
uniform float maxY;
uniform float maxZ;
uniform float minX;
uniform float minY;
uniform float minZ;

out vec3 fragPos;

void main(){
  vec3 floatPosition = vec3(
    (aPosition.x - minX) / (maxX - minX),
    (aPosition.z - minZ) / (maxZ - minZ),
    (aPosition.y - minY) / (maxY - minY));
  floatPosition *= vec3(1.0, -1.0, 1.0);
  gl_Position = projection * view * model * vec4(-5.0 * 10.0 * floatPosition, 1.0);
  fragPos = vec3(model * vec4(floatPosition, 1.0));
}
