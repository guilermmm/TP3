#ifndef _GEOWARS_KAMIKAZE_H_
#define _GEOWARS_KAMIKAZE_H_

#include "Object.h"
#include "Player.h"
#include "Sprite.h"
#include "Types.h"
#include "Vector.h"

class Kamikaze : public Object
{
  private:
    Sprite *sprite;
    Vector *speed;
    Player *player;
    float factor;

  public:
    Kamikaze(float pX, float pY);
    ~Kamikaze();

    void OnCollision(Object *obj);
    void Update();
    void Draw();
};

inline void Kamikaze::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, rotation);
}

#endif
