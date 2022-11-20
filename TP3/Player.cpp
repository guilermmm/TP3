
#include "Player.h"
#include "GeoWars.h"
#include "Missile.h"
#include "Util.h"

Player::Player()
{
    sprite = new Sprite("Resources/WIP/Player.png");
    basicWeapon = new Sprite("Resources/WIP/Weapon_Basic.png");
    speed = new Vector(90.0f, 0.0f);
    weaponAngle = 90.0f;
    BBox(new Circle(18.0f));
    MoveTo(game->CenterX(), game->CenterY());
    type = PLAYER;

    Generator emitter;
    emitter.imgFile = "Resources/Spark.png";
    emitter.angle = 270.0f;
    emitter.spread = 50;
    emitter.lifetime = 0.3f;
    emitter.frequency = 0.010f;
    emitter.percentToDim = 0.6f;
    emitter.minSpeed = 50.0f;
    emitter.maxSpeed = 100.0f;
    emitter.color.r = 1.0f;
    emitter.color.g = 1.0f;
    emitter.color.b = 1.0f;
    emitter.color.a = 1.0f;

    tail = new Particles(emitter);
}

Player::~Player()
{
    delete sprite;
    delete basicWeapon;
    delete tail;
}

void Player::Move(Vector &&v)
{
    speed->Add(v);

    if (speed->Magnitude() > 10.0f)
        speed->ScaleTo(10.0f);
}

void Player::Update()
{
    float accel = 40.0f * gameTime;

    if (window->KeyDown(VK_RIGHT) && window->KeyDown(VK_UP))
        Move(Vector(45.0f, accel));
    else if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_UP))
        Move(Vector(135.0f, accel));
    else if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_DOWN))
        Move(Vector(225.0f, accel));
    else if (window->KeyDown(VK_RIGHT) && window->KeyDown(VK_DOWN))
        Move(Vector(315.0f, accel));
    else if (window->KeyDown(VK_RIGHT))
        Move(Vector(0.0f, accel));
    else if (window->KeyDown(VK_LEFT))
        Move(Vector(180.0f, accel));
    else if (window->KeyDown(VK_UP))
        Move(Vector(90.0f, accel));
    else if (window->KeyDown(VK_DOWN))
        Move(Vector(270.0f, accel));
    else if (speed->Magnitude() > 0.1f)
        Move(Vector(speed->Angle() + 180.0f, 5.0f * gameTime));
    else
        speed->ScaleTo(0.0f);

    if (window->KeyPress(VK_SPACE))
    {
        if (fireRate.Ready())
        {
            GeoWars::audio->Play(FIRE);
            GeoWars::scene->Add(new Missile(), STATIC);
            fireRate.Restart();
        }
    }

    if (GeoWars::gamepadOn)
    {
        Controller *&gamepad = GeoWars::gamepad;
        long deadzone = 0.2f * 32767.0f;

        if (gamepad->XboxAnalog(ThumbLX) > deadzone)
            Move(Vector(0.0f, accel));
        else if (gamepad->XboxAnalog(ThumbLX) < -deadzone)
            Move(Vector(180.0f, accel));

        if (gamepad->XboxAnalog(ThumbLY) > deadzone)
            Move(Vector(90.0f, accel));
        else if (gamepad->XboxAnalog(ThumbLY) < -deadzone)
            Move(Vector(270.0f, accel));

        Vector weapon = Vector(0.f, 0.f);

        float rx = gamepad->XboxAnalog(ThumbRX);
        if (rx > deadzone || rx < -deadzone)
            weapon.Add(Vector(0.0f, rx));

        float ry = gamepad->XboxAnalog(ThumbRY);
        if (ry > deadzone || ry < -deadzone)
            weapon.Add(Vector(90.0f, ry));

        if (weapon.Magnitude() > 0.1f)
        {
            weaponAngle = weapon.Angle();
            if (fireRate.Ready())
            {
                GeoWars::audio->Play(FIRE);
                GeoWars::scene->Add(new Missile(), STATIC);
                fireRate.Restart();
            }
        }
    }

    Translate(speed->XComponent() * 50.0f * gameTime, -speed->YComponent() * 50.0f * gameTime);

    tail->Config().angle = speed->Angle() + 180;
    tail->Generate(x - 10 * cos(speed->Radians()), y + 10 * sin(speed->Radians()));
    tail->Update(gameTime);

    fireRate.Update(gameTime);

    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);
}

void Player::Draw()
{
    sprite->Draw(x, y, LAYER_PLAYER, 1.0f, -speed->Angle() + 90.0f);
    basicWeapon->Draw(x, y, LAYER_PLAYER_BASIC_WEAPON, 1.0f, -weaponAngle + 90.0f);
    tail->Draw(Layer::LOWER, 1.0f);
}
