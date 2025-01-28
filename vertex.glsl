#version 330 core

layout (location = 0) in ivec3 aPosition;
layout (location = 1) in int aIntensity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float centerX;
uniform float centerY;
uniform float centerZ;

out vec3 fragPos;
out float intensity;

void main(){
  vec3 floatPosition = vec3(
    0.01 * aPosition.x - centerX,
    0.01 * aPosition.y - centerY, 
    0.01 * aPosition.z - centerZ
  );
  gl_Position = projection * view * model * vec4(floatPosition, 1.0);
  // fragPos = vec3(model * vec4(floatPosition, 1.0));
  intensity = float(aIntensity);
  fragPos = vec3(aPosition);
}
