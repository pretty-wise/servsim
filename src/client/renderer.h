#pragma once
#include <stdint.h>
#include "../common/mat4.h"

class Renderer {
public:
  Renderer();
  ~Renderer();
  
  void BeginScene(int width, int height);
  void RenderScene(vec3 cameraPosition, vec3 modelTranslation);
  void EndScene();
private:
  uint32_t m_programId;
  uint32_t m_cubeVbo;
  uint32_t m_cubeIbo;
  uint32_t m_cubeVao;
  
  float m_ar;
  
  mat4 m_model;
  mat4 m_modelView;
  mat4 m_modelViewProjection;
};
