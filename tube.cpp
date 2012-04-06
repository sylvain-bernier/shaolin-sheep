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

#include "tube.h"
#include <QtOpenGL>

#define STACKS  1

Tube::Tube(double i_top_radius, double i_bas_radius, double i_length)
  :Quadric(),
   m_top_radius(i_top_radius),
   m_bas_radius(i_bas_radius),
   m_length(i_length)
{
}

void Tube::setRadius(double i_top_radius, double i_bas_radius)
{
  m_top_radius = i_top_radius;
  m_bas_radius = i_bas_radius;
}

double Tube::length() const
{
  return m_length;
}

void Tube::drawQuadric(GLUquadric* i_quadric)
{
  gluCylinder(i_quadric, m_bas_radius, m_top_radius, m_length,
              slices(), STACKS);
}
