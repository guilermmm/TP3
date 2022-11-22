#include "Missile.h"
#include "Entity.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Util.h"
#include "WallHit.h"

Player *&Missile::player = GeoWars::player;

Missile::Missile(TileSet *tileSet)
{
    anim = new Animation(tileSet, 0.2f, true);

    BBox(new Circle(8));

    speed.RotateTo(player->WeaponAngle());
    speed.ScaleTo(15.0f);

    MoveTo(player->X() + 22 * cos(speed.Radians()), player->Y() - 22 * sin(speed.Radians()));
    RotateTo(-player->WeaponAngle() + 90.0f);

    type = MISSILE;
}

Missile::~Missile()
{
    delete anim;
}

void Missile::Update()
{
    Translate(speed.XComponent() * 90.0f * gameTime, -speed.YComponent() * 90.0f * gameTime);

    if (x > game->Width() || x < 0 || y > game->Height() || y < 0)
    {
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new WallHit(x, y), STATIC);
        GeoWars::scene->Delete();
    }

    anim->NextFrame();
}

void Missile::OnCollision(Object *obj)
{
    if (obj->Type() == ENEMY)
    {
        ((Entity *)obj)->TakeDamage(5);
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(x, y), STATIC);
        GeoWars::scene->Delete(this, STATIC);
    }
}

void Missile::Draw()
{
    anim->Draw(x, y, Layer::UPPER, scale, rotation);
}
