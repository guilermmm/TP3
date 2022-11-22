#include "EnemyMissile.h"
#include "GeoWars.h"
#include "Geometry.h"
#include "Util.h"
#include "WallHit.h"

Player *&EnemyMissile::player = GeoWars::player;

EnemyMissile::EnemyMissile(EnemyShip *enemy, TileSet *tileSet)
{
    anim = new Animation(tileSet, 0.2f, true);

    BBox(new Circle(8.f));

    speed.RotateTo(Line::Angle(Point(enemy->X(), enemy->Y()), Point(player->X(), player->Y())));
    speed.ScaleTo(15.0f);

    MoveTo(enemy->X() + 22 * cos(speed.Radians()), enemy->Y() - 22 * sin(speed.Radians()));
    RotateTo(-Line::Angle(Point(enemy->X(), enemy->Y()), Point(player->X(), player->Y())) + 90.0f);

    type = ENEMYMISSILE;
}

EnemyMissile::~EnemyMissile()
{
    delete anim;
}

void EnemyMissile::Update()
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

void EnemyMissile::OnCollision(Object *obj)
{
    if (obj->Type() == PLAYER)
    {
        Player *player = (Player *)obj;
        if (player->TakeDamage(1))
        {
            GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
            GeoWars::scene->Add(new WallHit(x, y), STATIC);
            GeoWars::scene->Delete(this, STATIC);
        }
    }
}

void EnemyMissile::Draw()
{
    anim->Draw(x, y, Layer::UPPER, scale, rotation);
}
