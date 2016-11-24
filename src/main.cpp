#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "window.h"
#include "sprite.h"
#include "utility.h"
#include "texture.h"
#include "timer.h"
#include "defines.h"

class BoulderDash
{
    public:
        // Needs to be 640x480 by default
        BoulderDash(std::string title, int width, int height);

        // Deallocates memory
        ~BoulderDash();

        // Update the game model
        void Update();

        // Render the game model
        void Render();

        // Render necessary text
        void RenderText();

        // Handle user input
        bool HandleInput(SDL_Event &event);

        // Load a map file
        void LoadMap(std::string fileName);

        // Close the currently opened map
        void CloseMap();

        // Game main loop
        void MainLoop();

    private:
        // Pointer to the game window
        Window* mainWindow = nullptr;

        // Pointer to the player
        Player* mainPlayer = nullptr;

        // Pointer to the exit location
        Exit* mainExit = nullptr;

        // Pointer to the timer
        Timer* mainTimer = nullptr;

        // Pointer to cave timer
        Timer* mainCaveTimer = nullptr;

        // Pointer to the texture sheet
        TextureSheet mainTexture;

        // Pointer to the text sheet
        TextureSheet mainTextTexture;

        // Vector of dirt objects
        std::vector<Dirt> dirt;

        // Vector of boulder objects
        std::vector<Boulder> boulder;

        // Vector of wall objects
        std::vector<Wall> wall;

        // Vector of coin objects
        std::vector<Coin> coin;

        // Vector of impassable objects
        std::vector<Impassable> impassable;

        // Vector of nothing objects
        std::vector<Nothing> nothing;

        // Vector of amoeba objects
        std::vector<Amoeba> amoeba;

        // Vector of MagicWall objects
        std::vector<MagicWall> magicWall;

        // Vector of butterfly objects
        std::vector<Butterfly> butterfly;

        // Vector of firefly objects
        std::vector<Firefly> firefly;

        // Quit boolean for game
        bool m_quit = false;

        // Width and height of tiles
        float m_cellWidth;
        float m_cellHeight;

        // Player score
        int m_totalScore;

        // Coins collected;
        int m_coinsCollected;

        // Required coins to leave cave
        int m_requiredCoins;

        // Cave time
        int m_timeInCave;

        // Boolean to test if map is closed or open
        bool m_isMapOpen;

        // Boolean to test if player is at the exit
        bool m_isPlayerExit;

        // Save key-press states
        std::map<int, bool> m_keyboard;
};

BoulderDash::BoulderDash(std::string title, int width, int height)
{
    m_coinsCollected = 0;
    m_totalScore = 0;
    m_requiredCoins = 12;
    m_timeInCave = 150;
    m_isMapOpen = true;
    m_isPlayerExit = false;

    m_cellWidth = (float)width / 40;
    m_cellHeight = (float)height / 22;

    mainWindow = new Window(title, width, height);

    mainCaveTimer = new Timer();
    mainTimer = new Timer();

    if(mainWindow->InitializeWindow() != 0)
    {
        std::cout << "Error initializing SDL, Please ensure you are using SDL2" << std::endl;
        m_quit = true;
    }

    InitializeGL(mainWindow->GetWindowWidth(), mainWindow->GetWindowHeight());

    InitializeDevIL();

    if(mainTexture.LoadTextureFromFile(IMG_FOLDER + "spritesheet.png") != true)
    {
        std::cout << "Error loading sprite textures!" << std::endl;
        m_quit = true;
    }

    if(mainTextTexture.LoadTextureFromFile(IMG_FOLDER + "textsheet.png") != true)
    {
        std::cout << "Error loading text sheet!" << std::endl;
        m_quit = true;
    }
}

// Delete all pointers
BoulderDash::~BoulderDash()
{
    delete mainCaveTimer;
    delete mainPlayer;
    delete mainWindow;
    delete mainExit;
    delete mainTimer;
}

// Update the model, process collisions, update the camera view
void BoulderDash::Update()
{
    // Prevent diagonal movement
    if(m_keyboard[SDLK_w] == true && m_keyboard[SDLK_a] == true)
    {
        m_keyboard[SDLK_w] = false;
        m_keyboard[SDLK_a] = false;
    }
    if(m_keyboard[SDLK_w] == true && m_keyboard[SDLK_d] == true)
    {
        m_keyboard[SDLK_w] = false;
        m_keyboard[SDLK_d] = false;
    }
    if(m_keyboard[SDLK_s] == true && m_keyboard[SDLK_a] == true)
    {
        m_keyboard[SDLK_s] = false;
        m_keyboard[SDLK_a] = false;
    }
    if(m_keyboard[SDLK_s] == true && m_keyboard[SDLK_d] == true)
    {
        m_keyboard[SDLK_s] = false;
        m_keyboard[SDLK_d] = false;
    }
    // End preventing diagonal movement

    if(m_isPlayerExit != true)
    {
        // Process keyboard events
        if(m_keyboard[SDLK_w] == true)
            mainPlayer->Move("up", m_cellWidth, m_cellHeight, nothing);
        if(m_keyboard[SDLK_a] == true)
            mainPlayer->Move("left", m_cellWidth, m_cellHeight, nothing);
        if(m_keyboard[SDLK_s] == true)
            mainPlayer->Move("down", m_cellWidth, m_cellHeight, nothing);
        if(m_keyboard[SDLK_d] == true)
            mainPlayer->Move("right", m_cellWidth, m_cellHeight, nothing);
        // End processing keyboard events

        m_timeInCave = mainCaveTimer->GetTicks() / 1000 - 150;
    }
    else
    {
        if(m_timeInCave != 0)
        {
            m_timeInCave++;
            m_totalScore++;
        }

        mainExit->m_triggerAnimation = false;
    }

    if(m_coinsCollected == m_requiredCoins && m_isPlayerExit == false)
        mainExit->m_triggerAnimation = true;

    if(abs(mainExit->m_coordinates.m_xLocation) == abs(mainPlayer->m_coordinates.m_xLocation) &&
       abs(mainExit->m_coordinates.m_yLocation) == abs(mainPlayer->m_coordinates.m_yLocation) &&
       mainExit->m_triggerAnimation == true)
    {
        m_isPlayerExit = true;
    }

    for(int i = 0; i < dirt.size(); i++)
    {
        // If player has collided with dirt, the dirt is erased
        if(abs(dirt[i].m_coordinates.m_xLocation) == abs(mainPlayer->m_coordinates.m_xLocation) &&
           abs(dirt[i].m_coordinates.m_yLocation) == abs(mainPlayer->m_coordinates.m_yLocation))
        {
            dirt.erase(dirt.begin() + i);
        }
    }

    for(int i = 0; i < boulder.size(); i++)
    {
        // If player has collided with boulder, the player is moved back to their previous position
        if(abs(boulder[i].m_coordinates.m_xLocation) == abs(mainPlayer->m_coordinates.m_xLocation) &&
           abs(boulder[i].m_coordinates.m_yLocation) == abs(mainPlayer->m_coordinates.m_yLocation))
        {
            // Remove nothing from the players last position so the boulder doesn't switch position with player
            for(int j = 0; j < nothing.size(); j++)
            {
                if(abs(nothing[j].m_coordinates.m_xLocation) == abs(mainPlayer->m_lastCoordinates.m_xLocation) &&
                   abs(nothing[j].m_coordinates.m_yLocation) == abs(mainPlayer->m_lastCoordinates.m_yLocation))
                {
                    nothing.erase(nothing.begin() + j);
                }
            }

            mainPlayer->m_coordinates.m_xLocation = mainPlayer->m_lastCoordinates.m_xLocation;
            mainPlayer->m_coordinates.m_yLocation = mainPlayer->m_lastCoordinates.m_yLocation;
        }
    }

    for(int i = 0; i < impassable.size(); i++)
    {
        // If player has collided with impassable, the player is moved back to their previous position
        if(abs(impassable[i].m_coordinates.m_xLocation) == abs(mainPlayer->m_coordinates.m_xLocation) &&
           abs(impassable[i].m_coordinates.m_yLocation) == abs(mainPlayer->m_coordinates.m_yLocation))
        {
            // Remove nothing from the players last position so objects don't enter the player when he walks into a wall
            for(int j = 0; j < nothing.size(); j++)
            {
                if(abs(nothing[j].m_coordinates.m_xLocation) == abs(mainPlayer->m_lastCoordinates.m_xLocation) &&
                   abs(nothing[j].m_coordinates.m_yLocation) == abs(mainPlayer->m_lastCoordinates.m_yLocation))
                {
                    nothing.erase(nothing.begin() + j);
                }
            }

            mainPlayer->m_coordinates.m_xLocation = mainPlayer->m_lastCoordinates.m_xLocation;
            mainPlayer->m_coordinates.m_yLocation = mainPlayer->m_lastCoordinates.m_yLocation;
        }
    }

    for(int i = 0; i < coin.size(); i++)
    {
        // If the coin is in the same position as the player, and has no velocity, the player collects the coin
        if(abs(coin[i].m_coordinates.m_xLocation) == abs(mainPlayer->m_coordinates.m_xLocation) &&
           abs(coin[i].m_coordinates.m_yLocation) == abs(mainPlayer->m_coordinates.m_yLocation) &&
           coin[i].m_velocity == 0)
        {
            m_coinsCollected++;
            m_totalScore += 10;
            coin.erase(coin.begin() + i);
        }
    }

    for(int i = 0; i < wall.size(); i++)
    {
        // If player has collided with wall, the player is moved back to their previous position
        if(abs(wall[i].m_coordinates.m_xLocation) == abs(mainPlayer->m_coordinates.m_xLocation) &&
           abs(wall[i].m_coordinates.m_yLocation) == abs(mainPlayer->m_coordinates.m_yLocation))
        {
            // Remove nothing from the players last position so objects don't enter the player when he walks into a wall
            for(int j = 0; j < nothing.size(); j++)
            {
                if(abs(nothing[j].m_coordinates.m_xLocation) == abs(mainPlayer->m_lastCoordinates.m_xLocation) &&
                   abs(nothing[j].m_coordinates.m_yLocation) == abs(mainPlayer->m_lastCoordinates.m_yLocation))
                {
                    nothing.erase(nothing.begin() + j);
                }
            }

            mainPlayer->m_coordinates.m_xLocation = mainPlayer->m_lastCoordinates.m_xLocation;
            mainPlayer->m_coordinates.m_yLocation = mainPlayer->m_lastCoordinates.m_yLocation;
        }
    }

    for(int i = 0; i < boulder.size(); i++)
    {
        // If the player is below the boulder, and the boulder has velocity, the player is crushed
        if(abs(mainPlayer->m_coordinates.m_xLocation) == abs(boulder[i].m_coordinates.m_xLocation) &&
           abs(mainPlayer->m_coordinates.m_yLocation - m_cellHeight) == abs(boulder[i].m_coordinates.m_yLocation) &&
           boulder[i].m_velocity == 1)
        {
            boulder[i].Destroy(nothing);
            boulder.erase(boulder.begin() + i);
        }
    }

    for(int i = 0; i < boulder.size(); i++)
    {
        bool isBoulderBelow = false, isCoinBelow = false, isLeft = false, isRight = false, isBottomLeft = false, isBottomRight = false, isPlayerBelow = false;
        for(int j = 0; j < boulder.size(); j++)
        {
            if(abs(boulder[i].m_coordinates.m_xLocation) == abs(boulder[j].m_coordinates.m_xLocation) &&
               abs(boulder[i].m_coordinates.m_yLocation + m_cellHeight) == abs(boulder[j].m_coordinates.m_yLocation))
            {
                isBoulderBelow = true;
            }
        }
        for(int m = 0; m < coin.size(); m++)
        {
            if(abs(boulder[i].m_coordinates.m_xLocation) == abs(coin[m].m_coordinates.m_xLocation) &&
               abs(boulder[i].m_coordinates.m_yLocation + m_cellHeight) == abs(coin[m].m_coordinates.m_yLocation))
            {
                isCoinBelow = true;
            }
        }
        for(int k = 0; k < nothing.size(); k++)
        {
            if(abs(boulder[i].m_coordinates.m_xLocation + m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(boulder[i].m_coordinates.m_yLocation) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isRight = true;
            }
            if(abs(boulder[i].m_coordinates.m_xLocation - m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(boulder[i].m_coordinates.m_yLocation) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isLeft = true;
            }
            if(abs(boulder[i].m_coordinates.m_xLocation + m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(boulder[i].m_coordinates.m_yLocation + m_cellHeight) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isBottomRight = true;
            }
            if(abs(boulder[i].m_coordinates.m_xLocation - m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(boulder[i].m_coordinates.m_yLocation + m_cellHeight) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isBottomLeft = true;
            }
        }
        if(isBoulderBelow == true || isCoinBelow == true)
        {
            if(isBottomLeft == true && isLeft == true)
            {
                boulder[i].Move("left", m_cellWidth, m_cellHeight, nothing);
                boulder[i].m_velocity = 1;
            }
            if(isBottomRight == true && isRight == true)
            {
                boulder[i].Move("right", m_cellWidth, m_cellHeight, nothing);
                boulder[i].m_velocity = 1;
            }
        }
    }

    for(int i = 0; i < coin.size(); i++)
    {
        bool isBoulderBelow = false, isCoinBelow = false, isLeft = false, isRight = false, isBottomLeft = false, isBottomRight = false, isPlayerBelow = false;
        for(int j = 0; j < coin.size(); j++)
        {
            if(abs(coin[i].m_coordinates.m_xLocation) == abs(coin[j].m_coordinates.m_xLocation) &&
               abs(coin[i].m_coordinates.m_yLocation + m_cellHeight) == abs(coin[j].m_coordinates.m_yLocation))
            {
                isCoinBelow = true;
            }
        }
        for(int m = 0; m < boulder.size(); m++)
        {
            if(abs(coin[i].m_coordinates.m_xLocation) == abs(boulder[m].m_coordinates.m_xLocation) &&
               abs(coin[i].m_coordinates.m_yLocation + m_cellHeight) == abs(boulder[m].m_coordinates.m_yLocation))
            {
                isBoulderBelow = true;
            }
        }
        for(int k = 0; k < nothing.size(); k++)
        {
            if(abs(coin[i].m_coordinates.m_xLocation + m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(coin[i].m_coordinates.m_yLocation) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isRight = true;
            }
            if(abs(coin[i].m_coordinates.m_xLocation - m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(coin[i].m_coordinates.m_yLocation) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isLeft = true;
            }
            if(abs(coin[i].m_coordinates.m_xLocation + m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(coin[i].m_coordinates.m_yLocation + m_cellHeight) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isBottomRight = true;
            }
            if(abs(coin[i].m_coordinates.m_xLocation - m_cellWidth) == abs(nothing[k].m_coordinates.m_xLocation) &&
               abs(coin[i].m_coordinates.m_yLocation + m_cellHeight) == abs(nothing[k].m_coordinates.m_yLocation))
            {
                isBottomLeft = true;
            }
        }
        if(isBoulderBelow == true || isCoinBelow == true)
        {
            if(isBottomLeft == true && isLeft == true)
            {
                coin[i].Move("left", m_cellWidth, m_cellHeight, nothing);
                coin[i].m_velocity = 1;
            }
            if(isBottomRight == true && isRight == true)
            {
                coin[i].Move("right", m_cellWidth, m_cellHeight, nothing);
                coin[i].m_velocity = 1;
            }
        }
    }

    for(int i = 0; i < boulder.size(); i++)
    {
        // If there is 'nothing' below the boulder, drop the boulder down, and give it velocity
        for(int j = 0; j < nothing.size(); j++)
        {
            if(abs(boulder[i].m_coordinates.m_xLocation) == abs(nothing[j].m_coordinates.m_xLocation) &&
               abs((nothing[j].m_coordinates.m_yLocation - m_cellHeight)) == abs(boulder[i].m_coordinates.m_yLocation))
            {
                boulder[i].Move("down", m_cellWidth, m_cellHeight, nothing);
                boulder[i].m_velocity = 1;
            }
            else
            {
                boulder[i].m_velocity = 0;
            }
        }
    }

    for(int i = 0; i < coin.size(); i++)
    {
        // If the player is below the coin, and the coin has velocity, the player is crushed
        if(abs(mainPlayer->m_coordinates.m_xLocation) == abs(coin[i].m_coordinates.m_xLocation) &&
           abs(mainPlayer->m_coordinates.m_yLocation - m_cellHeight) == abs(coin[i].m_coordinates.m_yLocation) &&
           coin[i].m_velocity == 1)
        {
            coin[i].Destroy(nothing);
            coin.erase(coin.begin() + i);
        }
    }

    for(int i = 0; i < coin.size(); i++)
    {
        // If there is nothing below the coin, drop the coin down, and give it velocity
        for(int j = 0; j < nothing.size(); j++)
        {
            if(abs(coin[i].m_coordinates.m_xLocation) == abs(nothing[j].m_coordinates.m_xLocation) &&
               abs((nothing[j].m_coordinates.m_yLocation - m_cellHeight)) == abs(coin[i].m_coordinates.m_yLocation))
            {
                coin[i].Move("down", m_cellWidth, m_cellHeight, nothing);
                coin[i].m_velocity = 1;
            }
            else
            {
                coin[i].m_velocity = 0;
            }
        }
    }
}

// Render the entire scene
void BoulderDash::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glViewport(50.0f, 50.0f, (float)mainWindow->GetWindowWidth()-100.0f, (float)mainWindow->GetWindowHeight()-100.0f);

    mainPlayer->Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    mainExit->Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < dirt.size(); i++)
        dirt[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < impassable.size(); i++)
        impassable[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < boulder.size(); i++)
        boulder[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < coin.size(); i++)
        coin[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < wall.size(); i++)
        wall[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < amoeba.size(); i++)
        amoeba[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < magicWall.size(); i++)
        magicWall[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < butterfly.size(); i++)
        butterfly[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);

    for(int i = 0; i < firefly.size(); i++)
        firefly[i].Draw(mainTexture.m_textureID, m_cellWidth, m_cellHeight);
}

void BoulderDash::RenderText()
{
    glViewport(50.0f, 10.0f, (float)mainWindow->GetWindowWidth()-100.0f, (float)mainWindow->GetWindowHeight()-20.0f);

    glLoadIdentity();

    glTranslatef(175.0f, 0, 0);
    UtilityRenderText(m_requiredCoins, mainTextTexture.m_textureID, m_cellWidth, m_cellHeight);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_coinsCollected, mainTextTexture.m_textureID, m_cellWidth, m_cellHeight);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_timeInCave, mainTextTexture.m_textureID, m_cellWidth, m_cellHeight);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_totalScore, mainTextTexture.m_textureID, m_cellWidth, m_cellHeight);
}

// Handle all user events and deal with it accordingly
bool BoulderDash::HandleInput(SDL_Event &event)
{
    while(SDL_PollEvent(&event) != 0)
    {
        if(event.type == SDL_QUIT)
            return false;

        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                    case SDLK_w: m_keyboard[SDLK_w] = true; mainPlayer->m_direction = mainPlayer->m_lastDirection; break;
                    case SDLK_a: m_keyboard[SDLK_a] = true; mainPlayer->m_direction = 4; break;
                    case SDLK_s: m_keyboard[SDLK_s] = true; mainPlayer->m_direction = mainPlayer->m_lastDirection; break;
                    case SDLK_d: m_keyboard[SDLK_d] = true; mainPlayer->m_direction = 5; break;
            }
        }

        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                    case SDLK_w: m_keyboard[event.key.keysym.sym] = false; mainPlayer->m_direction = 0; break;
                    case SDLK_a: m_keyboard[event.key.keysym.sym] = false; mainPlayer->m_direction = 0; mainPlayer->m_lastDirection = 4; break;
                    case SDLK_s: m_keyboard[event.key.keysym.sym] = false; mainPlayer->m_direction = 0; break;
                    case SDLK_d: m_keyboard[event.key.keysym.sym] = false; mainPlayer->m_direction = 0; mainPlayer->m_lastDirection = 5; break;
            }
        }

        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            mainWindow->ResizeWindow(event.window.data1, event.window.data2);
        }
    }

    return true;
}

// Takes the name of the map file as input and creates game objects specified by the file
void BoulderDash::LoadMap(std::string fileName)
{
    std::ifstream mapFile(fileName);

    if(!mapFile.good())
        std::cout << "Error opening map, ensure file exists" << std::endl;
    else
    {
        char tileData;
        float tileX = 0.0;
        float tileY = 0.0;

        while(mapFile.get(tileData))
        {
            switch(tileData)
            {
                case 'p': mainPlayer = new Player(tileX, tileY); tileX += m_cellWidth; break;
                case 'd': dirt.push_back(Dirt(tileX, tileY)); tileX += m_cellWidth; break;
                case 'b': boulder.push_back(Boulder(tileX, tileY)); tileX += m_cellWidth; break;
                case 'w': wall.push_back(Wall(tileX, tileY)); tileX += m_cellWidth; break;
                case 'c': coin.push_back(Coin(tileX, tileY)); tileX += m_cellWidth; break;
                case 'e': mainExit = new Exit(tileX, tileY); tileX += m_cellWidth; break;
                case 'i': impassable.push_back(Impassable(tileX, tileY)); tileX += m_cellWidth; break;
                case 'n': nothing.push_back(Nothing(tileX, tileY)); tileX += m_cellWidth; break;
                case 'a': amoeba.push_back(Amoeba(tileX, tileY)); tileX += m_cellWidth; break;
                case 'm': magicWall.push_back(MagicWall(tileX, tileY)); tileX += m_cellWidth; break;
                case 's': butterfly.push_back(Butterfly(tileX, tileY)); tileX += m_cellWidth; break;
                case 'f': firefly.push_back(Firefly(tileX, tileY)); tileX += m_cellWidth; break;
                default: break;
            }

            if(tileX >= (float)mainWindow->GetWindowWidth())
            {
                tileX = 0.0f;
                tileY += m_cellHeight;
            }
        }
    }

    mapFile.close();
}

void BoulderDash::CloseMap()
{
    // Erase all game objects
    for(int i = 0; i < dirt.size(); i++)
    {
        dirt.erase(dirt.begin() + i);
    }
    for(int i = 0; i < boulder.size(); i++)
    {
        boulder.erase(boulder.begin() + i);
    }
    for(int i = 0; i < wall.size(); i++)
    {
        wall.erase(wall.begin() + i);
    }
    for(int i = 0; i < coin.size(); i++)
    {
        coin.erase(coin.begin() + i);
    }
    for(int i = 0; i < impassable.size(); i++)
    {
        impassable.erase(impassable.begin() + i);
    }
    for(int i = 0; i < nothing.size(); i++)
    {
        nothing.erase(nothing.begin() + i);
    }
    for(int i = 0; i < amoeba.size(); i++)
    {
        amoeba.erase(amoeba.begin() + i);
    }
    for(int i = 0; i < magicWall.size(); i++)
    {
        magicWall.erase(magicWall.begin() + i);
    }
    for(int i = 0; i < butterfly.size(); i++)
    {
        butterfly.erase(butterfly.begin() + i);
    }
    for(int i = 0; i < firefly.size(); i++)
    {
        firefly.erase(firefly.begin() + i);
    }

    // Delete pointers to game objects
    delete mainPlayer;
    delete mainExit;
}

void BoulderDash::MainLoop()
{
    SDL_Event event;

    LoadMap(CAVE_FOLDER + "level1.txt");

    mainCaveTimer->Start();

    while(m_quit != true)
    {
        mainTimer->Start();

        // Then there is no user event to handle because he's quit...
        if(HandleInput(event) == false)
            m_quit = true;

        Update();

        Render();

        RenderText();

        SDL_GL_SwapWindow(mainWindow->GetWindowHandle());

        if(mainTimer->GetTicks() < TICKS_PER_FRAME)
            SDL_Delay(TICKS_PER_FRAME - mainTimer->GetTicks());
    }
}

// Program executes here
int main(int argc, char** argv)
{
    BoulderDash game("Boulder Dash", 640, 480);

    game.MainLoop();

    return 0;
}
