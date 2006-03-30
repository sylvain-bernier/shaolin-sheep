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

#include "cylinder.h"
#include "disk.h"
#include "tube.h"
#include "texture.h"
#include "material.h"
#include "transform.h"

Cylinder::Cylinder(double i_top_radius, double i_bas_radius, double i_height)
  :Globject(),
   mp_disk_top(new Disk(i_top_radius)),
   mp_disk_bas(new Disk(i_bas_radius)),
   mp_tube_outer(new Tube(i_top_radius, i_bas_radius, i_height)),
   mp_tube_inner(0)
{
  if (mp_disk_top) {
    mp_disk_top->transform().setTranslation(Vector(0., 0., height()));
    addChild(mp_disk_top);
  }

  if (mp_disk_bas) {
    mp_disk_bas->setOutsideIn(true);
    addChild(mp_disk_bas);
  }

  if (mp_tube_outer) addChild(mp_tube_outer);
}

Cylinder::~Cylinder()
{
  // all the cylinder parts should be children of the gloject
  mp_disk_top   = 0;
  mp_disk_bas   = 0;
  mp_tube_outer = 0;
  mp_tube_inner = 0;
}

double Cylinder::height() const
{
  return (mp_tube_outer ? mp_tube_outer->length() : 0.);
}

void Cylinder::setInnerRadius(double i_top_radius, double i_bas_radius)
{
  // first, we need to adjust the top and base of the cylinder
  if (mp_disk_top) mp_disk_top->setInnerRadius(i_top_radius);
  if (mp_disk_bas) mp_disk_bas->setInnerRadius(i_bas_radius);

  if ((i_top_radius == 0.) && (i_bas_radius == 0.)) {
    // we then either remove the inner tube
    removeChild(mp_tube_inner);
    delete mp_tube_inner; mp_tube_inner = 0;
  }
  else {
    if (mp_tube_inner == 0) {
      // create a new one
      mp_tube_inner = new Tube(i_top_radius, i_bas_radius, height());
      mp_tube_inner->setOutsideIn(true);
      mp_tube_inner->setTexture(mp_tube_outer->texture());
      mp_tube_inner->setSlices(mp_tube_outer->slices());
      addChild(mp_tube_inner);

      // set the material if one has been applied before
      const Material* p = mp_tube_outer->material();
      if (p) mp_tube_inner->setMaterial(*p);
    }
    else {
      // or adjust the radius of the current one
      mp_tube_inner->setRadius(i_top_radius, i_bas_radius);
    }
  }
}

void Cylinder::setTexture(Texture* i_texture)
{
  for (vec_globject::const_iterator it = children().begin();
       it != children().end(); it++)
    ((Quadric*)(*it))->setTexture(i_texture);
}

void Cylinder::setMaterial(const Material& i_material)
{
  for (vec_globject::const_iterator it = children().begin();
       it != children().end(); it++)
    ((Quadric*)(*it))->setMaterial(i_material);
}

bool Cylinder::setSlices(int i_slices)
{
  bool res = false;
  for (vec_globject::const_iterator it = children().begin();
       it != children().end(); it++)
    if (((Quadric*)(*it))->setSlices(i_slices)) res = true;
  return res;
}

int Cylinder::slices() const
{
  return (mp_tube_outer ? mp_tube_outer->slices() : 0);
}
