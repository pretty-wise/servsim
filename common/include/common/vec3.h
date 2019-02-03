#pragma once

#include <string>
#include <math.h>

class vec3 {
public:
  static const vec3 kZero;
  static const vec3 kIdentity;

  static const vec3 kUnitY;
  static const vec3 kUnitZ;
  static const vec3 kUnitX;

  inline vec3();
  inline vec3(const vec3 &);
  inline vec3(float x, float y, float z);
  inline vec3(const float *array);

  inline vec3 &operator+=(const vec3 &);
  inline vec3 &operator-=(const vec3 &);
  inline vec3 &operator*=(float);
  inline vec3 &operator/=(float);

  inline vec3 operator+() const;
  inline vec3 operator-() const;

  inline vec3 operator+(const vec3 &) const;
  inline vec3 operator-(const vec3 &) const;

  friend vec3 operator*(float, const vec3 &);

  inline vec3 operator*(float) const;
  inline vec3 operator/(float) const;

  inline void set(float x, float y, float z);

  inline bool operator==(const vec3 &) const;
  inline bool operator!=(const vec3 &) const;

  inline float length() const;
  inline void normalize();
  
  static inline vec3 normalize(const vec3& v);

  /**
   * @param vector u
   * @return vector orthogonal to u and *this
   */
  inline vec3 cross(const vec3 &u) const;
  static inline vec3 cross(const vec3& a, const vec3& b);

  /**
   * the dot product:
   * u * v = 0, vectors are orthogonal (perpendicular ;)
   * u * v > 0, vectors make acute angle (< 90)
   * u * v < 0, vectors make obtuse angle(> 90)
   */
  inline float dot(const vec3 &v) const;
  static inline float dot(const vec3& a, const vec3& b);

  /**
   * linear interpolation of two vectors.
   * @return intermediate vector between a and b. factor ranges <0,1>
   */
  static inline vec3 lerp(const vec3 &a, const vec3 &b, float factor);

public:
  union {
    struct {
      float x, y, z;
    };
    float coords[3];
  };
};

vec3::vec3() {
  memset(&coords, 0x00, 3 * sizeof(float));
}

vec3::vec3(const vec3 &v) {
  memcpy(&coords, &v.coords, 3 * sizeof(float));
}

inline vec3::vec3(float _x, float _y, float _z)
    : x(_x), y(_y), z(_z) {}

inline vec3::vec3(const float *array)
    : x(array[0]), y(array[1]), z(array[2]) {}

vec3 &vec3::operator+=(const vec3 &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

vec3 &vec3::operator-=(const vec3 &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

vec3 &vec3::operator*=(float a) {
  x *= a;
  y *= a;
  z *= a;
  return *this;
}

vec3 &vec3::operator/=(float a) {
  x /= a;
  y /= a;
  z /= a;
  return *this;
}

vec3 vec3::operator+() const { return *this; }

vec3 vec3::operator-() const {
  return vec3(-x, -y, -z);
}

vec3 vec3::operator+(const vec3 &v) const {
  return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator-(const vec3 &v) const {
  return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator*(float a) const {
  return vec3(x * a, y * a, z * a);
}

vec3 vec3::operator/(float a) const {
  float inverse = 1 / a; // dzielenie wolniejsze.
  return vec3(x * inverse, y * inverse, z * inverse);
}

void vec3::set(float _x, float _y, float _z) {
  x = _x;
  y = _y;
  z = _z;
}

bool vec3::operator==(const vec3 &v) const {
  return x == v.x && y == v.y && z == v.z;
}

bool vec3::operator!=(const vec3 &v) const {
  return x != v.x || y != v.y || z != v.z;
}

float vec3::length() const {
  return sqrtf(x * x + y * y + z * z);
}

void vec3::normalize() {
  assert(x || y || z);
  float inv_len = 1 / length();
  x *= inv_len;
  y *= inv_len;
  z *= inv_len;
}

inline vec3 vec3::normalize(const vec3& v) {
  float inv_len = 1 / v.length();
  return vec3(v.x * inv_len, v.y * inv_len, v.z * inv_len);
}

float vec3::dot(const vec3 &v) const {
  return x * v.x + y * v.y + z * v.z;
}

float vec3::dot(const vec3& a, const vec3& b) {
  return a.dot(b);
}

vec3 vec3::cross(const vec3 &v) const {
  return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
vec3 vec3::cross(const vec3& a, const vec3& b) {
  return a.cross(b);
}

inline vec3 vec3::lerp(const vec3 &a, const vec3 &b, float factor) {
  // LERP(A,B,f) = (1-f)A + fB = [ (1-f)Ax + fBx, (1-f)Ay + fBy, (1-f)Az + fBz ]
  return vec3((1 - factor) * a.x + factor * b.x,
                  (1 - factor) * a.y + factor * b.y,
                  (1 - factor) * a.z + factor * b.z);
}
