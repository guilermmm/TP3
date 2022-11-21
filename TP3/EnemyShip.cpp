#include "EnemyShip.h"
#include "EnemyMissile.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Random.h"
#include "Util.h"

EnemyShip::EnemyShip(Image *img, Player *p) : player(p)
{
    sprite = new Sprite(img);
    speed = new Vector(0, 2.0f);
    BBox(new Circle(20.0f));

    RandF posX{game->Width() - 400, game->Width() - 300};
    RandF posY{300, 400};
    MoveTo(posX.Rand(), posY.Rand());

    factor = -0.25f;

    hp = 15;

    type = ENEMY;
}

EnemyShip::~EnemyShip()
{
    delete sprite;
    delete speed;
}

void EnemyShip::TakeDamage(uint damage)
{
    if (hp > damage)
        hp -= damage;
    else
        hp = 0;
}

void EnemyShip::Update()
{
    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 1.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    speed->Add(target);

    if (speed->Magnitude() > 2.5f)
        speed->ScaleTo(2.5f);

    Translate(speed->XComponent() * 100.0f * gameTime, -speed->YComponent() * 100.0f * gameTime);

    RotateTo(90.f - Line::Angle(Point(x, y), Point(player->X(), player->Y())));

    if (x < 0)
        MoveTo(0, y);
    if (y < 0)
        MoveTo(x, 0);
    if (x > game->Width() - 0)
        MoveTo(game->Width() - 0, y);
    if (y > game->Height() - 0)
        MoveTo(x, game->Height() - 0);

    if (attackCd.Ready())
    {
        attackCd.Restart();
        GeoWars::scene->Add(new EnemyMissile(this), STATIC);
    }

    attackCd.Update(gameTime);
}

void EnemyShip::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, rotation);
}

void EnemyShip::OnCollision(Object *obj)
{
    switch (obj->Type())
    {
    case MISSILE: {
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(this->x, this->y), STATIC);
        GeoWars::scene->Delete(this, MOVING);
    }
    break;
    case PLAYER: {
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(this->x, this->y), STATIC);
        GeoWars::scene->Delete(this, MOVING);
    }
    break;
    case ENEMY: {
        Point self = Point(this->x, this->y);
        Point other = Point(obj->X(), obj->Y());

        Vector vec = Vector(Line::Angle(other, self), (36.f - Point::Distance(self, other)));

        Translate(vec.XComponent() * 5.f * gameTime, -vec.YComponent() * 5.f * gameTime);
    }
    break;
    }
}
