#ifndef AI_HEADER
#define AI_HEADER

#include "Player.hpp"

/*
    Enemies have 3 states: passive, investigating and aggressive

    Passive: roam around their spawn area
    Investigating: move towards heard gunfire then stay on guard for a little while
    Aggressive: chase and shoot the player
*/

// Enemy entity and AI
struct Enemy : public PlayerModel {
    Enemy(string name, VOBuffer& vo, Vector2f pos) : PlayerModel(name, vo) {
        this->setPosition(pos);
        BasePosition = pos;
        Stats.EquipWeapon(GlobalWeapons[rand() % GlobalWeapons.size()]);
        HearRadius.setRadius(400);
        HearRadius.setOrigin(400, 400);

        ShootRadius.setRadius(600);
        ShootRadius.setOrigin(600, 600);

        this->Status.setFont(BarlowB);
        this->Status.setCharacterSize(25);
        OutlineText(this->Status);
    }

    void SetState(string state, string look) {
        Stats.Holding = &Stats.Weapon1;
        this->AnimState = state;
        this->AnimLook = look;
    }

    void AI(PlayerModel& model, vector<Bullet>& bullets) {
        this->Status.setString(AlertState);
        // Logic states
        float aiTime = AIClock.getElapsedTime().asSeconds();

        if (!Shooting) {
            if (State == "idle" || State == "idleGun") {
                int idleChance = 11;
                if (AlertState == "investigate") idleChance = 6;
                if (aiTime > 3) {
                    int randIdle = rand() % idleChance;
                    if (randIdle > 4) {
                        State = "move";
                        xGoal = rand() % 2501;
                        yGoal = rand() % 2501;

                        if (rand() % 2 == 0) xGoal = -xGoal;
                        if (rand() % 2 == 0) yGoal = -yGoal;

                        xGoal += BasePosition.x;
                        yGoal += BasePosition.y;
                        AlertState = "passive";
                    }
                    else {
                        if (Look == "Left") Look = "Right";
                        else Look = "Left";
                        AIClock.restart();
                    }
                }
            }
        }

        if (State == "move") {
            AIClock.restart();
            Vector2f pos = this->getPosition();
            if (pos.x < xGoal - 5) {
                this->move(5, 0);
                Look = "Right";
                DoneX = false;
            }
            else if (pos.x > xGoal + 5) {
                this->move(-5, 0);
                Look = "Left";
                DoneX = false;
            }
            else DoneX = true;

            if (pos.y < yGoal - 5) {
                this->move(0, 5);
                DoneY = false;
            }
            else if (pos.y > yGoal + 5) {
                this->move(0, -5);
                DoneY = false;
            }
            else DoneY = true;

            if (DoneX && DoneY) {
                if (Stats.Holding->Name == "") State = "idle";
                else State = "idleGun";
                DoneX = false;
                DoneY = false;
            }
        }
        else if (Shooting) {
            float shotTime = ShotDelay.getElapsedTime().asSeconds();

            float Delay = 0.2;
            if (Stats.Holding->Type == "shotgun" || Stats.Holding->Type == "special" || Stats.Holding->Type == "lever") Delay = 0.05; // ai weapon nerf
            if (Stats.Holding->Type == "sniper") Delay = 0.025;

            if (shotTime < Delay && this->ShootClock.getElapsedTime().asSeconds() > 0.05) {
                this->WeaponBuf.loadFromFile("Engine\\Sounds\\" + Stats.Holding->Name + "_Fire.ogg"); // temporary
                this->WeaponSnd.setBuffer(WeaponBuf);
                this->WeaponSnd.play();
                Vector2f pos;
                float Angle;
                if (Look == "Left") {
                    this->Weapon.setRotation(20);
                    pos = Vector2f(this->getPosition().x + Stats.Holding->OffsetLeft.x - 20, this->getPosition().y + Stats.Holding->OffsetLeft.y);
                }
                if (Look == "Right") {
                    this->Weapon.setRotation(-20);
                    pos = Vector2f(this->getPosition().x + Stats.Holding->OffsetRight.x + 20, this->getPosition().y + Stats.Holding->OffsetRight.y);
                }

                float damage = 0;
                string a = Stats.Holding->Type;

                if (a == "shotgun") damage = 10;
                if (a == "lever") damage = 30;
                if (a == "ar") damage = 25;
                if (a == "special") damage = 1;
                if (a == "sniper") damage = 90;

                Angle = atan2(model.getPosition().y - this->getPosition().y, model.getPosition().x - this->getPosition().x);

                if (a == "shotgun") { // shotgun spread
                    bullets.push_back(Bullet(pos.x, pos.y, Angle + ((rand() % 31) * 3.14/180), damage));
                    bullets.push_back(Bullet(pos.x, pos.y, Angle - ((rand() % 31) * 3.14/180), damage));
                }

                float offset = (rand() % 6) * 3.14/180;
                if (rand() % 2 == 0) offset = -offset;

                float velo = 15;
                if (a == "sniper") velo = 30;
                bullets.push_back(Bullet(pos.x, pos.y, Angle + offset, damage, velo));

                Stats.Holding->Loaded--;

                this->ShotRadius.setRadius(900);

                this->RecoilOffset = -20;
                this->ShootClock.restart();
                this->HeldDown = true;
            }

            if (shotTime > 3) ShotDelay.restart();
        }

        if (this->getGlobalBounds().intersects(model.ShotRadius.getGlobalBounds()) && AlertState != "aggressive") { // Investigate shots
            State = "move";
            AlertState = "investigate";
            xGoal = model.getPosition().x;
            yGoal = model.getPosition().y;
            DoneX = false;
            DoneY = false;
        }
        else if (HearRadius.getGlobalBounds().intersects(model.getGlobalBounds())) { // Sneak up on enemy until spotted
            if (Look == "Left" && model.getPosition().x < this->getPosition().x || Look == "Right" && model.getPosition().x > this->getPosition().x) {
                State = "move";
                AlertState = "aggressive";
                AIClock.restart();
            }
        }

        if (AlertState == "aggressive") {
            if (!VOSpotted) {
                int r = rand() % 3;
                if (r == 0) this->Reload1.play();
                if (r == 1) this->Reload2.play();
                if (r == 2) this->Reload3.play();

                VOSpotted = true;
            }

            xGoal = model.getPosition().x;
            yGoal = model.getPosition().y;

            Vector2f pos = this->getPosition();
            int bounds = model.getGlobalBounds().height/4;

            if (xGoal < this->getPosition().x) Look = "Left";
            else Look = "Right";

            if (model.getGlobalBounds().intersects(ShootRadius.getGlobalBounds())) {
                Shooting = true;
                State = "idleGun";
                AggroCooldown.restart();
            }
            else { // Evasion
                Shooting = false;
                State = "move";
                if (AggroCooldown.getElapsedTime().asSeconds() > 10) {
                    AlertState = "investigating";
                    State = "move";
                }
            }
        }
        else {
            AggroCooldown.restart();
            Shooting = false;
        }

        SetState(State, Look);

        // Senses
        Vector2f pos = this->getPosition();
        FloatRect bounds = this->getGlobalBounds();
        HearRadius.setPosition(pos.x + bounds.width/2, pos.y + bounds.height/2);
        ShootRadius.setPosition(HearRadius.getPosition());
    }

    Vector2f BasePosition;

    PlayerStats Stats;
    Clock AIClock;
    Clock AggroCooldown;

    string State = "idleGun";
    string Look = "Right";
    float xGoal = 0, yGoal = 0;
    bool DoneX = false, DoneY = false, Shooting = false;
    string AlertState = "passive";
    CircleShape HearRadius, ShootRadius;

    Clock ShotDelay;

    bool LootDropped = false;
    bool VOSpotted = false;
};

#endif
