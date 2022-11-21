#ifndef _GEOWARS_MISSILE_H_
#define _GEOWARS_MISSILE_H_

#include "Animation.h"
#include "Object.h"
#include "Player.h"
#include "Vector.h"

class Missile : public Object
{
  private:
    const float MaxDistance = 4406;
    const float BaseVolume = 0.8f;

    static Player *&player;
    Animation *anim;
    Vector speed;

  public:
    Missile(TileSet *tileSet);
    ~Missile();

    Vector &Speed();
    void Update();
    void Draw();

    void OnCollision(Object *obj);
};

inline Vector &Missile::Speed()
{
    return speed;
}

#endif
