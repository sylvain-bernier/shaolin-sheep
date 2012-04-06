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

#include "globject.h"
#include "transform.h"
#include "boundingsphere.h"
#include <QGLWidget>
#include <QtOpenGL>
#include <cmath>
#include <algorithm>

Globject::Globject()
  :mp_transform(0),
   m_velocity(),
   m_movable(false),
   m_children(),
   mp_containerLimits(0)
{
  // by default, a globject is not movable
  setMovable(false);
}

Globject::~Globject()
{
  // free dynamically allocated members
  delete mp_transform;       mp_transform = 0;
  delete mp_containerLimits; mp_containerLimits = 0;

  // free all the children
  for (vec_globject::iterator i = m_children.begin();
       i != m_children.end(); i++) {
    delete (*i); (*i) = 0;
  }
  m_children.clear();
}

// -------------------------------------------------------------------------
// transform() : access to the globject transformations
//
// notes : transformations only exist if used
// -------------------------------------------------------------------------

Transform& Globject::transform()
{
  if (mp_transform == 0) mp_transform = new Transform;
  return (*mp_transform);
}

// -------------------------------------------------------------------------
// (set)position() : shortcuts to the globject translation transformation
//
// notes : extra care is taken to prevent uselessly creating a Transform
// -------------------------------------------------------------------------

Vector Globject::position() const
{
  return (mp_transform ? mp_transform->translation() : Vector());
}

void Globject::setPosition(const Vector& i_position)
{
  if (mp_transform || !i_position.isNull())
    transform().setTranslation(i_position);
}

// -------------------------------------------------------------------------
// (set)velocity / (set)movable : x, y, z velocity and movability
//
// movable = if the object can move and be affected by collisions
//           (unmovable objects always have a null velocity)
//
// notes : velocity unit is meter / second
// -------------------------------------------------------------------------

const Vector& Globject::velocity() const
{
  return m_velocity;
}

void Globject::setVelocity(const Vector& i_velocity)
{
  if (movable())
    m_velocity = i_velocity;
}

bool Globject::movable() const
{
  return m_movable;
}

void Globject::setMovable(bool i_movable)
{
  m_movable = i_movable;
  if (!m_movable)
    setVelocity(Vector());
}

// -------------------------------------------------------------------------
// (add/remove)child, children() : add, remove child, access children
//
// notes                    : children are destroyed with the parent
// removeChild return value : 'true' if the child was found and removed
// -------------------------------------------------------------------------

void Globject::addChild(Globject* p)
{
  m_children.push_back(p);
}

bool Globject::removeChild(const Globject* p)
{
  vec_globject::iterator it =
    std::find(m_children.begin(), m_children.end(), p);
  if (it != m_children.end()) {
    m_children.erase(it);
    return true;
  }
  else return false;
}

const Globject::vec_globject& Globject::children() const
{
  return (m_children);
}

// -------------------------------------------------------------------------
// draw(QGLWidget) : common globject drawing request
//
// notes : this is the method to call to draw the globject using opengl.
//         the QGLWidget* is only used when binding textures using qt.
// -------------------------------------------------------------------------

void Globject::draw(QGLWidget* i_gl)
{
  // transformations are applied to this object and all of its children
  if (mp_transform) {
    glPushMatrix();
    mp_transform->apply();
  }

  // first, draw the children
  for (vec_globject::iterator i = m_children.begin();
       i != m_children.end(); i++)
    (*i)->draw(i_gl);

  // then, draw this object
  globject_draw(i_gl);

  // revert back to the original transformation matrix
  if (mp_transform)
    glPopMatrix();
}

// -------------------------------------------------------------------------
// tick(seconds) : common globject animation request ('seconds' elapsed)
//
// return value  : 'true' if the object moved and needs to be redrawn
// -------------------------------------------------------------------------

bool Globject::tick(double i_sec)
{
  bool ret = false;
  for (vec_globject::iterator i = m_children.begin();
       i != m_children.end(); i++)
    if ((*i)->tick(i_sec)) ret = true;

  if (globject_tick(i_sec)) ret = true;
  return ret;
}

// -------------------------------------------------------------------------
// boundingSphere() : outer bounding sphere encompassing all the children
// -------------------------------------------------------------------------

BoundingSphere Globject::boundingSphere() const
{
  BoundingSphere res;
  res = globject_boundingSphere();
  for (vec_globject::const_iterator i = m_children.begin();
       i != m_children.end(); i++)
    res = res.theUnion((*i)->boundingSphere());

  // we need to apply the globject transformations (translation, scaling)
  // for the bounding sphere to be useful to the outside world.
  if (mp_transform)
    res.applyTransform(*mp_transform);
  return res;
}

BoundingSphere Globject::containerLimits() const
{
  if (mp_containerLimits)
    return (*mp_containerLimits);
  else
    return globject_boundingSphere();
}

// -------------------------------------------------------------------------
// setContainerLimits(BoundingSphere) : set the world limits for children
//
// notes : movable children will not be able to wander beyond those limits
// -------------------------------------------------------------------------

void Globject::setContainerLimits(const BoundingSphere& l)
{
  delete mp_containerLimits;
  mp_containerLimits = new BoundingSphere(l);
}

// -------------------------------------------------------------------------
// introduceTo(Globject, is_container) : make the globject aware of the
//   existence of another globject. (mostly for collision checking)
//
// container : 'true' if 'bob' is 'this' container ('this' is inside 'bob')
// return value : 'true' if one of the two globjects moved
// -------------------------------------------------------------------------

bool Globject::introduceTo(Globject& bob, bool is_container)
{
  return introduceTo(&bob, bob.boundingSphere(), is_container);
}

bool Globject::checkLimits(const BoundingSphere& i_limits)
{
  return (introduceTo(0, i_limits, true));
}

void Globject::globject_draw(QGLWidget* i_gl)
{
  // --------------------------------------------------------------------
  // this is where the globject needs to be drawn, using opengl functions
  // --------------------------------------------------------------------
}

bool Globject::globject_tick(double i_sec)
{
  // ---------------------------------------------------------------
  // by default, the object will be translated to match its velocity
  // ---------------------------------------------------------------

  if ((i_sec == 0) || !movable() || m_velocity.isNull())
    return false;

  transform().addTranslation(m_velocity * i_sec);
  return true;
}

BoundingSphere Globject::globject_boundingSphere() const
{
  // ---------------------------------------------------
  // each globject should return a valid bounding sphere
  // ---------------------------------------------------

  if (mp_containerLimits)
    return (*mp_containerLimits);
  else
    return BoundingSphere();
}

// -------------------------------------------------------------------------
// introduceTo(Globject, BS, is_container) : make the globject aware of the
//   existence of another globject. (mostly for collision checking)
//
// bob       : if non-null, the object will be moved if a collision occured
// BS        : the bounding sphere associated with 'bob'
// container : 'true' if 'bob' is 'this' container ('this' is inside 'bob')
// return value : 'true' if one of the two globjects moved
// -------------------------------------------------------------------------

bool Globject::introduceTo(Globject* bob, const BoundingSphere& bsphere_b,
                           bool is_container)
{
  // if neither object is movable, there is no use to check for collision
  if (!movable() && ((!bob) || !bob->movable())) return false;

  // direction from this bounding sphere to bob's
  BoundingSphere bsphere_a = boundingSphere();
  Vector direction = bsphere_b.center() - bsphere_a.center();

  // distance between the two sphere centers
  double distance  = direction.l2norm();
  direction.l2normalize();

  // distance between the two sphere surfaces
  {
    double radius_a  = bsphere_a.radius();
    double radius_b  = bsphere_b.radius();

    // if 'this' is inside 'bob', we will use the mirror image of 'bob'
    if (is_container) {
      direction *= -1.;
      distance = (radius_b - distance) + radius_b;
    }

    distance -= (radius_a + radius_b);
  }

  if (distance < 0.) {
    // BANG!
    double vel_transfer_a = 0., vel_transfer_b = 0.;
    {
      // only use velocities in the direction of 'bob'
      Vector velocity_a = velocity();
      if ((velocity_a.x()< 0.) != ( direction.x()< 0.)) velocity_a.setX(0.);
      if ((velocity_a.y()< 0.) != ( direction.y()< 0.)) velocity_a.setY(0.);
      if ((velocity_a.z()< 0.) != ( direction.z()< 0.)) velocity_a.setZ(0.);

      vel_transfer_a = velocity_a.l2norm();
      if (vel_transfer_a > 0.) {
        velocity_a.l2normalize();
        // the angle of collision will affect the transfered velocity
        double cos_theta = direction.dotProduct(velocity_a);
        vel_transfer_a *= cos_theta;
      }
    }
    if (bob) {
      // only use velocities in the direction of 'this'
      Vector velocity_b = bob->velocity();
      if ((velocity_b.x()< 0.) != (-direction.x()< 0.)) velocity_b.setX(0.);
      if ((velocity_b.y()< 0.) != (-direction.y()< 0.)) velocity_b.setY(0.);
      if ((velocity_b.z()< 0.) != (-direction.z()< 0.)) velocity_b.setZ(0.);

      vel_transfer_b = velocity_b.l2norm();
      if (vel_transfer_b > 0.) {
        velocity_b.l2normalize();
        // the angle of collision will affect the transfered velocity
        double cos_theta = (direction * -1.).dotProduct(velocity_b);
        vel_transfer_b *= cos_theta;
      }
    }
    // velocity change
    {
      Vector vec_a = direction *  vel_transfer_a;
      Vector vec_b = direction * -vel_transfer_b;

      static const double LOSS = 0.20;
             setVelocity(velocity()      - vec_a + (vec_b * (1. - LOSS)));
      if (bob)
        bob->setVelocity(bob->velocity() - vec_b + (vec_a * (1. - LOSS)));
    }
    // position correction (if one object is partly inside the other)
    {
      if (movable() && bob && bob->movable())
        distance /= 2.;

      // move the objects apart from each other
      if (movable())
        transform().addTranslation(direction * distance);

      if (bob && bob->movable())
        bob->transform().addTranslation(direction * -distance);
    }
    return true;
  }
  else return false;
}
