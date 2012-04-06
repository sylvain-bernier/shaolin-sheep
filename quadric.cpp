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

#include "quadric.h"
#include "texture.h"
#include "material.h"
#include <QtOpenGL>
#include <QGLWidget>

#define DEFAULT_SLICES 10

Quadric::Quadric()
  :mp_texture(0),
   mp_material(0),
   mp_quadric(gluNewQuadric()),
   m_slices(DEFAULT_SLICES)
{
  setOutsideIn(false);
}

Quadric::~Quadric()
{
  if (mp_quadric) {
    gluDeleteQuadric(mp_quadric);
    mp_quadric = 0;
  }

  delete mp_material; mp_material = 0;
}

void Quadric::setOutsideIn(bool i_outside_in)
{
  if (mp_quadric)
    gluQuadricOrientation(mp_quadric, (i_outside_in ? GLU_INSIDE : GLU_OUTSIDE));
}

void Quadric::setWireFrame(bool i_wireframe)
{
  if (mp_quadric)
    gluQuadricDrawStyle(mp_quadric, (i_wireframe ? GLU_LINE : GLU_FILL));
}

void Quadric::setTexture(Texture* i_texture)
{
  if (mp_texture != i_texture) {
    mp_texture = i_texture;
    gluQuadricTexture(mp_quadric, (mp_texture ? GL_TRUE : GL_FALSE));
  }
}

void Quadric::setMaterial(const Material& i_material)
{
  if ((mp_material == 0) || !(*mp_material == i_material)) {
    delete mp_material;
    mp_material = new Material(i_material);
  }
}

bool Quadric::setSlices(int i_slices)
{
  if (i_slices > 0) {
    m_slices = i_slices;
    return true;
  }
  return false;
}

int Quadric::slices() const
{
  return m_slices;
}

Texture* Quadric::texture()
{
  return mp_texture;
}

const Material* Quadric::material()
{
  return mp_material;
}

void Quadric::globject_draw(QGLWidget* i_gl)
{
  if (mp_texture) {
    // apply texture
    Texture::pushAttrib();
    mp_texture->bind(i_gl);
  }

  if (mp_material) {
    // apply material
    Material::pushAttrib();
    mp_material->apply();
  }

  if (mp_quadric)
    // draw quadric
    drawQuadric(mp_quadric);

  // restore previous state
  if (mp_material) Material::popAttrib();
  if (mp_texture ) Texture::popAttrib();
}
