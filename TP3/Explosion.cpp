#include "Explosion.h"
#include "GeoWars.h"
#include "Random.h"

Explosion::Explosion(float pX, float pY)
{
    RandF color{0, 1};

    Generator explosion;
    explosion.imgFile = "Resources/WIP/Particle.png";
    explosion.angle = 0.0f;
    explosion.spread = 360.0f;
    explosion.lifetime = 1.0f;
    explosion.frequency = 0.000f;
    explosion.percentToDim = 0.6f;
    explosion.minSpeed = 25.0f;
    explosion.maxSpeed = 250.0f;
    explosion.color.r = color.Rand();
    explosion.color.g = color.Rand();
    explosion.color.b = color.Rand();
    explosion.color.a = 1.0f;

    sparks = new Particles(explosion);

    sparks->Generate(pX, pY, 50);
    type = EXPLOSION;
}

Explosion::~Explosion()
{
    delete sparks;
}

void Explosion::Update()
{
    sparks->Update(gameTime);

    if (sparks->Inactive())
        GeoWars::scene->Delete();
}
