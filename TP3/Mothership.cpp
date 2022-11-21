#include "Mothership.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Kamikaze.h"
#include "Random.h"
#include "Util.h"

Mothership::Mothership(Image *img, Player *p) : player(p)
{
    sprite = new Sprite(img);
    kamikazeImg = new Image("Resources/WIP/Kamikaze.png");
    speed = new Vector(0, 2.0f);
    BBox(new Circle(20.0f));

    distance = 600;

    RandF posX{game->Width(), game->Width()};
    RandF posY{game->Height(), game->Height()};
    MoveTo(posX.Rand(), posY.Rand());

    spawnCd.Restart();

    hp = 50;

    type = ENEMY;
}

Mothership::~Mothership()
{
    delete sprite;
    delete speed;
}

void Mothership::TakeDamage(uint damage)
{
    if (hp > damage)
        hp -= damage;
    else
        hp = 0;
}

void Mothership::Update()
{
    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 10.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance)
    {
        target.Rotate(180.0f);
        target.ScaleTo(20.0f * gameTime);
    }

    speed->Add(target);

    if (speed->Magnitude() > 5)
        speed->ScaleTo(5.0f);

    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);

    if (x < 0)
        MoveTo(0, y);
    if (y < 0)
        MoveTo(x, 0);
    if (x > game->Width() - 0)
        MoveTo(game->Width() - 0, y);
    if (y > game->Height() - 0)
        MoveTo(x, game->Height() - 0);

    if (spawnCd.Ready())
    {
        spawnCd.Restart();

        GeoWars::scene->Add(new Kamikaze(kamikazeImg, x, y), MOVING);
    }

    spawnCd.Update(gameTime);
}

void Mothership::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, 90.f - speed->Angle());
}

void Mothership::OnCollision(Object *obj)
{
    switch (obj->Type())
    {
    case MISSILE: {
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
