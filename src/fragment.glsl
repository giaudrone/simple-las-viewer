#version 330 core

const float PI = 3.14159265359;


in vec3 fragPos;
in float intensity;
in vec3 colors;
in vec3 intensityColors;

// uniform float maxIntensity;
// uniform float minIntensity;
// uniform float minY;
// uniform float maxY;

uniform int colorMode;

out vec4 FragColor;

// float colorShiftRed(float value) {
//   return (cos(PI + PI * (((0.01 * value) - minY) / (maxY - minY)))) / 2.0 + 0.5;
// }
//
// float colorShiftGreen(float value) {
//   return (sin(PI * (((0.01 * value) - minY) / (maxY - minY))));
// }
//
// float colorShiftBlue(float value) {
//   return (-cos(PI + PI * (((0.01 * value) - minY) / (maxY - minY)))) / 2.0 + 0.5;
// }

// float intensityStrength(float intensity){
//   return (intensity - minIntensity) / (maxIntensity - minIntensity);
// }

void main(){
  if(colorMode == 1)
    FragColor = vec4(colors, 1.0);
  if(colorMode == 2)
    FragColor = vec4(intensityColors, 1.0);
  if(colorMode == 3)
    FragColor = vec4(mix(colors, intensityColors), 1.0);
}
