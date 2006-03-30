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

#ifndef GLDEMOWIDGET_H
#define GLDEMOWIDGET_H
class   GLDemoWidget;

#include "camera.h"
#include "scene.h"
#include <QGLWidget>
#include <QPoint>

class GLDemoWidget : public QGLWidget
//
// GLDemoWidget : main opengl widget
//
{
 public:
  GLDemoWidget(QWidget* parent = 0);

  // called at each frame (are up, left, down and right buttons pressed?)
  void tick(int i_ms, bool i_up, bool i_left, bool i_down, bool i_right);

  // make the current target jump! (false -> not possible)
  bool jump();

 protected:
  // standard QGLWidget opengl methods
  virtual void initializeGL();
  virtual void resizeGL(int width, int height);
  virtual void paintGL();

  // mouse event handling for camera control
  virtual void mousePressEvent(QMouseEvent* e);
  virtual void mouseMoveEvent(QMouseEvent* e);

 private:
  Scene  m_scene;      // the scene
  Camera m_camera;     // main camera
  bool   m_mouse_grab; // is the mouse grabbed for camera control?
  QPoint m_mouse_pos;  // last mouse position
  bool   m_big_ball;   // true -> target is the big ball
};

#endif // GLDEMOWIDGET_H
