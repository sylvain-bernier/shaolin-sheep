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

#include "physics.h"
#include "globject.h"
#include "vector.h"
#include "boundingsphere.h"

// -------------------------------------------------------------------------
// tick(seconds, container) : apply simple physics to container's children
//
// seconds   : time elapsed since last tick (in seconds)
// container : globject holding the objects to which the physics will apply
// return value : 'true' if a change occured
// -------------------------------------------------------------------------

bool Physics::tick(double i_sec, Globject& i_container)
{
  // physics will be calculated for each 1/1000 of a second
  // (this value must match the longest time spent in one
  //  iteration of the 'while' loop. the worst case)
  static const double CALCULATION_TIME = 0.050;

  // gravitational acceleration
  static const Vector G(0., -9.8, 0.);

  // velocity loss due to friction, etc. : 20% per second
  static const double FRICTION = 0.20;
  bool res = false;

  double time_left = i_sec;
  const Globject::vec_globject& objs = i_container.children();
  BoundingSphere limits = i_container.containerLimits();

  while (time_left > 0.) {
    double delta_t = (CALCULATION_TIME < time_left ?
                      CALCULATION_TIME : time_left);
    time_left -= delta_t;

    // gravity + movement
    for (Globject::vec_globject::const_iterator it = objs.begin();
         it != objs.end(); it++) {
      if ((*it)->movable()) {
        // friction
        (*it)->setVelocity((*it)->velocity() * (1. - (FRICTION * delta_t)));

        // gravity
        (*it)->setVelocity((*it)->velocity() + (G * delta_t));

        // movement / animation
        if ((*it)->tick(delta_t)) res = true;
      }
    }

    // collision check
    for (unsigned int i = 0; i < objs.size(); i++) {
      if (objs[i]->checkLimits(limits)) res = true;

      for (unsigned int j = i + 1; j < objs.size(); j++) {
        if (objs[i]->introduceTo(*(objs[j]))) res = true;
      }
    }
  }
  return res;
}
