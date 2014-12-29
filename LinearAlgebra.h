#pragma once
#include "types.h"

namespace dawn
{
  static mat4f rotation(double angle, char axis)
  {
    mat4f r;

    double cov = cos(angle);
    double siv = sin(angle);

    switch (axis)
    {
      case 'x':
        r << 1.0f, 0.0f, 0.0f, 0.0f,
             0.0f,  cov, -siv, 0.0f,
             0.0f,  siv,  cov, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f;
        break;
      case 'y':
        r <<  cov, 0.0f,  siv, 0.0f,
             0.0f, 1.0f, 0.0f, 0.0f,
             -siv, 0.0f,  cov, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f;
        break;
      case 'z':
        r <<  cov, -siv, 0.0f, 0.0f,
              siv,  cov, 0.0f, 0.0f,
             0.0f, 0.0f, 1.0f, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f;
        break;
    }

    return r;
  }

  static mat4f translation(float x, float y, float z)
  {
    mat4f t;
    t <<  1.0f, 0.0f, 0.0f, x,
          0.0f, 1.0f, 0.0f, y,
          0.0f, 0.0f, 1.0f, z,
          0.0f, 0.0f, 0.0f, 1.0f;

    return t;
  }

  static mat4f scaling(float x, float y, float z)
  {
    mat4f s;
    s <<  x   , 0.0f, 0.0f, 0.0f,
          0.0f, y   , 0.0f, 0.0f,
          0.0f, 0.0f, z   , 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f;

    return s;
  }

  static mat4f ortho(float left, float right, float bottom, float top, float near, float far)
  {
    mat4f o = mat4f::Zero();

    o(0,0) =  2.0/(right - left); o(0,3) = -(right + left)/(right - left);
    o(1,1) =  2.0/(top - bottom); o(1,3) = -(top + bottom)/(top - bottom);
    o(2,2) = -2.0/(far - near);   o(2,3) = -(far + near)/(far - near);
    o(3,3) =  1.0;

	  return o;
  }

  static mat4f perspective(float fovy, float aspect, float near, float far)
  {
    mat4f p = mat4f::Zero();

    float f = 1.0f / tan(fovy / 2.0f);

    p(0,0) =  f / aspect;
    p(1,1) =  f;
    p(2,2) =  (far + near) / (near - far);
    p(2,3) = -1.0f;
    p(3,2) =  (2.0f * far * near) / (near - far);

	  return p;
  }
}
