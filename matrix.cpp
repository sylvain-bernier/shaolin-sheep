/*
    Shaolin Sheeps - OpenGL/Qt Demo
    Copyright (c) 2006  Sylvain Bernier <sylvain_bernier03@yahoo.ca>

    This file is part of Shaolin Sheeps.

    Shaolin Sheeps is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Shaolin Sheeps is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Shaolin Sheeps; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "matrix.h"

#define N 4

Matrix::Matrix()
{
  setIdentity();
}

Matrix::Matrix(double m00, double m01, double m02, double m03,
               double m10, double m11, double m12, double m13,
               double m20, double m21, double m22, double m23,
               double m30, double m31, double m32, double m33)
{
  set(m00, m01, m02, m03,
      m10, m11, m12, m13,
      m20, m21, m22, m23,
      m30, m31, m32, m33);
}

double Matrix::m(int i, int j) const
{
  return (mp[i][j]);
}

const double* Matrix::array() const
{
  return (const double*)(mp);
}

void Matrix::setIdentity()
{
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
    mp[i][j] = (i == j ? 1. : 0.);
}

void Matrix::set(double m00, double m01, double m02, double m03,
                 double m10, double m11, double m12, double m13,
                 double m20, double m21, double m22, double m23,
                 double m30, double m31, double m32, double m33)
{
  double* p = (double*)mp;
  *(p++) = m00; *(p++) = m01; *(p++) = m02; *(p++) = m03;
  *(p++) = m10; *(p++) = m11; *(p++) = m12; *(p++) = m13;
  *(p++) = m20; *(p++) = m21; *(p++) = m22; *(p++) = m23;
  *(p++) = m30; *(p++) = m31; *(p++) = m32; *(p++) = m33;
}

void Matrix::transpose()
{
  double t;
  for (int i = 0; i < (N-1); i++)
    for (int j = i+1; j < N; j++) {
      t = mp[i][j];
      mp[i][j] = mp[j][i];
      mp[j][i] = t;
    }
}

Matrix Matrix::operator*(const Matrix& m) const
{
  Matrix res;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      res.mp[i][j] = 0.;
      for (int k = 0; k < N; k++)
        res.mp[i][j] += (mp[i][k] * m.mp[k][j]);
    }

  return res;      
}

const Matrix& Matrix::operator=(const Matrix& m)
{
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      mp[i][j] = m.mp[i][j];

  return (*this);
}
