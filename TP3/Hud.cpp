#include "Hud.h"
#include "GeoWars.h"
#include "Util.h"

Hud::Hud()
{
    hpTileSet = new TileSet("Resources/WIP/Hp.png", 1, 2);
    for (int i = 0; i < 5; i++)
        hpBar[i] = new Animation(hpTileSet, 0.f, false);

    font = new Font("Resources/Tahoma14b.png");
    font->Spacing("Resources/Tahoma14b.dat");
}

Hud::~Hud()
{
    delete font;
    delete hpTileSet;
    for (int i = 0; i < 5; i++)
        delete hpBar[i];
}

void Hud::Update()
{
    for (int i = 0; i < 5; i++)
        hpBar[i]->Frame(GeoWars::player->Hp() > i ? 0 : 1);
}

void Hud::Draw()
{
    switch (GeoWars::state)
    {
    case PLAYING:
        for (int i = 0; i < 5; i++)
            hpBar[i]->Draw(game->viewport.left + 32.f + (i * 32.f), game->viewport.top + 40.f, LAYER_HUD);

        text.str("");
        text << "Score: " << GeoWars::EnemyCount();
        font->Draw(60, 1060, text.str(), {0.7f, 0.7f, 0.7f, 1.0f}, 0.0f, 2.0f);
        break;
    case GAMEOVER:
        text.str("");
        text << "GAME OVER";
        font->Draw(860, 500, text.str(), { 0.7f, 0.7f, 0.7f, 1.0f }, 0.0f, 5.0f);
        text.str("");
        text << "Score: " << GeoWars::EnemyCount();
        font->Draw(920, 960, text.str(), {0.7f, 0.7f, 0.7f, 1.0f}, 0.0f, 3.0f);
        break;
    }
}
