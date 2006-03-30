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

#ifndef GLOBJECT_H
#define GLOBJECT_H
class   Globject;

class Transform;
class BoundingSphere;
class QGLWidget;
#include "vector.h"
#include <vector>

class Globject
//
// Globject : opengl scene object
//
//   - optional transform (translation, rotation, scaling)
//   - a x,y,z velocity   (if movable)
//   - children globjects (the transform is applied to children.
//                         children are also destroyed with their parent)
//   - drawing / animation mecanism
//   - collision checking mecanism
//
{
 public:
  Globject();
  virtual ~Globject();

  // access to the globject transformations
  Transform& transform();

  // shortcuts to the globject translation transformation
  Vector position() const;
  void setPosition(const Vector& i_position);

  // x, y, z velocity
  const Vector& velocity() const;
  virtual void setVelocity(const Vector& i_velocity);

  // set if the object can move and be affected by collisions
  bool movable() const;
  void setMovable(bool i_movable);

  // add / remove child, child globject is destroyed with the parent
  void addChild(Globject* p);
  bool removeChild(const Globject* p);

  typedef std::vector<Globject*> vec_globject;
  const vec_globject& children() const;

  // common globject drawing request
  void draw(QGLWidget* i_gl);

  // common globject animation request (time unit is the second)
  bool tick(double i_sec);

  // outer and inner (containerLimits) bounding spheres
  BoundingSphere boundingSphere() const;
  BoundingSphere containerLimits() const;
  void setContainerLimits(const BoundingSphere& l);

  // collision / interaction / limits check
  bool introduceTo(Globject& bob, bool is_container = false);
  bool checkLimits(const BoundingSphere& i_limits);

 protected:
  // each globject should specialize these three methods
  virtual void           globject_draw(QGLWidget* i_gl);
  virtual bool           globject_tick(double i_sec);
  virtual BoundingSphere globject_boundingSphere() const;

  // general collision checking using bounding spheres
  bool introduceTo(Globject* bob, const BoundingSphere& bsphere_b,
                   bool is_container);

 private:
  Transform*      mp_transform;       // TRS transformations (0 if none )
  Vector          m_velocity;         // current velocity    (unit : m/s)
  bool            m_movable;          // moves, is affected by collisions
  vec_globject    m_children;
  BoundingSphere* mp_containerLimits; // inner limits for children
};

#endif // GLOBJECT_H
