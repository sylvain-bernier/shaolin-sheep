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

#ifndef SHEEP_H
#define SHEEP_H
class   Sheep;

class Texture;
class Quadric;
class Cylinder;
#include "globject.h"

class Sheep : public Globject
//
// Sheep : simple animated sheep model
//         (sheep size is the length of the sheep's body, in meters)
//
{
 public:
  Sheep(double i_size = 1.);
  ~Sheep();

  virtual void setVelocity(const Vector& i_velocity);

 protected:
  // walking / running animation methods

  virtual bool globject_tick(double i_sec);

  void   walkOrRun(double i_distance);
  void   setAnimationPhase(double i_phase);
  void   setWaitingPosition();
  void   setLegPosition(bool i_front, bool i_right, double i_position);
  void   setDisplacementMode(bool i_walking);
  double getStride(bool i_walking);

 private:
  bool      m_walking;     // true : walking, false : running
  bool      m_waiting;     // true : waiting, false : see m_walking
  double    m_phase;       // phase of the animation cycle [0., 1.[
  double    m_orientation; // current orientation, in degrees, 0 --> i

  double    m_size;        // sheep body size (length), in meters
  Globject* mp_sheep;      // sheep model + body parts
  Quadric*  mp_body;
  Globject* mp_head;
  Globject* mp_head_a;
  Globject* mp_eye[2];
  Globject* mp_eye_w[2];
  Globject* mp_eye_p[2];
  Globject* mp_ear[2];
  Globject* mp_mouth;
  Globject* mp_tail;
  Cylinder* mp_leg[2][2];

 public:
  static Texture* sp_wool; // dynamically generated wool texture
  static int s_wool_count; // how many sheeps share this texture?
};

#endif // SHEEP_H
