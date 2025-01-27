#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "prints.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define VERTEX_SHADER_PATH "vertex.glsl"
#define FRAGMENT_SHADER_PATH "fragment.glsl"

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

modelBoundingBox createBoundingBox(LASFHeader header) {
  modelBoundingBox box;
  box.centerX = (header.maxX + header.minX) / 2.0f;
  box.centerY = (header.maxY + header.minY) / 2.0f;
  box.centerZ = (header.maxZ + header.minZ) / 2.0f;
  return box;
};


int compare(const void *arg1, const void *arg2){
  const data *d1 = (const data *)arg1;
  const data *d2 = (const data *)arg2;

  if (d1->intensity < d2->intensity)
    return -1;
  else if (d1->intensity > d2->intensity)
    return 1;
  else
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

vec3 cameraPosition = {0.0f, 0.0f, 20.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};
vec3 center;
float lastX = 400.0f, lastY = 300.0f;
float pitch, yaw = -90.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int firstMouse = 1;
float fov = 45.0f;

void renderWindow(PointDataRecord *records, LASFHeader header) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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

  unsigned int vertexShader, lightvs;
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

  data *points = malloc(sizeof(data) * header.numPointRecords);

  modelBoundingBox box = createBoundingBox(header);

  for(int i=0; i < header.numPointRecords; i++) {
    points[i].x = records[i].x;
    points[i].y = records[i].y;
    points[i].z = records[i].z;
    points[i].intensity = records[i].intensity;
  }


  float max = points[0].intensity;

  for(int i=0; i < header.numPointRecords; i++) {
    if(!(points[i].intensity > 2.0 * max)){
      max = points[i].intensity;
    } else {
      points[i].intensity = max;
    }
  }

  points[0].intensity = 600.0f;
  max = 10000.0f;

  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data) * header.numPointRecords, points, GL_STATIC_DRAW);
  

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(data), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  int objcLoc = glGetUniformLocation(shaderProgram, "objectColor");

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


  int maxYloc = glGetUniformLocation(shaderProgram, "maxY");
  int minYloc = glGetUniformLocation(shaderProgram, "minY");

  glUniform1f(maxYloc, header.maxY);
  glUniform1f(minYloc, header.minY);


  uint maxIntensityLoc = glGetUniformLocation(shaderProgram, "maxIntensity");
  uint minIntensityLoc = glGetUniformLocation(shaderProgram, "minIntensity");

  glUniform1f(maxIntensityLoc, max);
  glUniform1f(minIntensityLoc, points[0].intensity);


  while(!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    mat4 proj, model, view;

    glUseProgram(shaderProgram);

    glm_mat4_identity(proj);
    glm_perspective(glm_rad(fov), (float)800/(float)600, 0.1f, 1000.0f, proj);

    glm_mat4_identity(model);

    glm_mat4_identity(view);

    glm_vec3_add(cameraPosition, cameraFront, center);
    glm_lookat(cameraPosition, center, cameraUp, view);

    glm_scale(model, (vec3){0.01f, 0.01f, 0.01f});

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)proj);

    glUniform3fv(viewPosLoc, 1, cameraPosition);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, header.numPointRecords);

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

  glfwTerminate();
}

void processInput(GLFWwindow* window) {
  vec3 temp;

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 3.0f * deltaTime;

  if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    cameraSpeed *= 2.0f;
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
