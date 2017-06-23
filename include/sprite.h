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
    m_velocity = false;
  }
  virtual ~Sprite() {}
  virtual void Draw(float cw, float ch) = 0;

  // Members
  Coordinates m_coordinates;
  GLuint* m_texID;
  SpriteType m_type;
  std::vector<Direction> m_intent;
  bool m_velocity;

  // these attributes change depending on specifics
  bool m_rounded;
  bool m_explodable;
  bool m_consumable;
};

class Dirt : public Sprite
{
public:
  // Methods
  Dirt(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  {
    m_rounded = false;
    m_explodable = true;
    m_consumable = true;
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
    m_rounded = true;
    m_explodable = true;
    m_consumable = true;
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
    m_rounded = true;
    m_explodable = true;
    m_consumable = true;
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
    m_rounded = true;
    m_explodable = true;
    m_consumable = true;
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
    m_rounded = false;
    m_explodable = false;
    m_consumable = false;
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
    m_rounded = false;
    m_explodable = false;
    m_consumable = false;
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
    m_rounded = false;
    m_explodable = true;
    m_consumable = true;
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
    m_rounded = false;
    m_explodable = false;
    m_consumable = true;
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
  , m_triggered(false)
  {

  }
  ~MagicWall();
  void Draw(float cw, float ch);
  int m_animationSequence;
  bool m_triggered;
};

class Butterfly : public Sprite
{
public:
  Butterfly(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(2)
  , m_direction(Direction::Right)
  {
    m_rounded = false;
    m_explodable = true;
    m_consumable = true;
  }
  ~Butterfly();
  void Draw(float cw, float ch);
  int m_animationSequence;
  Direction m_direction;
};

class Firefly : public Sprite
{
public:
  Firefly(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(0)
  , m_direction(Direction::Left)
  {
    m_rounded = false;
    m_explodable = true;
    m_consumable = true;
  }
  ~Firefly();
  void Draw(float cw, float ch);
  int m_animationSequence;
  Direction m_direction;
};

class Explosion : public Sprite
{
public:
  Explosion(GLuint *texID, SpriteType t, float x, float y)
  : Sprite(texID, t, x, y)
  , m_animationSequence(7)
  , m_done(false)
  , m_gem(false)
  {
    m_rounded = false;
    m_explodable = false;
    m_consumable = false;
  }
  ~Explosion();
  void Draw(float cw, float ch);
  int m_animationSequence;
  bool m_done; // have we finished our explosion animations?
  bool m_gem; // should this explosion produce a gem in its wake?
};

#endif // SPRITE_H
