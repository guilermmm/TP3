#ifndef _GEOWARS_PLAYER_H_
#define _GEOWARS_PLAYER_H_

#include "Animation.h"
#include "Controller.h"
#include "Cooldown.h"
#include "Object.h"
#include "Particles.h"
#include "Sprite.h"
#include "Vector.h"

class Player : public Object
{
  private:
    Sprite *sprite;
    Sprite *basicWeapon;
    TileSet *missileTileSet;
    Particles *tailLeft;
    Particles *tailRight;
    Vector *speed;
    float weaponAngle;
    Cooldown fireRate = {0.2f};

  public:
    Player();
    ~Player();

    Vector *Speed() const
    {
        return speed;
    }

    float WeaponAngle() const
    {
        return weaponAngle;
    }

    void Move(Vector &&v);
    void Update();
    void Draw();
};

#endif
