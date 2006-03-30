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

#include "mainwidget.h"
#include "gldemowidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>

#define DEMO_FPS       60
#define TICK_INTERVAL (1000 / DEMO_FPS)

MainWidget::MainWidget(QWidget* parent)
  :QWidget(parent),
   mp_glwidget(0),
   mp_box(0),
   mp_timer(0),
   m_tick_timer(),
   mp_fps_timer(0),
   m_current_fps(0),
   m_fps_counter(0)
{
  // build main window components and layout
  {
    // buttons setup
    {
      mp_buttons[0]    = new QPushButton(tr("Forward (W)" ));
      mp_buttons[1]    = new QPushButton(tr("Left (A)"    ));
      mp_buttons[2]    = new QPushButton(tr("Backward (S)"));
      mp_buttons[3]    = new QPushButton(tr("Right (D)"   ));
    
      m_button_keys[0] = Qt::Key_W;
      m_button_keys[1] = Qt::Key_A;
      m_button_keys[2] = Qt::Key_S;
      m_button_keys[3] = Qt::Key_D;

      for (int i=0; i<4; i++)
        mp_buttons[i]->setFocusPolicy(Qt::NoFocus);
    }

    // create main components
    mp_glwidget          = new GLDemoWidget;
    mp_box               = new QGroupBox;
    QBoxLayout*  box_l   = new QHBoxLayout(mp_box);
    QGridLayout* layout  = new QGridLayout;

    QLabel* label = new QLabel
      (QString(tr("Shaolin Sheeps (c) 2006  Sylvain Bernier\n"))+
       QString(tr("< sylvain_bernier03@yahoo.ca >\n"))+
       QString(tr("Version %1\n")).arg(SS_VERSION)+
       QString(tr("This is free software, see the file COPYING.\n")));
    {
      label->setAlignment(Qt::AlignTop);
      label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
      label->setLineWidth(2);
      label->setMargin(2);
    }

    QLabel* label2 = new QLabel
      (tr(" Click on the scene to (des)activate camera control. Press [SpaceBar] to jump. Protect the sheeps! (or try the right click) "));
    {
      label2->setAlignment(Qt::AlignTop | Qt::AlignCenter);
      label2->setFrameStyle(QFrame::Box | QFrame::Sunken);
      label2->setLineWidth(1);
      label->setMargin(1);
    }
    
    // create main layout
    layout->addWidget(mp_box,        0, 0, 1, 4);
    layout->addWidget(mp_buttons[0], 1, 1      );
    layout->addWidget(mp_buttons[1], 2, 0      );
    layout->addWidget(mp_buttons[2], 2, 1      );
    layout->addWidget(mp_buttons[3], 2, 2      );
    layout->addWidget(label,         1, 3, 2, 1);
    layout->addWidget(label2,        3, 0, 1, 4);
    layout->setRowStretch(0, 1);
    layout->setRowMinimumHeight(0, 200);
    setLayout(layout);

    mp_box->setTitle(tr("Welcome! "));
    box_l->addWidget(mp_glwidget);
    resize(625, 550);
  }

  // initialize main application timer
  mp_timer = new QTimer;
  connect(mp_timer, SIGNAL(timeout()), this, SLOT(tick()));
  mp_timer->start(TICK_INTERVAL);
  m_tick_timer.start();

  // initialize 'frame per second' timer
  mp_fps_timer = new QTimer;
  connect(mp_fps_timer, SIGNAL(timeout()), this, SLOT(fps_tick()));
  mp_fps_timer->start(1000);
}

MainWidget::~MainWidget()
{
  for (int i=0; i<4; i++) mp_buttons[i] = 0;
  mp_glwidget = 0;        mp_box        = 0;
  delete mp_timer;        mp_timer      = 0;
  delete mp_fps_timer;    mp_fps_timer  = 0;
}

void MainWidget::tick()
{
  if (mp_glwidget)
    // each tick asks for a new frame
    mp_glwidget->tick(m_tick_timer.restart(),
                      mp_buttons[0] && mp_buttons[0]->isDown(),
                      mp_buttons[1] && mp_buttons[1]->isDown(),
                      mp_buttons[2] && mp_buttons[2]->isDown(),
                      mp_buttons[3] && mp_buttons[3]->isDown());

  // a new frame has been drawn
  m_fps_counter++;
}

void MainWidget::fps_tick()
{
  // keep the number of frames displayed during last second
  m_current_fps = m_fps_counter;
  m_fps_counter = 0;

  // display the new fps rate over the opengl widget
  if (mp_box)
    mp_box->setTitle(QString(tr("%1 fps ")).arg(m_current_fps));
}

void MainWidget::keyPressEvent(QKeyEvent* e)
{
  bool ignore = true;
  if (e->isAutoRepeat() == false) {
    if (setButtonState(e->key(), true)) ignore = false;
    if (mp_glwidget && (e->key() == Qt::Key_Space)) {
      // JUMP!
      mp_glwidget->jump();
      ignore = false;
    }
  }
  if (ignore) e->ignore();
}

void MainWidget::keyReleaseEvent(QKeyEvent* e)
{
  if (e->isAutoRepeat() ||
      setButtonState(e->key(), false) == false)
    e->ignore();
}

bool MainWidget::setButtonState(int i_key, bool i_down)
{
  for (int i=0; i<4; i++)
    if (i_key == m_button_keys[i]) {
      mp_buttons[i]->setDown(i_down);
      return true;
    }
  return false;
}
