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

#ifndef TUBE_H
#define TUBE_H
class   Tube;

#include "quadric.h"

class Tube : public Quadric
//
// Tube : GLU quadric as defined by gluCylinder. The tube will have
//        its base at the origin and will extend along the z-axis.
//
{
 public:
  Tube(double i_top_radius, double i_bas_radius, double i_length);

  void setRadius(double i_top_radius, double i_bas_radius);

  double length() const;

 protected:
  virtual void drawQuadric(GLUquadric* i_quadric);

 private:
  double m_top_radius;
  double m_bas_radius;
  double m_length;
};

#endif // TUBE_H
