#ifndef _GEOWARS_H_
#define _GEOWARS_H_

#include "Audio.h"
#include "Background.h"
#include "Controller.h"
#include "Font.h"
#include "Game.h"
#include "Hud.h"
#include "Player.h"
#include "Scene.h"
#include "Sprite.h"

class GeoWars : public Game
{
  private:
    Background *backg = nullptr;
    Image *motherShipImg = nullptr;
    Image *enemyShipImg = nullptr;
    Hud *hud = nullptr;
    bool viewBBox = false;

    GameState state = TITLESCREEN;

    Sprite *titleScreen = nullptr;
    Sprite *playButton = nullptr;
    float playScale = 1.0f;
    bool playScaleUp = true;
    Sprite *quitButton = nullptr;
    bool quitScaleUp = true;
    float quitScale = 1.0f;
    Sprite *gameOverScreen = nullptr;
    Sprite *restartButton = nullptr;
    float restartScale = 1.0f;
    bool restartScaleUp = true;

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
