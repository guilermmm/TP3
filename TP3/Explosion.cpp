/**********************************************************************************
// WallHit (C�digo Fonte)
//
// Cria��o:     03 Ago 2019
// Atualiza��o: 01 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Efeito de explos�o de fa�scas usando sistema de part�culas
//
**********************************************************************************/

#include "GeoWars.h"
#include "Explosion.h"
#include "Random.h"

// ---------------------------------------------------------------------------------

Explosion::Explosion(float pX, float pY)
{
    // n�mero aleat�rio entre 0 e 1
    RandF color{ 0, 1 };

    // configura emissor de part�culas
    Generator explosion;
    explosion.imgFile = "Resources/Spark.png";    // arquivo de imagem
    explosion.angle = 0.0f;                     // dire��o da explos�o
    explosion.spread = 360.0f;                   // espalhamento em graus
    explosion.lifetime = 1.0f;                     // tempo de vida em segundos
    explosion.frequency = 0.000f;                   // tempo entre gera��o de novas part�culas
    explosion.percentToDim = 0.6f;                  // desaparece ap�s 60% da vida
    explosion.minSpeed = 25.0f;                    // velocidade m�nima das part�culas
    explosion.maxSpeed = 250.0f;                   // velocidade m�xima das part�culas
    explosion.color.r = color.Rand();             // cor da part�cula entre 0 e 1
    explosion.color.g = color.Rand();             // cor da part�cula entre 0 e 1
    explosion.color.b = color.Rand();             // cor da part�cula entre 0 e 1
    explosion.color.a = 1.0f;                     // transpar�ncia da part�cula

    // cria sistema de part�culas
    sparks = new Particles(explosion);

    // gera 25 part�culas na posi��o de contato
    sparks->Generate(pX, pY, 25);
    type = EXPLOSION;
}

// ---------------------------------------------------------------------------------

Explosion::~Explosion()
{
    delete sparks;
}

// -------------------------------------------------------------------------------

void Explosion::Update()
{
    // atualiza posi��o de cada part�cula
    sparks->Update(gameTime);

    // remove da cena quando todas as part�culas est�o inativas
    if (sparks->Inactive())
        GeoWars::scene->Delete();
}

// -------------------------------------------------------------------------------