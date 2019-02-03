#pragma once

struct Input {
  enum class Key : uint32_t {
    kForward = 1,
    kBack = 2,
    kLeft = 3,
    kRight = 4
  };
  
  bool IsKeyDown(Key key) const {
    return 0 != (m_buttons & (1 << (uint32_t)key));
  }
  
  bool IsAnyDown() const {
    return m_buttons != 0;
  }
  
  void SetKeyDown(Key key) {
    m_buttons |= (1 << (uint32_t)key);
  }
  
  void SetKeyUp(Key key) {
    m_buttons &= ~(1 << (uint32_t)key);
  }
  
private:
  uint32_t m_buttons = 0;
};
