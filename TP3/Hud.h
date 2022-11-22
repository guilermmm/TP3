#ifndef _GEOWARS_HUD_H_
#define _GEOWARS_HUD_H_

#include "Animation.h"
#include "Font.h"
#include "Object.h"
#include <sstream>

class Hud : public Object
{
  private:
    TileSet *hpTileSet = nullptr;
    Animation *hpBar[5] = {nullptr};
    Font *font = nullptr;
    std::stringstream text;
    std::stringstream text2;

  public:
    Hud();
    ~Hud();

    void Update();
    void Draw();
};

#endif
