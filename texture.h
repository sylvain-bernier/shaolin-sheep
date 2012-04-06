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

#ifndef TEXTURE_H
#define TEXTURE_H
class   Texture;

class Color;
class QGLWidget;
#include <QPixmap>
#include <map>

class Texture
//
// Texture : opengl texture handling using qt tools
//
// notes : - must not be destroyed while it is still in use somewhere
//         - texture size is (size * size) pixels, where size is 2^n
//
{
 public:
  Texture(int i_size);
  ~Texture();

  // access to the texture pixmap
  QPixmap& pixmap();

  // opengl texture binding
  static void pushAttrib();
  void bind(QGLWidget* i_gl);
  static void popAttrib();

  // various texture generation functions
  static Texture* newCheckeredTexture(int i_size, const Color& i_color);

 protected:
  void deleteTexture();

 private:
  typedef std::map<QGLWidget*,int> td_bindmap;

  td_bindmap m_bindmap; // used to remember all its past bindings
  QPixmap    m_pixmap;  // QPixmap holding the texture image
};

#endif // TEXTURE_H
