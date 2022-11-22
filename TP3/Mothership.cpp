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
    BBox(new Circle(72.0f));

    distance = 600;

    MoveTo(player->X(), player->Y());
    RandF ang{0, 360};
    RandF mag{400, 800};
    Vector v = Vector(ang.Rand(), mag.Rand());
    Translate(v.XComponent(), v.YComponent());

    spawnCd.Restart();

    Generator emitter;
    emitter.imgFile = "Resources/WIP/Particle.png";
    emitter.angle = 270.0f;
    emitter.spread = 50;
    emitter.lifetime = .5f;
    emitter.frequency = .005f;
    emitter.percentToDim = .1f;
    emitter.minSpeed = 10.f;
    emitter.maxSpeed = 40.f;
    emitter.color.r = .1f;
    emitter.color.g = .5f;
    emitter.color.b = .1f;
    emitter.color.a = .75f;

    tailLeft = new Particles(emitter);
    tailMiddle = new Particles(emitter);
    tailRight = new Particles(emitter);

    hp = 40;

    type = ENEMY;
}

Mothership::~Mothership()
{
    delete sprite;
    delete speed;
    GeoWars::IncrementEnemyCount();
}

bool Mothership::TakeDamage(uint damage)
{
    if (hp > damage)
        hp -= damage;
    else
        hp = 0;

    return true;
}

void Mothership::Update()
{
    if (hp == 0)
    {
        GeoWars::audio->Play(EXPLODE, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(x, y), STATIC);
        GeoWars::scene->Delete();
        return;
    }

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

    if (x < 72.f)
        MoveTo(72.f, y);
    if (y < 72.f)
        MoveTo(x, 72.f);
    if (x > game->Width() - 72.f)
        MoveTo(game->Width() - 72.f, y);
    if (y > game->Height() - 72.f)
        MoveTo(x, game->Height() - 72.f);

    Vector tail(speed->Angle() + 140.0f, 96.0f);

    tailLeft->Config().angle = speed->Angle() + 180;
    tailLeft->Generate(x + tail.XComponent(), y - tail.YComponent());
    tailLeft->Update(gameTime);

    tail.Rotate(-40.0f);

    tailMiddle->Config().angle = speed->Angle() + 180;
    tailMiddle->Generate(x + tail.XComponent(), y - tail.YComponent());
    tailMiddle->Update(gameTime);

    tail.Rotate(-40.0f);

    tailRight->Config().angle = speed->Angle() + 180;
    tailRight->Generate(x + tail.XComponent(), y - tail.YComponent());
    tailRight->Update(gameTime);

    if (spawnCd.Ready())
    {
        spawnCd.Restart();

        GeoWars::scene->Add(new Kamikaze(kamikazeImg, x, y), MOVING);
        GeoWars::audio->Play(ENEMYSPAWN);
    }

    spawnCd.Update(gameTime);
}

void Mothership::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, 90.f - speed->Angle());
    tailLeft->Draw(Layer::LOWER, 1.0f);
    tailMiddle->Draw(Layer::LOWER, 1.0f);
    tailRight->Draw(Layer::LOWER, 1.0f);
}

void Mothership::OnCollision(Object *obj)
{
    if (hp == 0)
        return;

    switch (obj->Type())
    {
    case PLAYER: {
        Player *player = (Player *)obj;
        if (player->TakeDamage(1))
        {
            GeoWars::audio->Play(HITWALL, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
            GeoWars::scene->Add(new Explosion(this->x, this->y), STATIC);
        }
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
