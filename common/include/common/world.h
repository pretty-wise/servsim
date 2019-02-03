#pragma once
#include "vec3.h"
#include "input.h"

typedef uint64_t tick_t;

struct Cube {
  vec3 m_translation = vec3::kZero;
  float m_rotation = 0.f;
  float m_scale = 1.f;
  vec3 m_color = vec3(1.f, 0.f, 1.f);
};

struct World {
  Cube m_cube;
};

class Game {
public:
  Game();
  void Update(float dt);
  void UpdateInput(const Input& input);
  const World& GetCurrentState() const { return m_state[m_current_tick]; }
  
private:
  static const uint32_t kGameLoopLength = 100;
  
private:
  void Step();
  
private:
  Input m_input[kGameLoopLength];
  World m_state[kGameLoopLength];
  tick_t m_current_tick;
  float m_tick_time;
  float m_tick_countdown;
};
