#include "common/world.h"

static void tick_world(const World& previous, const Input& input, World& next) {
  const float kMoveDelta = 0.1f;
  
  next = previous;
  
  if(input.IsKeyDown(Input::Key::kLeft)) {
    next.m_cube.m_translation.x -= kMoveDelta;
  } else if(input.IsKeyDown(Input::Key::kRight)) {
    next.m_cube.m_translation.x += kMoveDelta;
  } else if(input.IsKeyDown(Input::Key::kForward)) {
    next.m_cube.m_translation.z -= kMoveDelta;
  } else if(input.IsKeyDown(Input::Key::kBack)) {
    next.m_cube.m_translation.z += kMoveDelta;
  }
}

Game::Game()
: m_current_tick(0)
, m_tick_time(100.f)
, m_tick_countdown(m_tick_time){
  World& initial_state = m_state[0];
  initial_state.m_cube.m_translation = vec3(0.f, 0.f, 0.f);
  initial_state.m_cube.m_rotation = 0.f;
  initial_state.m_cube.m_scale = 1.f;
}

void Game::UpdateInput(const Input& input) {
  m_input[m_current_tick] = input;
}

void Game::Update(float dt) {
  m_tick_countdown -= dt;
  uint32_t num_steps = 0;
  while(m_tick_countdown <= 0.f) {
    Step();
    num_steps += 1;
    m_tick_countdown += m_tick_time;
  }
  if(num_steps > 1) {
    printf("warning: %u steps in one frame\n", num_steps);
  }
}

void Game::Step() {
  tick_t previous_tick = m_current_tick;
  ++m_current_tick;
  if(m_current_tick >= kGameLoopLength) {
    m_current_tick = 0;
  }
  
  for(tick_t t = 0; t < m_current_tick; ++t) {
    tick_world(m_state[t], m_input[t], m_state[t+1]);
  }
  
  printf("tick: %llu, ", m_current_tick);
  printf("input: %c%c%c%c\n", m_input[previous_tick].IsKeyDown(Input::Key::kForward) ? 'w' : '-',
                               m_input[previous_tick].IsKeyDown(Input::Key::kBack) ? 's' : '-',
                               m_input[previous_tick].IsKeyDown(Input::Key::kLeft) ? 'a' : '-',
                               m_input[previous_tick].IsKeyDown(Input::Key::kRight) ? 'd' : '-');
}
