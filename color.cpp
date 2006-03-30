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

#include "color.h"
#include <QColor>

Color::Color()
{
  m_rgba[0] = 0.;
  m_rgba[1] = 0.;
  m_rgba[2] = 0.;
  m_rgba[3] = 0.;
}

Color::Color(double R, double G, double B, double alpha)
{
  m_rgba[0] = R;
  m_rgba[1] = G;
  m_rgba[2] = B;
  m_rgba[3] = alpha;
}

Color::Color(int R, int G, int B, double alpha)
{
  m_rgba[0] = ((float)R / 255.f);
  m_rgba[1] = ((float)G / 255.f);
  m_rgba[2] = ((float)B / 255.f);
  m_rgba[3] = alpha;
}

// -------------------------------------------------------------------------
// array() : RGBA array compatible with opengl
// -------------------------------------------------------------------------

const float* Color::array() const
{
  return m_rgba;
}

bool Color::operator==(const Color& c) const
{
  bool ret = true;
  for (int i=0; i<4; i++) ret = ret && (m_rgba[i] == c.m_rgba[i]);
  return (ret);
}

Color Color::operator*(double s) const
{
  return Color((m_rgba[0] * s), (m_rgba[1] * s), (m_rgba[2] * s), m_rgba[3]);
}

Color::operator QColor() const
{
  return QColor((int)(m_rgba[0] * 255.),
                (int)(m_rgba[1] * 255.),
                (int)(m_rgba[2] * 255.),
                (int)(m_rgba[3] * 255.));
}

const Color Color::white(1., 1., 1.);
const Color Color::black(0., 0., 0.);
const Color Color::null (0., 0., 0., 0.);
const Color Color::red  (1., 0., 0.);
const Color Color::green(0., 1., 0.);
const Color Color::blue (0., 0., 1.);
