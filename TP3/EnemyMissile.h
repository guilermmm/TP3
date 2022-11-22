#ifndef _GEOWARS_ENEMY_MISSILE_H_
#define _GEOWARS_ENEMY_MISSILE_H_

#include "Animation.h"
#include "EnemyShip.h"
#include "Object.h"
#include "Vector.h"

class EnemyMissile : public Object
{
  private:
    const float MaxDistance = 4406;
    const float BaseVolume = 0.8f;

    EnemyShip *enemy;
    Animation *anim;
    Vector speed;
    static Player *&player;

  public:
    EnemyMissile(EnemyShip *enemy, TileSet *tileSet);
    ~EnemyMissile();

    void Update();
    void Draw();

    void OnCollision(Object *obj);
};

#endif
