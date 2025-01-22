// Main player code (movement, animation, some AI stuff)

#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "Engine.hpp"
#include "Bullets.hpp"
using namespace sf;
using namespace std;

// Item base
struct Item {
    Item(string name = "", string type = "", int price = 0, string hint = "", Vector2f offLeft = Vector2f(0, 0), Vector2f offRight = Vector2f(0, 0)) {
        Name = name;
        Type = type;
        Price = price;
        SalePrice = price * 0.9;
        OffsetLeft = offLeft;
        OffsetRight = offRight;
        Hint = hint;
    }

    string Name, Type, Hint;
    int Price, SalePrice;
    Vector2f OffsetLeft, OffsetRight;
    bool Insured = false;
};

// Weapon base
struct Weapon : public Item {
    void Equip(Item item, bool ammo = false, int load = 0, int res = 0) {
        string name = item.Name;
        if (name == "AR-12") {
            Loaded = 30;
            Reserve = 100;
        }
        if (name == "Auto-6" || name == "BnL Repeater" || name == "HCR200") {
            Loaded = 6;
            Reserve = 36;
        }
        if (name == "Trumpet") {
            Loaded = 1;
            Reserve = 6;
        }

        Capacity = Loaded;

        if (ammo) {
            Loaded = load;
            Reserve = res;
        }

        this->Name = item.Name;
        this->Type = item.Type;
        this->Price = item.Price;
        this->OffsetLeft = item.OffsetLeft;
        this->OffsetRight = item.OffsetRight;
        this->SalePrice = item.SalePrice;
        this->Hint = item.Hint;
    }

    void Remove() {
        Loaded = 0;
        Reserve = 0;
        Capacity = 0;
        this->Name = "";
        this->Type = "";
        this->Price = 0;
        this->SalePrice = 0;
    }

    int Loaded;
    int Reserve;
    int Capacity;
};

// Character Stats (caps, rads, etc.)
struct PlayerStats {
    void AddToBackpack(Item item) { Backpack.push_back(item); }
    void RemoveFromBackpack(int num) { Backpack.erase(Backpack.begin() + num); }

    void EquipWeapon(Item item, bool ammo = false, int load = 0, int res = 0) {
        if (Weapon1.Name == "") Weapon1.Equip(item, ammo, load, res);
        else if (Weapon2.Name == "") Weapon2.Equip(item, ammo, load, res);
    }

    void RemoveWeapon(int num) {
        if (num == 0) Weapon1.Remove();
        if (num == 1) Weapon2.Remove();
    }

    void Routine() {
        if (SwapClock.getElapsedTime().asSeconds() > 0.5) {
            if (Keyboard::isKeyPressed(Keyboard::Tab) || Y()) {
                ControllerHoldPrimary = !ControllerHoldPrimary;
                if (ControllerHoldPrimary) Holding = &Weapon1;
                else Holding = &Weapon2;
                SwapClock.restart();
            }
        }
    }

    Weapon Weapon1;
    Weapon Weapon2;
    vector<Item> Backpack;
    int Health = 100;
    int Caps = 0;
    int Rads = 0;

    int Level = 0;
    int CurrentXP = 0;

    Weapon* Holding = &Weapon1;
    bool ControllerHoldPrimary = true;
    Clock SwapClock;
};

// Voice line set, mainly to make it so that we dont load new VOs every time we make an enemy
struct VOBuffer {
    VOBuffer(string model) {
        string dir = "Engine\\Assets\\" + model + "\\";
        Reload1.loadFromFile(dir + "Reload1.ogg");
        Reload2.loadFromFile(dir + "Reload2.ogg");
        Reload3.loadFromFile(dir + "Reload3.ogg");
        Hurt1.loadFromFile(dir + "Hurt1.ogg");
        Hurt2.loadFromFile(dir + "Hurt2.ogg");
        Hurt3.loadFromFile(dir + "Hurt3.ogg");
        Dead1.loadFromFile(dir + "Dead1.ogg");
        Dead2.loadFromFile(dir + "Dead2.ogg");
    }

    SoundBuffer Reload1, Reload2, Reload3;
    SoundBuffer Hurt1, Hurt2, Hurt3;
    SoundBuffer Dead1, Dead2;
};

// Player Model, handles shooting movement etc
struct PlayerModel : public Sprite {
    PlayerModel(string model, VOBuffer& vo) {
        string dir = "Engine\\Assets\\" + model + "\\";
        this->setPosition(0, 0);
        Sheet = new Texture;
        Sheet->loadFromFile(dir + "Sheet.png");
        this->setTextureRect(IntRect(0, 0, 162, 323));
        this->setTexture(*Sheet);

        ShotRadius.setFillColor(Color(255, 255, 0, 150));
        ShotRadius.setPointCount(360);

        Status.setCharacterSize(25);

        Reload1.setBuffer(vo.Reload1);
        Reload2.setBuffer(vo.Reload2);
        Reload3.setBuffer(vo.Reload3);

        Hurt1.setBuffer(vo.Hurt1);
        Hurt2.setBuffer(vo.Hurt2);
        Hurt3.setBuffer(vo.Hurt3);

        Dead1.setBuffer(vo.Dead1);
        Dead2.setBuffer(vo.Dead2);

        MuzzleTex.loadFromFile("Engine\\Assets\\MuzzleFlash.png");
        MuzzleFlash.setTexture(MuzzleTex);
        MuzzleFlash.setOrigin(30, 30);
    }

    void Routine(PlayerStats& stats, vector<Bullet>& bullets, RenderWindow* window = nullptr, View* view = nullptr, bool ai = false) {
        Status.setPosition(this->getPosition().x + 35, this->getPosition().y - 30);

        if (AnimLook == "Left") {
            Weapon.setPosition(this->getPosition().x + stats.Holding->OffsetLeft.x, this->getPosition().y + stats.Holding->OffsetLeft.y + RecoilOffset);
            Weapon.setScale(0.6, 0.6);
            MuzzleFlash.setPosition(this->getPosition().x + stats.Holding->OffsetLeft.x - 50, this->getPosition().y + stats.Holding->OffsetLeft.y);
        }
        else if (AnimLook == "Right") {
            Weapon.setPosition(this->getPosition().x + stats.Holding->OffsetRight.x, this->getPosition().y + stats.Holding->OffsetRight.y + RecoilOffset);
            Weapon.setScale(-0.6, 0.6);
            MuzzleFlash.setPosition(this->getPosition().x + stats.Holding->OffsetRight.x + 50, this->getPosition().y + stats.Holding->OffsetRight.y);
        }

        if (CurrentHolding != stats.Holding->Name) {
            CurrentHolding = stats.Holding->Name;
            if (CurrentHolding != "") WeaponTex.loadFromFile("Engine\\Assets\\Items\\" + CurrentHolding + ".png");
            Weapon.setOrigin(100, 39);
            Weapon.setTexture(WeaponTex);
        }

        // Move animation
        float AnimTickTime = 0.06;
        if (Sprinting) AnimTickTime = 0.04;
        if (AnimTick.getElapsedTime().asSeconds() > AnimTickTime) {
            if (AnimState == "dead") x = 0;
            if (AnimState == "move") { // Walking/Running
                RecoilOffset = 0;
                Weapon.setRotation(0);

                if (AnimAsc) x += 187;
                else x -= 187;

                if (x > 540) AnimAsc = false;
                if (x < 187) AnimAsc = true;

                if (AnimLook == "Left") this->setTextureRect(IntRect(x, 323, 180, 323));
                if (AnimLook == "Right") this->setTextureRect(IntRect(x + 180, 323, -180, 323));
            }

            AnimTick.restart();
        }

        if (AnimState == "idle") {
            if (AnimLook == "Left") this->setTextureRect(IntRect(0, 0, 162, 323));
            if (AnimLook == "Right") this->setTextureRect(IntRect(162, 0, -162, 323));
        }

        if (AnimState == "idleGun") {
            if (AnimLook == "Left") {
                this->setTextureRect(IntRect(181, 0, 175, 323));
            }
            else if (AnimLook == "Right") {
                this->setTextureRect(IntRect(356, 0, -175, 323));
            }
        }

        // Move left/right
        if (!ai) {
            if (!LB()) {
                if (Keyboard::isKeyPressed(Keyboard::A) || LStickLeft()) {
                    this->move(-MoveSpeed, 0);
                    AnimState = "move";
                }
                else if (Keyboard::isKeyPressed(Keyboard::D) || LStickRight()) {
                    this->move(MoveSpeed, 0);
                    AnimState = "move";
                }
                else if (!Keyboard::isKeyPressed(Keyboard::W) && !LStickUp() && !Keyboard::isKeyPressed(Keyboard::S) && !LStickDown()) { // idle with gun
                    if (stats.Holding->Name != "") AnimState = "idleGun"; // holding weapon idle
                    else AnimState = "idle"; // default idle
                }

                // move up or down
                if (Keyboard::isKeyPressed(Keyboard::W) || LStickUp()) {
                    this->move(0, -MoveSpeed);
                    AnimState = "move";
                }
                else if (Keyboard::isKeyPressed(Keyboard::S) || LStickDown()) {
                    this->move(0, MoveSpeed);
                    AnimState = "move";
                }
            }
            else {
                if (stats.Holding->Name != "") AnimState = "idleGun";
                else AnimState = "idle";
            }

            // Look towards mouse
            Vector2f mousePos = window->mapPixelToCoords(Mouse::getPosition(*window), *view);
            if (mousePos.x < this->getPosition().x + this->getGlobalBounds().width/2) AnimLook = "Left";
            else AnimLook = "Right";

            // Shooting
            if (!Reloading) {
                if (Mouse::isButtonPressed(Mouse::Left) || RB()) {
                    float cooldown;
                    string a = stats.Holding->Type;
                    if (a == "shotgun") cooldown = 0.3;
                    if (a == "lever") cooldown = 1;
                    if (a == "ar") cooldown = 0.1;
                    if (a == "special") cooldown = 1;
                    if (a == "sniper") cooldown = 1.8;

                    if (stats.Holding->Loaded > 0 && ShootClock.getElapsedTime().asSeconds() > cooldown && AnimState == "idleGun") {
                        if (stats.Holding->Type == "sniper" && !HeldDown || stats.Holding->Type == "shotgun" && !HeldDown || stats.Holding->Type == "lever" && !HeldDown || stats.Holding->Type == "ar" || stats.Holding->Type == "special" && !HeldDown) {
                            WeaponBuf.loadFromFile("Engine\\Sounds\\" + stats.Holding->Name + "_Fire.ogg");
                            WeaponSnd.setBuffer(WeaponBuf);
                            WeaponSnd.play();
                            Vector2f pos;
                            float Angle;
                            if (AnimLook == "Left") {
                                Weapon.setRotation(20);
                                pos = Vector2f(this->getPosition().x + stats.Holding->OffsetLeft.x - 20, this->getPosition().y + stats.Holding->OffsetLeft.y);
                            }
                            if (AnimLook == "Right") {
                                Weapon.setRotation(-20);
                                pos = Vector2f(this->getPosition().x + stats.Holding->OffsetRight.x + 20, this->getPosition().y + stats.Holding->OffsetRight.y);
                            }

                            float damage = 0; // got lazy
                            if (a == "shotgun") damage = 10;
                            if (a == "lever") damage = 30;
                            if (a == "ar") damage = 25;
                            if (a == "special") damage = 1;
                            if (a == "sniper") damage = 90;

                            Angle = (atan2(mousePos.y - pos.y, mousePos.x - pos.x));

                            if (a == "shotgun") { // shotgun spread
                                for (int i = 0; i != 2; i++) bullets.push_back(Bullet(pos.x, pos.y, Angle + ((rand() % 16) * 3.14/180), damage));
                            }

                            float offset = (rand() % 6) * 3.14/180;
                            if (rand() % 2 == 0) offset = -offset;

                            float velo = 15;
                            if (a == "sniper") velo = 30;
                            bullets.push_back(Bullet(pos.x, pos.y, Angle + offset, damage, velo));

                            stats.Holding->Loaded--;

                            ShotRadius.setRadius(900);

                            RecoilOffset = -20;
                            MuzzleFlash.setRotation(rand() % 361);
                            ShootClock.restart();
                            HeldDown = true;
                            Muzzle = true;
                        }
                    }
                    else Muzzle = false;
                }
                else HeldDown = false;

                if (stats.Holding->Loaded < stats.Holding->Capacity && stats.Holding->Reserve > 0) {
                    if (Keyboard::isKeyPressed(Keyboard::R) || X()) { // point to this, point to that
                        WeaponBuf.loadFromFile("Engine\\Sounds\\" + stats.Holding->Name + "_Reload.ogg");
                        WeaponSnd.setBuffer(WeaponBuf);
                        WeaponSnd.play();
                        Reloading = true;

                        if (rand() % 10 > 5) {
                            int r = rand() % 3;
                            while (r == LastReloadVO) r = rand() % 3;
                            if (r == 0) Reload1.play();
                            if (r == 1) Reload2.play();
                            if (r == 2) Reload3.play();
                            LastReloadVO = r;
                        }
                    }
                }
            }
            else {
                if (WeaponSnd.getStatus() == 0) {
                    if (stats.Holding->Reserve > 0 && stats.Holding->Loaded < stats.Holding->Capacity) {
                        int diff = stats.Holding->Capacity - stats.Holding->Loaded;
                        if (stats.Holding->Reserve > stats.Holding->Capacity) { // Reserve larger than Capacity
                            stats.Holding->Reserve -= diff;
                            stats.Holding->Loaded += diff;
                        }
                        else {
                            if (stats.Holding->Reserve >= diff) { // Reserve larger than the Difference
                                stats.Holding->Loaded += diff;
                                stats.Holding->Reserve -= diff;
                            }
                            else { // Out of ammo case
                                stats.Holding->Loaded += stats.Holding->Reserve;
                                stats.Holding->Reserve = 0;
                            }
                        }
                    }

                    Reloading = false;
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::LShift) || A()) {
                if (Stamina >= 20) {
                    Sprinting = true;
                    MoveSpeed = 7;
                }
            }
            else {
                Sprinting = false;
                MoveSpeed = 5;
            }
        }

        float stTime = StaminaClock.getElapsedTime().asSeconds();
        if (Sprinting) {
            if (stTime > 0.05) {
                Stamina--;
                StaminaClock.restart();
            }

            if (Stamina <= 0) {
                Sprinting = false;
            }
        }
        else if (Stamina < 100 && stTime > 0.05) {
            Stamina++;
            StaminaClock.restart();
        }

        Status.setString("Stamina: " + to_string((int)Stamina));

        if (AnimLook == "Left") if (Weapon.getRotation() > 0) Weapon.rotate(-2);
        if (AnimLook == "Right") if (Weapon.getRotation() > 0) Weapon.rotate(2);
        if (ShotRadius.getRadius() > 0) ShotRadius.setRadius(ShotRadius.getRadius() - 35);

        if (RecoilOffset < 0) RecoilOffset += 2;

        // Radiuses
        ShotRadius.setOrigin(ShotRadius.getRadius(), ShotRadius.getRadius());

        Vector2f pos = this->getPosition();
        FloatRect bounds = this->getGlobalBounds();
        ShotRadius.setPosition(pos.x + bounds.width/2, pos.y + bounds.height/2);
    }

    void DeathAnimation() { // lazy workaround for death animation in loop
        // Move animation
        float AnimTickTime = 0.04;
        if (AnimTick.getElapsedTime().asSeconds() > AnimTickTime) {
            if (AnimState == "dead") {
                if (x < 778) {
                    x += 389;

                    if (AnimLook == "Left") this->setTextureRect(IntRect(x, 677, 389, 323));
                    if (AnimLook == "Right") this->setTextureRect(IntRect(x + 389, 677, -389, 323));
                }
            }
            else {
                x = 0;
                AnimState = "dead";
            }

            AnimTick.restart();
        }
    }

    void Draw(RenderWindow& window, bool debug = false) {
        if (debug) window.draw(Status);
        if (AnimState == "idleGun") {
            if (Muzzle) window.draw(MuzzleFlash);
            window.draw(Weapon);
        }
        window.draw(*this);
    }

    Texture* Sheet;
    Sprite Weapon;
    Sound WeaponSnd;
    Texture WeaponTex;
    SoundBuffer WeaponBuf, ReloadBuf;
    string AnimState, AnimLook = "Left";
    bool AnimAsc = true;
    Clock AnimTick, ShootClock;
    float x = 0; // Animation x
    float RecoilOffset = 0;

    CircleShape ShotRadius;

    string CurrentHolding;
    bool HeldDown = false;
    bool Sprinting = false;
    bool Reloading = false;
    bool Muzzle = false;

    Clock StaminaClock;
    float Stamina = 100;
    float MoveSpeed = 5;

    Text Status;

    SoundBuffer ReloadB1, ReloadB2, ReloadB3;
    Sound Reload1, Reload2, Reload3;
    int LastReloadVO;

    SoundBuffer HurtB1, HurtB2, HurtB3;
    Sound Hurt1, Hurt2, Hurt3;
    int LastHurtVO;

    SoundBuffer DeadB1, DeadB2;
    Sound Dead1, Dead2;
    bool DeathShout = false;

    Texture MuzzleTex;
    Sprite MuzzleFlash;
};

#endif // PLAYER_HEADER
