#ifndef _GEOWARS_MOTHERSHIP_H_
#define _GEOWARS_MOTHERSHIP_H_

#include "Entity.h"
#include "Player.h"
#include "Sprite.h"
#include "Types.h"
#include "Vector.h"

class Mothership : public Entity
{
  private:
    Sprite *sprite;
    Image *kamikazeImg;
    Vector *speed;
    Player *player;
    Cooldown spawnCd{2.0f};
    int distance;

  public:
    Mothership(Image *img, Player *p);
    ~Mothership();

    bool TakeDamage(uint damage);

    void OnCollision(Object *obj);
    void Update();
    void Draw();
};

#endif
