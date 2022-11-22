#ifndef _GEOWARS_PLAYER_H_
#define _GEOWARS_PLAYER_H_

#include "Animation.h"
#include "Controller.h"
#include "Cooldown.h"
#include "Entity.h"
#include "Particles.h"
#include "Sprite.h"
#include "Vector.h"

class Player : public Entity
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
    Cooldown dmgCd = {0.5f};

  public:
    Player();
    ~Player();

    bool TakeDamage(uint damage);

    Vector *Speed() const
    {
        return speed;
    }

    float WeaponAngle() const
    {
        return weaponAngle;
    }

    uint Hp() const
    {
        return hp;
    }

    void Move(Vector &&v);
    void Update();
    void Draw();
};

#endif
