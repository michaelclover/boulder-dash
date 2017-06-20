#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "window.h"
#include "sprite.h"
#include "utility.h"
#include "texture.h"
#include "timer.h"
#include "defines.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

class Game
{
public:
  Game(const std::string title, const int width, const int height);
  ~Game();

  void MainLoop(); // enter main game loop

protected:
  // Methods
  void Update(); // perform update
  void Render(); // perform render

  void RenderText(); // render text when in cave
  void RenderMainMenu(); // render the main menu

  void LoadPlaylist(); // load playlist
  void LoadNextCave(); // load next cave in playlist

  bool HandleInput(SDL_Event &event); // handle user events/input

  void LoseLife(); // process player death

  Sprite* GetAbove(Sprite* s); // get a sprite relative to another sprite
  Sprite* GetBelow(Sprite* s); // below given sprite
  Sprite* GetLeft(Sprite* s); // left of given sprite
  Sprite* GetRight(Sprite* s); // right of given sprite
  Sprite* GetBottomRight(Sprite* s); // bottom-right of given sprite
  Sprite* GetBottomLeft(Sprite* s); // bottom-left of given sprite
  Sprite* GetTopRight(Sprite* s); // top-right of given sprite
  Sprite* GetTopLeft(Sprite* s); // top-left of given sprite

  std::pair<std::vector<Sprite*>, std::vector<Coordinates>> GetSurrounding(Sprite* s); // get sprites/empty coords surrounding this one

  void DestroyObject(Sprite* s); // destroy a game object
  void DestroyExplosion(Sprite* s); // destroy an explosion

  // Attributes
  Window* window; // our game instance window helper class

  Timer* game_timer; // timer for limiting game update speed
  Timer* cave_timer; // timer for tracking time spent in current cave

  TextureSheet sprites; // texture helper class for sprites
  TextureSheet font; // texture helper class for font

  std::vector<Sprite*> game_objects; // all game sprites/objects in current cave
  std::vector<Explosion*> explosions; // keep seperate track of explosions so we know when to reload our cave
  Player* rockford; // rockford sprite/object for current cave
  Exit* exit; // exit sprite/object for current cave
  Coordinates start; // entrance to the current cave

  int cw; // cell width
  int ch; // cell height

  bool quit; // mainloop quit flag
  bool at_menu; // true if we're in the menus
  bool player_at_exit; // true if rockford is at the cave exit
  bool explode; // rockford has died in an explosion and we're waiting for explosion animations to finish

  int total_score; // total score throughout playlist
  int diamonds_collected; // the amount of diamonds collected per cave
  int lives; // the amount of lives the player has - loaded from play.json
  int cave_time; // tick-down timer for how long we can spend in cave

  std::map<int, bool> keys; // keep track of pressed keys
  std::vector<Cave> caves; // information about caves in playlist - we'll use this to determine how long we can spend in cave etc...
  int caves_index; // which cave we're currently playing
};

#endif // GAME_H
