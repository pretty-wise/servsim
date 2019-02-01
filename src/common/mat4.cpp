#include "mat4.h"

const mat4 mat4::kIdentity(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f);

const mat4 mat4::kZero(0.f, 0.f, 0.f, 0.f,
                       0.f, 0.f, 0.f, 0.f,
                       0.f, 0.f, 0.f, 0.f,
                       0.f, 0.f, 0.f, 0.f);
