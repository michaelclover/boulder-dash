#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "utility.h"

class Sprite
{
public:
  // Methods
  Sprite(GLuint *texID, SpriteType &t, float &x, float &y)
  {
    m_texID = texID;
    m_coordinates.m_x = x;
    m_coordinates.m_y = y;
    m_type = t;
  }
  virtual ~Sprite() {}
  virtual void Draw(float cw, float ch) = 0;

  // Members
  Coordinates m_coordinates;
  GLuint* m_texID;
  SpriteType m_type;
};

class Dirt : public Sprite
{
public:
  // Methods
  Dirt(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  {

  }
  ~Dirt();
  void Draw(float cw, float ch);
};

class Wall : public Sprite
{
public:
  Wall(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  {

  }
  ~Wall();
  void Draw(float cw, float ch);
};

class Boulder : public Sprite
{
public:
  Boulder(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  {

  }
  ~Boulder();
  void Draw(float w, float h);
};

class Coin : public Sprite
{
public:
  Coin(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(0)
  {

  }
  ~Coin();
  void Draw(float cw, float ch);
  int m_animationSequence;
};

class Exit : public Sprite
{
public:
  Exit(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(1)
  , m_triggerAnimation(false)
  {

  }
  ~Exit();
  void Draw(float cw, float ch);
  int m_animationSequence;
  bool m_triggerAnimation;
};

class Impassable : public Sprite
{
public:
  Impassable(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  {

  }
  ~Impassable();
  void Draw(float cw, float ch);
};

class Player : public Sprite
{
public:
  Player(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(0)
  , m_direction(0)
  {

  }
  ~Player();
  void Draw(float cw, float ch);
  int m_animationSequence;
  int m_direction; // 4 for left, 5 for right
};

class Amoeba : public Sprite
{
public:
  Amoeba(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(0)
  {

  }
  ~Amoeba();
  void Draw(float cw, float ch);
  int m_animationSequence;
};

class MagicWall : public Sprite
{
public:
  MagicWall(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(4)
  {

  }
  ~MagicWall();
  void Draw(float cw, float ch);
  int m_animationSequence;
};

class Butterfly : public Sprite
{
public:
  Butterfly(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(2)
  {

  }
  ~Butterfly();
  void Draw(float cw, float ch);
  int m_animationSequence;
};

class Firefly : public Sprite
{
public:
  Firefly(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(0)
  {

  }
  ~Firefly();
  void Draw(float cw, float ch);
  int m_animationSequence;
};

#endif // SPRITE_H
