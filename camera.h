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

#ifndef CAMERA_H
#define CAMERA_H
class   Camera;

class Vector;

class Camera
//
// Camera : simple camera transformations
//
//          distance   = distance from target (meters)
//          range      = distance from the target to the horizon (meters)
//          view field = angle, y-axis direction (degrees)
//          target     = point the camera is looking at (x, y, z)
//          tilt       = angle (0-90) between line of view and floor
//          rotate     = rotation around y-axis  (0 -> looking along -z-axis)
//          roll       = MTV-like camera rolling (0 -> normal position)
//
{
 public:
  Camera(double distance, double range, double field_view_y = 90.,
         double target_x = 0., double target_y = 0., double target_z = 0.,
         double tilt_x   = 0., double rotate_y = 0., double roll_z   = 0.);

  // place the camera : apply opengl view transformations
  void place() const;

  double distance() const;
  double tilt() const;
  double rotate() const;
  double roll() const;
  Vector direction() const;
  Vector target() const;

  void setDistance(double v);
  void setTarget(double x, double y, double z);
  void setTarget(const Vector& v);
  void setTilt(double v);
  void setRoll(double v);
  void setRotate(double v);

  // set the rotate parameter to look in the direction of 'v'
  bool setRotate(const Vector& v);

  // set the aspect ratio given by (width / height)
  void setAspectRatio(double v);

 private:
  double m_distance, m_range,    m_field_view_y;
  double m_target_x, m_target_y, m_target_z;
  double m_tilt_x,   m_rotate_y, m_roll_z;
  double m_aspect_ratio;
};

#endif // CAMERA_H
