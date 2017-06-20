#include "game.h"

Game::Game(const std::string title, const int width, const int height)
{
  explode = false;
  diamonds_collected = 0;
  total_score = 0;
  player_at_exit = false;
  caves_index = 0;
  lives = 0; // will be loaded from play.json

  cw = ORTHO_WIDTH / 40;
  ch = ORTHO_HEIGHT / 22;

  window = new Window(title, width, height);

  cave_timer = new Timer();
  game_timer = new Timer();

  if(window->Initialise() != 0)
  {
      std::cerr << "Error initializing SDL, Please ensure you are using SDL2" << std::endl;
      quit = true;
  }

  InitializeGL(window->GetWidth(), window->GetHeight());
  InitializeDevIL();

  if(sprites.LoadTextureFromFile(IMG_FOLDER + "spritesheet.png") != true)
  {
      std::cerr << "Error loading sprite textures" << std::endl;
      quit = true;
  }

  if(font.LoadTextureFromFile(IMG_FOLDER + "textsheet.png") != true)
  {
      std::cerr << "Error loading text sheet" << std::endl;
      quit = true;
  }
}

Game::~Game()
{
  delete window;
  delete game_timer;
  delete cave_timer;

  for(int i = 0; i < game_objects.size(); ++i)
    delete game_objects[i];
}

void Game::MainLoop()
{
  SDL_Event event;

  while(quit != true)
  {
    if(at_menu == true)
    {
      if(HandleInput(event) == false)
        quit = true;

      RenderMainMenu();

      SDL_GL_SwapWindow(window->GetHandle());
    }
    else
    {
      game_timer->Start();

      if(HandleInput(event) == false)
          quit = true;

      Update();
      Render();
      RenderText();

      SDL_GL_SwapWindow(window->GetHandle());

      if(game_timer->GetTicks() < TICKS_PER_FRAME)
          SDL_Delay(TICKS_PER_FRAME - game_timer->GetTicks());
    }
  }
}

void Game::Update()
{
  // Prevent diagonal movement
  if(keys[SDLK_w] == true && keys[SDLK_a] == true)
  {
      keys[SDLK_w] = false;
      keys[SDLK_a] = false;
  }
  if(keys[SDLK_w] == true && keys[SDLK_d] == true)
  {
      keys[SDLK_w] = false;
      keys[SDLK_d] = false;
  }
  if(keys[SDLK_s] == true && keys[SDLK_a] == true)
  {
      keys[SDLK_s] = false;
      keys[SDLK_a] = false;
  }
  if(keys[SDLK_s] == true && keys[SDLK_d] == true)
  {
      keys[SDLK_s] = false;
      keys[SDLK_d] = false;
  }
  // End preventing diagonal movement

  if(player_at_exit != true)
  {
      // Process keyboard events
      if(keys[SDLK_w] == true && rockford != nullptr)
      {
        // move rockford up
        Sprite* s = GetAbove(rockford);
        if(s == nullptr)
        {
          rockford->m_coordinates.m_y -= ch; // perform movement
        }
        else if(s->m_type == SpriteType::Dirt)
        {
          rockford->m_coordinates.m_y -= ch; // perform movement
          DestroyObject(s); // if it exists, destroy it
        }
        else if(s->m_type == SpriteType::Coin)
        {
          rockford->m_coordinates.m_y -= ch; // perform movement
          diamonds_collected += 1;
          DestroyObject(s);
        }
        else if(s->m_type == SpriteType::Exit && diamonds_collected >= caves[caves_index - 1].coins)
        {
          rockford->m_coordinates.m_y -= ch;
          player_at_exit = true;
        }
      }
      if(keys[SDLK_a] == true && rockford != nullptr)
      {
        // move rockford left
        Sprite* s = GetLeft(rockford);
        if(s == nullptr)
        {
          rockford->m_coordinates.m_x -= cw; // perform movement
        }
        else if(s->m_type == SpriteType::Dirt)
        {
          rockford->m_coordinates.m_x -= cw; // perform movement
          DestroyObject(s); // if it exists, destroy it
        }
        else if(s->m_type == SpriteType::Coin)
        {
          rockford->m_coordinates.m_x -= cw; // perform movement
          diamonds_collected += 1;
          DestroyObject(s);
        }
        else if(s->m_type == SpriteType::Boulder)
        {
          Sprite* sr = GetLeft(s); // get sprite at left side of boulder if there is one
          if(sr == nullptr) // nothing's there - we can try to push it
          {

          }
        }
        else if(s->m_type == SpriteType::Exit && diamonds_collected >= caves[caves_index - 1].coins)
        {
          rockford->m_coordinates.m_x -= cw; // perform movement
          player_at_exit = true;
        }
      }
      if(keys[SDLK_s] == true && rockford != nullptr)
      {
        // move rockford down
        Sprite* s = GetBelow(rockford);
        if(s == nullptr)
        {
          rockford->m_coordinates.m_y += ch; // perform movement
        }
        else if(s->m_type == SpriteType::Dirt)
        {
          rockford->m_coordinates.m_y += ch; // perform movement
          DestroyObject(s); // if it exists, destroy it
        }
        else if(s->m_type == SpriteType::Coin)
        {
          rockford->m_coordinates.m_y += ch; // perform movement
          diamonds_collected += 1;
          DestroyObject(s);
        }
        else if(s->m_type == SpriteType::Exit && diamonds_collected >= caves[caves_index - 1].coins)
        {
          rockford->m_coordinates.m_y += ch; // perform movement
          player_at_exit = true;
        }
      }
      if(keys[SDLK_d] == true && rockford != nullptr)
      {
        // move rockford right
        Sprite* s = GetRight(rockford);
        if(s == nullptr)
        {
          rockford->m_coordinates.m_x += cw; // perform movement
        }
        else if(s->m_type == SpriteType::Dirt)
        {
          rockford->m_coordinates.m_x += cw; // perform movement
          DestroyObject(s); // if it exists, destroy it
        }
        else if(s->m_type == SpriteType::Coin)
        {
          rockford->m_coordinates.m_x += cw; // perform movement
          diamonds_collected += 1;
          DestroyObject(s);
        }
        else if(s->m_type == SpriteType::Boulder)
        {
          Sprite* sr = GetRight(s); // get sprite at right side of boulder if there is one
          if(sr == nullptr) // nothing's there - we can try to push it
          {

          }
        }
        else if(s->m_type == SpriteType::Exit && diamonds_collected >= caves[caves_index - 1].coins)
        {
          rockford->m_coordinates.m_x += cw; // perform movement
          player_at_exit = true;
        }
      }

      if(!explosions.empty())
      {
        for(int i = 0; i < explosions.size(); ++i)
        {
          if(explosions[i]->m_done)
            DestroyExplosion(explosions[i]);
        }
      }

      if(explode && explosions.empty())
      {
        LoseLife();
        return;
      }

      if(cave_time != 0)
      {
        cave_time = cave_timer->GetTicks() / 1000 - caves[caves_index - 1].time; // caves_index - 1 refers to the current cave
      }
      else if(cave_time == 0)
      {
        explode = true; // handle player death and respawn
      }

      // Process all game objects
      for(auto& i : game_objects)
      {
        switch(i->m_type)
        {
          case SpriteType::Boulder:
          {
            Sprite* s = GetBelow(i); // get sprite below
            Coordinates c = i->m_coordinates; // keep coordinates before moving
            if(s == nullptr && !i->m_velocity) // nothing's there, skip frame
            {
              i->m_velocity = true;
              //i->m_coordinates.m_y += ch; // move boulder down
            }
            else if(s == nullptr && i->m_velocity)
            {
              i->m_coordinates.m_y += ch;
            }
            else if(s->m_type == SpriteType::Player) // player is below
            {
              if(i->m_velocity == true) // player is below and boulder has velocity
              {
                explode = true; // handle player death and respawn
                break;
              }
            }
            else // check to see if below object is rounded and bottom left or right is empty
            {
              i->m_velocity = false;
              if(s->m_rounded == true) // below object is rounded
              {
                Sprite* left = GetLeft(i); // get sprite to left of boulder
                if(left == nullptr) // nothing's to the left
                {
                  Sprite* below = GetBottomLeft(i); // get sprite in bottom left
                  if(below == nullptr) // nothing's in bottom left
                  {
                    i->m_velocity = true;
                    i->m_coordinates.m_x -= cw; // move boulder left
                  }
                }
                else
                {
                  Sprite* right = GetRight(i); // get sprite to right of boulder
                  if(right == nullptr) // nothing's to the right
                  {
                    Sprite* below = GetBottomRight(i); // get sprite in bottom right
                    if(below == nullptr) // nothing's in bottom right
                    {
                      i->m_velocity = true;
                      i->m_coordinates.m_x += cw; // move boulder right
                    }
                  }
                }
              }
            }
          }
          break;

          case SpriteType::Coin:
          {
            Sprite* s = GetBelow(i); // get sprite below
            Coordinates c = i->m_coordinates; // keep coordinates before moving
            if(s == nullptr && !i->m_velocity) // nothing's there, skip frame
            {
              i->m_velocity = true;
              //i->m_coordinates.m_y += ch; // move boulder down
            }
            else if(s == nullptr && i->m_velocity)
            {
              i->m_coordinates.m_y += ch; // move boulder down
            }
            else if(s->m_type == SpriteType::Player) // player is below
            {
              if(i->m_velocity == true) // player is below and boulder has velocity
              {
                explode = true; // handle player death and respawn
              }
            }
            else // check to see if below object is rounded and bottom left or right is empty
            {
              i->m_velocity = false;
              if(s->m_rounded == true) // below object is rounded
              {
                Sprite* left = GetLeft(i); // get sprite to left of boulder
                if(left == nullptr) // nothing's to the left
                {
                  Sprite* below = GetBottomLeft(i); // get sprite in bottom left
                  if(below == nullptr) // nothing's in bottom left
                  {
                    i->m_velocity = true;
                    i->m_coordinates.m_x -= cw; // move boulder left
                  }
                }
                else
                {
                  Sprite* right = GetRight(i); // get sprite to right of boulder
                  if(right == nullptr) // nothing's to the right
                  {
                    Sprite* below = GetBottomRight(i); // get sprite in bottom right
                    if(below == nullptr) // nothing's in bottom right
                    {
                      i->m_velocity = true;
                      i->m_coordinates.m_x += cw; // move boulder right
                    }
                  }
                }
              }
            }
          }
          break;

          case SpriteType::Amoeba:
          {

          }
          break;

          case SpriteType::Butterfly:
          {
            Butterfly* bf = (Butterfly*)i;

            // check if we're next to amoeba or rockford for exploding

            Direction d = bf->m_direction;
            if(d == Direction::Left) // butterfly is facing left
            {
              Sprite* above = GetAbove(bf); // get sprite above
              if(above == nullptr)
              {
                bf->m_direction = Direction::Up;
                bf->m_coordinates.m_y -= ch;
              }
              else if(GetLeft(bf) == nullptr)
              {
                bf->m_coordinates.m_x -= cw;
              }
              else
              {
                bf->m_direction = Direction::Down;
              }
            }
            else if(d == Direction::Right)
            {
              Sprite* below = GetBelow(bf);
              if(below == nullptr)
              {
                bf->m_direction = Direction::Down;
                bf->m_coordinates.m_y += ch;
              }
              else if(GetRight(bf) == nullptr)
              {
                bf->m_coordinates.m_x += cw;
              }
              else
              {
                bf->m_direction = Direction::Up;
              }
            }
            else if(d == Direction::Up)
            {
              Sprite* right = GetRight(bf);
              if(right == nullptr)
              {
                bf->m_direction = Direction::Right;
                bf->m_coordinates.m_x += cw;
              }
              else if(GetAbove(bf) == nullptr)
              {
                bf->m_coordinates.m_y -= ch;
              }
              else
              {
                bf->m_direction = Direction::Left;
              }
            }
            else if(d == Direction::Down)
            {
              Sprite* left = GetLeft(bf);
              if(left == nullptr)
              {
                bf->m_direction = Direction::Left;
                bf->m_coordinates.m_x -= cw;
              }
              else if(GetBelow(bf) == nullptr)
              {
                bf->m_coordinates.m_y += ch;
              }
              else
              {
                bf->m_direction = Direction::Right;
              }
            }
          }
          break;

          case SpriteType::Firefly:
          {
            Firefly* ff = (Firefly*)i;

            // check if we're next to amoeba or rockford for exploding

            Sprite* a = GetAbove(ff);
            Sprite* r = GetRight(ff);
            Sprite* l = GetLeft(ff);
            Sprite* b = GetBelow(ff);
            bool player = false;

            // check whether we should explode
            if(a != nullptr)
            {
              if(a->m_type == SpriteType::Player)
              {
                player = true;
                explode = true;
              }
            }
            if(r != nullptr)
            {
              if(r->m_type == SpriteType::Player)
              {
                player = true;
                explode = true;
              }
            }
            if(l != nullptr)
            {
              if(l->m_type == SpriteType::Player)
              {
                player = true;
                explode = true;
              }
            }
            if(b != nullptr)
            {
              if(b->m_type == SpriteType::Player)
              {
                player = true;
                explode = true;
              }
            }

            if(explode && player) // we should explode
            {
              std::pair<std::vector<Sprite*>, std::vector<Coordinates>> spri = GetSurrounding(ff);
              for(auto& it : spri.first) // destroy sprite objects and replace with explosion
              {
                  Explosion* e = new Explosion(&sprites.m_texID, SpriteType::Explosion, it->m_coordinates.m_x, it->m_coordinates.m_y);
                  explosions.push_back(e);
                  game_objects.push_back(e);
                  DestroyObject(it);
              }
              for(auto& its : spri.second) // create explosion in empty tiles
              {
                Explosion* e = new Explosion(&sprites.m_texID, SpriteType::Explosion, its.m_x, its.m_y);
                explosions.push_back(e);
                game_objects.push_back(e);
              }

              Explosion* e = new Explosion(&sprites.m_texID, SpriteType::Explosion, ff->m_coordinates.m_x, ff->m_coordinates.m_y);
              explosions.push_back(e);
              game_objects.push_back(e);
              DestroyObject(ff);
              break; // we're done with the firefly
            }

            Direction d = ff->m_direction;
            if(d == Direction::Left) // firefly is facing left
            {
              Sprite* below = GetBelow(ff); // get sprite below
              if(below == nullptr) // nothing's below
              {
                ff->m_direction = Direction::Down; // firefly is now facing down
                ff->m_coordinates.m_y += ch; // move down
              }
              else if(GetLeft(ff) == nullptr) // nothing's left
              {
                ff->m_coordinates.m_x -= cw;
              }
              else // turn right
              {
                ff->m_direction = Direction::Up;
              }
            }
            else if(d == Direction::Right) // firefly is facing right
            {
              Sprite* above = GetAbove(ff); // get sprite above
              if(above == nullptr)
              {
                ff->m_direction = Direction::Up; // firefly is now facing up
                ff->m_coordinates.m_y -= ch; // move up
              }
              else if(GetRight(ff) == nullptr) // nothing's right
              {
                ff->m_coordinates.m_x += cw;
              }
              else // turn right
              {
                ff->m_direction = Direction::Down; // firefly is now facing down
              }
            }
            else if(d == Direction::Up) // firefly is facing up
            {
              Sprite* left = GetLeft(ff); // get sprite to the left
              if(left == nullptr)
              {
                ff->m_direction = Direction::Left; // firefly facing left
                ff->m_coordinates.m_x -= cw; // move left
              }
              else if(GetAbove(ff) == nullptr) // nothing's above
              {
                ff->m_coordinates.m_y -= ch; // move up
              }
              else // turn right
              {
                ff->m_direction = Direction::Right; // firefly is now facing right
              }
            }
            else if(d == Direction::Down) // firefly is facing down
            {
              Sprite* right = GetRight(ff); // get sprite to the right
              if(right == nullptr) // nothing's to the right
              {
                ff->m_direction = Direction::Right; // firefly facing right;
                ff->m_coordinates.m_x += cw; // move right
              }
              else if(GetBelow(ff) == nullptr) // nothing's below
              {
                ff->m_coordinates.m_y += ch; // move down
              }
              else // turn right
              {
                ff->m_direction = Direction::Left; // firefly is now facing left
              }
            }
          }
          break;

          case SpriteType::Wall: // in case magic walls are enabled for this cave
          {

          }
          break;
        }
      }
  }
  else
  {
    // Stop the exit animation; the rockford is at the exit
    if(exit->m_triggerAnimation == true)
      exit->m_triggerAnimation = false;

    // Drain the coins collected and move them into the total score
    if(diamonds_collected != 0)
    {
      diamonds_collected--;
      total_score++;
    }
    // Then drain the time remaining and move it into the total score
    if(cave_time != 0 && diamonds_collected == 0)
    {
        cave_time++;
        total_score++;
    }
    // The cave has ended, we need to load the next cave
    if(cave_time == 0 && diamonds_collected == 0)
    {
      // Perhaps create and render a loading screen and SDL_Delay for
      // a few seconds to give the rockford time to ready himself.
      LoadNextCave();
    }
  }

  if(diamonds_collected == caves[caves_index - 1].coins && player_at_exit == false)
      exit->m_triggerAnimation = true;

  if(exit->m_coordinates.m_x == rockford->m_coordinates.m_x &&
     exit->m_coordinates.m_y == rockford->m_coordinates.m_y &&
     exit->m_triggerAnimation == true)
  {
      player_at_exit = true;
  }
}

void Game::Render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glViewport(50.0f, 50.0f, (float)window->GetWidth()-100.0f, (float)window->GetHeight()-100.0f);

  // Call the draw function for all objects currently in the cave
  for(int i = 0; i < game_objects.size(); ++i)
    game_objects[i]->Draw(cw, ch);
}

void Game::RenderText()
{
  glViewport(0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight());
  glLoadIdentity();

  glColor3f(1.0f,1.0f,1.0f);
  glBindTexture(GL_TEXTURE_2D, sprites.m_texID);

  for(int i = 0; i < lives; i++) // render how many lives we have left graphically
  {
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f/8*0, 1.0f/12*0);
        glVertex2f(0, 0);
        glTexCoord2f(1.0f/8*1, 1.0f/12*0);
        glVertex2f(cw, 0);
        glTexCoord2f(1.0f/8*1, 1.0f/12*1);
        glVertex2f(cw, ch);
        glTexCoord2f(1.0f/8*0, 1.0f/12*1);
        glVertex2f(0, ch);
    glEnd();
    glTranslatef(cw, 0, 0);
  }

  //glBindTexture(GL_TEXTURE_2D, NULL);

  glViewport(50.0f, 10.0f, (float)window->GetWidth()-100.0f, (float)window->GetHeight()-20.0f);
  glLoadIdentity();

  glTranslatef(90.0f, 0, 0);
  UtilityRenderText(caves[caves_index - 1].coins, font.m_texID, cw, ch); // render the diamonds required to leave the cave

  glTranslatef(50.0f, 0, 0);
  UtilityRenderText(diamonds_collected, font.m_texID, cw, ch); // render the diamonds collected in the current cave

  glTranslatef(50.0f, 0, 0);
  UtilityRenderText(cave_time, font.m_texID, cw, ch); // render the cave time - the time we have to complete the cave

  glTranslatef(50.0f, 0, 0);
  UtilityRenderText(total_score, font.m_texID, cw, ch); // render the total score throughout the playlist
}

void Game::RenderMainMenu()
{
  float w = static_cast<float>(window->GetWidth());
  float h = static_cast<float>(window->GetHeight());

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0.0f, 0.0f, w, h);

  glLoadIdentity();
  glTranslatef((ORTHO_WIDTH / 2) - (cw * 6), (ORTHO_HEIGHT / 1.75) - (ch / 2), 0.0f);
  UtilityRenderText("press return", font.m_texID, cw, ch);

  glLoadIdentity();
  glTranslatef((ORTHO_WIDTH / 2) - (cw * 4), (ORTHO_HEIGHT / 1.75) - (ch / 2) + ch, 0.0f);
  UtilityRenderText("to begin", font.m_texID, cw, ch);

  glLoadIdentity();
  glTranslatef((ORTHO_WIDTH / 2) - (cw * 3), (ORTHO_HEIGHT / 5), 0.0f);
  glColor3f(1.0f,1.0f,1.0f);

  glBindTexture(GL_TEXTURE_2D, sprites.m_texID);
  glBegin(GL_QUADS);
      glTexCoord2f(1.0f/8*0, 1.0f/12*0);
      glVertex2f(0, 0);
      glTexCoord2f(1.0f/8*1, 1.0f/12*0);
      glVertex2f(cw * 6, 0);
      glTexCoord2f(1.0f/8*1, 1.0f/12*1);
      glVertex2f(cw * 6, ch * 6);
      glTexCoord2f(1.0f/8*0, 1.0f/12*1);
      glVertex2f(0, ch * 6);
  glEnd();
  //glBindTexture(GL_TEXTURE_2D, NULL);
}

void Game::LoadPlaylist()
{
  caves.clear(); // clear the caves down incase of new playlist

  std::ifstream ifs(ROOT_FOLDER + "play.json");
  rapidjson::IStreamWrapper isw(ifs);

  rapidjson::Document d;
  d.ParseStream(isw);

  rapidjson::Value& live = d["lives"];
  lives = live.GetInt(); // load how many lives we have according to playlist

  rapidjson::Value& cave = d["caves"];
  for(auto itr = cave.Begin(); itr != cave.End(); ++itr)
  {
    Cave c;

    const rapidjson::Value& cave_name = (*itr)["cave_name"];
    const rapidjson::Value& alloc_time = (*itr)["allocated_time"];
    const rapidjson::Value& diamonds_req = (*itr)["diamonds_req"];

    c.name = cave_name.GetString();
    c.time = alloc_time.GetInt();
    c.coins = diamonds_req.GetInt();

    caves.push_back(c);
  }
}

void Game::LoadNextCave()
{
  // Clear the previous cave, reset the cave timer
  for(int i = 0; i < game_objects.size(); ++i)
    delete game_objects[i];
  game_objects.clear();
  explosions.clear();

  diamonds_collected = 0;
  player_at_exit = false;

  std::ifstream cave;
  if(caves_index + 1 <= caves.size())
  {
    cave.open(CAVE_FOLDER + caves[caves_index].name);
    cave_time = caves[caves_index].time;
    caves_index++;
  }
  else
  {
    // No more caves to load; perhaps render an end of game screen and then
    // move back to the title screen
    at_menu = true;
    total_score = 0;
    caves_index = 0;
    return;
  }

  if(!cave.good())
    std::cerr << "Error opening map, ensure file exists" << std::endl;
  else
  {
    char tile;
    int x = 0;
    int y = 0;

    GLuint* texID = &sprites.m_texID;
    while(cave.get(tile))
    {
      switch(tile)
      {
        case 'p':
          game_objects.push_back(new Player(texID, SpriteType::Player, x, y));
          rockford = reinterpret_cast<Player*>(game_objects.back());
          start.m_x = x;
          start.m_y = y;
          x += cw;
          break;

        case 'd':
          game_objects.push_back(new Dirt(texID, SpriteType::Dirt, x, y));
          x += cw;
          break;

        case 'b':
          game_objects.push_back(new Boulder(texID, SpriteType::Boulder, x, y));
          x += cw;
          break;

        case 'w':
          game_objects.push_back(new Wall(texID, SpriteType::Wall, x, y));
          x += cw;
          break;

        case 'c':
          game_objects.push_back(new Coin(texID, SpriteType::Coin, x, y));
          x += cw;
          break;

        case 'e':
          game_objects.push_back(new Exit(texID, SpriteType::Exit, x, y));
          exit = reinterpret_cast<Exit*>(game_objects.back());
          x += cw;
          break;

        case 'i':
          game_objects.push_back(new Impassable(texID, SpriteType::Impassable, x, y));
          x += cw;
          break;

        case 'n':
          x += cw;
          break;

        case 'a':
          game_objects.push_back(new Amoeba(texID, SpriteType::Amoeba, x, y));
          x += cw;
          break;

        case 's':
          game_objects.push_back(new Butterfly(texID, SpriteType::Butterfly, x, y));
          x += cw;
          break;

        case 'f':
          game_objects.push_back(new Firefly(texID, SpriteType::Firefly, x, y));
          x += cw;
          break;

        default:
          break;
      }

      if(x >= (int)ORTHO_WIDTH)
      {
        x = 0;
        y += ch;
      }
    }
  }

  cave_timer->Stop();
  cave_timer->Start();

  cave.close();
}

bool Game::HandleInput(SDL_Event& event)
{
  while(SDL_PollEvent(&event) != 0)
  {
    if(at_menu == true)
    {
      if(event.type == SDL_KEYDOWN)
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_RETURN:
          {
            LoadPlaylist();
            LoadNextCave();
            at_menu = false;
          }
          break;
        }
      }
      if(event.type == SDL_QUIT)
        return false;

      if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
      {
          window->Resize(event.window.data1, event.window.data2);
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
                  case SDLK_w: keys[SDLK_w] = true; rockford->m_direction = 4; break;
                  case SDLK_a: keys[SDLK_a] = true; rockford->m_direction = 4; break;
                  case SDLK_s: keys[SDLK_s] = true; rockford->m_direction = 5; break;
                  case SDLK_d: keys[SDLK_d] = true; rockford->m_direction = 5; break;
                  case SDLK_ESCAPE: LoseLife(); break; // restart cave
          }
      }

      if(event.type == SDL_KEYUP)
      {
          switch(event.key.keysym.sym)
          {
                  case SDLK_w: keys[event.key.keysym.sym] = false; rockford->m_direction = 0; break;
                  case SDLK_a: keys[event.key.keysym.sym] = false; rockford->m_direction = 0; break;
                  case SDLK_s: keys[event.key.keysym.sym] = false; rockford->m_direction = 0; break;
                  case SDLK_d: keys[event.key.keysym.sym] = false; rockford->m_direction = 0; break;
          }
      }

      if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
      {
          window->Resize(event.window.data1, event.window.data2);
      }
    }
  }

  return true;
}

void Game::LoseLife()
{
  explode = false; // reset flag - rockford will be reborn
  lives -= 1; // remove life
  if(lives == 0)
  {
    // game over - to main menu
  }
  //rockford->m_coordinates = start; // move back to cave entrance
  cave_time = caves[caves_index - 1].time; // give us some more time
  cave_timer->Stop(); // reset the SDL cave timer
  cave_timer->Start(); // start the SDL cave timer

  caves_index -= 1; // move the index back
  LoadNextCave(); // reload the cave
}

Sprite* Game::GetAbove(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x &&
       i->m_coordinates.m_y == s->m_coordinates.m_y - ch)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetBelow(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x &&
       i->m_coordinates.m_y == s->m_coordinates.m_y + ch)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetLeft(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x - cw &&
       i->m_coordinates.m_y == s->m_coordinates.m_y)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetRight(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x + cw &&
       i->m_coordinates.m_y == s->m_coordinates.m_y)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetBottomRight(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x + cw &&
       i->m_coordinates.m_y == s->m_coordinates.m_y + ch)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetBottomLeft(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x - cw &&
       i->m_coordinates.m_y == s->m_coordinates.m_y + ch)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetTopRight(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x + cw &&
       i->m_coordinates.m_y == s->m_coordinates.m_y - ch)
    {
      return i;
    }
  }
  return nullptr;
}

Sprite* Game::GetTopLeft(Sprite* s)
{
  for(auto& i : game_objects)
  {
    if(i->m_coordinates.m_x == s->m_coordinates.m_x - cw &&
       i->m_coordinates.m_y == s->m_coordinates.m_y - ch)
    {
      return i;
    }
  }
  return nullptr;
}

std::pair<std::vector<Sprite*>, std::vector<Coordinates>> Game::GetSurrounding(Sprite* s)
{
  std::pair<std::vector<Sprite*>, std::vector<Coordinates>> spri;
  Coordinates c;

  if(GetTopLeft(s) != nullptr)
    spri.first.push_back(GetTopLeft(s));
  else
  {
    c.m_x = s->m_coordinates.m_x - cw;
    c.m_y = s->m_coordinates.m_y - ch;
    spri.second.push_back(c);
  }

  if(GetAbove(s) != nullptr)
    spri.first.push_back(GetAbove(s));
  else
  {
    c.m_x = s->m_coordinates.m_x;
    c.m_y = s->m_coordinates.m_y - ch;
    spri.second.push_back(c);
  }

  if(GetTopRight(s) != nullptr)
    spri.first.push_back(GetTopRight(s));
  else
  {
    c.m_x = s->m_coordinates.m_x + cw;
    c.m_y = s->m_coordinates.m_y - ch;
    spri.second.push_back(c);
  }

  if(GetRight(s) != nullptr)
    spri.first.push_back(GetRight(s));
  else
  {
    c.m_x = s->m_coordinates.m_x + cw;
    c.m_y = s->m_coordinates.m_y;
    spri.second.push_back(c);
  }

  if(GetBottomRight(s) != nullptr)
    spri.first.push_back(GetBottomRight(s));
  else
  {
    c.m_x = s->m_coordinates.m_x + cw;
    c.m_y = s->m_coordinates.m_y + ch;
    spri.second.push_back(c);
  }

  if(GetBelow(s) != nullptr)
    spri.first.push_back(GetBelow(s));
  else
  {
    c.m_x = s->m_coordinates.m_x;
    c.m_y = s->m_coordinates.m_y + ch;
    spri.second.push_back(c);
  }

  if(GetBottomLeft(s) != nullptr)
    spri.first.push_back(GetBottomLeft(s));
  else
  {
    c.m_x = s->m_coordinates.m_x - cw;
    c.m_y = s->m_coordinates.m_y + ch;
    spri.second.push_back(c);
  }

  if(GetLeft(s) != nullptr)
    spri.first.push_back(GetLeft(s));
  else
  {
    c.m_x = s->m_coordinates.m_x - cw;
    c.m_y = s->m_coordinates.m_y;
    spri.second.push_back(c);
  }
  return spri;
}

void Game::DestroyObject(Sprite* s)
{
  for(int i = 0; i < game_objects.size(); ++i)
  {
    if(game_objects[i]->m_coordinates.m_x == s->m_coordinates.m_x &&
       game_objects[i]->m_coordinates.m_y == s->m_coordinates.m_y &&
       game_objects[i]->m_type == s->m_type)
    {
      delete game_objects[i];
      game_objects.erase(game_objects.begin() + i);
    }
  }
}

void Game::DestroyExplosion(Sprite* s)
{
  for(int i = 0; i < game_objects.size(); ++i) // first erase it from the game objects
  {
    if(game_objects[i]->m_coordinates.m_x == s->m_coordinates.m_x &&
       game_objects[i]->m_coordinates.m_y == s->m_coordinates.m_y &&
       game_objects[i]->m_type == SpriteType::Explosion)
    {
      game_objects.erase(game_objects.begin() + i);
    }
  }
  for(int i = 0; i < explosions.size(); ++i) // now erase it from the explosions and free up memory
  {
    if(explosions[i]->m_coordinates.m_x == s->m_coordinates.m_x &&
       explosions[i]->m_coordinates.m_y == s->m_coordinates.m_y &&
       explosions[i]->m_type == SpriteType::Explosion)
    {
      delete explosions[i];
      explosions.erase(explosions.begin() + i);
    }
  }
}
