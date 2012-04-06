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

#include "sphere.h"
#include "boundingsphere.h"
#include <QtOpenGL>

Sphere::Sphere(double i_radius)
  :Quadric(),
   m_radius(i_radius)
{
}

double Sphere::radius() const
{
  return m_radius;
}

void Sphere::drawQuadric(GLUquadric* i_quadric)
{
  gluSphere(i_quadric, m_radius, slices(), slices());
}

BoundingSphere Sphere::globject_boundingSphere() const
{
  return BoundingSphere(m_radius);
}
