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

#ifndef QUADRIC_H
#define QUADRIC_H
class   Quadric;

class Texture;
class Material;
class GLUquadric;

#include "globject.h"

class Quadric : public Globject
//
// Quadric : GLU quadric encapsulation
//
{
 public:
  Quadric();
  virtual ~Quadric();

  void setOutsideIn(bool i_outside_in);
  void setWireFrame(bool i_wireframe);
  void setTexture(Texture* i_texture);
  void setMaterial(const Material& i_material);

  // subdivision settings
  bool setSlices(int i_slices);
  int  slices() const;

  // Texture* and Material* are null if none applied
  Texture* texture();
  const Material* material();

 protected:
  virtual void globject_draw(QGLWidget* i_gl);
  virtual void drawQuadric(GLUquadric* i_quadric) = 0;

 private:
  Texture*    mp_texture;  // null if none applied
  Material*   mp_material; // null if none applied
  GLUquadric* mp_quadric;
  int         m_slices;    // slices (number of subdivisions)
};

#endif // QUADRIC_H
