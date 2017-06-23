#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <IL/il.h>
#include <IL/ilu.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>

#include "defines.h"

// Sets necessary OpenGL states, the viewport, and the projection matrix
void InitializeGL(int width, int height);

// Initialise DevIL
void InitializeDevIL();

// Receives an integer and renders it as string
void UtilityRenderText(int num, GLuint &texID, float cw, float ch);

// Receives a string and renders it
void UtilityRenderText(std::string s, GLuint &texID, float cw, float ch);

// Returns coordinates of requested character
float* GetCoordinates(char requestedChar);

// Generic structure that holds sprite coordinates
struct Coordinates
{
    public:
        int m_x;
        int m_y;
};

// Types of sprites available in our game
enum class SpriteType : uint8_t
{
  Dirt,
  Nothing,
  Wall,
  Boulder,
  Coin,
  Exit,
  Impassable,
  Player,
  Amoeba,
  Magicwall,
  Butterfly,
  Firefly,
  Explosion
};

enum class Direction : uint8_t
{
  Up,
  Down,
  Left,
  Right
};

struct Cave
{
  std::string name;
  int coins;
  int time;
  int ws; // optional -- magicwall start time
  int we; // optional -- magicwall end time
  int growth; // optional -- amoeba growth rate
};

#endif // UTILITY_H
