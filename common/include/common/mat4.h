#pragma once

#include "vec4.h"
#include "vec3.h"

/* Translation Matrix
[ 1 0 0 Tx ]
[ 0 1 0 Ty ]
[ 0 0 1 Tz ]
[ 0 0 0 1	]

** Scaling Matrix
[ Sx 0	0	0 ]
[ 0	Sy 0	0 ]
[ 0	0	Sz 0 ]
[ 0	0	0	1 ]

** Rotation Matrices. Rotate 'd' radians around x-, y-, z-axes respectively.
** Around X.			    ** Around Y.          ** Around Z.
[ 1 0		 0		 0 ]		[ cosd	0 sind 0 ]		[ cosd -sind 0 0 ]
[ 0 cosd -sind 0 ]		[ 0		  1 0		 0 ]    [ sind cosd	 0 0 ]
[ 0 sind cosd	 0 ]		[ -sind 0 cosd 0 ]    [ 0		 0		 1 0 ]
[ 0 0		 0		 1 ]		[ 0		  0 0		 1 ]		[ 0		 0		 0 1 ]

*/

// matrix is stored column-ordered (OpenGL style). i.e. matrix[0 - 3] is the first
// column, so matrix[12] is Tx (on the last column).

class mat4 {
public:
  static const mat4 kIdentity;
  static const mat4 kZero;

public:
  inline mat4();
  inline mat4(const mat4 &m);

  // creates matrix from column-ordered float-array.
  inline mat4(const float *p);

  // creates matrix, arguments provided row-ordered.
  inline mat4(float _00, float _10, float _20, float _30,
              float _01, float _11, float _21, float _31,
              float _02, float _12, float _22, float _32,
              float _03, float _13, float _23, float _33);

  // accessors
  inline float &operator()(uint32_t c, uint32_t r);
  inline float operator()(uint32_t c, uint32_t r) const;

  // assignment operators
  inline mat4 &operator+=(const mat4 &);
  inline mat4 &operator-=(const mat4 &);
  inline mat4 &operator*=(const mat4 &);
  inline mat4 &operator*=(float);
  inline mat4 &operator/=(float);

  inline vec4 operator*(const vec4 &) const;
  inline vec3 operator*(const vec3 &) const;

  // unary operators
  inline mat4 operator+() const;
  inline mat4 operator-() const;

  // binary operators
  inline mat4 operator+(const mat4 &) const;
  inline mat4 operator-(const mat4 &) const;
  inline mat4 operator*(const mat4 &) const;
  inline mat4 operator*(float) const;
  inline mat4 operator/(float) const;

  friend mat4 operator*(float, const mat4 &);

  inline mat4 &makeIdentity();
  inline mat4 &makeZero();

  static inline mat4 rotation(const vec3 &vAxis, float fAngle);
  static inline mat4 translation(float x, float y, float z);
  static inline mat4 translation(vec3 translation);
  static inline mat4 scale(float x, float y, float z);
  static inline mat4 scale(vec3 scale);
  inline mat4 &InverseIt();
  inline mat4 &transposeIt();

  inline void ToArray(float *array) const;
  inline void ToColumnOrderedArray(float *array) const;

public:
  float matrix[16];
};

mat4::mat4() {
  // memset_s(&matrix, 0x00, 4*4*sizeof(float));
  matrix[0] = (float)0;
  matrix[1] = (float)0;
  matrix[2] = (float)0;
  matrix[3] = (float)0;
  matrix[4] = (float)0;
  matrix[5] = (float)0;
  matrix[6] = (float)0;
  matrix[7] = (float)0;
  matrix[8] = (float)0;
  matrix[9] = (float)0;
  matrix[10] = (float)0;
  matrix[11] = (float)0;
  matrix[12] = (float)0;
  matrix[13] = (float)0;
  matrix[14] = (float)0;
  matrix[15] = (float)0;
}

inline mat4::mat4(const mat4 &m) {
  memcpy(&matrix, &m.matrix, 4*4*sizeof(float));
}


inline mat4::mat4(float _00, float _10, float _20, float _30,
                  float _01, float _11, float _21, float _31,
                  float _02, float _12, float _22, float _32,
                  float _03, float _13, float _23, float _33) {
  matrix[0] = _00;
  matrix[1] = _01;
  matrix[2] = _02;
  matrix[3] = _03;
  matrix[4] = _10;
  matrix[5] = _11;
  matrix[6] = _12;
  matrix[7] = _13;
  matrix[8] = _20;
  matrix[9] = _21;
  matrix[10] = _22;
  matrix[11] = _23;
  matrix[12] = _30;
  matrix[13] = _31;
  matrix[14] = _32;
  matrix[15] = _33;
}

inline mat4::mat4(const float *array) {
  memcpy(&matrix, (const void*)array, 4*4*sizeof(float));
}

inline float &mat4::operator()(uint32_t column, uint32_t row) {
  return matrix[row + column * 4];
}

inline float mat4::operator()(uint32_t column, uint32_t row) const {
  return matrix[row + column * 4];
}

inline mat4 &mat4::operator+=(const mat4 &m) {
  matrix[0] += m.matrix[0];
  matrix[1] += m.matrix[1];
  matrix[2] += m.matrix[2];
  matrix[3] += m.matrix[3];
  matrix[4] += m.matrix[4];
  matrix[5] += m.matrix[5];
  matrix[6] += m.matrix[6];
  matrix[7] += m.matrix[7];
  matrix[8] += m.matrix[8];
  matrix[9] += m.matrix[9];
  matrix[10] += m.matrix[10];
  matrix[11] += m.matrix[11];
  matrix[12] += m.matrix[12];
  matrix[13] += m.matrix[13];
  matrix[14] += m.matrix[14];
  matrix[15] += m.matrix[15];
  return *this;
}

inline mat4 &mat4::operator-=(const mat4 &m) {
  matrix[0] -= m.matrix[0];
  matrix[1] -= m.matrix[1];
  matrix[2] -= m.matrix[2];
  matrix[3] -= m.matrix[3];
  matrix[4] -= m.matrix[4];
  matrix[5] -= m.matrix[5];
  matrix[6] -= m.matrix[6];
  matrix[7] -= m.matrix[7];
  matrix[8] -= m.matrix[8];
  matrix[9] -= m.matrix[9];
  matrix[10] -= m.matrix[10];
  matrix[11] -= m.matrix[11];
  matrix[12] -= m.matrix[12];
  matrix[13] -= m.matrix[13];
  matrix[14] -= m.matrix[14];
  matrix[15] -= m.matrix[15];
  return *this;
}

inline mat4 &mat4::operator*=(const mat4 &m) {
  mat4 tthis(*this);
  *this = tthis * m;
  return *this;
}

inline mat4 &mat4::operator*=(float scalar) {
  matrix[0] *= scalar;
  matrix[1] *= scalar;
  matrix[2] *= scalar;
  matrix[3] *= scalar;
  matrix[4] *= scalar;
  matrix[5] *= scalar;
  matrix[6] *= scalar;
  matrix[7] *= scalar;
  matrix[8] *= scalar;
  matrix[9] *= scalar;
  matrix[10] *= scalar;
  matrix[11] *= scalar;
  matrix[12] *= scalar;
  matrix[13] *= scalar;
  matrix[14] *= scalar;
  matrix[15] *= scalar;
  return *this;
}

inline mat4 &mat4::operator/=(float scalar) {
  matrix[0] /= scalar;
  matrix[1] /= scalar;
  matrix[2] /= scalar;
  matrix[3] /= scalar;
  matrix[4] /= scalar;
  matrix[5] /= scalar;
  matrix[6] /= scalar;
  matrix[7] /= scalar;
  matrix[8] /= scalar;
  matrix[9] /= scalar;
  matrix[10] /= scalar;
  matrix[11] /= scalar;
  matrix[12] /= scalar;
  matrix[13] /= scalar;
  matrix[14] /= scalar;
  matrix[15] /= scalar;
  return *this;
}

inline mat4 mat4::operator+() const {
  return mat4(*this);
}

inline mat4 mat4::operator-() const {
  return mat4(*this) * -1;
}

inline mat4 mat4::operator+(const mat4 &m) const {
  mat4 result;
  result.matrix[0] = matrix[0] + m.matrix[0];
  result.matrix[1] = matrix[1] + m.matrix[1];
  result.matrix[2] = matrix[2] + m.matrix[2];
  result.matrix[3] = matrix[3] + m.matrix[3];
  result.matrix[4] = matrix[4] + m.matrix[4];
  result.matrix[5] = matrix[5] + m.matrix[5];
  result.matrix[6] = matrix[6] + m.matrix[6];
  result.matrix[7] = matrix[7] + m.matrix[7];
  result.matrix[8] = matrix[8] + m.matrix[8];
  result.matrix[9] = matrix[9] + m.matrix[9];
  result.matrix[10] = matrix[10] + m.matrix[10];
  result.matrix[11] = matrix[11] + m.matrix[11];
  result.matrix[12] = matrix[12] + m.matrix[12];
  result.matrix[13] = matrix[13] + m.matrix[13];
  result.matrix[14] = matrix[14] + m.matrix[14];
  result.matrix[15] = matrix[15] + m.matrix[15];
  return result;
}

inline mat4 mat4::operator-(const mat4 &m) const {
  mat4 result;
  result.matrix[0] = matrix[0] - m.matrix[0];
  result.matrix[1] = matrix[1] - m.matrix[1];
  result.matrix[2] = matrix[2] - m.matrix[2];
  result.matrix[3] = matrix[3] - m.matrix[3];
  result.matrix[4] = matrix[4] - m.matrix[4];
  result.matrix[5] = matrix[5] - m.matrix[5];
  result.matrix[6] = matrix[6] - m.matrix[6];
  result.matrix[7] = matrix[7] - m.matrix[7];
  result.matrix[8] = matrix[8] - m.matrix[8];
  result.matrix[9] = matrix[9] - m.matrix[9];
  result.matrix[10] = matrix[10] - m.matrix[10];
  result.matrix[11] = matrix[11] - m.matrix[11];
  result.matrix[12] = matrix[12] - m.matrix[12];
  result.matrix[13] = matrix[13] - m.matrix[13];
  result.matrix[14] = matrix[14] - m.matrix[14];
  result.matrix[15] = matrix[15] - m.matrix[15];
  return result;
}

inline mat4 mat4::operator*(const mat4 &m) const {
  mat4 result;
  result.matrix[0] = m.matrix[0] * matrix[0] + m.matrix[1] * matrix[4] + m.matrix[2] * matrix[8] + m.matrix[3] * matrix[12];
  result.matrix[1] = m.matrix[0] * matrix[1] + m.matrix[1] * matrix[5] + m.matrix[2] * matrix[9] + m.matrix[3] * matrix[13];
  result.matrix[2] = m.matrix[0] * matrix[2] + m.matrix[1] * matrix[6] + m.matrix[2] * matrix[10] + m.matrix[3] * matrix[14];
  result.matrix[3] = m.matrix[0] * matrix[3] + m.matrix[1] * matrix[7] + m.matrix[2] * matrix[11] + m.matrix[3] * matrix[15];
  result.matrix[4] = m.matrix[4] * matrix[0] + m.matrix[5] * matrix[4] + m.matrix[6] * matrix[8] + m.matrix[7] * matrix[12];
  result.matrix[5] = m.matrix[4] * matrix[1] + m.matrix[5] * matrix[5] + m.matrix[6] * matrix[9] + m.matrix[7] * matrix[13];
  result.matrix[6] = m.matrix[4] * matrix[2] + m.matrix[5] * matrix[6] + m.matrix[6] * matrix[10] + m.matrix[7] * matrix[14];
  result.matrix[7] = m.matrix[4] * matrix[3] + m.matrix[5] * matrix[7] + m.matrix[6] * matrix[11] + m.matrix[7] * matrix[15];
  result.matrix[8] = m.matrix[8] * matrix[0] + m.matrix[9] * matrix[4] + m.matrix[10] * matrix[8] + m.matrix[11] * matrix[12];
  result.matrix[9] = m.matrix[8] * matrix[1] + m.matrix[9] * matrix[5] + m.matrix[10] * matrix[9] + m.matrix[11] * matrix[13];
  result.matrix[10] = m.matrix[8] * matrix[2] + m.matrix[9] * matrix[6] + m.matrix[10] * matrix[10] + m.matrix[11] * matrix[14];
  result.matrix[11] = m.matrix[8] * matrix[3] + m.matrix[9] * matrix[7] + m.matrix[10] * matrix[11] + m.matrix[11] * matrix[15];
  result.matrix[12] = m.matrix[12] * matrix[0] + m.matrix[13] * matrix[4] + m.matrix[14] * matrix[8] + m.matrix[15] * matrix[12];
  result.matrix[13] = m.matrix[12] * matrix[1] + m.matrix[13] * matrix[5] + m.matrix[14] * matrix[9] + m.matrix[15] * matrix[13];
  result.matrix[14] = m.matrix[12] * matrix[2] + m.matrix[13] * matrix[6] + m.matrix[14] * matrix[10] + m.matrix[15] * matrix[14];
  result.matrix[15] = m.matrix[12] * matrix[3] + m.matrix[13] * matrix[7] + m.matrix[14] * matrix[11] + m.matrix[15] * matrix[15];
  return result;
}

inline mat4 mat4::operator*(float scalar) const {
  mat4 result(*this);
  result.matrix[0] *= scalar;
  result.matrix[1] *= scalar;
  result.matrix[2] *= scalar;
  result.matrix[3] *= scalar;
  result.matrix[4] *= scalar;
  result.matrix[5] *= scalar;
  result.matrix[6] *= scalar;
  result.matrix[7] *= scalar;
  result.matrix[8] *= scalar;
  result.matrix[9] *= scalar;
  result.matrix[10] *= scalar;
  result.matrix[11] *= scalar;
  result.matrix[12] *= scalar;
  result.matrix[13] *= scalar;
  result.matrix[14] *= scalar;
  result.matrix[15] *= scalar;
  return result;
}

inline mat4 mat4::operator/(float scalar) const {
  float invScalar = ((float)1) / scalar;
  mat4 result(*this);
  result.matrix[0] *= invScalar;
  result.matrix[1] *= invScalar;
  result.matrix[2] *= invScalar;
  result.matrix[3] *= invScalar;
  result.matrix[4] *= invScalar;
  result.matrix[5] *= invScalar;
  result.matrix[6] *= invScalar;
  result.matrix[7] *= invScalar;
  result.matrix[8] *= invScalar;
  result.matrix[9] *= invScalar;
  result.matrix[10] *= invScalar;
  result.matrix[11] *= invScalar;
  result.matrix[12] *= invScalar;
  result.matrix[13] *= invScalar;
  result.matrix[14] *= invScalar;
  result.matrix[15] *= invScalar;
  return result;
}

inline mat4 &mat4::makeIdentity() {
  *this = kIdentity;
  return *this;
}

inline mat4 &mat4::makeZero() {
  // memset_s(&matrix, 0x00, 4*4*sizeof(float));
  matrix[0] = 0.f;
  matrix[1] = 0.f;
  matrix[2] = 0.f;
  matrix[3] = 0.f;
  matrix[4] = 0.f;
  matrix[5] = 0.f;
  matrix[6] = 0.f;
  matrix[7] = 0.f;
  matrix[8] = 0.f;
  matrix[9] = 0.f;
  matrix[10] = 0.f;
  matrix[11] = 0.f;
  matrix[12] = 0.f;
  matrix[13] = 0.f;
  matrix[14] = 0.f;
  matrix[15] = 0.f;
  return *this;
}

inline mat4 mat4::translation(vec3 v) {
  return translation(v.x, v.y, v.z);
}

inline mat4 mat4::translation(float x, float y, float z) {
  mat4 result(kIdentity);
  result.matrix[12] = x;
  result.matrix[13] = y;
  result.matrix[14] = z;
  return result;
}

inline mat4 mat4::scale(float x, float y, float z) {
  mat4 result(kIdentity);
  result.matrix[0] = x;
  result.matrix[5] = y;
  result.matrix[10] = z;
  return result;
}

inline mat4 &mat4::transposeIt() {
  float temp;
  
  temp = matrix[1];
  matrix[1] = matrix[4];
  matrix[4] = temp;
  
  temp = matrix[2];
  matrix[2] = matrix[8];
  matrix[8] = temp;
  
  temp = matrix[3];
  matrix[3] = matrix[12];
  matrix[12] = temp;
  
  temp = matrix[6];
  matrix[6] = matrix[9];
  matrix[9] = temp;
  
  temp = matrix[7];
  matrix[7] = matrix[13];
  matrix[13] = temp;
  
  temp = matrix[11];
  matrix[11] = matrix[14];
  matrix[14] = temp;

  return *this;
}

inline mat4 &mat4::InverseIt() {
  float fA0 = matrix[0] * matrix[5] - matrix[1] * matrix[4];
  float fA1 = matrix[0] * matrix[6] - matrix[2] * matrix[4];
  float fA2 = matrix[0] * matrix[7] - matrix[3] * matrix[4];
  float fA3 = matrix[1] * matrix[6] - matrix[2] * matrix[5];
  float fA4 = matrix[1] * matrix[7] - matrix[3] * matrix[5];
  float fA5 = matrix[2] * matrix[7] - matrix[3] * matrix[6];
  float fB0 = matrix[8] * matrix[13] - matrix[9] * matrix[12];
  float fB1 = matrix[8] * matrix[14] - matrix[10] * matrix[12];
  float fB2 = matrix[8] * matrix[15] - matrix[11] * matrix[12];
  float fB3 = matrix[9] * matrix[14] - matrix[10] * matrix[13];
  float fB4 = matrix[9] * matrix[15] - matrix[11] * matrix[13];
  float fB5 = matrix[10] * matrix[15] - matrix[11] * matrix[14];

  float fDet =
      fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;

  float fInvDet = float(1) / fDet;
  float temp[16];

  temp[0] = (+matrix[5] * fB5 - matrix[6] * fB4 + matrix[7] * fB3) * fInvDet;
  temp[1] = (-matrix[1] * fB5 + matrix[2] * fB4 - matrix[3] * fB3) * fInvDet;
  temp[2] = (+matrix[13] * fA5 - matrix[14] * fA4 + matrix[15] * fA3) * fInvDet;
  temp[3] = (-matrix[9] * fA5 + matrix[10] * fA4 - matrix[11] * fA3) * fInvDet;
  temp[4] = (-matrix[4] * fB5 + matrix[6] * fB2 - matrix[7] * fB1) * fInvDet;
  temp[5] = (+matrix[0] * fB5 - matrix[2] * fB2 + matrix[3] * fB1) * fInvDet;
  temp[6] = (-matrix[12] * fA5 + matrix[14] * fA2 - matrix[15] * fA1) * fInvDet;
  temp[7] = (+matrix[8] * fA5 - matrix[10] * fA2 + matrix[11] * fA1) * fInvDet;
  temp[8] = (+matrix[4] * fB4 - matrix[5] * fB2 + matrix[7] * fB0) * fInvDet;
  temp[9] = (-matrix[0] * fB4 + matrix[1] * fB2 - matrix[3] * fB0) * fInvDet;
  temp[10] =
      (+matrix[12] * fA4 - matrix[13] * fA2 + matrix[15] * fA0) * fInvDet;
  temp[11] = (-matrix[8] * fA4 + matrix[9] * fA2 - matrix[11] * fA0) * fInvDet;
  temp[12] = (-matrix[4] * fB3 + matrix[5] * fB1 - matrix[6] * fB0) * fInvDet;
  temp[13] = (+matrix[0] * fB3 - matrix[1] * fB1 + matrix[2] * fB0) * fInvDet;
  temp[14] =
      (-matrix[12] * fA3 + matrix[13] * fA1 - matrix[14] * fA0) * fInvDet;
  temp[15] = (+matrix[8] * fA3 - matrix[9] * fA1 + matrix[10] * fA0) * fInvDet;

  memcpy(matrix, temp, 16 * sizeof(float));
  return *this;
}

inline vec3 mat4::operator*(const vec3 &p) const {
  return vec3(matrix[0] * p.x + matrix[1] * p.y + matrix[2] * p.z,
              matrix[4] * p.x + matrix[5] * p.y + matrix[6] * p.z,
              matrix[8] * p.x + matrix[9] * p.y + matrix[10] * p.z);
}

inline mat4 mat4::rotation(const vec3 &vAxis, float fAngle) {
  float cs = cosf(fAngle);
  float sn = sinf(fAngle);
  float oneMinusCos = 1.0f - cs;
  float x2 = vAxis.x * vAxis.x;
  float y2 = vAxis.y * vAxis.y;
  float z2 = vAxis.z * vAxis.z;
  float xym = vAxis.x * vAxis.y * oneMinusCos;
  float xzm = vAxis.x * vAxis.z * oneMinusCos;
  float yzm = vAxis.y * vAxis.z * oneMinusCos;
  float xSin = vAxis.x * sn;
  float ySin = vAxis.y * sn;
  float zSin = vAxis.z * sn;

  mat4 result;
  result.matrix[0] = x2 * oneMinusCos + cs;
  result.matrix[1] = xym - zSin;
  result.matrix[2] = xzm + ySin;
  result.matrix[3] = 0.0f;
  result.matrix[4] = xym + zSin;
  result.matrix[5] = y2 * oneMinusCos + cs;
  result.matrix[6] = yzm - xSin;
  result.matrix[7] = 0.0f;
  result.matrix[8] = xzm - ySin;
  result.matrix[9] = yzm + xSin;
  result.matrix[10] = z2 * oneMinusCos + cs;
  result.matrix[11] = 0.0f;
  result.matrix[12] = 0.0f;
  result.matrix[13] = 0.0f;
  result.matrix[14] = 0.0f;
  result.matrix[15] = 1.0f;

  return result;
}

inline vec4 mat4::operator*(const vec4 &p) const {
  return vec4(matrix[0] * p.x + matrix[4] * p.y + matrix[8] * p.z + matrix[12] * p.w,
              matrix[1] * p.x + matrix[5] * p.y + matrix[9] * p.z + matrix[13] * p.w,
              matrix[2] * p.x + matrix[6] * p.y + matrix[10] * p.z + matrix[14] * p.w,
              matrix[3] * p.x + matrix[7] * p.y + matrix[11] * p.z + matrix[15] * p.w);
}

inline void mat4::ToArray(float *array) const {
  memcpy( array, matrix, 16 * sizeof(float) );
}

inline void mat4::ToColumnOrderedArray(float *array) const {
  array[0] = matrix[0];
  array[1] = matrix[4];
  array[2] = matrix[8];
  array[3] = matrix[12];
  array[4] = matrix[1];
  array[5] = matrix[5];
  array[6] = matrix[9];
  array[7] = matrix[13];
  array[8] = matrix[2];
  array[9] = matrix[6];
  array[10] = matrix[10];
  array[11] = matrix[14];
  array[12] = matrix[3];
  array[13] = matrix[7];
  array[14] = matrix[11];
  array[15] = matrix[15];
}
