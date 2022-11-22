#ifndef _GEOWARS_ENEMY_SHIP_H_
#define _GEOWARS_ENEMY_SHIP_H_

#include "Cooldown.h"
#include "Entity.h"
#include "Player.h"
#include "Sprite.h"
#include "Types.h"
#include "Vector.h"

class EnemyShip : public Entity
{
  private:
    Sprite *sprite;
    Vector *speed;
    Player *player;
    Particles* tail;
    float factor;

    RandF cd{ 1.0f,2.0f };

    Cooldown attackCd{cd.Rand(), false};

  public:
    EnemyShip(Image *img, Player *p);
    ~EnemyShip();

    bool TakeDamage(uint damage);

    void OnCollision(Object *obj);
    void Update();
    void Draw();
};

#endif
