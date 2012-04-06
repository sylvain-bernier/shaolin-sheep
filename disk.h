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

#ifndef DISK_H
#define DISK_H
class   Disk;

#include "quadric.h"

class Disk : public Quadric
//
// Disk : GLU quadric as defined by gluDisk
//
{
 public:
  Disk(double i_outer_radius = 1., double i_inner_radius = 0.);

  // an inner radius greater than 0 will make a hole inside the disk
  void setInnerRadius(double i_inner_radius);

 protected:
  virtual void drawQuadric(GLUquadric* i_quadric);

 private:
  double m_outer_radius;
  double m_inner_radius;
};

#endif // DISK_H
