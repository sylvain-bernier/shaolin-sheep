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

#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H
class   BoundingSphere;

class Transform;
#include "vector.h"

class BoundingSphere
//
// BoudingSphere : spherical bounding information defined by a center point
//                 and a radius.
//
{
 public:
  BoundingSphere(double i_radius = 0., const Vector& i_center = Vector());

  bool isNull() const;
  double radius() const;
  const Vector& center() const;

  bool intersects(const BoundingSphere& s) const;
  bool contains(const BoundingSphere& s) const;
  BoundingSphere theUnion(const BoundingSphere& s) const;

  void applyTransform(const Transform& t);

  const BoundingSphere& operator=(const BoundingSphere& obj);

 private:
  double m_radius;  // radius of the sphere
  Vector m_center;  // center of the sphere
};

#endif // BOUNDINGSPHERE_H
