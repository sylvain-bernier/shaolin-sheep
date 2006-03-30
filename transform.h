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

#ifndef TRANSFORM_H
#define TRANSFORM_H
class   Transform;

#include "vector.h"
#include "matrix.h"

class Transform
//
// Transform : translation, rotation and scaling operations
//
{
 public:
  Transform();

  // apply opengl transformations
  void apply() const;

  // clear the object's transforms
  void clear();
  void clearTranslation();
  void clearRotation();
  void clearScaling();

  // access/modification to all three operations
  const Vector& translation() const;
  const Vector& scaling() const;
  const Matrix& rotation() const;

  void setTranslation(const Vector& v);
  void addTranslation(const Vector& v);

  void setRotation(double degrees, const Vector& v);
  bool addRotation(double degrees, const Vector& v);

  void setScaling(const Vector& v);
  void addScaling(const Vector& v);

 private:
  Vector m_translation;
  Matrix m_rotation;
  Vector m_scaling;
};

#endif // TRANSFORM_H
