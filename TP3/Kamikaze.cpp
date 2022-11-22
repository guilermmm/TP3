#include "Kamikaze.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Missile.h"
#include "Random.h"
#include "Util.h"

Kamikaze::Kamikaze(Image *img, float pX, float pY) : player(GeoWars::player)
{
    sprite = new Sprite(img);
    speed = new Vector(0, 5.0f);
    BBox(new Circle(18.0f));

    RandF posX{300, 400};
    RandF posY{game->Height() - 400, game->Height() - 300};
    MoveTo(pX, pY);

    hp = 5;

    type = ENEMY;
}

Kamikaze::~Kamikaze()
{
    delete sprite;
    delete speed;
}

bool Kamikaze::TakeDamage(uint damage)
{
    if (hp > damage)
        hp -= damage;
    else
        hp = 0;

    return true;
}

void Kamikaze::Update()
{
    if (hp == 0)
    {
        GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(x, y), STATIC);
        GeoWars::scene->Delete();
    }

    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 16.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    speed->Add(target);

    if (speed->Magnitude() > 5.f)
        speed->ScaleTo(5.f);

    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);

    if (x < 20)
        MoveTo(20, y);
    if (y < 20)
        MoveTo(x, 20);
    if (x > game->Width() - 20)
        MoveTo(game->Width() - 20, y);
    if (y > game->Height() - 20)
        MoveTo(x, game->Height() - 20);
}

void Kamikaze::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, 90.f - speed->Angle());
}

void Kamikaze::OnCollision(Object *obj)
{
    if (hp == 0)
        return;

    switch (obj->Type())
    {
    case MISSILE: {
        GeoWars::scene->Add(new Explosion(x, y), STATIC);
        GeoWars::scene->Delete(this, MOVING);
    }
    break;
    case ENEMY: {
        Point self = Point(x, y);
        Point other = Point(obj->X(), obj->Y());

        Vector vec = Vector(Line::Angle(other, self), (36.f - Point::Distance(self, other)));

        Translate(vec.XComponent() * 5.f * gameTime, -vec.YComponent() * 5.f * gameTime);
    }
    break;
    case PLAYER: {
        Player *player = (Player *)obj;
        if (player->TakeDamage(1))
        {
            GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
            GeoWars::scene->Add(new Explosion(x, y), STATIC);
            GeoWars::scene->Delete(this, MOVING);
        }
    }
    break;
    }
}
