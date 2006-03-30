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

#include "ball.h"
#include "vector.h"
#include "transform.h"
#include <cmath>

Ball::Ball(double i_radius)
  :Sphere(i_radius)
{
}

// -------------------------------------------------------------------------
// globject_tick(seconds) : animation tick
//
// notes        : will make the ball roll around
// return value : 'true' if a change occured
// -------------------------------------------------------------------------

bool Ball::globject_tick(double i_sec)
{
  bool res = Sphere::globject_tick(i_sec);

  // only velocities on the X,Z plane are considered
  Vector vel_xz  = velocity(); vel_xz.setY(0.);
  double circ    = (2. * M_PI * radius());
  double degrees = 360. * ((i_sec * vel_xz.l2norm()) / circ);

  // rotation is applied around the vector perpendicular (X,Z) to velocity
  Vector vec_90(vel_xz.z(), 0., -vel_xz.x());
  if (transform().addRotation(degrees, vec_90)) res = true;
  return res;
}
