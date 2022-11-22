#include "GeoWars.h"
#include "EnemyShip.h"
#include "Engine.h"
#include "Kamikaze.h"
#include "Mothership.h"
#include "Resources.h"

Player *GeoWars::player = nullptr;
Audio *GeoWars::audio = nullptr;
Scene *GeoWars::scene = nullptr;
Controller *GeoWars::gamepad = nullptr;
bool GeoWars::gamepadOn = false;
uint GeoWars::xboxPlayer = PLAYER1;
GameState GeoWars::state = TITLESCREEN;
int GeoWars::enemyCount = 0;

void GeoWars::Init()
{
    gamepad = new Controller();
    gamepadOn = gamepad->XboxInitialize(xboxPlayer);

    enemyCount = 0;

    audio = new Audio();
    audio->Add(THEME, "Resources/audio/theme.wav");
    audio->Add(FIRE, "Resources/audio/laser.wav");
    audio->Add(ENEMYFIRE, "Resources/audio/laser2.wav");
    audio->Add(ENEMYSPAWN, "Resources/audio/spawn.wav");
    audio->Add(HITWALL, "Resources/audio/laserdot.wav");
    audio->Add(EXPLODE, "Resources/audio/radioactivity.wav");
    audio->Add(SGAMEOVER, "Resources/audio/gameover.wav");

    audio->Volume(FIRE, 0.1f);
    audio->Volume(ENEMYFIRE, 0.2f);
    audio->Volume(ENEMYSPAWN, 0.3f);
    audio->Volume(HITWALL, 0.2f);
    audio->Volume(EXPLODE, 0.1f);
    audio->Volume(THEME, 0.8f);

    audio->Play(THEME, 0.8f, true);

    backg = new Background("Resources/WIP/background.png");
    motherShipImg = new Image("Resources/WIP/MotherShip.png");
    enemyShipImg = new Image("Resources/WIP/EnemyShip.png");
    kamikazeImg = new Image("Resources/WIP/Kamikaze.png");

    titleScreen = new Sprite("Resources/WIP/TitleScreen.png");
    playButton = new Sprite("Resources/WIP/TextPlay.png");
    quitButton = new Sprite("Resources/WIP/TextQuit.png");
    gameOverScreen = new Sprite("Resources/WIP/GameOver.png");
    restartButton = new Sprite("Resources/WIP/TextRestart.png");

    hud = new Hud();

    Setup();

    float difx = (game->Width() - window->Width()) / 2.0f;
    float dify = (game->Height() - window->Height()) / 2.0f;

    viewport.left = 0.0f + difx;
    viewport.right = viewport.left + window->Width();
    viewport.top = 0.0f + dify;
    viewport.bottom = viewport.top + window->Height();
}

void GeoWars::Setup()
{
    scene = new Scene();
    player = new Player();
    scene->Add(player, MOVING);

    wave = FIRST;
    firstCtrl = true;
    secondCtrl = true;
    thirdCtrl = true;

    enemyCount = 0;

    posX = RandF{0, game->Width()};
    posY = RandF{0, game->Height()};

    /*scene->Add(new Mothership(motherShipImg, player), MOVING);
    scene->Add(new EnemyShip(enemyShipImg, player), MOVING);*/
}

void GeoWars::Cleanup()
{
    delete scene;
}

void GeoWars::Update()
{
    if (window->KeyDown(VK_ESCAPE))
        window->Close();

    if (gamepadOn)
    {
        gamepad->XboxUpdateState(xboxPlayer);

        if (gamepad->XboxButton(ButtonY))
            window->Close();
    }

    if (playScaleUp)
    {
        playScale += 0.2f * gameTime;
        if (playScale >= 1.1f)
            playScaleUp = false;
    }
    else
    {
        playScale -= 0.2f * gameTime;
        if (playScale <= 1.0f)
            playScaleUp = true;
    }
    if (quitScaleUp)
    {
        quitScale += 0.2f * gameTime;
        if (quitScale >= 1.1f)
            quitScaleUp = false;
    }
    else
    {
        quitScale -= 0.2f * gameTime;
        if (quitScale <= 1.0f)
            quitScaleUp = true;
    }
    if (restartScaleUp)
    {
        restartScale += 0.2f * gameTime;
        if (restartScale >= 1.1f)
            restartScaleUp = false;
    }
    else
    {
        restartScale -= 0.2f * gameTime;
        if (restartScale <= 1.0f)
            restartScaleUp = true;
    }

    switch (state)
    {
    case TITLESCREEN: {
        viewport.left = 0.0f;
        viewport.right = viewport.left + window->Width();
        viewport.top = 0.0f;
        viewport.bottom = viewport.top + window->Height();

        if (window->KeyDown(VK_RETURN) || (gamepadOn && gamepad->XboxButton(ButtonA)))
            state = PLAYING;

        break;
    }
    case GAMEOVER: {
        viewport.left = 0.0f;
        viewport.right = viewport.left + window->Width();
        viewport.top = 0.0f;
        viewport.bottom = viewport.top + window->Height();

        if (window->KeyDown(VK_RETURN) || (gamepadOn && gamepad->XboxButton(ButtonA)))
        {
            Cleanup();
            Setup();

            state = PLAYING;
        }

        break;
    }
    case PLAYING:
    default: {
        scene->Update();
        scene->CollisionDetection();

        if (window->KeyPress('B'))
            viewBBox = !viewBBox;

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

        break;
    }
    }

    if (enemyCount >= 5)
        wave = SECOND;
    if (enemyCount >= 16)
        wave = THIRD;
    if (enemyCount >= 26)
        wave = NONSTOP;

    switch (wave)
    {
    case FIRST:
        if (firstCtrl)
        {
            firstCtrl = false;
            for (int i = 0; i < 5; i++)
            {
                scene->Add(new Kamikaze(kamikazeImg, player->X() + (i % 2 ? 800.f : -800.f), posY.Rand()), MOVING);
            }
        }
        break;
    case SECOND:
        if (secondCtrl)
        {
            secondCtrl = false;
            for (int i = 0; i < 10; i++)
            {
                scene->Add(new Kamikaze(kamikazeImg, player->X() + (i % 2 ? 800.f : -800.f), posY.Rand()), MOVING);
            }
            scene->Add(new EnemyShip(enemyShipImg, player), MOVING);
        }
        break;
    case THIRD:
        if (thirdCtrl)
        {
            thirdCtrl = false;
            for (int i = 0; i < 5; i++)
            {
                scene->Add(new Kamikaze(kamikazeImg, player->X() + (i % 2 ? 800.f : -800.f), posY.Rand()), MOVING);
            }
            for (int i = 0; i < 2; i++)
            {
                scene->Add(new EnemyShip(enemyShipImg, player), MOVING);
            }
            scene->Add(new Mothership(motherShipImg, player), MOVING);
        }
        break;
    case NONSTOP:
        if (shipCd.Ready())
        {
            shipCd.Restart();
            scene->Add(new EnemyShip(enemyShipImg, player), MOVING);
        }
        if (motherShipCd.Ready())
        {
            motherShipCd.Restart();
            scene->Add(new Mothership(motherShipImg, player), MOVING);
        }
        break;
    default:
        break;
    }

    hud->Update();

    kamikazeCd.Update(gameTime);
    shipCd.Update(gameTime);
    motherShipCd.Update(gameTime);
}

void GeoWars::Draw()
{
    switch (state)
    {
    case TITLESCREEN:
        titleScreen->Draw(window->CenterX(), window->CenterY(), LAYER_MENU);
        playButton->Draw(window->CenterX() - 320.f, window->CenterY() + 200.f, LAYER_BUTTON, playScale);
        quitButton->Draw(window->CenterX() + 320.f, window->CenterY() + 200.f, LAYER_BUTTON, quitScale);
        break;
    case GAMEOVER:
        gameOverScreen->Draw(window->CenterX(), window->CenterY(), LAYER_MENU);
        restartButton->Draw(window->CenterX() - 320.f, window->CenterY() + 200.f, LAYER_BUTTON, restartScale);
        quitButton->Draw(window->CenterX() + 320.f, window->CenterY() + 200.f, LAYER_BUTTON, quitScale);
        hud->Draw();
        break;
    case PLAYING:
    default:
        backg->Draw(viewport);
        scene->Draw();

        hud->Draw();

        if (viewBBox)
            scene->DrawBBox();
        break;
    }
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
    engine->window->HideCursor(true);

    Game *game = new GeoWars();

    game->Size(3840, 2160);

    int status = engine->Start(game);

    delete engine;
    return status;
}
