#ifndef _GEOWARS_ENEMY_MISSILE_H_
#define _GEOWARS_ENEMY_MISSILE_H_

#include "Object.h"
#include "EnemyShip.h"
#include "Sprite.h"
#include "Vector.h"

class EnemyMissile : public Object
{
private:
    const float MaxDistance = 4406;
    const float BaseVolume = 0.8f;

    EnemyShip* enemy;
    Sprite* sprite;
    Vector speed;
    static Player *&player;

public:
    EnemyMissile(EnemyShip* enemy);
    ~EnemyMissile();

    Vector& Speed();
    void Update();
    void Draw();

    void OnCollision(Object* obj);
};

inline Vector& EnemyMissile::Speed()
{
    return speed;
}

inline void EnemyMissile::Draw()
{
    sprite->Draw(x, y, Layer::UPPER, scale, rotation);
}

#endif
