#ifndef _GEOWARS_MISSILE_H_
#define _GEOWARS_MISSILE_H_

#include "Object.h"
#include "Player.h"
#include "Sprite.h"
#include "Vector.h"

class Missile : public Object
{
  private:
    const float MaxDistance = 4406;
    const float BaseVolume = 0.8f;

    static Player *&player;
    Sprite *sprite;
    Vector speed;

  public:
    Missile();
    ~Missile();

    Vector &Speed();
    void Update();
    void Draw();

    void OnCollision(Object* obj);
};

inline Vector &Missile::Speed()
{
    return speed;
}

inline void Missile::Draw()
{
    sprite->Draw(x, y, Layer::UPPER, scale, rotation);
}

#endif
