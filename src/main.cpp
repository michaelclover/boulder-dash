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

    m_window = new Window(title, width, height);

    m_cavetimer = new Timer();
    m_gametimer = new Timer();

    if(m_window->Initialise() != 0)
    {
        std::cerr << "Error initializing SDL, Please ensure you are using SDL2" << std::endl;
        m_quit = true;
    }

    InitializeGL(m_window->GetWidth(), m_window->GetHeight());

    InitializeDevIL();

    if(m_sprites.LoadTextureFromFile(IMG_FOLDER + "spritesheet.png") != true)
    {
        std::cerr << "Error loading sprite textures" << std::endl;
        m_quit = true;
    }

    if(m_text.LoadTextureFromFile(IMG_FOLDER + "textsheet.png") != true)
    {
        std::cerr << "Error loading text sheet" << std::endl;
        m_quit = true;
    }
}

// Delete all pointers
BoulderDash::~BoulderDash()
{
    delete m_cavetimer;
    delete m_window;
    delete m_gametimer;

    for(int i = 0; i < m_objects.size(); ++i)
      delete m_objects[i];
}

void BoulderDash::ProcessIntent(Sprite* s, Direction d)
{
  SpriteType st = s->m_type;
  int sx = s->m_coordinates.m_x;
  int sy = s->m_coordinates.m_y;

  int x, y;
  switch(d)
  {
    case Direction::Down:
    {
      x = sx;
      y = sy + m_ch;
    }
    break;
    case Direction::Up:
    {
      x = sx;
      y = sy - m_ch;
    }
    break;
    case Direction::Left:
    {
      x = sx - m_cw;
      y = sy;
    }
    break;
    case Direction::Right:
    {
      x = sx + m_cw;
      y = sy;
    }
    break;
    default:
      break;
  }

  bool e = false;
  for(int i = 0; i < m_objects.size(); ++i)
  {
    // This object is occupying the space our object wishes to enter
    if(m_objects[i]->m_coordinates.m_x == x && m_objects[i]->m_coordinates.m_y == y)
    {
      e = true;

      SpriteType t = m_objects[i]->m_type;
      switch(st)
      {
        case SpriteType::Boulder:
        {
          switch(t)
          {
            case SpriteType::Player:
            {
              // Destroy the boulder and the player.
              // Respawn the player at the spawn point if they have lives.
            }
            break;
            case SpriteType::Boulder:
            {
              // If there's space to the left and bottom left
              // or right and bottom right, the boulder should slide off.
            }
            break;
            case SpriteType::Coin:
            {
              // If there's space to the left and bottom left
              // or right and bottom right, the boulder should slide off.
            }
            break;
            default:
              break;
          }
        }
        break;
        case SpriteType::Coin:
        {
          switch(t)
          {
            case SpriteType::Coin:
            {
              // If there's space to the left and bottom left
              // or right and bottom right, the coin should slide off.
            }
            break;
            case SpriteType::Boulder:
            {
              // If there's space to the left and bottom left
              // or right and bottom right, the coin should slide off.
            }
            break;
            default:
              break;
          }
        }
        break;
        case SpriteType::Player:
        {
          switch(t)
          {
            case SpriteType::Coin:
            {
              s->m_coordinates.m_x = x;
              s->m_coordinates.m_y = y;

              m_coinsCollected += 1;

              delete m_objects[i];
              m_objects.erase(m_objects.begin() + i);
            }
            break;
            case SpriteType::Dirt:
            {
              s->m_coordinates.m_x = x;
              s->m_coordinates.m_y = y;

              delete m_objects[i];
              m_objects.erase(m_objects.begin() + i);
            }
            break;
            case SpriteType::Exit:
            {
              Exit* ex = reinterpret_cast<Exit*>(m_objects[i]);
              if(ex->m_triggerAnimation == true)
              {
                s->m_coordinates.m_x = x;
                s->m_coordinates.m_y = y;
              }
            }
            break;
            default:
              break;
          }
        }
        break;
        case SpriteType::Amoeba:
        {

        }
        break;
        case SpriteType::Butterfly:
        {

        }
        break;
        case SpriteType::Firefly:
        {

        }
        break;
        default:
          break;
      }
    }
  }

  // If nothing was present at the intended space
  // move the object into the empty space
  if(e != true)
  {
    s->m_coordinates.m_x = x;
    s->m_coordinates.m_y = y;
  }
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
          ProcessIntent(m_player, Direction::Up);
        if(m_keyboard[SDLK_a] == true)
          ProcessIntent(m_player, Direction::Left);
        if(m_keyboard[SDLK_s] == true)
          ProcessIntent(m_player, Direction::Down);
        if(m_keyboard[SDLK_d] == true)
          ProcessIntent(m_player, Direction::Right);

        m_timeInCave = m_cavetimer->GetTicks() / 1000 - 150;
        // if time in cave == 0, kill character, respawn him back where he
        // spawned and reset the cave time to the original allocated time for
        // this cave

        // Process AI here
        for(int i = 0; i < m_objects.size(); ++i)
        {
          int x = m_objects[i]->m_coordinates.m_x;
          int y = m_objects[i]->m_coordinates.m_y;

          switch(m_objects[i]->m_type)
          {
            // Should set a flag for the boulder if it dropped last tick
            // so we know whether to send it to the left/right or not
            case SpriteType::Boulder:
            {
              ProcessIntent(m_objects[i], Direction::Down);
            }
            break;
            // Should set a flag for the coin if it dropped last tick
            // so we know whether to send it to the left/right or not
            case SpriteType::Coin:
            {
              ProcessIntent(m_objects[i], Direction::Down);
            }
            break;
            default:
              break;
          }
        }
    }
    else
    {
      // Stop giving the player a seizure
      if(m_exit->m_triggerAnimation == true)
        m_exit->m_triggerAnimation = false;

      // Drain the coins collected and move them into the total score
      if(m_coinsCollected != 0)
      {
        m_coinsCollected--;
        m_totalScore++;
      }
      // Then drain the time remaining and move it into the total score
      if(m_timeInCave != 0 && m_coinsCollected == 0)
      {
          m_timeInCave++;
          m_totalScore++;
      }
      // The cave has ended, we need to load the next cave
      if(m_timeInCave == 0 && m_coinsCollected == 0)
      {
        // Perhaps create and render a loading screen and SDL_Delay for
        // a few seconds to give the player time to ready himself.
      }
    }

    if(m_coinsCollected == m_requiredCoins && m_isPlayerExit == false)
        m_exit->m_triggerAnimation = true;

    if(m_exit->m_coordinates.m_x == m_player->m_coordinates.m_x &&
       m_exit->m_coordinates.m_y == m_player->m_coordinates.m_y &&
       m_exit->m_triggerAnimation == true)
    {
        m_isPlayerExit = true;
    }
}

// Render the entire scene
void BoulderDash::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glViewport(50.0f, 50.0f, (float)m_window->GetWidth()-100.0f, (float)m_window->GetHeight()-100.0f);

    // Call the draw function for all objects currently in the cave
    for(int i = 0; i < m_objects.size(); ++i)
      m_objects[i]->Draw(m_cw, m_ch);
}

void BoulderDash::RenderText()
{
    glViewport(50.0f, 10.0f, (float)m_window->GetWidth()-100.0f, (float)m_window->GetHeight()-20.0f);
    glLoadIdentity();

    glTranslatef(90.0f, 0, 0);
    UtilityRenderText(m_requiredCoins, m_text.m_texID, m_cw, m_ch);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_coinsCollected, m_text.m_texID, m_cw, m_ch);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_timeInCave, m_text.m_texID, m_cw, m_ch);

    glTranslatef(50.0f, 0, 0);
    UtilityRenderText(m_totalScore, m_text.m_texID, m_cw, m_ch);
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
              LoadCave(CAVE_FOLDER + m_caves.front());
              m_cavetimer->Start();
              break;
          }
        }
        if(event.type == SDL_QUIT)
          return false;

        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            m_window->Resize(event.window.data1, event.window.data2);
        }

        return true;
      }
      else
      {
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
            m_window->Resize(event.window.data1, event.window.data2);
        }
      }
    }

    return true;
}

// Takes the name of the map file as input and creates game objects specified by the file
void BoulderDash::LoadCave(std::string c)
{
  std::ifstream cave(c);

  if(!cave.good())
    std::cerr << "Error opening map, ensure file exists" << std::endl;
  else
  {
    char tile;
    int x = 0;
    int y = 0;

    GLuint* texID = &m_sprites.m_texID;
    while(cave.get(tile))
    {
      switch(tile)
      {
        case 'p':
          m_objects.push_back(new Player(texID, SpriteType::Player, x, y));
          m_player = reinterpret_cast<Player*>(m_objects.back());
          x += m_cw;
          break;

        case 'd':
          m_objects.push_back(new Dirt(texID, SpriteType::Dirt, x, y));
          x += m_cw;
          break;

        case 'b':
          m_objects.push_back(new Boulder(texID, SpriteType::Boulder, x, y));
          x += m_cw;
          break;

        case 'w':
          m_objects.push_back(new Wall(texID, SpriteType::Wall, x, y));
          x += m_cw;
          break;

        case 'c':
          m_objects.push_back(new Coin(texID, SpriteType::Coin, x, y));
          x += m_cw;
          break;

        case 'e':
          m_objects.push_back(new Exit(texID, SpriteType::Exit, x, y));
          m_exit = reinterpret_cast<Exit*>(m_objects.back());
          x += m_cw;
          break;

        case 'i':
          m_objects.push_back(new Impassable(texID, SpriteType::Impassable, x, y));
          x += m_cw;
          break;

        case 'n':
          x += m_cw;
          break;

        case 'a':
          m_objects.push_back(new Amoeba(texID, SpriteType::Amoeba, x, y));
          x += m_cw;
          break;

        case 'm':
          m_objects.push_back(new MagicWall(texID, SpriteType::Magicwall, x, y));
          x += m_cw;
          break;

        case 's':
          m_objects.push_back(new Butterfly(texID, SpriteType::Butterfly, x, y));
          x += m_cw;
          break;

        case 'f':
          m_objects.push_back(new Firefly(texID, SpriteType::Firefly, x, y));
          x += m_cw;
          break;

        default:
          break;
      }

      if(x >= (int)ORTHO_WIDTH)
      {
        x = 0;
        y += m_ch;
      }
    }
  }

  cave.close();
}

void BoulderDash::CloseMap()
{
    // Erase all game objects
    for(int i = 0; i < m_objects.size(); ++i)
      delete m_objects[i];
}

void BoulderDash::RenderMainMenu()
{
  float w = static_cast<float>(m_window->GetWidth());
  float h = static_cast<float>(m_window->GetHeight());

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0.0f, 0.0f, w, h);

  glLoadIdentity();
  glTranslatef((ORTHO_WIDTH / 2) - (m_cw * 6), (ORTHO_HEIGHT / 1.75) - (m_ch / 2), 0.0f);
  UtilityRenderText("press return", m_text.m_texID, m_cw, m_ch);

  glLoadIdentity();
  glTranslatef((ORTHO_WIDTH / 2) - (m_cw * 4), (ORTHO_HEIGHT / 1.75) - (m_ch / 2) + m_ch, 0.0f);
  UtilityRenderText("to begin", m_text.m_texID, m_cw, m_ch);

  glLoadIdentity();
  glTranslatef((ORTHO_WIDTH / 2) - (m_cw * 3), (ORTHO_HEIGHT / 5), 0.0f);
  glColor3f(1.0f,1.0f,1.0f);

  glBindTexture(GL_TEXTURE_2D, m_sprites.m_texID);
  glBegin(GL_QUADS);
      glTexCoord2f(1.0f/8*0, 1.0f/12*0);
      glVertex2f(0, 0);
      glTexCoord2f(1.0f/8*1, 1.0f/12*0);
      glVertex2f(m_cw * 6, 0);
      glTexCoord2f(1.0f/8*1, 1.0f/12*1);
      glVertex2f(m_cw * 6, m_ch * 6);
      glTexCoord2f(1.0f/8*0, 1.0f/12*1);
      glVertex2f(0, m_ch * 6);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, NULL);
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

        SDL_GL_SwapWindow(m_window->GetHandle());
      }
      else
      {
        m_gametimer->Start();

        // Then there is no user event to handle because he's quit...
        if(HandleInput(event) == false)
            m_quit = true;

        Update();

        Render();

        RenderText();

        SDL_GL_SwapWindow(m_window->GetHandle());

        if(m_gametimer->GetTicks() < TICKS_PER_FRAME)
            SDL_Delay(TICKS_PER_FRAME - m_gametimer->GetTicks());
      }
    }
}

void BoulderDash::LoadPlaylist()
{
  m_caves.clear();

  std::string name;
  std::ifstream cplaylist(ROOT_FOLDER + "playlist.txt");
  if(!cplaylist.good())
  {
    std::cerr << "Error determining which playlist to select. Ensure \
    you've chosen a playlist in 'playlist.txt'" << std::endl;
  }
  else
  {
    std::getline(cplaylist, name);
  }

  std::ifstream playlist(PLAYLIST_FOLDER + name);
  if(!playlist.good())
  {
    std::cerr << "Error opening playlist: " << name << ". " << "Ensure \
    that file exists in: " << PLAYLIST_FOLDER << "." << std::endl;
  }
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
