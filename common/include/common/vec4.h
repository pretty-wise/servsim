#pragma once

#include <math.h>

class vec4 {
public:
  inline vec4();
  inline vec4(const vec4 &);
  inline vec4(float x, float y, float z, float w);
  inline vec4(const float *array);

  inline vec4 &operator+=(const vec4 &);
  inline vec4 &operator-=(const vec4 &);
  inline vec4 &operator*=(float);
  inline vec4 &operator/=(float);

  inline vec4 operator+();
  inline vec4 operator-();

  inline vec4 operator+(const vec4 &) const;
  inline vec4 operator-(const vec4 &) const;

  friend vec4 operator*(float, const vec4 &);

  inline vec4 operator*(float) const;
  inline vec4 operator/(float) const;

  inline bool operator==(const vec4 &) const;
  inline bool operator!=(const vec4 &) const;

  inline float length() const;
  inline void normalize();

public:
  union {
    struct {
      float x, y, z, w;
    };
    float coords[4];
  };
};

inline vec4::vec4() {}


inline vec4::vec4(const vec4 &v)
    : x(v.x), y(v.y), z(v.z), w(v.w) {}

inline vec4::vec4(float _x, float _y, float _z, float _w)
    : x(_x), y(_y), z(_z), w(_w) {}

inline vec4::vec4(const float *array)
    : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}

inline vec4 &vec4::operator+=(const vec4 &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  w += v.w;
  return *this;
}

inline vec4 &vec4::operator-=(const vec4 &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  w -= v.w;
  return *this;
}

inline vec4 &vec4::operator*=(float a) {
  x *= a;
  y *= a;
  z *= a;
  w *= a;
  return *this;
}

inline vec4 &vec4::operator/=(float a) {
  x /= a;
  y /= a;
  z /= a;
  w /= a;
  return *this;
}

inline vec4 vec4::operator+() { return *this; }

inline vec4 vec4::operator-() {
  return vec4(-x, -y, -z, -w);
}

inline vec4 vec4::operator+(const vec4 &v) const {
  return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline vec4 vec4::operator-(const vec4 &v) const {
  return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline vec4 vec4::operator*(float a) const {
  return vec4(x * a, y * a, z * a, w * a);
}

inline vec4 vec4::operator/(float a) const {
  float inverse = 1 / a; // dzielenie wolniejsze.
  return vec4(x * inverse, y * inverse, z * inverse, w * inverse);
}

inline bool vec4::operator==(const vec4 &v) const {
  return x == v.x && y == v.y && z == v.z && w == v.w;
}

inline bool vec4::operator!=(const vec4 &v) const {
  return x != v.x || y != v.y || z != v.z || w != v.w;
}

inline float vec4::length() const {
  return sqrtf(x * x + y * y + z * z + w * w);
}

void vec4::normalize() {
  float inv_len = 1 / length();
  x *= inv_len;
  y *= inv_len;
  z *= inv_len;
  w *= inv_len;
}
