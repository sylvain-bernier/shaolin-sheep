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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H
class   MainWidget;

class GLDemoWidget;
class QTimer;
class QGroupBox;
class QPushButton;
#include <QWidget>
#include <QTime>

class MainWidget : public QWidget
//
// MainWidget : main application window
//
{
  Q_OBJECT

 public:
  MainWidget(QWidget* parent = 0);
  virtual ~MainWidget();

 protected slots:
  void tick();      // tick at each frame
  void fps_tick();  // tick at each second to update current fps rate

 protected:
  virtual void keyPressEvent(QKeyEvent* e);
  virtual void keyReleaseEvent(QKeyEvent* e);

  bool setButtonState(int i_key, bool i_down);

 private:
  GLDemoWidget* mp_glwidget;       // main opengl widget
  QGroupBox*    mp_box;            // group box that holds mp_glwidget
  QTimer*       mp_timer;          // main application timer
  QTime         m_tick_timer;      // keeps time elapsed since last tick
  QTimer*       mp_fps_timer;      // 'frame per second' timer
  int           m_current_fps;     // current 'frame per second' rate
  int           m_fps_counter;     // frames displayed since last check

  QPushButton*  mp_buttons   [4];  // top, left, down, right buttons
  int           m_button_keys[4];  // top, left, down, right keys
};

#endif // MAINWIDGET_H
