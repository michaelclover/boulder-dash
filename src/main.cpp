#include "main.h"

BoulderDash::BoulderDash(std::string title, int width, int height)
{
    m_coinsCollected = 0;
    m_totalScore = 0;
    m_requiredCoins = 12;
    m_timeInCave = 150;
    m_isMapOpen = true;
    m_isPlayerExit = false;
    m_currentcave = 0;
    m_lives = 0;

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

bool BoulderDash::IsSolid(Sprite* s)
{
  return (s->m_type == SpriteType::Wall || s->m_type == SpriteType::Boulder ||
          s->m_type == SpriteType::Coin || s->m_type == SpriteType::Exit ||
          s->m_type == SpriteType::Impassable || s->m_type == SpriteType::Amoeba ||
          s->m_type == SpriteType::Dirt);
}

void BoulderDash::RemoveGameObject(Sprite* s)
{
  for(int i = 0; i < m_objects.size(); ++i)
  {
    if(m_objects[i] == s)
    {
      delete m_objects[i];
      m_objects.erase(m_objects.begin() + i);
    }
  }
}

void BoulderDash::ProcessIntent(Boulder* o, Direction d)
{
  Sprite* s;
  int x, y;
  switch(d)
  {
    case Direction::Down:
    {
      s = GetTile(o->m_coordinates.m_x, o->m_coordinates.m_y + m_ch);

      x = o->m_coordinates.m_x;
      y = o->m_coordinates.m_y + m_ch;
    }
    break;

    case Direction::Up:
    {
      s = GetTile(o->m_coordinates.m_x, o->m_coordinates.m_y - m_ch);

      x = o->m_coordinates.m_x;
      y = o->m_coordinates.m_y - m_ch;
    }
    break;

    case Direction::Left:
    {
      s = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y);

      x = o->m_coordinates.m_x - m_cw;
      y = o->m_coordinates.m_y;
    }
    break;

    case Direction::Right:
    {
      s = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y);

      x = o->m_coordinates.m_x + m_cw;
      y = o->m_coordinates.m_y;
    }
    break;
  }

  if(s == nullptr)
  {
    o->m_coordinates.m_x = x;
    o->m_coordinates.m_y = y;
  }
  else
  {
    if(d == Direction::Down && o->m_velocity == true)
    {
      if(s->m_type == SpriteType::Boulder || s->m_type == SpriteType::Coin)
      {
        // Perform slide checks
        Sprite* left = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y);
        Sprite* bottomleft = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y + m_ch);
        if(left == nullptr)
        {
          if(bottomleft == nullptr || !IsSolid(bottomleft))
          {
            o->m_intent.push_back(Direction::Left);
          }
        }

        Sprite* right = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y);
        Sprite* bottomright = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y + m_ch);
        if(right == nullptr)
        {
          if(bottomright == nullptr || !IsSolid(bottomright))
          {
            o->m_intent.push_back(Direction::Right);
          }
        }
      }
      else
      {
        // It's landed on something squidgy, let's check what it is in order
        // to see what needs to be done
        switch(s->m_type)
        {
          case SpriteType::Player:
          {
            // Destroy player and boulder, trigger explosion animation,
            // Respawn the player back at the start if he has lives remaining
          }
          break;
        }
      }
    }
  }

  // Set the velocity depending on whether the boulder moved this tick
  if(o->m_coordinates.m_x == x && o->m_coordinates.m_y == y)
    o->m_velocity = true;
  else
    o->m_velocity = false;
}

void BoulderDash::ProcessIntent(Coin* o, Direction d)
{
  Sprite* s;
  int x, y;
  switch(d)
  {
    case Direction::Down:
    {
      s = GetTile(o->m_coordinates.m_x, o->m_coordinates.m_y + m_ch);

      x = o->m_coordinates.m_x;
      y = o->m_coordinates.m_y + m_ch;
    }
    break;

    case Direction::Up:
    {
      s = GetTile(o->m_coordinates.m_x, o->m_coordinates.m_y - m_ch);

      x = o->m_coordinates.m_x;
      y = o->m_coordinates.m_y - m_ch;
    }
    break;

    case Direction::Left:
    {
      s = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y);

      x = o->m_coordinates.m_x - m_cw;
      y = o->m_coordinates.m_y;
    }
    break;

    case Direction::Right:
    {
      s = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y);

      x = o->m_coordinates.m_x + m_cw;
      y = o->m_coordinates.m_y;
    }
    break;
  }

  if(s == nullptr)
  {
    o->m_coordinates.m_x = x;
    o->m_coordinates.m_y = y;
  }
  else
  {
    if(d == Direction::Down && o->m_velocity == true)
    {
      if(s->m_type == SpriteType::Boulder || s->m_type == SpriteType::Coin)
      {
        // Perform slide checks
        Sprite* left = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y);
        Sprite* bottomleft = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y + m_ch);
        if(left == nullptr)
        {
          if(bottomleft == nullptr || !IsSolid(bottomleft))
          {
            o->m_intent.push_back(Direction::Left);
          }
        }

        Sprite* right = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y);
        Sprite* bottomright = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y + m_ch);
        if(right == nullptr)
        {
          if(bottomright == nullptr || !IsSolid(bottomright))
          {
            o->m_intent.push_back(Direction::Right);
          }
        }
      }
      else
      {
        // It's landed on something squidgy, let's check what it is in order
        // to see what needs to be done
      }
    }
  }

  // Set the velocity depending on whether the coin moved this tick
  if(o->m_coordinates.m_x == x && o->m_coordinates.m_y == y)
    o->m_velocity = true;
  else
    o->m_velocity = false;
}

void BoulderDash::ProcessIntent(Player* o, Direction d)
{
  Sprite* s;
  int x, y;
  switch(d)
  {
    case Direction::Down:
    {
      s = GetTile(o->m_coordinates.m_x, o->m_coordinates.m_y + m_ch);

      x = o->m_coordinates.m_x;
      y = o->m_coordinates.m_y + m_ch;
    }
    break;

    case Direction::Up:
    {
      s = GetTile(o->m_coordinates.m_x, o->m_coordinates.m_y - m_ch);

      x = o->m_coordinates.m_x;
      y = o->m_coordinates.m_y - m_ch;
    }
    break;

    case Direction::Left:
    {
      s = GetTile(o->m_coordinates.m_x - m_cw, o->m_coordinates.m_y);

      x = o->m_coordinates.m_x - m_cw;
      y = o->m_coordinates.m_y;
    }
    break;

    case Direction::Right:
    {
      s = GetTile(o->m_coordinates.m_x + m_cw, o->m_coordinates.m_y);

      x = o->m_coordinates.m_x + m_cw;
      y = o->m_coordinates.m_y;
    }
    break;
  }

  if(s == nullptr)
  {
    o->m_coordinates.m_x = x;
    o->m_coordinates.m_y = y;
  }
  else
  {
    switch(s->m_type)
    {
      case SpriteType::Coin:
      {
        o->m_coordinates.m_x = x;
        o->m_coordinates.m_y = y;

        m_coinsCollected++;
        RemoveGameObject(s);
      }
      break;

      case SpriteType::Dirt:
      {
        o->m_coordinates.m_x = x;
        o->m_coordinates.m_y = y;

        RemoveGameObject(s);
      }
      break;

      case SpriteType::Exit:
      {
        Exit* e = reinterpret_cast<Exit*>(s);
        if(e->m_triggerAnimation == true)
        {
          o->m_coordinates.m_x = x;
          o->m_coordinates.m_y = y;
        }
      }
      break;

      case SpriteType::Boulder:
      {
        switch(d)
        {
          case Direction::Left:
          {
            if(GetTile(s->m_coordinates.m_x - m_cw, s->m_coordinates.m_y) == nullptr)
            {
              s->m_coordinates.m_x -= m_cw;
              o->m_coordinates.m_x -= m_cw;
            }
          }
          break;

          case Direction::Right:
          {
            if(GetTile(s->m_coordinates.m_x + m_cw, s->m_coordinates.m_y) == nullptr)
            {
              s->m_coordinates.m_x += m_cw;
              o->m_coordinates.m_x += m_cw;
            }
          }
          break;
        }
      }
    }
  }
}

void BoulderDash::ProcessIntent(Amoeba* o, Direction d)
{

}

void BoulderDash::ProcessIntent(Butterfly* o, Direction d)
{

}

void BoulderDash::ProcessIntent(Firefly* o, Direction d)
{

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

        if(m_timeInCave != 0)
        {
          m_timeInCave = m_cavetimer->GetTicks() / 1000 - m_caves[m_currentcave - 1].time;
        }
        else if(m_timeInCave == 0)
        {
          // if time in cave == 0, kill character, respawn him back where he
          // spawned and reset the cave time to the original allocated time for
          // this cave
        }

        // Process AI here
        for(int i = 0; i < m_objects.size(); ++i)
        {
          switch(m_objects[i]->m_type)
          {
            case SpriteType::Boulder:
            {
              Boulder* o = reinterpret_cast<Boulder*>(m_objects[i]);
              if(!o->m_intent.empty())
              {
                Direction d = o->m_intent[0];
                ProcessIntent(o, d);
                o->m_intent.clear();
              }
              else
              {
                ProcessIntent(o, Direction::Down);
              }
            }
            break;

            case SpriteType::Coin:
            {
              Coin* o = reinterpret_cast<Coin*>(m_objects[i]);
              if(!o->m_intent.empty())
              {
                Direction d = o->m_intent[0];
                ProcessIntent(o, d);
                o->m_intent.clear();
              }
              else
              {
                ProcessIntent(o, Direction::Down);
              }
            }
            break;
            default:
              break;
          }
        }
    }
    else
    {
      // Stop the exit animation; the player is at the exit
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
        LoadNextCave();
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
    glViewport(0.0f, 0.0f, (float)m_window->GetWidth(), (float)m_window->GetHeight());
    glLoadIdentity();

    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, m_sprites.m_texID);

    for(int i = 0; i < m_lives; i++)
    {
      glBegin(GL_QUADS);
          glTexCoord2f(1.0f/8*0, 1.0f/12*0);
          glVertex2f(0, 0);
          glTexCoord2f(1.0f/8*1, 1.0f/12*0);
          glVertex2f(m_cw, 0);
          glTexCoord2f(1.0f/8*1, 1.0f/12*1);
          glVertex2f(m_cw, m_ch);
          glTexCoord2f(1.0f/8*0, 1.0f/12*1);
          glVertex2f(0, m_ch);
      glEnd();
      glTranslatef(m_cw, 0, 0);
    }

    glBindTexture(GL_TEXTURE_2D, NULL);

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
            {
              LoadPlaylist();
              LoadNextCave();
              m_mainMenu = false;
            }
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
                    case SDLK_w: m_keyboard[SDLK_w] = true; m_player->m_direction = 4; break;
                    case SDLK_a: m_keyboard[SDLK_a] = true; m_player->m_direction = 4; break;
                    case SDLK_s: m_keyboard[SDLK_s] = true; m_player->m_direction = 5; break;
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

void BoulderDash::LoadNextCave()
{
  // Clear the previous cave, reset the cave timer
  for(int i = 0; i < m_objects.size(); ++i)
  {
    delete m_objects[i];
  }
  m_objects.clear();

  m_coinsCollected = 0;
  m_isMapOpen = true;
  m_isPlayerExit = false;

  std::ifstream cave;
  if(m_currentcave + 1 <= m_caves.size())
  {
    cave.open(CAVE_FOLDER + m_caves[m_currentcave].name);
    m_requiredCoins = m_caves[m_currentcave].coins;
    m_timeInCave = m_caves[m_currentcave].time;
    m_currentcave++;
  }
  else
  {
    // No more caves to load; perhaps render an end of game screen and then
    // move back to the title screen
    m_mainMenu = true;
    m_totalScore = 0;
    m_currentcave = 0;
    return;
  }

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

  m_cavetimer->Stop();
  m_cavetimer->Start();

  cave.close();
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

Sprite* BoulderDash::GetTile(int x, int y)
{
  std::vector<Sprite*>::iterator it = std::find_if(m_objects.begin(), m_objects.end(),
  [x, y] (const Sprite* s) -> bool { return s->m_coordinates.m_x == x && s->m_coordinates.m_y == y;});
  if(it == m_objects.end())
    return nullptr;
  else
    return *it;
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

  std::ifstream ifs(ROOT_FOLDER + "play.json");
  rapidjson::IStreamWrapper isw(ifs);

  rapidjson::Document d;
  d.ParseStream(isw);

  rapidjson::Value& lives = d["lives"];
  m_lives = lives.GetInt();

  rapidjson::Value& caves = d["caves"];
  for(auto itr = caves.Begin(); itr != caves.End(); ++itr)
  {
    Cave c;

    const rapidjson::Value& cave_name = (*itr)["cave_name"];
    const rapidjson::Value& alloc_time = (*itr)["allocated_time"];
    const rapidjson::Value& diamonds_req = (*itr)["diamonds_req"];

    c.name = cave_name.GetString();
    c.time = alloc_time.GetInt();
    c.coins = diamonds_req.GetInt();

    m_caves.push_back(c);
  }
}

// Program executes here
int main(int argc, char** argv)
{
    BoulderDash game("Boulder Dash", 640, 480);
    game.MainLoop();

    return 0;
}
