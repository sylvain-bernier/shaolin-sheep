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

#include "transform.h"
#include <QtOpenGL>
#include <cmath>

Transform::Transform()
  :m_translation(0., 0., 0.),
   m_rotation(),
   m_scaling(1., 1., 1.)
{
}

// -------------------------------------------------------------------------
// apply() : apply opengl transformations
//
// note : things will first be scaled, rotated, and then translated
// -------------------------------------------------------------------------

void Transform::apply() const
{
  glTranslated(m_translation.x(), m_translation.y(), m_translation.z());
  glMultMatrixd(m_rotation.array());
  glScaled(m_scaling.x(), m_scaling.y(), m_scaling.z());
}

// -------------------------------------------------------------------------
// clear() : clear the object's transforms
// -------------------------------------------------------------------------

void Transform::clear()
{
  clearTranslation();
  clearRotation();
  clearScaling();
}

void Transform::clearTranslation()
{
  m_translation.clear();
}

void Transform::clearRotation()
{
  m_rotation.setIdentity();
}

void Transform::clearScaling()
{
  m_scaling.set(1., 1., 1.);
}

// -------------------------------------------------------------------------
// access/modification to all three operations
// -------------------------------------------------------------------------

const Vector& Transform::translation() const
{
  return m_translation;
}

const Vector& Transform::scaling() const
{
  return m_scaling;
}

const Matrix& Transform::rotation() const
{
  return m_rotation;
}

void Transform::setTranslation(const Vector& v)
{
  m_translation.set(v);
}

void Transform::addTranslation(const Vector& v)
{
  m_translation += v;
}

void Transform::setRotation(double degrees, const Vector& v)
{
  clearRotation();
  addRotation(degrees, v);
}

bool Transform::addRotation(double degrees, const Vector& v)
{
  Vector nv = v; // will hold normalized 'v' vector

  // can't do anything is the rotation axis is null
  if ((degrees == 0.) || !nv.l2normalize()) return false;

  // the rotation matrix is exactly as defined in the blue book,
  // under the glRotate entry.

  double x = nv.x(); double y = nv.y(); double z = nv.z();
  double c = cos((degrees * M_PI) / 180.);
  double s = sin((degrees * M_PI) / 180.);

  Matrix rot
    ((x*x)*(1.-c)+c,     (x*y)*(1.-c)-(z*s), (x*z)*(1.-c)+(y*s), 0.,
     (y*x)*(1.-c)+(z*s), (y*y)*(1.-c)+c,     (y*z)*(1.-c)-(x*s), 0.,
     (x*z)*(1.-c)-(y*s), (y*z)*(1.-c)+(x*s), (z*z)*(1.-c)+c,     0.,
     0.,                 0.,                 0.,                 1.);

  // matrix is transposed so it will work with glMultMatrix
  rot.transpose();
  m_rotation = m_rotation * rot;
  return true;
}

void Transform::setScaling(const Vector& v)
{
  m_scaling.set(v);
}

void Transform::addScaling(const Vector& v)
{
  m_scaling.set(m_scaling.x() * v.x(),
                m_scaling.y() * v.y(),
                m_scaling.z() * v.z());
}
