#include "GeoWars.h"
#include "Blue.h"
#include "Delay.h"
#include "Engine.h"
#include "Mothership.h"
#include "Orange.h"
#include "Resources.h"

Player *GeoWars::player = nullptr;
Audio *GeoWars::audio = nullptr;
Scene *GeoWars::scene = nullptr;
bool GeoWars::viewHUD = false;
Controller *GeoWars::gamepad = nullptr;
bool GeoWars::gamepadOn = false;
uint GeoWars::xboxPlayer = PLAYER1;

void GeoWars::Init()
{
    gamepad = new Controller();
    gamepadOn = gamepad->XboxInitialize(xboxPlayer);

    audio = new Audio();
    audio->Add(THEME, "Resources/Theme.wav");
    audio->Add(FIRE, "Resources/Fire.wav");
    audio->Add(HITWALL, "Resources/Hitwall.wav");
    audio->Add(EXPLODE, "Resources/Explode.wav");
    audio->Add(START, "Resources/Start.wav");

    audio->Volume(FIRE, 0.2f);
    audio->Volume(START, 0.8f);

    backg = new Background("Resources/Space.jpg");
    player = new Player();
    scene = new Scene();

    hud = new Hud();

    scene->Add(player, MOVING);
    scene->Add(new Mothership(player), MOVING);
    scene->Add(new Blue(player), MOVING);
    scene->Add(new Orange(player), MOVING);

    float difx = (game->Width() - window->Width()) / 2.0f;
    float dify = (game->Height() - window->Height()) / 2.0f;

    viewport.left = 0.0f + difx;
    viewport.right = viewport.left + window->Width();
    viewport.top = 0.0f + dify;
    viewport.bottom = viewport.top + window->Height();
}

void GeoWars::Update()
{
    if (window->KeyDown(VK_ESCAPE))
        window->Close();

    scene->Update();
    scene->CollisionDetection();

    if (window->KeyPress('B'))
        viewBBox = !viewBBox;

    if (window->KeyPress('H'))
        viewHUD = !viewHUD;

    if (gamepadOn)
    {
        gamepad->XboxUpdateState(xboxPlayer);
    }

    viewport.left = player->X() - window->CenterX();
    viewport.right = player->X() + window->CenterX();
    viewport.top = player->Y() - window->CenterY();
    viewport.bottom = player->Y() + window->CenterY();

    if (viewport.left < 0)
    {
        viewport.left = 0;
        viewport.right = window->Width();
    }
    else if (viewport.right > game->Width())
    {
        viewport.left = game->Width() - window->Width();
        viewport.right = game->Width();
    }

    if (viewport.top < 0)
    {
        viewport.top = 0;
        viewport.bottom = window->Height();
    }
    else if (viewport.bottom > game->Height())
    {
        viewport.top = game->Height() - window->Height();
        viewport.bottom = game->Height();
    }
}

void GeoWars::Draw()
{
    backg->Draw(viewport);

    scene->Draw();

    if (viewHUD)
        hud->Draw();

    if (viewBBox)
        scene->DrawBBox();
}

void GeoWars::Finalize()
{
    delete audio;
    delete hud;
    delete scene;
    delete backg;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow)
{
    Engine *engine = new Engine();

    engine->window->Mode(BORDERLESS);
    engine->window->Color(0, 0, 0);
    engine->window->Title("GeoWars");
    engine->window->Icon(IDI_ICON);
    engine->window->Cursor(IDC_CURSOR);
    engine->window->HideCursor(true);

    Game *game = new GeoWars();

    game->Size(3840, 2160);

    int status = engine->Start(game);

    delete engine;
    return status;
}
