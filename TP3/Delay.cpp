
#include "Delay.h"
#include "GeoWars.h"
#include "Hud.h"

Delay::Delay()
{
    logo = new Sprite("Resources/Logo.png");
    timer.Start();
    notPlayed = true;
}

Delay::~Delay()
{
    delete logo;
}

void Delay::Update()
{
    if (notPlayed && timer.Elapsed(2.0f))
    {

        GeoWars::audio->Play(START);
        notPlayed = false;
    }

    if (timer.Elapsed(6.0f))
    {

        GeoWars::audio->Play(THEME, true);
        GeoWars::viewHUD = true;
        GeoWars::scene->Delete();
    }
}

void Delay::Draw()
{
    logo->Draw(game->viewport.left + window->CenterX(), game->viewport.top + window->CenterY(), Layer::FRONT);
}
