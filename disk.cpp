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

#include "disk.h"
#include <QtOpenGL>

#define LOOPS  1

Disk::Disk(double i_outer_radius, double i_inner_radius)
  :Quadric(),
   m_outer_radius(i_outer_radius),
   m_inner_radius(i_inner_radius)
{
}

// -------------------------------------------------------------------------
// setInnerRadius(radius) : an inner radius greater than 0 will make a hole
//                          inside the disk
// -------------------------------------------------------------------------

void Disk::setInnerRadius(double i_inner_radius)
{
  m_inner_radius = i_inner_radius;
}

void Disk::drawQuadric(GLUquadric* i_quadric)
{
  gluDisk(i_quadric, m_inner_radius, m_outer_radius, slices(), LOOPS);
}
