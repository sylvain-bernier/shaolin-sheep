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

#ifndef CYLINDER_H
#define CYLINDER_H
class   Cylinder;

class Texture;
class Material;
class Disk;
class Tube;
#include "globject.h"

class Cylinder : public Globject
//
// Cylinder : a cylinder globject defined by its top, base radius and
//            its height. The cylinder will have its base sitting on
//            the origin and will rise along the z-axis.
//
// notes    : if an inner radius is defined, there will be a hole
//            inside the cylinder.
//
{
 public:
  Cylinder(double i_top_radius, double i_bas_radius, double i_height);
  ~Cylinder();

  double height() const;

  void setInnerRadius(double i_top_radius, double i_bas_radius);
  void setTexture(Texture* i_texture);
  void setMaterial(const Material& i_material);

  // subdivision settings
  bool setSlices(int i_slices);
  int  slices() const;

 private:
  Disk* mp_disk_top;    // disk at the top of the cylinder
  Disk* mp_disk_bas;    // disk at the base of the cylinder (origin)
  Tube* mp_tube_outer;  // outer tube, along the z-axis
  Tube* mp_tube_inner;  // inner tube, if an inner radius is defined
};

#endif // CYLINDER_H
