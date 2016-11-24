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

// Sets necessary OpenGL states, the viewport, and the projection matrix
void InitializeGL(int width, int height);

// Initialise DevIL
void InitializeDevIL();

// Receives an integer and renders it as string
void UtilityRenderText(int numbersToDraw, GLuint &textureID, float cellWidth, float cellHeight);

// Returns coordinates of requested character
float* GetTextCoordinates(char requestedChar);

// Generic structure that holds sprite coordinates
struct Coordinates
{
    public:
        float m_xLocation;
        float m_yLocation;
};

struct TextureCoordinates
{
    public:
        float m_topLeft;
        float m_topRight;
        float m_bottomLeft;
        float m_bottomRight;
};

#endif // UTILITY_H
