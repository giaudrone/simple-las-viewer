#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in float aIntensity;
layout (location = 2) in vec3 aColors;
layout (location = 3) in vec3 aIntensityColors;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float centerX;
uniform float centerY;
uniform float centerZ;

uniform float xScaleFactor;
uniform float yScaleFactor;
uniform float zScaleFactor;

out vec3 fragPos;
out float intensity;
out vec3 colors;
out vec3 intensityColors;

void main(){
  vec3 floatPosition = vec3(
    xScaleFactor * aPosition.x - centerX,
    yScaleFactor * aPosition.y - centerY, 
    zScaleFactor * aPosition.z - centerZ
  );
  gl_Position = projection * view * model * vec4(floatPosition, 1.0);
  intensity = aIntensity;
  fragPos = aPosition;
  colors = aColors;
  intensityColors = aIntensityColors;
}
