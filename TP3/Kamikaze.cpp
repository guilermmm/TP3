#include "Kamikaze.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Missile.h"
#include "Random.h"
#include "Util.h"

Kamikaze::Kamikaze(float pX, float pY) : player(GeoWars::player)
{
    sprite = new Sprite("Resources/Magenta.png");
    speed = new Vector(0, 5.0f);
    BBox(new Circle(18.0f));

    RandF posX{300, 400};
    RandF posY{game->Height() - 400, game->Height() - 300};
    MoveTo(pX, pY);

    type = ENEMY;
}

Kamikaze::~Kamikaze()
{
    delete sprite;
    delete speed;
}

void Kamikaze::OnCollision(Object *obj)
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
    case ENEMY: {
        Point self = Point(this->x, this->y);
        Point other = Point(obj->X(), obj->Y());

        Vector vec = Vector(Line::Angle(other, self), (36.f - Point::Distance(self, other)));

        Translate(vec.XComponent() * 5.f * gameTime, -vec.YComponent() * 5.f * gameTime);
    }
    break;
    }
}

void Kamikaze::Update()
{

    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 8.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    speed->Add(target);

    if (speed->Magnitude() > 2.5f)
        speed->ScaleTo(2.5f);

    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);

    Scale(1.0f + factor * gameTime);

    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);
}
