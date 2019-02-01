#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl.h>

struct CubeVertex
{
  float x,y,z;
  float nx,ny,nz;
};

CubeVertex kCubeVertices[] =
{
  { +1.f, -1.f, +1.f, 0.f, 0.f, +1.f },
  { -1.f, -1.f, +1.f, 0.f, 0.f, +1.f },
  { -1.f, +1.f, +1.f, 0.f, 0.f, +1.f },
  { +1.f, +1.f, +1.f, 0.f, 0.f, +1.f },
  
  { -1.f, -1.f, -1.f, 0.f, 0.f, -1.f },
  { +1.f, -1.f, -1.f, 0.f, 0.f, -1.f },
  { +1.f, +1.f, -1.f, 0.f, 0.f, -1.f },
  { -1.f, +1.f, -1.f, 0.f, 0.f, -1.f },
  
  { +1.f, +1.f, -1.f, +1.f, 0.f, 0.f },
  { +1.f, -1.f, -1.f, +1.f, 0.f, 0.f },
  { +1.f, -1.f, +1.f, +1.f, 0.f, 0.f },
  { +1.f, +1.f, +1.f, +1.f, 0.f, 0.f },
  
  { -1.f, -1.f, -1.f, -1.f, 0.f, 0.f },
  { -1.f, +1.f, -1.f, -1.f, 0.f, 0.f },
  { -1.f, +1.f, +1.f, -1.f, 0.f, 0.f },
  { -1.f, -1.f, +1.f, -1.f, 0.f, 0.f },
  
  { -1.f, +1.f, -1.f, 0.f, +1.f, 0.f },
  { +1.f, +1.f, -1.f, 0.f, +1.f, 0.f },
  { +1.f, +1.f, +1.f, 0.f, +1.f, 0.f },
  { -1.f, +1.f, +1.f, 0.f, +1.f, 0.f },
  
  { +1.f, -1.f, -1.f, 0.f, -1.f, 0.f },
  { -1.f, -1.f, -1.f, 0.f, -1.f, 0.f },
  { -1.f, -1.f, +1.f, 0.f, -1.f, 0.f },
  { +1.f, -1.f, +1.f, 0.f, -1.f, 0.f },
};
/*struct CubeVertex
{
  float x,y,z;
};
CubeVertex kCubeVertices[] =
{
  { +1.f, -1.f, +1.f},
  { -1.f, -1.f, +1.f},
  { -1.f, +1.f, +1.f},
  { +1.f, +1.f, +1.f},
  
  { -1.f, -1.f, -1.f},
  { +1.f, -1.f, -1.f},
  { +1.f, +1.f, -1.f},
  { -1.f, +1.f, -1.f},
  
  { +1.f, +1.f, -1.f},
  { +1.f, -1.f, -1.f},
  { +1.f, -1.f, +1.f},
  { +1.f, +1.f, +1.f},
  
  { -1.f, -1.f, -1.f},
  { -1.f, +1.f, -1.f},
  { -1.f, +1.f, +1.f},
  { -1.f, -1.f, +1.f},
  
  { -1.f, +1.f, -1.f},
  { +1.f, +1.f, -1.f},
  { +1.f, +1.f, +1.f},
  { -1.f, +1.f, +1.f},
  
  { +1.f, -1.f, -1.f},
  { -1.f, -1.f, -1.f},
  { -1.f, -1.f, +1.f},
  { +1.f, -1.f, +1.f},
};*/

uint16_t kCubeIndices[] =
{
  0, 1, 2,
  0, 2, 3,
  
  4, 5, 6,
  4, 6, 7,
  
  8, 9, 10,
  8, 10, 11,
  
  12, 13, 14,
  12, 14, 15,
  
  16, 17, 18,
  16, 18, 19,
  
  20, 21, 22,
  20, 22, 23,
};

static void gl_check_error(const char* tagName) {
  GLenum error = glGetError();
  printf("gl error status: %d, '%s'\n", error, tagName);
}

static const char* read_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if(!file)
    return 0;
  
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);
  
  if(fileSize <= 0) {
    fclose(file);
    return nullptr;
  }
  
  char* readBuffer = (char*)malloc(sizeof(char) * fileSize + 1);
  memset(readBuffer, 0, sizeof(char)*fileSize + 1);
  
  size_t readBytes = fread(readBuffer, 1, fileSize, file);
  
  fclose(file);
  
  if(readBytes != fileSize) {
    delete [] readBuffer;
    return nullptr;
  }
  
  return readBuffer;
}

static uint32_t load_shader(const char* filename, int shaderType) {
  const char* shaderSource = read_file(filename);
  if(!shaderSource)
    return 0;
  
  GLuint shaderId = glCreateShader(shaderType);
  gl_check_error("glCreateShader");
  if(shaderId == 0)
    return 0;
  
  glShaderSource(shaderId, 1, &shaderSource, NULL);
  glCompileShader(shaderId);
  
  delete [] shaderSource;
  
  GLint compileResult = GL_FALSE;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);
  
  if(compileResult == GL_FALSE) {
    GLint logLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    
    char* logBuffer = (char*)malloc(sizeof(char)*logLength);
    glGetShaderInfoLog(shaderId, logLength, NULL, logBuffer);
    printf("failed to compile shader, error: '%s'\n", logBuffer);
    
    delete [] logBuffer;
    glDeleteShader(shaderId);
    return 0;
  }
  
  return shaderId;
}

static uint32_t load_program(const char* vertexShaderPath, const char* fragmentShaderPath) {
  uint32_t vertexShaderId = load_shader(vertexShaderPath, GL_VERTEX_SHADER);
  if(vertexShaderId == 0) {
    return 0;
  }
  
  uint32_t fragmentShaderId = load_shader(fragmentShaderPath, GL_FRAGMENT_SHADER);
  if(fragmentShaderId == 0) {
    glDeleteShader(vertexShaderId);
    return 0;
  }
  
  GLuint programId = glCreateProgram();
  if(programId == 0) {
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    return 0;
  }
  
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  
  GLint linkResult = GL_FALSE;
  glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);
  if(linkResult == GL_FALSE) {
    int logLength = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    
    char* logBuffer = (char*)malloc(sizeof(char)*logLength);
    glGetProgramInfoLog(programId, logLength, nullptr, logBuffer);
    printf("failed to link program, error: '%s'\n", logBuffer);
    
    delete [] logBuffer;
    
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
    return 0;
  }
  
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
  return programId;
}

Renderer::Renderer() {
  printf("gl version: %s\n", glGetString(GL_VERSION));
  printf("glsl version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  m_programId = load_program("shader/main.vert", "shader/main.frag");
  if(0 == m_programId) {
    printf("error: program not created\n");
    return;
  }
  
  
  gl_check_error("load_program");
  
  {
    glUseProgram(m_programId);
    
    glGenBuffers(1, &m_cubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &m_cubeIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kCubeIndices), kCubeIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &m_cubeVao);
    glBindVertexArray(m_cubeVao);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVbo);
    
    const int positionLoc = glGetAttribLocation(m_programId, "VertexPosition");
    const int normalLoc = glGetAttribLocation(m_programId, "VertexNormal");
    //const int colorLoc = glGetAttribLocation(m_programId, "VertexColor");
    
    const int modelLoc = glGetUniformLocation(m_programId, "Model"); //glGetAttribLocation(m_programId, "Model");
    const int modelViewLoc = glGetUniformLocation(m_programId, "ModelView");
    const int modelViewProjectionLoc = glGetUniformLocation(m_programId, "ModelViewProjection");
    const int colorLoc = glGetUniformLocation(m_programId, "VertexColor");
    
    if(colorLoc >= 0) {
      vec4 color(1.f, 0.f, 1.f, 1.f);
      glUniform4fv(colorLoc, 1, color.coords);
    }
    
    if(positionLoc >= 0) {
      glEnableVertexAttribArray(positionLoc);
      glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLubyte*)0);
    }
    
    if(normalLoc >= 0) {
      glEnableVertexAttribArray(normalLoc);
      glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLubyte*)(3*sizeof(float)));
    }
    
    // unbind vbo and ibo
    glBindVertexArray(0);
    glUseProgram(0);
  }
  gl_check_error("post ctor");
}

Renderer::~Renderer() {
  glDeleteProgram(m_programId);
}

#define VECTORIAL_PI      3.14159265f
#define VECTORIAL_HALFPI  1.57079633f

static mat4 perspective(float fovy, float aspect, float znear, float zfar) {
  mat4 m;
  float radians = fovy * VECTORIAL_HALFPI / 180.0f;
  float deltaz = zfar - znear;
  float sine = sinf(radians);
  float cotangent = cosf(radians) / sine;
  
  float a = cotangent / aspect;
  float b = cotangent;
  float c = -(zfar + znear) / deltaz;
  float d = -2 * znear * zfar / deltaz;
  
  // -1 for RH on [11]
  return mat4(a, 0.f, 0.f, 0.f,
              0.f, b, 0.f, 0.f,
              0.f, 0.f, c, d,
              0.f, 0.f, -1.f, 0.f);
}

static mat4 ortho(float left, float right, float bottom, float top, float znear, float zfar) {
  float deltax = right - left;
  float deltay = top - bottom;
  float deltaz = zfar - znear;

  float a = 2.0f / deltax;
  float b = -(right + left) / deltax;
  float c = 2.0f / deltay;
  float d = -(top + bottom) / deltay;
  float e =  -2.0f / deltaz;
  float f = -(zfar + znear) / deltaz;
  
  return mat4(a, 0.f, 0.f, b,
              0.f, c, 0.f, d,
              0.f, 0.f, e, f,
              0.f, 0.f, 0.f, 1.f);
 }

static mat4 create_view(vec3 eye, vec3 lookat, vec3 up) {
  vec3 forward = vec3::normalize(eye - lookat);
  vec3 right = vec3::normalize(vec3::cross(up, forward));
  vec3 real_up = vec3::cross(forward, right);
  
  return mat4(right.x, right.y, right.z, -vec3::dot(right, eye),
              real_up.x, real_up.y, real_up.z, -vec3::dot(real_up, eye),
              forward.x, forward.y, forward.z, -vec3::dot(forward, eye),
              0.f, 0.f, 0.f, 1.f);
}

void Renderer::BeginScene(int width, int height) {
  // need to scale *2 for the retina display.
  // not sure how to detect that now.
  width *= 2; height *= 2;
  
  glViewport(0, 0, width, height);
  glEnable(GL_SCISSOR_TEST);
  glScissor(0, 0, width, height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  
  static float r,g,b = 0.f;
  
  //r+= 0.01f;
  //g+= 0.001f;
  //b+= 0.0001f;
  if(r > 1.f) r = 0.f;
  if(g > 1.f) g = 0.f;
  if(b > 1.f) b = 0.f;
  
  glClearColor(r, g, b, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  m_ar = width / (float)height;
  
  gl_check_error("beginscene");
}

static void printvec(const char* str, vec4 value) {
  printf("%s: (%.3f, %.3f, %.3f, %.3f)\n", str, value.x, value.y, value.z, value.w);
}

static void printmat(const char* str, mat4 mat) {
  printf("%s\n", str);
  printf("%.3f, %.3f, %.3f, %.3f\n", mat.matrix[0], mat.matrix[4], mat.matrix[8], mat.matrix[12]);
  printf("%.3f, %.3f, %.3f, %.3f\n", mat.matrix[1], mat.matrix[5], mat.matrix[9], mat.matrix[13]);
  printf("%.3f, %.3f, %.3f, %.3f\n", mat.matrix[2], mat.matrix[6], mat.matrix[10], mat.matrix[14]);
  printf("%.3f, %.3f, %.3f, %.3f\n", mat.matrix[3], mat.matrix[7], mat.matrix[11], mat.matrix[15]);
}

static void printvec(const char* str, vec3 value) {
  printvec(str, vec4(value.x, value.y, value.z, 0.f));
}

void Renderer::RenderScene(vec3 cameraPosition, vec3 modelTranslation) {
  glUseProgram(m_programId);
  
  const int eyeLoc = glGetUniformLocation(m_programId, "EyePosition");
  const int lightLoc = glGetUniformLocation(m_programId, "LightPosition");
  
  //const vec3 cameraPosition(0.f, 0.f, 10.f);
  //const vec3 cameraRight = vec3::kUnitX;
  //const vec3 cameraUp = vec3::kUnitY;
  //const vec3 cameraLookAt = -vec3::kUnitZ;
  
  //const vec3 cameraPosition(0.f, 0.f, 0.f);
  
  
  const vec3 lightPosition(10.f, 10.f, 10.f);
  
  if ( eyeLoc >= 0 )
  {
    glUniform3fv(eyeLoc, 1, cameraPosition.coords);
    gl_check_error("eyeLoc");
  }
  
  if (lightLoc >= 0)
  {
    glUniform3fv(lightLoc, 1, lightPosition.coords);
    gl_check_error("lightLoc");
  }
  
#if 1
  const vec3 lookAt = modelTranslation;
  //vec3 aim = vec3::normalize(lookAt - cameraPosition);
  vec3 aim = vec3::normalize(cameraPosition - lookAt); // is this for sure ok? not the above one?
  const vec3 cameraRight = vec3::cross(vec3::kUnitY, aim);
  const vec3 cameraUp = vec3::cross(aim, cameraRight);
  const vec3 cameraFront = aim;
#else
  const vec3 lookAt = cameraPosition - vec3::kUnitZ;
  const vec3 cameraRight = vec3::kUnitX;
  const vec3 cameraUp = vec3::kUnitY;
  const vec3 cameraFront = -vec3::kUnitZ;
#endif
  
  static float angle = 0.f;
  angle += 0.01f;
  static float scaleAmount = 1.f;
  scaleAmount -= 0.01f;
  if(scaleAmount <= 0.f) scaleAmount = 1.f;
  const mat4 translation = mat4::translation(modelTranslation);
  const mat4 rotation = mat4::rotation(vec3::kUnitY, angle);
  const mat4 scale = mat4::scale(scaleAmount, scaleAmount, scaleAmount);
  const mat4 model = translation * rotation * scale;
  
  const mat4 view = create_view(cameraPosition, lookAt, vec3::kUnitY);
  const mat4 projection = perspective(40.f, m_ar, 0.1f, 100.f);
  //const mat4 projection = ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
  const mat4 modelView = view * model;
  const mat4 modelViewProjection = projection * modelView;
  
  vec4 input(1.f, 0.f, 0.f, 1.f);
  vec4 view_out = modelView * input;
  vec4 proj_out = modelViewProjection * input;
  printvec("cam", cameraPosition);
  printvec("pos", modelTranslation);
  printmat("model", model);
  printmat("view", view);
  printmat("proj", projection);
  printmat("viewmodel", modelView);
  printmat("viewmodelproj", modelViewProjection);
  printvec("view", view_out);
  printvec("proj", proj_out);
  
  const int modelLoc = glGetUniformLocation(m_programId, "Model");
  const int modelViewLoc = glGetUniformLocation(m_programId, "ModelView");
  const int modelViewProjectionLoc = glGetUniformLocation(m_programId, "ModelViewProjection");
  
  if(modelLoc >= 0) {
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.matrix);
  }
  
  if(modelViewLoc >= 0) {
    glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView.matrix);
  }
  
  if(modelViewProjectionLoc >= 0) {
    glUniformMatrix4fv(modelViewProjectionLoc, 1, GL_FALSE, modelViewProjection.matrix);
  }
  gl_check_error("matrices");
  
  glBindVertexArray(m_cubeVao);
  
  uint32_t numIndices = sizeof(kCubeIndices) / sizeof(kCubeIndices[0]);
  uint32_t offset = 0;
  
  //numIndices /= 2;
  //offset = sizeof(kCubeIndices) / 2;
  
  uint32_t triOffset = 2;
  uint32_t triNum = 6;
  
  //numIndices = triNum * 3;
  //offset = triOffset * 3 * sizeof(uint16_t);
  
  glDrawElements(GL_TRIANGLES, numIndices,
                 GL_UNSIGNED_SHORT, (GLbyte*)offset);
  
  glBindVertexArray(0);
  glUseProgram(0);
  
  gl_check_error("postrender");
}

void Renderer::EndScene() {
  glFlush();
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_BLEND);
  
  gl_check_error("end");
}