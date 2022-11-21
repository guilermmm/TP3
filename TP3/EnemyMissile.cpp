#include "EnemyMissile.h"
#include "GeoWars.h"
#include "Util.h"
#include "WallHit.h"
#include "Geometry.h"

Player *&EnemyMissile::player = GeoWars::player;

EnemyMissile::EnemyMissile(EnemyShip* enemy)
{
    sprite = new Sprite("Resources/Missile.png");

    BBox(new Circle(8));

    speed.RotateTo(Line::Angle(Point(enemy->X(), enemy->Y()), Point(player->X(), player->Y())));
    speed.ScaleTo(15.0f);

    MoveTo(enemy->X() + 22 * cos(speed.Radians()), enemy->Y() - 22 * sin(speed.Radians()));
    RotateTo(-Line::Angle(Point(enemy->X(), enemy->Y()), Point(player->X(), player->Y())) + 90.0f);

    type = ENEMYMISSILE;
}

EnemyMissile::~EnemyMissile()
{
    delete sprite;
}

void EnemyMissile::Update()
{
    Translate(speed.XComponent() * 90.0f * gameTime, -speed.YComponent() * 90.0f * gameTime);

    if (x > game->Width() - 50 || x < 50 || y > game->Height() - 50 || y < 50)
    {
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new WallHit(x, y), STATIC);
        GeoWars::scene->Delete();
    }
}

void EnemyMissile::OnCollision(Object* obj)
{
}
