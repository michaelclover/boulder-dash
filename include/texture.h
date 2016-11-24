#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <IL/il.h>
#include <IL/ilu.h>

class TextureSheet
{
    public:
        // Base constructor; initialises member variables
        TextureSheet();

        // Base destructor; performs clean-up
        ~TextureSheet();

        // Load texture sheet from file
        bool LoadTextureFromFile(std::string file);

        // ID of the texture sheet
        GLuint m_textureID;

    private:
        // Height and width of loaded texture sheet
        GLuint m_imageWidth;
        GLuint m_imageHeight;

        // Load texture sheet from loaded pixels
        bool LoadTextureFromPixels(GLuint* pixels);
};

#endif // TEXTURE_H
