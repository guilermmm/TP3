#ifndef _GEOWARS_H_
#define _GEOWARS_H_

#include "Audio.h"
#include "Background.h"
#include "Controller.h"
#include "Game.h"
#include "Hud.h"
#include "Player.h"
#include "Scene.h"

enum ObjectIds
{
    PLAYER,
    MISSILE,
    ENEMYMISSILE,
    BLUE,
    GREEN,
    MAGENTA,
    ORANGE,
    WALLHIT,

    ENEMY,
    EXPLOSION,
};

enum SoundIds
{
    THEME,
    START,
    FIRE,
    EXPLODE,
    HITWALL
};

class GeoWars : public Game
{
  private:
    Background *backg = nullptr;
    Image *motherShipImg = nullptr;
    Image *enemyShipImg = nullptr;
    Hud *hud = nullptr;
    bool viewBBox = false;

  public:
    static Player *player;
    static Audio *audio;
    static Scene *scene;
    static bool viewHUD;
    static Controller *gamepad;
    static bool gamepadOn;
    static uint xboxPlayer;

    void Init();
    void Update();
    void Draw();
    void Finalize();
};

#endif
