#include "Mothership.h"
#include "GeoWars.h"
#include "Random.h" 
#include "Kamikaze.h"

// ---------------------------------------------------------------------------------

Mothership::Mothership(Player * p): player(p)
{
    sprite = new Sprite("Resources/Green.png");
    speed  = new Vector(0, 2.0f);
    BBox(new Circle(20.0f));

    distance = 600;

    // nasce em uma posi��o aleat�ria (canto inferior direito)
    RandF posX{ game->Width() - 50, game->Width() };
    RandF posY{ game->Height() - 50, game->Height() };
    MoveTo(posX.Rand(), posY.Rand());

    spawnCd.Restart();

    type = ENEMY;
}

// ---------------------------------------------------------------------------------

Mothership::~Mothership()
{
    delete sprite;
    delete speed;
}

// -------------------------------------------------------------------------------

void Mothership::OnCollision(Object * obj)
{
    if (obj->Type() == MISSILE)
        GeoWars::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void Mothership::Update()
{
    // a magnitude do vetor target controla qu�o 
    // r�pido o objeto converge para a dire��o do alvo
    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 10.0f * gameTime;
    Vector target = Vector(angle, magnitude);
    
    // fugir se o player chegar muito perto
    if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance)
    {
        target.Rotate(180.0f);
        target.ScaleTo(20.0f * gameTime);
    }

    // ajusta velocidade atual na dire��o do alvo
    speed->Add(target);
    
    // limita a magnitude da velocidade para impedir 
    // seu crescimento indefinido na soma vetorial
    if (speed->Magnitude() > 5)
        speed->ScaleTo(5.0f);


    // move o objeto pelo seu vetor velocidade
    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);
    Rotate(50 * gameTime);

    // mant�m o objeto dentro do mundo do jogo
    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);

    if (spawnCd.Ready())
    {
        spawnCd.Restart();

        GeoWars::scene->Add(new Kamikaze(this->x, this->y), MOVING);
    }

    spawnCd.Update(gameTime);
}

// -------------------------------------------------------------------------------