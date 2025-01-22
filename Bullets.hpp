#ifndef BULLETS_HEADER
#define BULLETS_HEADER

using namespace sf;
#include "math.h"

// Bullets!
// They turn into either blood splats when impact on a playermodel or a spark when hitting a collision barrier
// neat effect :)
struct Bullet : public RectangleShape {
    Bullet(float x, float y, float angle, float damage = 0, float velocity = 15) : RectangleShape(Vector2f(5, 5)) {
        Damage = damage;
        Velocity = velocity;
        this->setFillColor(Color::Yellow);
        this->setPosition(x, y);
        this->setOrigin(2.5, 2.5);
        this->setRotation(rand() % 361);
        Angle = angle;

        ImpactTex = new Texture;
        ImpactBuffer = new SoundBuffer;
    }

    void Routine() {
        float x = (cos(Angle));
        float y = (sin(Angle));

        if (ImpactType == "none") this->move(Velocity * x, Velocity * y);
        else {
            if (!ImpactPlayed) {
                if (ImpactType == "blood") {
                    ImpactTex->loadFromFile("Engine\\Assets\\BloodHit.png");
                    ImpactBuffer->loadFromFile("Engine\\Sounds\\Bullet_FleshHit.ogg");
                }
                if (ImpactType == "object") {
                    if (rand() % 2 == 0) ImpactTex->loadFromFile("Engine\\Assets\\ImpactHit1.png");
                    else ImpactTex->loadFromFile("Engine\\Assets\\ImpactHit2.png");

                    int r = rand() % 4;
                    if (r == 0) ImpactBuffer->loadFromFile("Engine\\Sounds\\Bullet_ImpactHit1.ogg");
                    if (r == 1) ImpactBuffer->loadFromFile("Engine\\Sounds\\Bullet_ImpactHit2.ogg");
                    if (r == 2) ImpactBuffer->loadFromFile("Engine\\Sounds\\Bullet_ImpactHit3.ogg");
                    if (r == 3) ImpactBuffer->loadFromFile("Engine\\Sounds\\Bullet_ImpactHit4.ogg");
                }

                ImpactPlayed = true;
                Impact.setBuffer(*ImpactBuffer);
                Impact.play();
            }

            float a = this->getFillColor().a;
            if (a > 0) this->setFillColor(Color(255, 255, 255, a - 5));
            else Destroy = true;

            if (ImpactType == "blood") {
                this->setSize(Vector2f(258, 208));
                this->setOrigin(179, 104);

            }

            if (ImpactType == "object") {
                this->setSize(Vector2f(64, 64));
                this->setOrigin(32, 32);
            }

            this->setTexture(ImpactTex);
        }
    }

    bool Destroy = false, ImpactPlayed = false;
    float Angle;
    Clock Lifetime;
    Texture* ImpactTex;
    SoundBuffer* ImpactBuffer;
    Sound Impact;

    string ImpactType = "none";

    float Damage;
    float Velocity;
};

#endif // BULLETS_HEADER
