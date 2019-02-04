#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl3.h>

#include "common/world.h"

struct CubeVertex
{
  float x,y,z;
  float nx,ny,nz;
};

CubeVertex kCubeVertices[] = {
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

uint16_t kCubeIndices[] = {
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

const float kGroundSize = 100.f;
CubeVertex kGroundVertices[] = {
  { -kGroundSize, 0.f, kGroundSize }, // far left 0
  { kGroundSize, 0.f, kGroundSize }, // far right 1
  { kGroundSize, 0.f, -kGroundSize }, // near right 2
  { -kGroundSize, 0.f, -kGroundSize } // near left 3
};

uint16_t kGroundIndices[] = {
  0, 1, 3,
  1, 2, 3
};

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

static void gl_check_error(const char* tagName) {
  GLenum error = glGetError();
  if(error != 0) {
    printf("gl error status: %d, '%s'\n", error, tagName);
  }
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

// creates perspective projection matrix for right-handed coordinate system
static mat4 perspective(float fovy, float aspect, float znear, float zfar) {
  
  const float halfpi = 1.57079633f;
  float radians = fovy * halfpi / 180.0f;
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

// creates orthagonal projection matrix for right-handed coordinate system
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

// creates view martrix
static mat4 create_view(vec3 eye, vec3 lookat, vec3 up) {
  vec3 forward = vec3::normalize(eye - lookat);
  vec3 right = vec3::normalize(vec3::cross(up, forward));
  vec3 real_up = vec3::cross(forward, right);
  
  return mat4(right.x, right.y, right.z, -vec3::dot(right, eye),
              real_up.x, real_up.y, real_up.z, -vec3::dot(real_up, eye),
              forward.x, forward.y, forward.z, -vec3::dot(forward, eye),
              0.f, 0.f, 0.f, 1.f);
}

// pitch, yaw in radians
static mat4 create_view(vec3 eye, float pitch, float yaw) {
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);
  float cosYaw = cos(yaw);
  float sinYaw = sin(yaw);
  
  vec3 right(cosYaw, 0, -sinYaw);
  vec3 up(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
  vec3 forward(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);
  
  return mat4(right.x, right.y, right.z, -vec3::dot(right, eye),
              up.x, up.y, up.z, -vec3::dot(up, eye),
              forward.x, forward.y, forward.z, -vec3::dot(forward, eye),
              0.f, 0.f, 0.f, 1.f);
}

static mat4 create_model(vec3 model_translation, float model_rotation, float model_scale) {
  const mat4 translation = mat4::translation(model_translation);
  const mat4 rotation = mat4::rotation(vec3::kUnitY, model_rotation);
  const mat4 scale = mat4::scale(model_scale, model_scale, model_scale);
  return translation * rotation * scale;
}

static void create_mesh(CubeVertex* vertices, uint32_t vertices_size, uint16_t* indices, uint16_t indices_size, uint32_t& vbo, uint32_t& ibo) {
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void create_vbo(uint32_t& vao, uint32_t program, uint32_t vbo, uint32_t ibo) {
  glUseProgram(program);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  const int positionLoc = glGetAttribLocation(program, "VertexPosition");
  const int normalLoc = glGetAttribLocation(program, "VertexNormal");
  
  if(positionLoc >= 0) {
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLubyte*)0);
  }
  
  if(normalLoc >= 0) {
    glEnableVertexAttribArray(normalLoc);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLubyte*)(3*sizeof(float)));
  }
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void render_object(uint32_t program, uint32_t vao, uint32_t vbo, uint32_t ibo,
                   uint32_t numIndices, vec3 color, mat4 model, mat4 view, mat4 projection,
                   vec3 cameraPosition) {
  glUseProgram(program);
  
  const int eyeLoc = glGetUniformLocation(program, "EyePosition");
  const int lightLoc = glGetUniformLocation(program, "LightPosition");
  const int vertexColorLoc = glGetUniformLocation(program, "VertexColor");
  
  if ( eyeLoc >= 0 ) {
    glUniform3fv(eyeLoc, 1, cameraPosition.coords);
    gl_check_error("eyeLoc");
  }
  
  if (lightLoc >= 0) {
    const vec3 lightPosition(10.f, 10.f, 10.f);
    glUniform3fv(lightLoc, 1, lightPosition.coords);
    gl_check_error("lightLoc");
  }
  
  if(vertexColorLoc) {
    vec4 color4(color.x, color.y, color.z, 1.f);
    glUniform4fv(vertexColorLoc, 1, color4.coords);
    gl_check_error("vertexColorLoc");
  }
  
  const mat4 modelView = view * model;
  const mat4 modelViewProjection = projection * modelView;
  
  const int modelLoc = glGetUniformLocation(program, "Model");
  const int modelViewLoc = glGetUniformLocation(program, "ModelView");
  const int modelViewProjectionLoc = glGetUniformLocation(program, "ModelViewProjection");
  
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
  
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glDrawElements(GL_TRIANGLES, numIndices,
                 GL_UNSIGNED_SHORT, nullptr);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
  
  gl_check_error("postrender");
}

Renderer::Renderer() {
  printf("gl version: %s\n", glGetString(GL_VERSION));
  printf("glsl version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  
  // load program
  m_programId = load_program("shader/main.vert", "shader/main.frag");
  if(0 == m_programId) {
    printf("error: program not created\n");
    return;
  }
  
  gl_check_error("load_program");
  
  // create vertex/index buffers
  create_mesh(kCubeVertices, sizeof(kCubeVertices), kCubeIndices, sizeof(kCubeIndices), m_cubeVbo, m_cubeIbo);
  create_mesh(kGroundVertices, sizeof(kGroundVertices), kGroundIndices, sizeof(kGroundIndices), m_groundVbo, m_groundIbo);
  
  // create vertex array object and bind the vertex attributes
  create_vbo(m_cubeVao, m_programId, m_cubeVbo, m_cubeIbo);
  create_vbo(m_groundVao, m_programId, m_groundVbo, m_groundIbo);
  
  gl_check_error("post ctor");
}

Renderer::~Renderer() {
  glDeleteProgram(m_programId);
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
  
  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  
  static float r,g,b = 0.f;
  //r += 0.01f;
  //g += 0.001f;
  //b += 0.0001f;
  if(r > 1.f) r = 0.f;
  if(g > 1.f) g = 0.f;
  if(b > 1.f) b = 0.f;
  
  glClearColor(r, g, b, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  m_ar = width / (float)height;
  
  gl_check_error("beginscene");
}

void Renderer::RenderWorld(vec3 cameraPosition, vec3 cameraForward, const World& world) {
#if 0
  const vec3 lookAt = model_translation;
#else
  //const vec3 lookAt = cameraPosition - vec3::kUnitZ;
  const vec3 lookAt = cameraPosition + cameraForward;
#endif
  
  const mat4 view = create_view(cameraPosition, lookAt, vec3::kUnitY);
  const mat4 projection = perspective(40.f, m_ar, 0.1f, 100.f);
  //const mat4 projection = ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
  
  {
    const mat4 model = create_model(world.m_cube.m_translation, world.m_cube.m_rotation, world.m_cube.m_scale);
    render_object(m_programId, m_cubeVao, m_cubeVbo, m_cubeIbo, sizeof(kCubeIndices) / sizeof(kCubeIndices[0]),
                  vec3(1.f, 0.f, 1.f), model, view, projection, cameraPosition);
  }
  {
    const mat4 model = create_model(world.m_cube.m_translation + vec3(4.f, 0.f, 0.f),
                                    world.m_cube.m_rotation + M_PI_2,
                                    world.m_cube.m_scale * 0.5f);
    render_object(m_programId, m_cubeVao, m_cubeVbo, m_cubeIbo, sizeof(kCubeIndices) / sizeof(kCubeIndices[0]),
                  vec3(0.f, 1.f, 1.f), model, view, projection, cameraPosition);
  }
  {
    const mat4 model = create_model(vec3::kZero, 0.f, 10000.f);
    render_object(m_programId, m_groundVao, m_groundVbo, m_groundIbo, sizeof(kGroundIndices) / sizeof(kGroundIndices[0]),
                  vec3(0.886f, 0.956f, 0.258f), model, view, projection, cameraPosition);
  }
}

void Renderer::EndScene() {
  glFlush();
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_BLEND);
  
  gl_check_error("end");
}
