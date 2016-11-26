#include "main.h"

BoulderDash::BoulderDash(std::string title, int width, int height)
{
    m_coinsCollected = 0;
    m_totalScore = 0;
    m_requiredCoins = 12;
    m_timeInCave = 150;
    m_isMapOpen = true;
    m_isPlayerExit = false;

    m_cw = ORTHO_WIDTH / 40;
    m_ch = ORTHO_HEIGHT / 22;

    mainWindow = new Window(title, width, height);

    mainCaveTimer = new Timer();
    mainTimer = new Timer();

    if(mainWindow->Initialise() != 0)
    {
        std::cerr << "Error initializing SDL, Please ensure you are using SDL2" << std::endl;
        m_quit = true;
    }

    InitializeGL(mainWindow->GetWidth(), mainWindow->GetHeight());

    InitializeDevIL();

    if(mainTexture.LoadTextureFromFile(IMG_FOLDER + "spritesheet.png") != true)
    {
        std::cerr << "Error loading sprite textures" << std::endl;
        m_quit = true;
    }

    if(mainTextTexture.LoadTextureFromFile(IMG_FOLDER + "textsheet.png") != true)
    {
        std::cerr << "Error loading text sheet" << std::endl;
        m_quit = true;
    }
}

// Delete all pointers
BoulderDash::~BoulderDash()
{
    delete mainCaveTimer;
    delete mainWindow;
    delete mainTimer;

    for(int i = 0; i < m_objects.size(); ++i)
      delete m_objects[i];
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
            // Move the player up
        if(m_keyboard[SDLK_a] == true)
            // Move the player left
        if(m_keyboard[SDLK_s] == true)
            // Move the player down
        if(m_keyboard[SDLK_d] == true) {}
            // Move the player right

        m_timeInCave = mainCaveTimer->GetTicks() / 1000 - 150;
        // if time in cave == 0, kill character, respawn him back where he
        // spawned and reset the cave time to the original allocated time for
        // this cave
    }
    else
    {
        // Take the time remaining and add it to the total score
        if(m_timeInCave != 0)
        {
            m_timeInCave++;
            m_totalScore++;
        }

        mainExit->m_triggerAnimation = false;
    }

    if(m_coinsCollected == m_requiredCoins && m_isPlayerExit == false)
        m_exit->m_triggerAnimation = true;

    if(abs(m_exit->m_coordinates.m_x) == abs(m_player->m_coordinates.m_x) &&
       abs(m_exit->m_coordinates.m_y) == abs(m_player->m_coordinates.m_y) &&
       m_exit->m_triggerAnimation == true)
    {
        m_isPlayerExit = true;
    }

    // Process AI movement and handle collisions here
}

// Render the entire scene
void BoulderDash::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glViewport(50.0f, 50.0f, (float)mainWindow->GetWidth()-100.0f, (float)mainWindow->GetHeight()-100.0f);

    // Call the draw function for all objects currently in the cave
    for(int i = 0; i < m_objects.size(); ++i)
      m_objects[i]->Draw(m_cw, m_ch);
}

void BoulderDash::RenderText()
{
    glViewport(50.0f, 10.0f, (float)mainWindow->GetWidth()-100.0f, (float)mainWindow->GetHeight()-20.0f);

    glLoadIdentity();

    glTranslatef(175.0f, 0, 0);
    UtilityRenderText(m_requiredCoins, mainTextTexture.m_textureID, m_cw, m_ch);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_coinsCollected, mainTextTexture.m_textureID, m_cw, m_ch);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_timeInCave, mainTextTexture.m_textureID, m_cw, m_ch);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_totalScore, mainTextTexture.m_textureID, m_cw, m_ch);
}

// Handle all user events and deal with it accordingly
bool BoulderDash::HandleInput(SDL_Event &event)
{
    while(SDL_PollEvent(&event) != 0)
    {
      if(m_mainMenu == true)
      {
        if(event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_RETURN:
              m_mainMenu = false;
              LoadPlaylist();
              LoadMap(CAVE_FOLDER + m_caves.front());
              mainCaveTimer->Start();
              break;
          }
        }
        if(event.type == SDL_QUIT)
          return false;

        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            mainWindow->Resize(event.window.data1, event.window.data2);
        }

        return true;
      }

        if(event.type == SDL_QUIT)
            return false;

        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                    case SDLK_w: m_keyboard[SDLK_w] = true; break;
                    case SDLK_a: m_keyboard[SDLK_a] = true; m_player->m_direction = 4; break;
                    case SDLK_s: m_keyboard[SDLK_s] = true; break;
                    case SDLK_d: m_keyboard[SDLK_d] = true; m_player->m_direction = 5; break;
            }
        }

        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                    case SDLK_w: m_keyboard[event.key.keysym.sym] = false; m_player->m_direction = 0; break;
                    case SDLK_a: m_keyboard[event.key.keysym.sym] = false; m_player->m_direction = 0; break;
                    case SDLK_s: m_keyboard[event.key.keysym.sym] = false; m_player->m_direction = 0; break;
                    case SDLK_d: m_keyboard[event.key.keysym.sym] = false; m_player->m_direction = 0; break;
            }
        }

        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            mainWindow->Resize(event.window.data1, event.window.data2);
        }
    }

    return true;
}

// Takes the name of the map file as input and creates game objects specified by the file
void BoulderDash::LoadMap(std::string fileName)
{
    std::ifstream mapFile(fileName);

    if(!mapFile.good())
        std::cerr << "Error opening map, ensure file exists" << std::endl;
    else
    {
        char tileData;
        int tileX = 0;
        int tileY = 0;

        GLuint* texID = &mainTexture.m_textureID;
        while(mapFile.get(tileData))
        {
            switch(tileData)
            {
              case 'p':
                m_objects.push_back(new Player(texID, SpriteType::Player, tileX, tileY));
                m_player = reinterpret_cast<Player*>(m_objects.back());
                tileX += m_cw;
                break;

              case 'd':
                m_objects.push_back(new Dirt(texID, SpriteType::Dirt, tileX, tileY));
                tileX += m_cw;
                break;

              case 'b':
                m_objects.push_back(new Boulder(texID, SpriteType::Boulder, tileX, tileY));
                tileX += m_cw;
                break;

              case 'w':
                m_objects.push_back(new Wall(texID, SpriteType::Wall, tileX, tileY));
                tileX += m_cw;
                break;

              case 'c':
                m_objects.push_back(new Coin(texID, SpriteType::Coin, tileX, tileY));
                tileX += m_cw;
                break;

              case 'e':
                m_objects.push_back(new Exit(texID, SpriteType::Exit, tileX, tileY));
                m_exit = reinterpret_cast<Exit*>(m_objects.back());
                tileX += m_cw;
                break;

              case 'i':
                m_objects.push_back(new Impassable(texID, SpriteType::Impassable, tileX, tileY));
                tileX += m_cw;
                break;

              case 'n':
                tileX += m_cw;
                break;

              case 'a':
                m_objects.push_back(new Amoeba(texID, SpriteType::Amoeba, tileX, tileY));
                tileX += m_cw;
                break;

              case 'm':
                m_objects.push_back(new MagicWall(texID, SpriteType::Magicwall, tileX, tileY));
                tileX += m_cw;
                break;

              case 's':
                m_objects.push_back(new Butterfly(texID, SpriteType::Butterfly, tileX, tileY));
                tileX += m_cw;
                break;

              case 'f':
                m_objects.push_back(new Firefly(texID, SpriteType::Firefly, tileX, tileY));
                tileX += m_cw;
                break;

              default:
                break;
            }

            if(tileX >= (int)ORTHO_WIDTH)
            {
                tileX = 0;
                tileY += m_ch;
            }
        }
    }

    mapFile.close();
}

void BoulderDash::CloseMap()
{
    // Erase all game objects
    for(int i = 0; i < m_objects.size(); ++i)
      delete m_objects[i];
}

void BoulderDash::RenderMainMenu()
{
  float w = static_cast<float>(mainWindow->GetWidth());
  float h = static_cast<float>(mainWindow->GetHeight());

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0.0f, 0.0f, w, h);

  glLoadIdentity();
  glTranslatef((w / 2 - m_cw * 5) , h / 2, 0.0f);
  UtilityRenderText("press return", mainTextTexture.m_textureID, m_cw, m_ch);

  glLoadIdentity();
  glTranslatef((w / 2 - m_cw * 5) + (m_cw * 2), (h / 2) + m_ch, 0.0f);
  UtilityRenderText("to begin", mainTextTexture.m_textureID, m_cw, m_ch);
}

void BoulderDash::MainLoop()
{
    SDL_Event event;

    // Whilst we've not quit the application...
    while(m_quit != true)
    {
      if(m_mainMenu == true)
      {
        if(HandleInput(event) == false)
          m_quit = true;

        RenderMainMenu();

        SDL_GL_SwapWindow(mainWindow->GetHandle());
      }
      else
      {
        mainTimer->Start();

        // Then there is no user event to handle because he's quit...
        if(HandleInput(event) == false)
            m_quit = true;

        Update();

        Render();

        RenderText();

        SDL_GL_SwapWindow(mainWindow->GetHandle());

        if(mainTimer->GetTicks() < TICKS_PER_FRAME)
            SDL_Delay(TICKS_PER_FRAME - mainTimer->GetTicks());
      }
    }
}

void BoulderDash::LoadPlaylist()
{
  m_caves.clear();

  std::ifstream playlist(PLAYLIST_FOLDER + "playlist1.txt");
  if(!playlist.good())
      std::cerr << "Error opening playlist, ensure playlist exists" << std::endl;
  else
  {
    for(std::string line; std::getline(playlist, line);)
      m_caves.push_back(line);
  }
}

// Program executes here
int main(int argc, char** argv)
{
    BoulderDash game("Boulder Dash", 640, 480);
    game.MainLoop();

    return 0;
}
