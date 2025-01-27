#version 330 core

const float PI = 3.14159265359;

in vec3 fragPos;

uniform float maxIntensity;
uniform float minIntensity;
uniform float minY;
uniform float maxY;

out vec4 FragColor;

float verticalColorShiftRed(vec3 fragPos) {
  return (cos(PI + PI * (((0.01 * fragPos.z) - minY) / (maxY - minY)))) / 2.0 + 0.5;
}

float verticalColorShiftGreen(vec3 fragPos) {
  return (sin(PI * (((0.01 * fragPos.z) - minY) / (maxY - minY))));
}

float verticalColorShiftBlue(vec3 fragPos) {
  return (-cos(PI + PI * (((0.01 * fragPos.z) - minY) / (maxY - minY)))) / 2.0 + 0.5;
}

void main(){
  // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  FragColor = vec4(verticalColorShiftRed(fragPos), verticalColorShiftGreen(fragPos), verticalColorShiftBlue(fragPos), 1.0);
  // vertexColor = vec3(0.5, (aIntensity - minIntensity) / (maxIntensity - minIntensity), 0.5);
}
