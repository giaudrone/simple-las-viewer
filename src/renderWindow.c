#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "recordFormats.h"

#include "../includes/glad.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define VERTEX_SHADER_PATH "src/vertex.glsl"
#define FRAGMENT_SHADER_PATH "src/fragment.glsl"

#define PI 3.142

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
char* readFile(char* path);


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

char* read_shader(char *path) {
  FILE *fp;
  long lSize;
  char *buffer;

  fp = fopen ( path , "rb" );
  if( !fp ) perror(path),exit(1);

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  buffer = calloc( 1, lSize+1 );
  if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

  if( 1!=fread( buffer , lSize, 1 , fp) )
    fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
  
  return buffer;
}

modelBoundingBox createBoundingBox(LASFHeader *header) {
  modelBoundingBox box;
  box.centerX = (header->maxX + header->minX) / 2.0f;
  box.centerY = (header->maxY + header->minY) / 2.0f;
  box.centerZ = (header->maxZ + header->minZ) / 2.0f;
  return box;
};

void calculateColors(data *points, LASFHeader *header, uint16_t minIntensity, uint16_t maxIntensity);
float colorShiftRed(float value, float minY, float maxY) ;
float colorShiftGreen(float value, float minY, float maxY);
float colorShiftBlue(float value, float minY, float maxY);

int compare(const void *arg1, const void *arg2){
  const PointDataRecord6 *d1 = (const PointDataRecord6 *)arg1;
  const PointDataRecord6 *d2 = (const PointDataRecord6 *)arg2;

  if (d1->z < d2->z)
    return -1;
  else if (d1->z > d2->z)
    return 1;
  else
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

vec3 cameraPosition = {0.0f, 0.0f, 10.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};
vec3 center;
float lastX = 400.0f, lastY = 300.0f;
float pitch, yaw = -90.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int firstMouse = 1;
float fov = 45.0f;
int colorMode = 1;
mat4 proj, model, view;
vec3 scaleFactor = {0.05f, 0.05f, 0.05f};
float renderDistance = 100.0f;

void renderWindow(PointDataRecord6 *records, LASFHeader *header, int noiseFlag) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1200, 720, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create window");
    glfwTerminate();
    abort();
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    abort();
  }

  char *vertexShaderSource = read_shader(VERTEX_SHADER_PATH);
  char *fragmentShaderSource = read_shader(FRAGMENT_SHADER_PATH);

  const char *vertexShaderSourceCopy = vertexShaderSource;
  const char *fragmentShaderSourceCopy = fragmentShaderSource;

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSourceCopy, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceCopy, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);


  if (!success){
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n %s", infoLog);
  }
  glUseProgram(shaderProgram);

  //free memory
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  data *points = malloc(sizeof(data) * header->numPointRecords);

  if(noiseFlag)
    qsort(records, header->numPointRecords, sizeof(PointDataRecord6), compare);

  uint16_t maxIntensity = 0, minIntensity = 2^(16);

  modelBoundingBox box = createBoundingBox(header);

  for(size_t i=0; i < header->numPointRecords; i++) {
    points[i].x = records[i].x;
    points[i].y = records[i].y;
    points[i].z = records[i].z;
    points[i].intensity = records[i].intensity;
  }


  if(noiseFlag) {

    for(size_t i = 1; i < header->numPointRecords * 0.01f; i++) {
      if(points[i].z - points[i-1].z > 10) {
        header->minZ = points[i].z * header->zScaleFactor;
      } else {
        break;
      }
    }

    for(size_t i = header->numPointRecords * 0.99f; i < header->numPointRecords; i++) {
      if(points[i].z - points[i-1].z > 10) {
        header->maxZ = points[i-1].z * header->zScaleFactor;
        break;
      }
    }
  }


  for(size_t i=0; i < header->numPointRecords; i++) {
    if(points[i].intensity > maxIntensity) {
      maxIntensity = points[i].intensity;
    } else if(points[i].intensity < minIntensity){
      minIntensity = points[i].intensity;
    }
  }

  calculateColors(points, header, minIntensity, maxIntensity);

  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data) * header->numPointRecords, points, GL_STATIC_DRAW);
  

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(data), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)(4 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)(9 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glEnable(GL_DEPTH_TEST);



  int modelLoc = glGetUniformLocation(shaderProgram, "model");
  int viewLoc = glGetUniformLocation(shaderProgram, "view");
  int projLoc = glGetUniformLocation(shaderProgram, "projection");
  int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");


  int centerXloc = glGetUniformLocation(shaderProgram, "centerX");
  int centerYloc = glGetUniformLocation(shaderProgram, "centerY");
  int centerZloc = glGetUniformLocation(shaderProgram, "centerZ");

  glUniform1f(centerXloc, box.centerX);
  glUniform1f(centerYloc, box.centerY);
  glUniform1f(centerZloc, box.centerZ);

  int xSFloc = glGetUniformLocation(shaderProgram, "xScaleFactor");
  int ySFloc = glGetUniformLocation(shaderProgram, "yScaleFactor");
  int zSFloc = glGetUniformLocation(shaderProgram, "zScaleFactor");

  glUniform1f(xSFloc, header->xScaleFactor);
  glUniform1f(ySFloc, header->yScaleFactor);
  glUniform1f(zSFloc, header->zScaleFactor);

  /*int maxYloc = glGetUniformLocation(shaderProgram, "maxY");*/
  /*int minYloc = glGetUniformLocation(shaderProgram, "minY");*/
  /**/
  /*glUniform1f(maxYloc, header->maxZ);*/
  /*glUniform1f(minYloc, header->minZ);*/

  /*int maxIntensityLoc = glGetUniformLocation(shaderProgram, "maxIntensity");*/
  /*int minIntensityLoc = glGetUniformLocation(shaderProgram, "minIntensity");*/
  /**/
  /*glUniform1f(maxIntensityLoc, maxIntensity);*/
  /*glUniform1f(minIntensityLoc, minIntensity);*/

  int colorModeLoc = glGetUniformLocation(shaderProgram, "colorMode");

  /*glPointSize(5.0f);*/

  while(!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    /*printf("FPS: %f", 1 / deltaTime);*/

    glm_mat4_identity(proj);
    glm_mat4_identity(model);
    glm_mat4_identity(view);

    processInput(window);
    glUniform1i(colorModeLoc, colorMode);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glUseProgram(shaderProgram);


    glm_perspective(glm_rad(fov), (float)800/(float)600, 0.1f, renderDistance, proj);

    glm_vec3_add(cameraPosition, cameraFront, center);
    glm_lookat(cameraPosition, center, cameraUp, view);

    glm_scale(model, scaleFactor);
    glm_rotate_at(model, (vec3){0.0f, 0.0f, 0.0f}, glm_rad(-90.0f), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate_at(model, (vec3){0.0f, 0.0f, 0.0f}, glm_rad(180.0f), (vec3){0.0f, 0.0f, 1.0f});

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)proj);

    glUniform3fv(viewPosLoc, 1, cameraPosition);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, header->numPointRecords);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      printf("OpenGL Error during rendering: %d\n", error);
      abort();
    }

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)proj);


    glfwSwapBuffers(window);
    glfwPollEvents();
  } 

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  free(points);

  glfwTerminate();
}

void processInput(GLFWwindow* window) {
  vec3 temp;

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 3.0f * deltaTime;

  if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    cameraSpeed *= 10.0f;
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    glm_vec3_muladds(cameraFront, cameraSpeed, cameraPosition);
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    glm_vec3_mulsubs(cameraFront, cameraSpeed, cameraPosition);
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    glm_vec3_crossn(cameraFront, cameraUp, temp);
    glm_vec3_mulsubs(temp, cameraSpeed, cameraPosition);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    glm_vec3_crossn(cameraFront, cameraUp, temp);
    glm_vec3_muladds(temp, cameraSpeed, cameraPosition);
  }
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    glm_vec3_muladds(cameraUp, cameraSpeed, cameraPosition);
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    glm_vec3_mulsubs(cameraUp, cameraSpeed, cameraPosition);

  if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    colorMode = 1;
  if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    colorMode = 2;
  if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    colorMode = 3;

  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glm_vec3_mul(scaleFactor, (vec3){0.99f, 0.99f, 0.99f}, scaleFactor);
  if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    glm_vec3_mul(scaleFactor, (vec3){1.01f, 1.01f, 1.01f}, scaleFactor);

  if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    if(!(renderDistance < 2.0f))
      renderDistance -= 1.0f;
  if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    renderDistance += 1.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
  if(firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = 0;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch = 89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  vec3 direction = {cos(glm_rad(yaw)) * cos(glm_rad(pitch)), sin(glm_rad(pitch)), sin(glm_rad(yaw)) * cos(glm_rad(pitch))};
  glm_normalize_to(direction, cameraFront);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  fov -= (float)yoffset;
  if(fov < 1.0f)
    fov = 1.0f;
  if(fov > 120.0f)
    fov = 120.0f;
}

void calculateColors(data *points, LASFHeader *header, uint16_t minIntensity, uint16_t maxIntensity) {
  for(uint64_t i = 0; i < header->numPointRecords; ++i) {
    float z = points[i].z;
    float in = points[i].intensity;
    points[i].r = colorShiftRed(z, (float)header->minY,  (float)header->maxY);
    points[i].g = colorShiftGreen(z, (float)header->minY,  (float)header->maxY);
    points[i].b = colorShiftBlue(z, (float)header->minY,  (float)header->maxY);
    points[i].ir = colorShiftRed(in, (float)header->minY,  (float)header->maxY);
    points[i].ig = colorShiftGreen(in, (float)header->minY,  (float)header->maxY);
    points[i].ib = colorShiftBlue(in, (float)header->minY,  (float)header->maxY);
  }
}

float colorShiftRed(float value, float minY, float maxY) {
  return (cos(PI + PI * (((0.01 * value) - minY) / (maxY - minY)))) / 2.0 + 0.5;
}

float colorShiftGreen(float value, float minY, float maxY) {
  return (sin(PI * (((0.01 * value) - minY) / (maxY - minY))));
}

float colorShiftBlue(float value, float minY, float maxY) {
  return (-cos(PI + PI * (((0.01 * value) - minY) / (maxY - minY)))) / 2.0 + 0.5;
}
