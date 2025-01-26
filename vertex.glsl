#version 330 core

layout (location = 0) in ivec3 aPosition;
layout (location = 1) in uint aIntensity;

// uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform uint maxIntensity;

out vec3 vertexColor;

void main(){
  vec3 floatPosition = vec3(aPosition);
  gl_Position = projection * view * model * vec4(vec3(floatPosition.x / (188624999.0 / 2.0), floatPosition.y / (59124999.0 / 2.0), floatPosition.z / (80905.0 / 2.0)), 1.0);
  // gl_Position = projection * view * model * vec4(floatPosition, 1.0);
  vertexColor = vec3(0.5, aIntensity / (float(maxIntensity) + 1e-5), 0.5);
}
