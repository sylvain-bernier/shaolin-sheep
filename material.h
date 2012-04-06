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

#ifndef MATERIAL_H
#define MATERIAL_H
class   Material;

#include "color.h"

class Material
{
 public:
  Material(const Color& ambient_and_diffuse);

  static void pushAttrib();
  void apply() const;
  static void popAttrib();

  const Color& ambientReflectance() const;
  const Color& diffuseReflectance() const;
  const Color& specularReflectance() const;
  float shininess() const;
  const Color& emission() const;

  bool operator==(const Material& m) const;

 private:
  Color  m_ambient;    // ambient reflectance
  Color  m_diffuse;    // diffuse reflectance
  Color* mp_specular;  // specular reflectance (null if none)
  float  m_shininess;  // [0, 1]
  Color* mp_emission;  // emission (null if none)
};

#endif // MATERIAL_H
