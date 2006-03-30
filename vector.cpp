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

#include "vector.h"
#include <cmath>

Vector::Vector()
{
  clear();
}

Vector::Vector(double x, double y, double z)
{
  set(x, y, z);
}

Vector::Vector(const Vector& v)
{
  set(v);
}

void Vector::set(double x, double y, double z)
{
  vp[0] = x; vp[1] = y; vp[2] = z;
}

void Vector::set(const Vector& v)
{
  set(v.x(), v.y(), v.z());
}

void Vector::clear()
{
  set(0., 0., 0.);
}

bool Vector::isNull() const
{
  return (x() == 0.) && (y() == 0.) && (z() == 0.);
}

double Vector::x() const
{
  return vp[0];
}

double Vector::y() const
{
  return vp[1];
}

double Vector::z() const
{
  return vp[2];
}

void Vector::setX(double v)
{
  vp[0] = v;
}

void Vector::setY(double v)
{
  vp[1] = v;
}

void Vector::setZ(double v)
{
  vp[2] = v;
}

double Vector::operator[](int i) const
{
  // out of range index will return an invalid value
  return (i < 0 || i > 2) ? NAN : vp[i];
}

// -------------------------------------------------------------------------
// l2norm : l2 norm is defined as sqrt(x^2 + y^2 + z^2)
// -------------------------------------------------------------------------

double Vector::l2norm() const
{
  return sqrt(x()*x() + y()*y() + z()*z());
}

// -------------------------------------------------------------------------
// l2normalize : normalize the vector using its l2 norm
//
// return value : true if the vector could be normalized (l2 norm > 0)
// -------------------------------------------------------------------------

bool Vector::l2normalize()
{
  // vector will be normalized only if it isn't null
  double norm = l2norm();
  if (norm != 0.) {
    (*this) /= norm;
    return true;
  }
  return false;
}

double Vector::dotProduct(const Vector& i_vector) const
{
  return (x()*i_vector.x() + y()*i_vector.y() + z()*i_vector.z());
}

Vector Vector::operator+(const Vector& i_vector) const
{
  return Vector(x()+i_vector.x(), y()+i_vector.y(), z()+i_vector.z());
}

Vector Vector::operator-(const Vector& i_vector) const
{
  return Vector(x()-i_vector.x(), y()-i_vector.y(), z()-i_vector.z());
}

Vector Vector::operator*(double i_scalar) const
{
  return Vector(x()*i_scalar, y()*i_scalar, z()*i_scalar);
}

Vector Vector::operator/(double i_scalar) const
{
  return Vector(x()/i_scalar, y()/i_scalar, z()/i_scalar);
}

Vector& Vector::operator+=(const Vector& i_vector)
{
  vp[0] += i_vector.vp[0]; vp[1] += i_vector.vp[1]; vp[2] += i_vector.vp[2];
  return (*this);
}

Vector& Vector::operator-=(const Vector& i_vector)
{
  vp[0] -= i_vector.vp[0]; vp[1] -= i_vector.vp[1]; vp[2] -= i_vector.vp[2];
  return (*this);
}

Vector& Vector::operator*=(double i_scalar)
{
  vp[0] *= i_scalar; vp[1] *= i_scalar; vp[2] *= i_scalar;
  return (*this);
}

Vector& Vector::operator/=(double i_scalar)
{
  vp[0] /= i_scalar; vp[1] /= i_scalar; vp[2] /= i_scalar;
  return (*this);
}

// -------------------------------------------------------------------------
// i(), j(), k() : useful vector constants
// -------------------------------------------------------------------------

const Vector Vector::i(1., 0., 0.);
const Vector Vector::j(0., 1., 0.);
const Vector Vector::k(0., 0., 1.);
