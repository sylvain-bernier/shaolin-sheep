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

#ifndef SCENE_H
#define SCENE_H
class   Scene;

class Texture;
#include "globject.h"
#include <map>
#include <vector>

class Scene : public Globject
//
// Scene : main opengl scene, a green valley with sheeps
//
{
 public:
  Scene();
  virtual ~Scene();

  // new frame tick (true -> the scene needs to be redrawn)
  bool tick(int i_ms);

  // is the big ball evil? (wants to attack the sheeps)
  void setEvilBigBall(bool i_evil);

  // access to interesting scene targets
  const Globject* target(int i_id) const;

 protected:
  void addTarget(const Globject* t);
  virtual void globject_draw(QGLWidget* i_gl);

 private:
  typedef std::map<int, const Globject*> map_globject;
  typedef std::vector<Texture*>          vec_textures;
  typedef std::vector<Globject*>         vec_victims;

  map_globject m_targets;         // available targets
  int          m_nextTargetId;    // next new target will get this id
  vec_textures m_textures;        // all textures used in the scene
  int          m_sheep_counter;   // how many sheeps to protect?
  vec_victims  m_victims;         // vector of sheeps to attack
  int          m_current_victim;  // current victim for the Big Red Ball
  Globject*    mp_big_ball;       // pointer to the Big Red Ball
  bool         m_evil_big_ball;   // is the Big Red Ball possessed?
};

#endif // SCENE_H
