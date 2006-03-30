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

#include "texture.h"
#include "color.h"
#include <QGLWidget>
#include <QPixmap>
#include <QPainter>

// -------------------------------------------------------------------------
// Texture(size) - create a new texture
//
// notes : texture size is (size * size) pixels, where size is 2^n
// -------------------------------------------------------------------------

Texture::Texture(int i_size)
  :m_bindmap(),
   m_pixmap(i_size, i_size)
{
}

// -------------------------------------------------------------------------
// ~Texture() : must not be destroyed while it is still in use somewhere!
// -------------------------------------------------------------------------

Texture::~Texture()
{
  deleteTexture();
}

// -------------------------------------------------------------------------
// pixmap() : access to the texture pixmap
// -------------------------------------------------------------------------

QPixmap& Texture::pixmap()
{
  return m_pixmap;
}

void Texture::pushAttrib()
{
  glPushAttrib(GL_ENABLE_BIT);
}

// -------------------------------------------------------------------------
// bind(QGLWidget*) opengl texture binding, a QGLWidget* is necessary
// -------------------------------------------------------------------------

void Texture::bind(QGLWidget* i_gl)
{
  glEnable(GL_TEXTURE_2D);
  int id = i_gl->bindTexture(m_pixmap);

  // we assume there is only one valid (i_gl, id) pair per texture
  td_bindmap::iterator it = m_bindmap.find(i_gl);
  if (it != m_bindmap.end()) {
    if ((*it).second == id) return;
    ((*it).first)->deleteTexture((*it).second);
    m_bindmap.erase(it);
  }
  m_bindmap.insert(td_bindmap::value_type(i_gl, id));
}

void Texture::popAttrib()
{
  glPopAttrib();
}

// -------------------------------------------------------------------------
// newCheckeredTexture(size, color) : size = 2^n
//
// notes : a new Texture is allocated and should be eventually deleted
// -------------------------------------------------------------------------

Texture* Texture::newCheckeredTexture(int i_size, const Color& i_color)
{
  Texture* t = new Texture(i_size);
  {
    QPainter p(&(t->pixmap()));
    int a = i_size / 4; int b = a / 2;
    for (int x = 0; x < i_size; x += a)
      for (int y = 0; y < i_size; y += a) {
        p.fillRect(x+0, y+0, x+b, y+b, Qt::gray);
        p.fillRect(x+b, y+0, x+a, y+b, Qt::darkGray);
        p.fillRect(x+0, y+b, x+b, y+a, Qt::lightGray);
        p.fillRect(x+b, y+b, x+a, y+a, (QColor)i_color);
      }
  }
  return t;
}

// -------------------------------------------------------------------------
// deleteTexture() : free up any QGLWidget binding
// -------------------------------------------------------------------------

void Texture::deleteTexture()
{
  for (td_bindmap::iterator i = m_bindmap.begin(); i != m_bindmap.end(); i++)
    ((*i).first)->deleteTexture((*i).second);
  m_bindmap.clear();
}
