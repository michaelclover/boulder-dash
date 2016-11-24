#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "utility.h"

class Dirt
{
    public:
        // Construct game objects by providing x and y coordinates
        Dirt(float xLocation, float yLocation);

        // Does not do much right now
        ~Dirt();

        // Generic draw function that all sprites use
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);

        // Stores game object coordinates; see: utility.h
        Coordinates m_coordinates;
};

class Nothing
{
    public:
        Nothing(float xLocation, float yLocation);
        ~Nothing();
        void Draw(float cellWidth, float cellHeight);
        Coordinates m_coordinates;
};

class Wall
{
    public:
        Wall(float xLocation, float yLocation);
        ~Wall();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        Coordinates m_coordinates;
};

class Boulder
{
    public:
        Boulder(float xLocation, float yLocation);
        ~Boulder();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        void Move(std::string whichWay, float cellWidth, float cellHeight, std::vector<Nothing> &nothing);
        void Destroy(std::vector<Nothing> &nothing);
        Coordinates m_coordinates;
        int m_velocity;
};

class Coin
{
    public:
        Coin(float xLocation, float yLocation);
        ~Coin();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        void Move(std::string whichWay, float cellWidth, float cellHeight, std::vector<Nothing> &nothing);
        void Destroy(std::vector<Nothing> &nothing);
        Coordinates m_coordinates;
        int m_animationSequence;
        int m_velocity;
};

class Exit
{
    public:
        Exit(float xLocation, float yLocation);
        ~Exit();
        void Draw(GLuint& textureID, float cellWidth, float cellHeight);
        Coordinates m_coordinates;
        int m_animationSequence;
        bool m_triggerAnimation;
};

class Impassable
{
    public:
        Impassable(float xLocation, float yLocation);
        ~Impassable();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        Coordinates m_coordinates;
};

class Player
{
    public:
        Player(float xLocation, float yLocation);
        ~Player();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        void Move(std::string whichWay, float cellWidth, float cellHeight, std::vector<Nothing> &nothing);
        Coordinates m_coordinates;
        Coordinates m_lastCoordinates;
        int m_animationSequence;
        int m_direction; // 4 for left, 5 for right
        int m_lastDirection; // 4 or 5
};

class Amoeba
{
    public:
        Amoeba(float xLocation, float yLocation);
        ~Amoeba();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        void Move(float cellWidth, float cellHeight, std::vector<Nothing> &nothing);
        Coordinates m_coordinates;
        int m_animationSequence;
};

class MagicWall
{
    public:
        MagicWall(float xLocation, float yLocation);
        ~MagicWall();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        Coordinates m_coordinates;
        int m_animationSequence;
};

class Butterfly
{
    public:
        Butterfly(float xLocation, float yLocation);
        ~Butterfly();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        void Move(float cellWidth, float cellHeight, std::vector<Nothing> &nothing);
        Coordinates m_coordinates;
        int m_animationSequence;
};

class Firefly
{
    public:
        Firefly(float xLocation, float yLocation);
        ~Firefly();
        void Draw(GLuint &textureID, float cellWidth, float cellHeight);
        void Move(float cellWidth, float cellHeight, std::vector<Nothing> &nothing);
        Coordinates m_coordinates;
        int m_animationSequence;
};

#endif // SPRITE_H

