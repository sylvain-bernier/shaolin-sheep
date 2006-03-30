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

#include "material.h"
#include <QtOpenGL>

Material::Material(const Color& ambient_and_diffuse)
  : m_ambient(ambient_and_diffuse),
    m_diffuse(ambient_and_diffuse),
    mp_specular(0),
    m_shininess(0.),
    mp_emission(0)
{
}

void Material::pushAttrib()
{
  glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
}

void Material::apply() const
{
  // ambient and diffuse reflectance
  glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient.array());
  glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse.array());
  glColor4fv(m_diffuse.array());

  // specular reflectance
  if (mp_specular) {
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mp_specular->array());
    glMaterialf (GL_FRONT, GL_SHININESS, m_shininess * 128.);
  }

  // emission
  if (mp_emission)
    glMaterialfv(GL_FRONT, GL_EMISSION,  mp_emission->array());
}

void Material::popAttrib()
{
  glPopAttrib();
}

const Color& Material::ambientReflectance() const
{
  return m_ambient;
}

const Color& Material::diffuseReflectance() const
{
  return m_diffuse;
}

const Color& Material::specularReflectance() const
{
  return mp_specular ? *mp_specular : Color::null;
}

float Material::shininess() const
{
  return m_shininess;
}

const Color& Material::emission() const
{
  return mp_emission ? *mp_emission : Color::null;
}

bool Material::operator==(const Material& m) const
{
  if (!(m_ambient == m.m_ambient) || !(m_diffuse == m.m_diffuse))
    return false;

  if (!mp_specular != !m.mp_specular) return false;

  if (mp_specular) {
    if (!(*mp_specular == *m.mp_specular) ||
         (m_shininess  !=  m.m_shininess))
      return false;
  }
  if (!mp_emission != !m.mp_emission)                     return false;
  if ((mp_emission) && !(*mp_emission == *m.mp_emission)) return false;

  return true;
}
