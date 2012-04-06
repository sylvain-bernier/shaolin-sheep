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

#include "sheep.h"
#include "texture.h"
#include "sphere.h"
#include "cylinder.h"
#include "material.h"
#include "color.h"
#include "transform.h"
#include "vector.h"
#include <QPainter>
#include <QPainterPath>
#include <cstdlib>
#include <cmath>

// Wool texture generation
#define WOOL_TEX_SIZE           256    // texture size      = x * x
#define WOOL_CUBIC_SIZE         10     // bezier curve size = x * x
#define WOOL_ITERATIONS         2000   // number of bezier curves

// Leg movement
#define WALK_LEG_MAX            25.    // (walking) maximum foward   rotation
#define WALK_LEG_MIN           -40.    // (walking) maximum backward rotation
#define RUN_LEG_MAX             80.    // (running) maximum forward  rotation
#define RUN_LEG_MIN            -60.    // (running) maximum backward rotation

// Sheep walking / running animation
#define MIN_VELOCITY            0.01   // below that, no animation
#define MAX_DEGREES_PER_SECOND   90.   // max. orientation change / second
#define WALK_OFFSET             0.5    // offset between front legs
#define RUN_OFFSET              0.15   // offset between front and back legs
#define MAX_STEPS_PER_MINUTE    142.   // max. steps per minute
                                       // (you can use a metronome)

#define MAX_STEPS_PER_SECOND    MAX_STEPS_PER_MINUTE / 60.

Texture* Sheep::sp_wool = 0;    // dynamically generated wool texture
int Sheep::s_wool_count = 0;    // how many sheep share this texture?

// -------------------------------------------------------------------------
// Sheep(size) : create a new animated sheep model
//               (body length 'size' meters)
//
// notes : wool texture is dynamically generated using Qt painter tools
// -------------------------------------------------------------------------

Sheep::Sheep(double i_size)
  :Globject(),
   m_walking(true),
   m_waiting(false),
   m_phase(NAN),
   m_orientation(0.),
   m_size(i_size),
   mp_sheep (0), mp_body (0), mp_head(0),
   mp_head_a(0), mp_mouth(0), mp_tail(0)
{
  // object initialisation
  for (int i=0; i<2; i++) {
    mp_eye[i]    = 0; mp_eye_w[i]  = 0;
    mp_eye_p[i]  = 0; mp_ear[i]    = 0;
    mp_leg[i][0] = 0; mp_leg[i][1] = 0;
  }

  // wool texture generation (using bezier curves)
  if (s_wool_count++ == 0) {
    sp_wool = new Texture(WOOL_TEX_SIZE);

    QPainter p(&(sp_wool->pixmap()));
    p.fillRect(0, 0, WOOL_TEX_SIZE, WOOL_TEX_SIZE, Qt::white);
    p.setPen(Qt::gray);

    for (int i = 0; i < WOOL_ITERATIONS; i++) {
      QPainterPath pp; int x; int y;
      int s = rand() % WOOL_CUBIC_SIZE + 1;
      pp.moveTo(x = rand() % (WOOL_TEX_SIZE - WOOL_CUBIC_SIZE),
                y = rand() % (WOOL_TEX_SIZE - WOOL_CUBIC_SIZE));
      pp.cubicTo(x,y+s, x+s,y+s, x+s,y);
      p.drawPath(pp);
      p.rotate(90);
    }
  }

  // sheep model crude definition
  Globject* sheep = new Globject;
  {
    // colors used in the model
    Material white   (Color(1.0, 1.0, 1.0));
    Material gray    (Color(0.7, 0.7, 0.7));
    Material brown   (Color(207, 173, 117));
    Material dk_brown(Color(207, 173, 117) * 0.9);
    Material black   (Color(0.0, 0.0, 0.0));

    // sheep's body
    Sphere* body = new Sphere(0.5);
    body->setSlices(12);
    body->transform().setScaling(Vector(1.0, 0.70, 0.70));
    body->setMaterial(white);
    body->setTexture(sp_wool);
    sheep->addChild(body);
    mp_body = body;

    // sheep's head
    Globject* head = new Globject;
    {
      Sphere* head_a = new Sphere(0.5);
      head_a->transform().setScaling(Vector(0.60, 0.40, 0.45));
      head_a->setMaterial(brown);
      head->addChild(head_a);
      mp_head_a = head_a;

      // sheep's eyes
      for (int i = 0, rl = 1; rl >= -1; rl -= (++i)*2) {
        Globject* eye = new Globject;
        {
          // eye's white
          Sphere* eye_w = new Sphere(0.5);
          eye_w->setSlices(5);
          eye_w->transform().setScaling(Vector(1., 0.4, 0.5));
          eye_w->setMaterial(white);
          eye->addChild(eye_w);
          mp_eye_w[i] = eye_w;

          // eye's pupil
          Sphere* eye_p = new Sphere(0.5);
          eye_p->setSlices(4);
          eye_p->transform().setScaling(Vector(0.3, 0.3, 0.3));
          eye_p->transform().setTranslation(Vector(0.25, 0.15, 0.));
          eye_p->setMaterial(black);
          eye->addChild(eye_p);
          mp_eye_p[i] = eye_p;
        }
        eye->transform().setScaling(Vector(0.15, 0.15, 0.15));
        eye->transform().setRotation(-2., Vector::k);
        eye->transform().setTranslation(Vector(-0.06, 0.19, rl*0.08));
        head->addChild(eye);
        mp_eye[i] = eye;
      }

      // sheep's ears
      for (int i = 0, rl = 1; rl >= -1; rl -= (++i)*2) {
        Sphere* ear = new Sphere(0.5);
        ear->setSlices(6);
        ear->transform().setScaling(Vector(0.175, 0.25, 0.05));
        ear->transform().setRotation(45., Vector::k);
        ear->transform().addRotation(rl*-30., Vector::i);
        ear->setMaterial(dk_brown);
        ear->transform().setTranslation(Vector(-0.10, 0.04, rl*0.2));
        head->addChild(ear);
        mp_ear[i] = ear;
      }

      // sheep's mouth
      Cylinder* mouth = new Cylinder(0.201, 0.201, 0.02);
      mouth->setSlices(12);
      mouth->transform().setRotation(90., Vector::i);
      mouth->transform().setTranslation(Vector(0.09, 0.01, 0.));
      mouth->setMaterial(black);
      head->addChild(mouth);
      mp_mouth = mouth;
    }
    head->transform().setRotation(-45., Vector::k);
    head->transform().setTranslation(Vector(0.5, 0.125, 0.));
    sheep->addChild(head);
    mp_head = head;

    // sheep's tail
    Sphere* tail = new Sphere(0.5);
    tail->setSlices(6);
    tail->transform().setScaling(Vector(0.15, 0.125, 0.125));
    tail->setMaterial(white);
    tail->transform().setTranslation(Vector(-0.5, 0.125, 0.));
    sheep->addChild(tail);
    mp_tail = tail;

    // sheep's legs
    for (int i = 0, fb = 1; fb >= -1; fb -= (++i)*2)
      for (int j = 0, rl = 1; rl >= -1; rl -= (++j)*2) {
        Cylinder* leg = new Cylinder(0.06, 0.06, 0.3);
        leg->setSlices(8);
        leg->transform().setRotation(90., Vector::i);
        leg->transform().setTranslation(Vector(fb*0.28, -0.18, rl*0.12));
        leg->setMaterial(gray);
        sheep->addChild(leg);
        mp_leg[i][j] = leg;
      }
  }
  sheep->transform().setScaling(Vector(m_size, m_size, m_size));
  mp_sheep = sheep;
  this->addChild(mp_sheep);

  // set the model to its starting configuration
  setAnimationPhase(0.);
  setWaitingPosition();
}

Sheep::~Sheep()
{
  // wool texture
  if (mp_body) mp_body->setTexture(0);
  if (s_wool_count == 1) {
    delete sp_wool; sp_wool = 0;
    s_wool_count--;
  }

  // sheep model : there is no need to free model parts since they are
  //               all children of this object.
  mp_sheep  = 0;  mp_body   = 0;  mp_head   = 0;
  mp_head_a = 0;  mp_mouth  = 0;  mp_tail   = 0;

  for (int i=0; i<2; i++) {
    mp_eye[i]    = 0;  mp_eye_w[i]  = 0;
    mp_eye_p[i]  = 0;  mp_ear[i]    = 0;
    mp_leg[i][0] = 0;  mp_leg[i][1] = 0;
  }
}

// -------------------------------------------------------------------------
// setVelocity(velocity) : set the current velocity
//
// notes : the method will check if the sheep should be running or walking
// -------------------------------------------------------------------------

void Sheep::setVelocity(const Vector& i_velocity)
{
  Globject::setVelocity(i_velocity);

  // check if the sheep should now be walking or running
  double vel = Vector(i_velocity.x(), 0., i_velocity.z()).l2norm();
  setDisplacementMode((vel / getStride(true)) <= MAX_STEPS_PER_SECOND);
}

// -------------------------------------------------------------------------
// globject_tick(seconds) : animation tick
//
// notes        : will set the sheep orientation and call walkOrRun
// return value : 'true' when the sheep has moved
// -------------------------------------------------------------------------

bool Sheep::globject_tick(double i_sec)
{
  bool res = Globject::globject_tick(i_sec);

  // only velocities on x and z-axis are considered
  Vector vec = velocity(); vec.setY(0.);
  double vel = vec.l2norm();

  if (vec.l2normalize() && (vel >= MIN_VELOCITY)) {
    // if the sheep is running, there is a limit to how fast the legs
    // will move. legs moving too fast would seem unrealistic.
    if (!m_walking) {
      double max_vel = MAX_STEPS_PER_SECOND * getStride(false);
      if (vel > max_vel) vel = max_vel;
    }

    // calculate what should be the sheep orientation
    // (degrees to rotate the sheep around the y-axis, 0 --> i)
    double new_orientation = (180. * acos(vec.x())) / M_PI;
    if (vec.z() > 0) new_orientation = -new_orientation;

    // calculate the rotation necessary to match wanted orientation
    double rotation = new_orientation - m_orientation;
    if (rotation   >   180.) rotation -= 360.; else
      if (rotation <= -180.) rotation += 360.;

    // see if it is possible to make all that rotation on time
    {
      double max_rotation = (i_sec * MAX_DEGREES_PER_SECOND);
      double difference = fabs(rotation) - max_rotation;
      if (difference > 0.) {
        rotation = (rotation > 0.) ? max_rotation : -max_rotation;

        // if the sheep can't turn fast enough, it will walk backwards
        if (difference > 90.) vel = -vel;
      }
    }

    // update the new orientation, that will stay within ]-180,180]
    m_orientation += rotation;
    if (m_orientation   >   180.) m_orientation -= 360.; else
      if (m_orientation <= -180.) m_orientation += 360.;

    transform().setRotation(m_orientation, Vector::j);

    // now that the orientation is set, lets make the sheep walk / run
    walkOrRun(vel * i_sec);
    res = true;
  }
  else if (!m_waiting) {
    // if the sheep isn't moving, it will stand there, waiting
    setWaitingPosition();
    res = true;
  }

  return res;
}

// -------------------------------------------------------------------------
// walkOrRun(distance) : will animate legs to walk or run given distance (m)
// -------------------------------------------------------------------------

void Sheep::walkOrRun(double i_distance)
{
  if (i_distance != 0.)
    setAnimationPhase(m_phase + (i_distance / getStride(m_walking)));
}

// -------------------------------------------------------------------------
// setAnimationPhase(phase) : set new animation phase and adjust body parts
//                            accordingly
//
// notes : 'phase' should be within [0, 1[, but out of range values
//         are also accepted
// -------------------------------------------------------------------------

void Sheep::setAnimationPhase(double i_phase)
{
  if (m_phase != i_phase) {
    m_phase = i_phase;

    // first, the body will be scaled to simulate subtle breathing
    {
      // two breathes per cycle
      double s = cos((m_phase / 2.) * (2. * M_PI));
      mp_body->transform().setScaling
        (Vector(1.,
                0.7 * (1. + (s * 0.02)),
                0.7 * (1. - (s * 0.02))));
    }

    // then, the sheep will rock a little bit while walking / running
    {
      double r = sin(m_phase * (2. * M_PI));
      mp_sheep->transform().setRotation(r * 2., Vector::i);
    }

    // now, lets get the legs moving
    if (m_walking) {
      double a = sin((m_phase              ) * (2. * M_PI));
      double b = sin((m_phase + WALK_OFFSET) * (2. * M_PI));

      setLegPosition(true,  true,  a);
      setLegPosition(false, false, a);
      setLegPosition(true,  false, b);
      setLegPosition(false, true,  b);
    }
    else {
      // while running, the sin curve is modified and cut so that the
      // legs remain a little bit motionless while at the back
      double a = (sin((m_phase             ) * (2. * M_PI)) * 1.5) - 0.5;
      double b = (sin((m_phase + RUN_OFFSET) * (2. * M_PI)) * 1.5) - 0.5;

      if (a < -1.) a = -1.;
      if (b < -1.) b = -1.;

      setLegPosition(true,  true,  a);
      setLegPosition(true,  false, a);
      setLegPosition(false, true,  b);
      setLegPosition(false, false, b);
    }
  }
}

// -------------------------------------------------------------------------
// setWaitingPosition : make the sheep stand straight for a while
// -------------------------------------------------------------------------

void Sheep::setWaitingPosition()
{
  if (!m_waiting) {
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        mp_leg[i][j]->transform().setRotation(90., Vector::i);

    m_waiting = true;
  }
}

// -------------------------------------------------------------------------
// setLegPosition(front, right, position) : set the position of one leg
//
// front, right : boolean values to select the leg
// position     : value within [-1, 1], 1 being the farthest forward
// -------------------------------------------------------------------------

void Sheep::setLegPosition(bool i_front, bool i_right, double i_position)
{
  // we will not be in the waiting position anymore
  m_waiting = false;

  // first, we'll have to put the leg back straight
  Globject* leg = mp_leg[i_front ? 0 : 1][i_right ? 0 : 1];
  leg->transform().setRotation(90., Vector::i);

  // while running, hind legs have their ranges inversed
  double angle = m_walking ?
    (WALK_LEG_MAX + WALK_LEG_MIN) / 2. :
    (RUN_LEG_MAX  + RUN_LEG_MIN ) / 2. * (i_front ? 1. : -1.);

  angle += m_walking ?
    (WALK_LEG_MAX - WALK_LEG_MIN) / 2. * i_position :
    (RUN_LEG_MAX  - RUN_LEG_MIN ) / 2. * i_position ;

  leg->transform().addRotation(angle, Vector::k);
}

// -------------------------------------------------------------------------
// setDisplacementMode(walking) : is the sheep walking or running?
// -------------------------------------------------------------------------

void Sheep::setDisplacementMode(bool i_walking)
{
  m_walking = i_walking;
}

// -------------------------------------------------------------------------
// getStride(walking) : distance, in meters, made while 'walking' a cycle
// -------------------------------------------------------------------------

double Sheep::getStride(bool i_walking)
{
  double legLength = (mp_leg[0][0] ? mp_leg[0][0]->height() : 0.) * m_size;
  double stride = i_walking ?
    (WALK_LEG_MAX - WALK_LEG_MIN) / 360. * 2. * (2. * M_PI * legLength) :
    (RUN_LEG_MAX  - RUN_LEG_MIN ) / 360. * 3. * (2. * M_PI * legLength) ;
  return stride;
}
