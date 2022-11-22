#ifndef _GEOWARS_KAMIKAZE_H_
#define _GEOWARS_KAMIKAZE_H_

#include "Entity.h"
#include "Player.h"
#include "Sprite.h"
#include "Types.h"
#include "Vector.h"

class Kamikaze : public Entity
{
  private:
    Sprite *sprite;
    Vector *speed;
    Player *player;
    Particles* tail;
    float factor;

  public:
    Kamikaze(Image *img, float pX, float pY);
    ~Kamikaze();

    bool TakeDamage(uint damage);

    void OnCollision(Object *obj);
    void Update();
    void Draw();
};

#endif
