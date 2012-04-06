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

#include "camera.h"
#include "vector.h"
#include <QtOpenGL>
#include <cmath>

#define CAM_NEAR 0.01

Camera::Camera(double distance, double range,    double field_view_y,
               double target_x, double target_y, double target_z,
               double tilt_x,   double rotate_y, double roll_z)
  : m_distance(distance), m_range(range),       m_field_view_y(field_view_y),
    m_target_x(target_x), m_target_y(target_y), m_target_z(target_z),
    m_tilt_x(tilt_x),     m_rotate_y(rotate_y), m_roll_z(roll_z),
    m_aspect_ratio(1.)
{
}

// -------------------------------------------------------------------------
// place() : place the camera, apply opengl view transformations
// -------------------------------------------------------------------------

void Camera::place() const
{
  glPushAttrib(GL_TRANSFORM_BIT);
  {
    // perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_field_view_y,
                   m_aspect_ratio,
                   CAM_NEAR,
                   CAM_NEAR + m_range);

    // set the distance between the camera and the target
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0., 0., -(m_distance + CAM_NEAR));

    // set the rotate, tilt and roll parameters
    glRotated(-m_roll_z,   0., 0., 1.);
    glRotated( m_tilt_x,   1., 0., 0.);
    glRotated(-m_rotate_y, 0., 1., 0.);

    // translate the target at the origin
    glTranslated(-m_target_x, -m_target_y, -m_target_z);
  }
  glPopAttrib();
}

double Camera::distance() const
{
  return m_distance;
}

double Camera::tilt() const
{
  return m_tilt_x;
}

double Camera::rotate() const
{
  return m_rotate_y;
}

double Camera::roll() const
{
  return m_roll_z;
}

Vector Camera::direction() const
{
  double rAd1 = m_rotate_y * M_PI / 180.;
  double rAd2 = m_tilt_x   * M_PI / 180.;
  Vector res(-sin(rAd1), -sin(rAd2), -cos(rAd1));
  res.l2normalize();
  return res;
}

Vector Camera::target() const
{
  return Vector(m_target_x, m_target_y, m_target_z);
}

void Camera::setDistance(double v)
{
  m_distance = v;
}

void Camera::setTarget(double x, double y, double z)
{
  m_target_x = x;
  m_target_y = y;
  m_target_z = z;
}

void Camera::setTarget(const Vector& v)
{
  setTarget(v.x(), v.y(), v.z());
}

void Camera::setTilt(double v)
{
  m_tilt_x = v;
}

void Camera::setRoll(double v)
{
  m_roll_z = v;
}

void Camera::setRotate(double v)
{
  m_rotate_y = v;
}

// -------------------------------------------------------------------------
// setRotate(v) : set the rotate parameter to look in the direction of 'v'
// -------------------------------------------------------------------------

bool Camera::setRotate(const Vector& v)
{
  Vector vec(v.x(), 0., v.z());
  if (vec.l2normalize()) {
    // remember : 0 -> looking along negative z-axis
    double angle = (180. * acos(-vec.z())) / M_PI;
    if (vec.x() > 0) angle = -angle;

    setRotate(angle);
    return true;
  }
  return false;
}

// -------------------------------------------------------------------------
// setAspectRatio(double) : set the aspect ratio given by (width / height)
// -------------------------------------------------------------------------

void Camera::setAspectRatio(double v)
{
  m_aspect_ratio = v;
}
