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

#include "gldemowidget.h"
#include "boundingsphere.h"
#include "vector.h"
#include <QtOpenGL>
#include <QCursor>
#include <cmath>

#define CAMERA_RANGE          100. // distance from the target to the horizon
#define JUMP_EPSILON          0.01 // maximum y-velocity to be able to jump
#define ZERO_100KMH_DELAY     15.  // seconds to go from 0 to 100 kmh
#define FRICTION_COMPENSATION 0.5  // extra acceleration to fight friction

#define DEGREES_PER_PIXEL   (360./800.)  // camera / mouse control precision
#define TARGET_ACCELERATION (100000. / (3600. * ZERO_100KMH_DELAY)) // m/s^2

GLDemoWidget::GLDemoWidget(QWidget* parent)
  :QGLWidget(parent),
   m_scene (),
   m_camera(0., CAMERA_RANGE),
   m_mouse_grab(false),
   m_mouse_pos(),
   m_big_ball(false)
{
  // initial camera position
  m_camera.setRotate(-37.5);
}

// -------------------------------------------------------------------------
// tick(ms, up, left, down, right) : called each time a new frame is needed
//
// ms                    = 1/1000 seconds elapsed since last tick
// up, left, down, right = current button states ('true' -> pressed)
// -------------------------------------------------------------------------

void GLDemoWidget::tick(int i_ms,
                        bool i_up, bool i_left, bool i_down, bool i_right)
{
  // first of all, we make sure we won't care for useless stuff
  if (i_up   && i_down ) { i_up   = false; i_down  = false; }
  if (i_left && i_right) { i_left = false; i_right = false; }

  const Globject* pt = m_scene.target((m_big_ball ? 1 : 0));
  if (pt) {
    Vector vel = pt->velocity();
    if (fabs(vel.y()) < JUMP_EPSILON) {
      if (i_up || i_left || i_down || i_right) {

        // lets have a unit vector in the direction of the target
        Vector uni_camera = m_camera.direction();
        uni_camera.setY(0.);
        uni_camera.l2normalize();

        // another one pointing hard right
        Vector uni_right(-uni_camera.z(), 0., uni_camera.x());

        // now, lets decompose the target velocity
        double dec_forward = uni_camera.dotProduct(vel);
        double dec_right   =  uni_right.dotProduct(vel);
        double dec_y       = vel.y();

        // check for braking
        if ((!i_up && !i_down) ||
            (i_up  && dec_forward < 0.) || (i_down && dec_forward > 0.))
          dec_forward = 0.;

        if ((!i_right && !i_left) ||
            (i_right  && dec_right < 0.) || (i_left && dec_right > 0.))
          dec_right = 0.;

        // calculate velocity gain using TARGET_ACCELERATION
        double vel_gain = TARGET_ACCELERATION  * ((double)i_ms / 1000.);
        vel_gain *= 1. + FRICTION_COMPENSATION * ((double)i_ms / 1000.);
        vel_gain *= (((i_up || i_down) && (i_left || i_right)) ? 0.5 : 1.0);

        // accelerate (your breath)
        if (i_up   ) dec_forward += vel_gain;
        if (i_down ) dec_forward -= vel_gain;
        if (i_right) dec_right   += vel_gain;
        if (i_left ) dec_right   -= vel_gain;

        // then, put back together the new target velocity
        vel = (uni_camera * dec_forward) + (uni_right * dec_right);
        vel.setY(dec_y);
      }
      else {
        // we assume that God doesn't want the target to move
        vel.setX(0.); vel.setZ(0.);
      }
      ((Globject*)pt)->setVelocity(vel);
    }
  }

  // animate the scene and refresh display if necessary
  if (m_scene.tick(i_ms)) updateGL();
}

// -------------------------------------------------------------------------
// jump() : make the current target jump!
//
// return value : false -> not possible (already jumping, etc.)
// -------------------------------------------------------------------------

bool GLDemoWidget::jump()
{
  const Globject* pt = m_scene.target((m_big_ball ? 1 : 0));
  if (pt) {
    Vector vel = pt->velocity();
    // we can only jump if the target isn't already jumping or falling
    if (fabs(vel.y()) < JUMP_EPSILON) {
      vel.setY(0.);
      // the target will jump upward as fast as it is moving now
      double jump_vel = vel.l2norm();

      // if the target isn't moving fast enough, it will still jump a bit
      double jump_min_vel = pt->boundingSphere().radius() * 10.;
      if (jump_vel < jump_min_vel) jump_vel = jump_min_vel;

      // JUMP!
      vel.setY(jump_vel);
      ((Globject*)pt)->setVelocity(vel);
      return true;
    }
  }
  return false;
}

// -------------------------------------------------------------------------
// initializeGL() : initialize the opengl state machine
// -------------------------------------------------------------------------

void GLDemoWidget::initializeGL()
{
  // blue sky as background
  glClearColor(0.0, 0.0, 0.8, 1.0);

  // enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // enable back face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // enable auto-normalization of scaled normal vectors
  glEnable(GL_NORMALIZE);

  // enable default lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

// -------------------------------------------------------------------------
// resizeGL() : update viewport and projection parameters to fit widget size
// -------------------------------------------------------------------------

void GLDemoWidget::resizeGL(int width, int height)
{
  // new viewport size
  glViewport(0, 0, width, height);

  // new perspective projection aspect ratio
  m_camera.setAspectRatio((double)width / (double)height);
}

// -------------------------------------------------------------------------
// paintGL() : opengl painting method, all the drawing is made in there
// -------------------------------------------------------------------------

void GLDemoWidget::paintGL()
{
  // camera ------------------------------
  {
    // set the new target position
    const Globject* pt = m_scene.target((m_big_ball ? 1 : 0));
    if (pt) {
      // camera will point just a little bit over the target
      Vector target = pt->position();
      double radius = pt->boundingSphere().radius();
      target.setY(target.y() + radius);

      // if the camera is pointing upward, the target will rise a bit
      double tilt_adj = 0.;
      if (m_camera.tilt() < 0.) {
        tilt_adj = fabs(m_camera.tilt()) / 90.;
        target.setY(target.y() + radius * 1.5 * tilt_adj);
      }

      // the camera will get a little bit closer while tilting
      m_camera.setDistance(radius + (radius * 1.5 * (1. - tilt_adj)));
      m_camera.setTarget(target);
    }

    // place the camera
    m_camera.place();
  }

  // scene -------------------------------
  {
    // make everything ready to draw the scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // draw the scene
    m_scene.draw(this); glFlush();
  }
}

// -------------------------------------------------------------------------
// mousePressEvent(e) : mouse button is pressed
//
// notes : clicking the widget enable / disable camera control
// -------------------------------------------------------------------------

void GLDemoWidget::mousePressEvent(QMouseEvent* e)
{
  // clicking the widget enable / disable camera control
  if (e->button() == Qt::LeftButton) {
    m_mouse_grab = !m_mouse_grab;
    setMouseTracking(m_mouse_grab);
    if (m_mouse_grab) {
      // the cursor is confusing when camera control is enabled
      grabMouse(Qt::BlankCursor);
      m_mouse_pos = QCursor::pos();
    }
    else releaseMouse();
  }

  // big ball control - sheep, beware!
  if (e->button() == Qt::RightButton) {
    m_big_ball = !m_big_ball;
    m_scene.setEvilBigBall(!m_big_ball);
  }
}

// -------------------------------------------------------------------------
// mouseMoveEvent(e) : the mouse is moving
//
// notes : moving the mouse while camera control is enabled will move the
//         camera
// -------------------------------------------------------------------------

void GLDemoWidget::mouseMoveEvent(QMouseEvent* e)
{
  if (m_mouse_grab && (QCursor::pos() != m_mouse_pos)) {
    QPoint delta = (QCursor::pos() - m_mouse_pos);
    QCursor::setPos(m_mouse_pos);
    {
      // rotating the camera around the target
      double degrees = (DEGREES_PER_PIXEL * (double)-delta.x());
      degrees += m_camera.rotate();
      m_camera.setRotate(degrees);
    }
    {
      // tilting the camera forward or backward
      double degrees = ((DEGREES_PER_PIXEL / 3.) * (double)delta.y());
      degrees += m_camera.tilt();
      if (degrees   >  90.) degrees =  90.; else
        if (degrees < -35.) degrees = -35.;
      m_camera.setTilt(degrees);
    }
  }
}
