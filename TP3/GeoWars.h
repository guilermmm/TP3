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

enum Wave {
    FIRST,
    SECOND,
    THIRD,
    NONSTOP
};

class GeoWars : public Game
{
  private:
    Background *backg = nullptr;
    Image *motherShipImg = nullptr;
    Image *enemyShipImg = nullptr;
    Image* kamikazeImg = nullptr;
    Hud *hud = nullptr;
    bool viewBBox = false;

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

    Wave wave = FIRST;
    bool firstCtrl = true;
    bool secondCtrl = true;
    bool thirdCtrl = true;

    Cooldown kamikazeCd{ 0.5f };
    Cooldown shipCd{ 2.0f };
    Cooldown motherShipCd{ 3.0f };

    RandF posX{ game->Width(), game->Width() };
    RandF posY{ game->Height(), game->Height() };

  public:
    static int enemyCount;
    static Player *player;
    static Audio *audio;
    static Scene *scene;
    static bool viewHUD;
    static Controller *gamepad;
    static bool gamepadOn;
    static uint xboxPlayer;
    static GameState state;

    void Setup();
    void Cleanup();

    void Init();
    void Update();
    void Draw();
    void Finalize();

    static void IncrementEnemyCount();
};


inline void GeoWars::IncrementEnemyCount()
{
    enemyCount++;
}

#endif
