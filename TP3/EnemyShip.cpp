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

    RandF posX{300, game->Width() - 300};
    RandF posY{300, game->Height() - 300};
    MoveTo(posX.Rand(), posY.Rand());

    Generator emitter;
    emitter.imgFile = "Resources/Spark.png";
    emitter.angle = 270.0f;
    emitter.spread = 50;
    emitter.lifetime = .2f;
    emitter.frequency = .005f;
    emitter.percentToDim = .1f;
    emitter.minSpeed = 25.f;
    emitter.maxSpeed = 40.f;
    emitter.color.r = .5f;
    emitter.color.g = .1f;
    emitter.color.b = .5f;
    emitter.color.a = .75f;

    tail = new Particles(emitter);

    factor = -0.25f;

    hp = 15;

    type = ENEMY;
}

EnemyShip::~EnemyShip()
{
    delete sprite;
    delete speed;
}

bool EnemyShip::TakeDamage(uint damage)
{
    if (hp > damage)
        hp -= damage;
    else
        hp = 0;

    return true;
}

void EnemyShip::Update()
{
    if (hp == 0)
    {
        GeoWars::audio->Play(EXPLODE, VolumeFromDistance(Point(x, y), Point(player->X(), player->Y())));
        GeoWars::scene->Add(new Explosion(x, y), STATIC);
        GeoWars::scene->Delete();
        return;
    }

    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 1.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    speed->Add(target);

    if (speed->Magnitude() > 2.5f)
        speed->ScaleTo(2.5f);

    Translate(speed->XComponent() * 100.0f * gameTime, -speed->YComponent() * 100.0f * gameTime);

    RotateTo(90.f - Line::Angle(Point(x, y), Point(player->X(), player->Y())));

    if (x < 20)
        MoveTo(20, y);
    if (y < 20)
        MoveTo(x, 20);
    if (x > game->Width() - 20)
        MoveTo(game->Width() - 20, y);
    if (y > game->Height() - 20)
        MoveTo(x, game->Height() - 20);

    Vector butt(Line::Angle(Point(x, y), Point(player->X(), player->Y())) + 180, 20.0f);

    tail->Config().angle = speed->Angle() + 180;
    tail->Generate(x + butt.XComponent(), y - butt.YComponent());
    tail->Update(gameTime);

    if (attackCd.Ready())
    {
        attackCd.Restart();
        GeoWars::scene->Add(new EnemyMissile(this), STATIC);
        GeoWars::audio->Play(ENEMYFIRE);

    }

    attackCd.Update(gameTime);
}

void EnemyShip::Draw()
{
    sprite->Draw(x, y, Layer::LOWER, scale, rotation);
    tail->Draw(Layer::LOWER, 1.0f);
}

void EnemyShip::OnCollision(Object *obj)
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
            GeoWars::scene->Add(new Explosion(x, y), STATIC);
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
