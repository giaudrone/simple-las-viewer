#version 330 core

const float PI = 3.14159265359;


in vec3 fragPos;
in float intensity;

uniform float maxIntensity;
uniform float minIntensity;
uniform float minY;
uniform float maxY;

uniform int colorMode;

out vec4 FragColor;

float colorShiftRed (float value)
{
  return (cos(PI + PI * (((0.01 * value) - minY) / (maxY - minY)))) / 2.0 + 0.5;
}

float colorShiftGreen (float value)
{
  return (sin(PI * (((0.01 * value) - minY) / (maxY - minY))));
}

float colorShiftBlue (float value)
{
  return (-cos(PI + PI * (((0.01 * value) - minY) / (maxY - minY)))) / 2.0 + 0.5;
}

float intensityStrength (float intensity)
{
  return (intensity - minIntensity) / (maxIntensity - minIntensity);
}

void main(){
  if(colorMode == 1)
    FragColor = vec4(colorShiftRed(fragPos.z), colorShiftGreen(fragPos.z), colorShiftBlue(fragPos.z), 1.0);
  if(colorMode == 2)
    FragColor = vec4(colorShiftRed(intensity), colorShiftGreen(intensity), colorShiftBlue(intensity), 1.0);
  if(colorMode == 3)
    FragColor = vec4(mix(colorShiftRed(intensity), colorShiftRed(fragPos.z), fragPos.z), mix(colorShiftGreen(intensity), colorShiftGreen(fragPos.z), fragPos.z), mix(colorShiftBlue(intensity), colorShiftBlue(fragPos.z), fragPos.z), 1.0);
}
