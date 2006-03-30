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

#include "scene.h"
#include "boundingsphere.h"
#include "vector.h"
#include "physics.h"
#include "sheep.h"
#include "transform.h"
#include "ball.h"
#include "texture.h"
#include "color.h"
#include "sphere.h"
#include <cstdlib>
#include <cmath>
#include <QtOpenGL>

#define WORLD_RADIUS   1.e10  // radius of the sphere the world is in
#define MAXIMUM_SHEEPS 7      // how many sheeps will we have to protect?
#define LIMIT_GRASS    500.   // where does the field of grass end?
#define BIG_BALL_ACCEL 0.08   // Big Red Ball acceleration (m/s^2)

Scene::Scene()
  :Globject(),
   m_targets(),
   m_nextTargetId(0),
   m_textures(),
   m_sheep_counter(0),
   m_victims(),
   m_current_victim(-1),
   mp_big_ball(0),
   m_evil_big_ball(true)
{
  // the scene is in a sphere so large that the floor is almost flat
  BoundingSphere limits(WORLD_RADIUS, Vector(0., WORLD_RADIUS, 0.));
  setContainerLimits(limits);

  // what is in the scene?
  {
    // first, there is our hero : the Shaolin Sheep !
    Sheep* bah = new Sheep(0.70);
    bah->transform().setTranslation(Vector(0., 0., 10.));
    bah->transform().setRotation( 30., Vector::i);
    bah->transform().addRotation( 40., Vector::k);
    bah->transform().addRotation(-35., Vector::j);
    bah->setMovable(true);
    addChild(bah);
    addTarget(bah);

    // and, its worst enemy : the Big Red Checkered Ball !
    Ball* red = new Ball(2.);
    red->transform().setTranslation(Vector(0., 1., -10.));
    red->setMovable(true);
    addChild(red);
    addTarget(red);

    // with its incredible red checkered texture !
    Texture* tex = Texture::newCheckeredTexture(128, Color::red);
    red->setTexture(tex);
    m_textures.push_back(tex);
    mp_big_ball = red;
  }
}

Scene::~Scene()
{
  // free all the textures
  for (vec_textures::iterator it = m_textures.begin();
       it != m_textures.end(); it++) {
    delete (*it); (*it) = 0;
  }
  m_textures.clear();
}

bool Scene::tick(int i_ms)
{
  // we need some sheeps to protect from the Big Red Checkered Ball
  if ((m_sheep_counter < MAXIMUM_SHEEPS) && (rand() % 100 == 0)) {  
    // new sheeps have 3/4 the size of our hero
    Sheep* sheep = new Sheep(0.70 * 0.75);
    sheep->setMovable(true);
    m_victims.push_back(sheep);
    m_sheep_counter++;

    // they come from the skies !
    sheep->transform().setTranslation(Vector(0., 25., 0.));
    sheep->setVelocity
      (Vector((rand()%10)/10.-0.5, (rand()%10)/10., (rand()%10)/10.-0.5));
    addChild(sheep);
  }

  // the evil Big Red Checkered Ball wants to roll over the sheeps!
  if (((m_current_victim < 0) || (rand() % 1000 == 0)) && m_sheep_counter)
    m_current_victim = (rand() % m_sheep_counter);
  if (m_evil_big_ball && mp_big_ball && (m_current_victim >= 0)) {
    Vector dir = (m_victims[m_current_victim])->position() -
      mp_big_ball->position();
    dir.setY(0.); dir.l2normalize();
    mp_big_ball->setVelocity
      (mp_big_ball->velocity() + dir * BIG_BALL_ACCEL);
  }

  // if a child fled away in the sky, teleport it back to the center
  for (vec_globject::const_iterator it = children().begin();
       it != children().end(); it++) {
    if ((fabs((*it)->position().x()) > LIMIT_GRASS) ||
        (fabs((*it)->position().z()) > LIMIT_GRASS)) {
      (*it)->setPosition(Vector(0., 5., 0.));
    }
  }  

  // move and animate each part of the scene
  double seconds = (double)i_ms / 1000.;
  bool res = Globject::tick(seconds);

  // check for collisions, etc.
  if (Physics::tick(seconds, *this)) res = true;
  return res;
}

// -------------------------------------------------------------------------
// setEvilBigBall(evil) : is the big ball evil? (wants to attack the sheeps)
// -------------------------------------------------------------------------
void Scene::setEvilBigBall(bool i_evil)
{
  m_evil_big_ball = i_evil;
}

const Globject* Scene::target(int i_id) const
{
  map_globject::const_iterator it = m_targets.find(i_id);
  return (it == m_targets.end() ? 0 : (*it).second);
}

void Scene::addTarget(const Globject* t)
{
  m_targets.insert(map_globject::value_type(m_nextTargetId++, t));
}

void Scene::globject_draw(QGLWidget* i_gl)
{
  // push current opengl states
  glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);

  // turn off lighting : we can't afford to use it here
  glDisable(GL_LIGHTING);

  // wool texture from sheeps makes great grass
  Texture::pushAttrib();
  Sheep::sp_wool->bind(i_gl);

  // grass field
  glBegin(GL_TRIANGLE_STRIP);
  glColor3d(0.0, 0.7, 0.0);
  glTexCoord2d(0., 0.);
  glVertex3d(-LIMIT_GRASS, 0., -LIMIT_GRASS);
  glTexCoord2d(0., 1.);
  glVertex3d(-LIMIT_GRASS, 0.,  LIMIT_GRASS);
  glTexCoord2d(1., 0.);
  glVertex3d( LIMIT_GRASS, 0., -LIMIT_GRASS);
  glTexCoord2d(1., 1.);
  glVertex3d( LIMIT_GRASS, 0.,  LIMIT_GRASS);
  glEnd();

  // pop back previous opengl states
  Texture::popAttrib();
  glPopAttrib();
}
