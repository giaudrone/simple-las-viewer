#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in uint aIntensity;

// uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform uint maxIntensity;

out vec3 vertexColor;

void main(){
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
  vertexColor = vec3(0.5, float(aIntensity / maxIntensity), 0.5);
}
