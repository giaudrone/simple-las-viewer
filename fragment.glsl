#version 330 core

out vec4 FragColor;

in vec3 fragPos;

void main(){
  FragColor = vec4(fragPos, 1.0);
  // vertexColor = vec3(0.5, (aIntensity - minIntensity) / (maxIntensity - minIntensity), 0.5);
}
