#include "Missile.h"
#include "GeoWars.h"
#include "Util.h"
#include "WallHit.h"

Player *&Missile::player = GeoWars::player;

Missile::Missile()
{
    sprite = new Sprite("Resources/Missile.png");

    BBox(new Circle(8));

    speed.RotateTo(player->WeaponAngle());
    speed.ScaleTo(15.0f);

    MoveTo(player->X() + 22 * cos(speed.Radians()), player->Y() - 22 * sin(speed.Radians()));
    RotateTo(-player->WeaponAngle() + 90.0f);

    type = MISSILE;
}

Missile::~Missile()
{
    delete sprite;
}

void Missile::Update()
{
    Translate(speed.XComponent() * 90.0f * gameTime, -speed.YComponent() * 90.0f * gameTime);

    if (x > game->Width() - 50 || x < 50 || y > game->Height() - 50 || y < 50)
    {
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new WallHit(x, y), STATIC);
        GeoWars::scene->Delete();
    }
}

void Missile::OnCollision(Object *obj)
{
    if (obj->Type() == ENEMY)
    {
        GeoWars::scene->Delete(this, STATIC);
    }
}
