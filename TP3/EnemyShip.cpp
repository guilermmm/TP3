#include "EnemyShip.h"
#include "EnemyMissile.h"
#include "Explosion.h"
#include "GeoWars.h"
#include "Random.h"
#include "Util.h"

EnemyShip::EnemyShip(Image *img, Player *p) : player(p)
{
    sprite = new Sprite(img);
    missileTileSet = new TileSet("Resources/WIP/EnemyMissile.png", 1, 2);

    speed = new Vector(0, 2.0f);
    BBox(new Circle(32.0f));

    MoveTo(player->X(), player->Y());
    RandF ang{0, 360};
    RandF mag{400, 800};
    Vector v = Vector(ang.Rand(), mag.Rand());
    Translate(v.XComponent(), v.YComponent());

    Generator emitter;
    emitter.imgFile = "Resources/WIP/Particle.png";
    emitter.angle = 270.0f;
    emitter.spread = 50;
    emitter.lifetime = .2f;
    emitter.frequency = .005f;
    emitter.percentToDim = .1f;
    emitter.minSpeed = 5.f;
    emitter.maxSpeed = 15.f;
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
    GeoWars::IncrementEnemyCount();
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

    if (x < 32.f)
        MoveTo(32.f, y);
    if (y < 32.f)
        MoveTo(x, 32.f);
    if (x > game->Width() - 32.f)
        MoveTo(game->Width() - 32.f, y);
    if (y > game->Height() - 32.f)
        MoveTo(x, game->Height() - 32.f);

    Vector butt(Line::Angle(Point(x, y), Point(player->X(), player->Y())) + 180, 32.0f);

    tail->Config().angle = speed->Angle() + 180;
    tail->Generate(x + butt.XComponent(), y - butt.YComponent());
    tail->Update(gameTime);

    if (attackCd.Ready())
    {
        attackCd.Restart();
        GeoWars::scene->Add(new EnemyMissile(this, missileTileSet), STATIC);
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
