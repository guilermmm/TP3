/**********************************************************************************
// Blue (C�digo Fonte)
// 
// Cria��o:     10 Out 2012
// Atualiza��o: 01 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Objeto faz uma persegui��o suavizada
//
**********************************************************************************/

#include "GeoWars.h"
#include "EnemyShip.h"
#include "Random.h" 
#include "EnemyMissile.h"
#include "Util.h"
#include "Explosion.h"

// ---------------------------------------------------------------------------------

EnemyShip::EnemyShip(Player * p) : player(p)
{
    sprite = new Sprite("Resources/Blue.png");
    speed  = new Vector(0, 2.0f);
    BBox(new Circle(20.0f));
    
    // move para uma posi��o aleat�ria (canto superior direito)
    RandF posX { game->Width() - 400, game->Width() - 300 };
    RandF posY { 300, 400 };
    MoveTo(posX.Rand(), posY.Rand());

    factor = -0.25f;
    type = ENEMY;
}

// ---------------------------------------------------------------------------------

EnemyShip::~EnemyShip()
{
    delete sprite;
    delete speed;
}

// -------------------------------------------------------------------------------

void EnemyShip::OnCollision(Object* obj)
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

// -------------------------------------------------------------------------------

void EnemyShip::Update()
{
    // a magnitude do vetor 'target' controla qu�o 
    // r�pido o objeto converge para a dire��o do alvo
    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 1.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    // ajusta velocidade atual na dire��o do alvo
    speed->Add(target);
    
    // limita a magnitude da velocidade para impedir 
    // que ela cres�a indefinidamente pelo soma vetorial
    if (speed->Magnitude() > 2.5f)
        speed->ScaleTo(2.5f);

    // move o objeto pelo seu vetor velocidade
    Translate(speed->XComponent() * 100.0f * gameTime, -speed->YComponent() * 100.0f * gameTime);

    // aplica fator de escala
    Scale(1.0f + factor * gameTime);

    // amplia e reduz objeto
    if (scale < 0.85f)
        factor = 0.25f;
    if (scale > 1.00f)
        factor = -0.25f;

    // mant�m o objeto dentro do mundo do jogo
    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);

    if (attackCd.Ready())
    {
        attackCd.Restart();
        GeoWars::scene->Add(new EnemyMissile(this), STATIC);
    }

    attackCd.Update(gameTime);
}

// -------------------------------------------------------------------------------