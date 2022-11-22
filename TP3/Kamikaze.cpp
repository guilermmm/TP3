#include "Kamikaze.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Missile.h"
#include "Random.h"
#include "Util.h"

Kamikaze::Kamikaze(Image *img, float pX, float pY) : player(GeoWars::player)
{
    sprite = new Sprite(img);
    maxSpeed = randSpeed.Rand();
    speed = new Vector(0, maxSpeed);
    BBox(new Circle(24.0f));

    RandF posX{300, 400};
    RandF posY{game->Height() - 400, game->Height() - 300};
    MoveTo(pX, pY);

    Generator emitter;
    emitter.imgFile = "Resources/WIP/Particle.png";
    emitter.angle = 270.0f;
    emitter.spread = 50;
    emitter.lifetime = .2f;
    emitter.frequency = .005f;
    emitter.percentToDim = .1f;
    emitter.minSpeed = 25.f;
    emitter.maxSpeed = 40.f;
    emitter.color.r = .1f;
    emitter.color.g = .5f;
    emitter.color.b = .1f;
    emitter.color.a = .75f;

    tail = new Particles(emitter);

    hp = 5;

    type = ENEMY;
}

Kamikaze::~Kamikaze()
{
    delete sprite;
    delete speed;
    delete tail;
    GeoWars::IncrementEnemyCount();
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
        GeoWars::audio->Play(EXPLODE, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(x, y), STATIC);
        GeoWars::scene->Delete();
        return;
    }

    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 16.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    speed->Add(target);

    if (speed->Magnitude() > maxSpeed)
        speed->ScaleTo(maxSpeed);

    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);

    if (x < 24)
        MoveTo(24, y);
    if (y < 24)
        MoveTo(x, 24);
    if (x > game->Width() - 24)
        MoveTo(game->Width() - 24, y);
    if (y > game->Height() - 24)
        MoveTo(x, game->Height() - 24);

    Vector butt(speed->Angle() + 180, 30.0f);

    tail->Config().angle = speed->Angle() + 180;
    tail->Generate(x + butt.XComponent(), y - butt.YComponent());
    tail->Update(gameTime);
}

void Kamikaze::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, 90.f - speed->Angle());
    tail->Draw(Layer::LOWER, 1.0f);
}

void Kamikaze::OnCollision(Object *obj)
{
    if (hp == 0)
        return;

    switch (obj->Type())
    {
    case ENEMY: {
        Point self = Point(x, y);
        Point other = Point(obj->X(), obj->Y());

        Vector vec = Vector(Line::Angle(other, self), (48.f - Point::Distance(self, other)));

        Translate(vec.XComponent() * 5.f * gameTime, -vec.YComponent() * 5.f * gameTime);
    }
    break;
    case PLAYER: {
        Player *player = (Player *)obj;
        if (player->TakeDamage(1))
        {
            GeoWars::audio->Play(EXPLODE, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
            GeoWars::scene->Add(new Explosion(x, y), STATIC);
            GeoWars::scene->Delete(this, MOVING);
        }
    }
    break;
    }
}
