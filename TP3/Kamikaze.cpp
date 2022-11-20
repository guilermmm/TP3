/**********************************************************************************
// Magenta (C�digo Fonte)
// 
// Cria��o:     10 Out 2012
// Atualiza��o: 01 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Objeto faz um persegui��o direta
//
**********************************************************************************/

#include "GeoWars.h"
#include "Kamikaze.h"
#include "Random.h" 
#include "Explosion.h"
#include "Missile.h"


// ---------------------------------------------------------------------------------

Kamikaze::Kamikaze(float pX, float pY) : player (GeoWars::player)
{
    sprite = new Sprite("Resources/Magenta.png");
    speed  = new Vector(0, 5.0f);
    BBox(new Circle(18.0f));
    
    // move para uma posi��o aleat�ria (canto inferior esquerdo)
    RandF posX{ 300, 400 };
    RandF posY{ game->Height() - 400, game->Height() - 300 };
    MoveTo(pX, pY);

    type = ENEMY;
}

// ---------------------------------------------------------------------------------

Kamikaze::~Kamikaze()
{
    delete sprite;
    delete speed;
}

// -------------------------------------------------------------------------------

void Kamikaze::OnCollision(Object * obj)
{
    if (obj->Type() == MISSILE)
    {
        Missile * m = (Missile *) obj;
        GeoWars::audio->Play(HITWALL);

        GeoWars::scene->Add(new Explosion(this->x, this->y), STATIC);

        GeoWars::scene->Delete(this, MOVING);
    }
    if (obj->Type() == PLAYER)
    {
        GeoWars::audio->Play(HITWALL);

        GeoWars::scene->Add(new Explosion(this->x, this->y), STATIC);

        GeoWars::scene->Delete(this, MOVING);
    }
}

// -------------------------------------------------------------------------------

void Kamikaze::Update()
{
    //// ajusta �ngulo do vetor 
    //speed->RotateTo(Line::Angle(Point(x, y), Point(player->X(), player->Y())));
    //Rotate(200 * gameTime);

    // a magnitude do vetor 'target' controla quão 
    // rápido o objeto converge para a direção do alvo
    float angle = Line::Angle(Point(x, y), Point(player->X(), player->Y()));
    float magnitude = 8.0f * gameTime;
    Vector target = Vector(angle, magnitude);

    // ajusta velocidade atual na direção do alvo
    speed->Add(target);

    // limita a magnitude da velocidade para impedir 
    // que ela cresça indefinidamente pelo soma vetorial
    if (speed->Magnitude() > 2.5f)
        speed->ScaleTo(2.5f);

    // move o objeto pelo seu vetor velocidade
    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);

    // aplica fator de escala
    Scale(1.0f + factor * gameTime);

    // movimenta objeto pelo seu vetor velocidade
    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);
}

// -------------------------------------------------------------------------------