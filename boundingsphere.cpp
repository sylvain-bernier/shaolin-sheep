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

#include "boundingsphere.h"
#include "transform.h"

BoundingSphere::BoundingSphere(double i_radius, const Vector& i_center)
  :m_radius(i_radius),
   m_center(i_center)
{
  // we won't tolerate any invalid bounding sphere!
  if (m_radius <= 0.) {
    m_radius = 0.;
    m_center = Vector(0., 0., 0.);
  }
}

bool BoundingSphere::isNull() const
{
  return (m_radius == 0.);
}

double BoundingSphere::radius() const
{
  return m_radius;
}

const Vector& BoundingSphere::center() const
{
  return m_center;
}

// -------------------------------------------------------------------------
// intersects(bounding sphere) : 'true' if the edges overlap
// -------------------------------------------------------------------------

bool BoundingSphere::intersects(const BoundingSphere& s) const
{
  // no intersection if one of the two spheres is null
  if (isNull() || s.isNull()) return false;

  double d = (m_center - s.m_center).l2norm();
  double min = (m_radius < s.m_radius ? m_radius : s.m_radius);
  double max = (m_radius > s.m_radius ? m_radius : s.m_radius);

  return ((d < (min + max)) && (d > (max - min)));
}

// -------------------------------------------------------------------------
// contains(bounding sphere) : 'true' if the sphere 's' is fully inside
// -------------------------------------------------------------------------

bool BoundingSphere::contains(const BoundingSphere& s) const
{
  // a null sphere contains nothing
  if (isNull()) return false;

  // a non-null sphere contains any null sphere
  if (s.isNull()) return true;

  double d = (m_center - s.m_center).l2norm();
  return (d <= m_radius - s.m_radius);
}

// -------------------------------------------------------------------------
// theUnion(bounding sphere) : return a sphere that encompass both spheres
// -------------------------------------------------------------------------

BoundingSphere BoundingSphere::theUnion(const BoundingSphere& s) const
{
  // if one sphere contains another, there is no need for a new sphere
  if (contains(s)) return (*this);
  if (s.contains(*this)) return s;

  // we'll use the distance and the vector between the two spheres
  Vector v = s.m_center - m_center;
  double d = v.l2norm();
  v.l2normalize();

  // the new sphere center will be along the vector
  double r = (m_radius + d + s.m_radius) / 2.;
  Vector c = m_center + (v * (r - m_radius));
  return BoundingSphere(r, c);
}

// -------------------------------------------------------------------------
// applyTransform(tranform) : apply translation and scaling to the sphere
// -------------------------------------------------------------------------

void BoundingSphere::applyTransform(const Transform& t)
{
  if (isNull()) return;

  // scaling of the radius
  Vector s = t.scaling();
  double max = s.x();
  if (s.y() > max) max = s.y();
  if (s.z() > max) max = s.z();
  m_radius *= max;

  // translation of the center
  m_center = (m_center * max) + t.translation();
}

const BoundingSphere& BoundingSphere::operator=(const BoundingSphere& obj)
{
  m_radius = obj.m_radius;
  m_center = obj.m_center;
  return (*this);
}
