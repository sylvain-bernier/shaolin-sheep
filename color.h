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

#ifndef COLOR_H
#define COLOR_H
class   Color;

class QColor;

class Color
//
// Color : RGB color (double -> [0-1], int -> [0-255])
//
{
 public:
  Color();
  Color(double R, double G, double B, double alpha = 1.);
  Color(int    R, int    G, int    B, double alpha = 1.);

  // RGBA array compatible with opengl
  const float* array() const;

  bool operator==(const Color& c) const;
  Color operator*(double scalar) const;
  operator QColor() const;

  static const Color white;
  static const Color black;
  static const Color null;
  static const Color red;
  static const Color green;
  static const Color blue;

 private:
  float m_rgba[4];
};

#endif // COLOR_H
