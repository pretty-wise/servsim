#pragma once

#include <string>
#include <math.h>

class vec2 {
public:
  static const vec2 kZero;

  static const vec2 kUnitY;
  static const vec2 kUnitX;

  inline vec2();
  inline vec2(const vec2 &);
  inline vec2(float x, float y);
  inline vec2(const float *array);

  inline vec2 &operator+=(const vec2 &);
  inline vec2 &operator-=(const vec2 &);
  inline vec2 &operator*=(float);
  inline vec2 &operator/=(float);

  inline vec2 operator+() const;
  inline vec2 operator-() const;

  inline vec2 operator+(const vec2 &) const;
  inline vec2 operator-(const vec2 &) const;

  friend vec2 operator*(float, const vec2 &);

  inline vec2 operator*(float) const;
  inline vec2 operator/(float) const;

  inline bool operator==(const vec2 &) const;
  inline bool operator!=(const vec2 &) const;
  
  static inline vec2 lerp(const vec2 &a, const vec2 &b, float factor);

public:
  union {
    struct {
      float x, y;
    };
    float coords[2];
  };
};

vec2::vec2() {
  memset(&coords, 0x00, 2 * sizeof(float));
}

vec2::vec2(const vec2 &v) {
  memcpy(&coords, &v.coords, 2 * sizeof(float));
}

inline vec2::vec2(float _x, float _y)
    : x(_x), y(_y) {}

inline vec2::vec2(const float *array)
    : x(array[0]), y(array[1]) {}

vec2 &vec2::operator+=(const vec2 &v) {
  x += v.x;
  y += v.y;
  return *this;
}

vec2 &vec2::operator-=(const vec2 &v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

vec2 &vec2::operator*=(float a) {
  x *= a;
  y *= a;
  return *this;
}

vec2 &vec2::operator/=(float a) {
  x /= a;
  y /= a;
  return *this;
}

vec2 vec2::operator+() const { return *this; }

vec2 vec2::operator-() const {
  return vec2(-x, -y);
}

vec2 vec2::operator+(const vec2 &v) const {
  return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator-(const vec2 &v) const {
  return vec2(x - v.x, y - v.y);
}

vec2 vec2::operator*(float a) const {
  return vec2(x * a, y * a);
}

vec2 vec2::operator/(float a) const {
  float inverse = 1 / a;
  return vec2(x * inverse, y * inverse);
}

bool vec2::operator==(const vec2 &v) const {
  return x == v.x && y == v.y;
}

bool vec2::operator!=(const vec2 &v) const {
  return x != v.x || y != v.y;
}

inline vec2 vec2::lerp(const vec2 &a, const vec2 &b, float factor) {
  return vec2((1 - factor) * a.x + factor * b.x,
                  (1 - factor) * a.y + factor * b.y);
}
