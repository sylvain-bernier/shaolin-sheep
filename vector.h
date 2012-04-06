/*
    Shaolin Sheep - OpenGL/Qt Demo
    Copyright (c) 2006  Sylvain Bernier <sylvain.bernier@gmail.com>

    This file is part of Shaolin Sheep.

    Shaolin Sheep is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Shaolin Sheep is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Shaolin Sheep; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef VECTOR_H
#define VECTOR_H
class   Vector;

class Vector
//
// Vector : simple three component vector
//
{
 public:
  Vector();
  Vector(double x, double y, double z);
  Vector(const Vector& v);

  void set(double x, double y, double z);
  void set(const Vector& v);
  void clear();

  bool isNull() const;

  double x() const;
  double y() const;
  double z() const;

  void setX(double v);
  void setY(double v);
  void setZ(double v);

  double operator[](int i) const;

  double l2norm() const;
  bool   l2normalize();

  double dotProduct(const Vector& i_vector) const;

  Vector operator+(const Vector& i_vector) const;
  Vector operator-(const Vector& i_vector) const;
  Vector operator*(double i_scalar) const;
  Vector operator/(double i_scalar) const;

  Vector& operator+=(const Vector& i_vector);
  Vector& operator-=(const Vector& i_vector);
  Vector& operator*=(double i_scalar);
  Vector& operator/=(double i_scalar);

  // useful vector constants
  static const Vector i;
  static const Vector j;
  static const Vector k;

 private:
  double vp[3];
};

#endif // VECTOR_H
