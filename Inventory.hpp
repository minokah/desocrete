// Player Global Inventory

#ifndef INVENTORY_HEADER
#define INVENTORY_HEADER

#include <iostream>
#include "Weapons.hpp"
#include "Loot.hpp"

using namespace sf;
using namespace std;

// Inventory Mechanics

// Item Slot in Inventory
struct InventoryItem : public RectangleShape {
    InventoryItem(string name, string type, int price, int num) : RectangleShape(Vector2f(110, 110)) {
        this->setFillColor(Color(80, 80, 80));
        Title.setFont(BarlowB);
        Title.setString(name + "\n" + to_string(price) + " Caps");
        Title.setCharacterSize(15);
        Fraction.setFont(BarlowB);
        Fraction.setCharacterSize(15);
        ItemPic.setSize(Vector2f(110, 110));
        SelectNum = num;

        ItemTex = new Texture;
        ItemTex->loadFromFile(Dir_Assets + "Items\\" + name + ".png");

        OutlineText(Title);
        OutlineText(Fraction);

        Reposition(0, 0);
    }

    bool CheckHighlight(int amount) {
        if (amount == SelectNum) {
            this->setOutlineThickness(2);
            this->setFillColor(Color(220, 0, 0));
            return true;
        }
        else {
            this->setOutlineThickness(0);
            this->setFillColor(Color(80, 80, 80));
        }
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        ItemPic.setPosition(x, y);
        Title.setPosition(x + 5, y + 5);
        Fraction.setPosition(x + 5, y + 24);
    }

    void Draw(RenderWindow& window) {
        ItemPic.setTexture(ItemTex); // fixes the white box problem, some weird scoping stuff??
        window.draw(*this);
        window.draw(ItemPic);
        window.draw(Title);
        window.draw(Fraction);
    }

    Texture* ItemTex;
    RectangleShape ItemPic;
    Text Title, Fraction;
    int Selected = 0;
    int SelectNum;
};

// Weapon Slot in Inventory
struct InventoryWeapon : public RectangleShape {
    InventoryWeapon(int num) : RectangleShape(Vector2f(334, 130)) {
        ItemTex = new Texture;
        this->setFillColor(Color(80, 80, 80));
        Title.setFont(BarlowB);
        Title.setString("Available Weapon Slot");
        Title.setCharacterSize(15);
        Fraction.setFont(BarlowB);
        Fraction.setCharacterSize(15);
        Fraction.setString("");
        ItemPic.setSize(Vector2f(334, 130));
        SelectNum = num;

        ItemPic.setFillColor(Color::Transparent);

        OutlineText(Title);
        OutlineText(Fraction);

        Reposition(0, 0);
    }

    void UpdateSlot(string name, int price) {
        if (name != "") {
            delete ItemTex;
            ItemTex = new Texture;
            ItemTex->loadFromFile(Dir_Assets + "Items\\" + name + ".png");
            ItemPic.setTexture(ItemTex);
            Title.setString(name + " - " + to_string(price) + " Caps");
            ItemPic.setFillColor(Color::White);
        }
        else {
            Title.setString("Available Weapon Slot");
            ItemPic.setFillColor(Color::Transparent);
            Fraction.setString("");
        }
    }

    void UpdateAmmo(int load, int res) {
        Fraction.setString(to_string(load) + "/" + to_string(res));
    }

    bool CheckHighlight(int amount) {
        if (amount == SelectNum) {
            this->setOutlineThickness(2);
            this->setFillColor(Color(220, 0, 0));
            return true;
        }
        else {
            this->setOutlineThickness(0);
            this->setFillColor(Color(80, 80, 80));
        }
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        ItemPic.setPosition(x, y);
        Title.setPosition(x + 5, y + 5);
        Fraction.setPosition(x + 5, y + 24);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(ItemPic);
        window.draw(Title);
        window.draw(Fraction);
    }

    Texture* ItemTex;
    RectangleShape ItemPic;
    Text Title, Fraction;
    int Selected = 0;
    int SelectNum;
};

// General container frame for anything
struct ContainerFrame : public RectangleShape {
    ContainerFrame(string title, float width, float height) : RectangleShape(Vector2f(width, height)) {
        this->setFillColor(Color(50, 50, 50));
        Bar.setSize(Vector2f(width, 40));
        Bar.setFillColor(Color(220, 0, 0));
        Header.setFont(BarlowB);
        Header.setCharacterSize(25);
        Header.setString(title);
        OutlineText(Header);

        Reposition(0, 0);
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        Bar.setPosition(x, y);
        Header.setPosition(x + 15, y + 5);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(Bar);
        window.draw(Header);
    }

    RectangleShape Bar;
    Text Header;
};

// Conatiner frame for character/vendor inventory
struct InventoryFrame : public ContainerFrame {
    InventoryFrame(string title, float width, float height) : ContainerFrame(title, width, height) {
        Slot1 = new InventoryWeapon(0);
        Slot2 = new InventoryWeapon(1);
        Hint.setFont(BarlowB);
        Hint.setCharacterSize(22);
        Discard.setFont(BarlowB);
        Discard.setCharacterSize(22);
        OutlineText(Discard);
        OutlineText(Hint);
    }

    void RefreshItems(vector<Item> items, bool sale = false) {
        Contains.clear();
        xPos = -112;
        yPos = 172;

        for (int i = 0; i != items.size(); i++) {
            float price = items[i].Price;
            if (sale) price = items[i].SalePrice;
            Contains.push_back(InventoryItem(items[i].Name, items[i].Type, price, i));
        }

        for (int i = 0; i != Contains.size(); i++) {
            xPos += 112;
            if (xPos >= 660) {
                xPos = 0;
                yPos += 112;
            }
            Contains[i].Reposition(this->getPosition().x + xPos, this->getPosition().y + yPos);
        }
    }

    void RefreshWeapons(Weapon slot1, Weapon slot2, bool sale = false) {
        int price = slot1.Price;
        if (sale) price = slot1.SalePrice;
        Slot1->UpdateSlot(slot1.Name, price);
        if (slot1.Name != "") Slot1->UpdateAmmo(slot1.Loaded, slot1.Reserve);
        price = slot2.Price;
        if (sale) price = slot2.SalePrice;
        Slot2->UpdateSlot(slot2.Name, price);
        if (slot2.Name != "") Slot2->UpdateAmmo(slot2.Loaded, slot2.Reserve);
    }

    bool CheckItemHighlight(int num) {
        for (int i = 0; i != Contains.size(); i++) {
            Contains[i].CheckHighlight(num);
        }
    }

    bool CheckWeaponHighlight(int num) {
        Slot1->CheckHighlight(num);
        Slot2->CheckHighlight(num);
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        this->Bar.setPosition(x, y);
        this->Header.setPosition(x + 15, y + 5);
        Slot1->Reposition(x, y + 40);
        Slot2->Reposition(x + 336, y + 40);
        Hint.setPosition(x + 15, y + this->getSize().y - 35);
        Discard.setPosition(Hint.getPosition());
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(this->Bar);
        window.draw(this->Header);
        for (auto i : Contains) i.Draw(window);
        Slot1->Draw(window);
        Slot2->Draw(window);
        if (Discarding) window.draw(Discard);
        else window.draw(Hint);
    }

    void Active(PlayerStats& stats, int& opsel, Audio& highlight, bool& prompt, bool vendor = false, PlayerStats* pstats = nullptr, bool inRaid = false) {
        // Swap between Weapons and Items
        if (Keyboard::isKeyPressed(Keyboard::Tab) || X()) {
            if (Contains.size() > 0) {
                highlight.play();
                WeaponCategory = !WeaponCategory;
                CheckItemHighlight(-1);
                CheckWeaponHighlight(-1);
                opsel = 0;
            }
            prompt = true;
        }

        // Use item
        if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
            if (!WeaponCategory && stats.Health < 100) {
                string obj = stats.Backpack[opsel].Name;

                if (obj == "Nuka-Cola") stats.Health += 15;
                if (obj == "Cola Quantum") stats.Health += 50;

                if (stats.Health >= 100) stats.Health == 100;

                UseBuffer.loadFromFile("Engine\\Sounds\\" + obj + "_Use.ogg");
                UseSnd.setBuffer(UseBuffer);
                UseSnd.play();

                stats.RemoveFromBackpack(opsel);

                RefreshItems(stats.Backpack);
                if (Contains.size() == 0) WeaponCategory = true;
                if (opsel > 0) opsel--;
            }

            prompt = true;
        }

        // Sell item from inventory
        if (!vendor) {
            if (Keyboard::isKeyPressed(Keyboard::R) || Y()) {
                if (stats.Backpack.size() > 0 && !WeaponCategory || WeaponCategory && opsel == 0 && stats.Weapon1.Name != "" || WeaponCategory && opsel == 1 && stats.Weapon2.Name != "") {
                    string name;
                    if (!WeaponCategory) name = stats.Backpack[opsel].Name;
                    if (WeaponCategory) {
                        if (opsel == 0) name = stats.Weapon1.Name;
                        if (opsel == 1) name = stats.Weapon2.Name;
                    }
                    Discarding = true;
                    string prefix = "Selling ";
                    if (inRaid) prefix = "Discarding ";
                    Discard.setString(prefix + name + " in " + to_string(2 - (int)DiscardClock.getElapsedTime().asSeconds()) + "...");
                }
                else Discarding = false;
            }
            else {
                Discarding = false;
                DiscardClock.restart();
            }

            if (Discarding && DiscardClock.getElapsedTime().asSeconds() > 2) {
                if (!WeaponCategory) {
                    DropName = stats.Backpack[opsel].Name;
                    DropType = stats.Backpack[opsel].Type;
                    if (!inRaid) stats.Caps += stats.Backpack[opsel].SalePrice;
                    stats.RemoveFromBackpack(opsel);
                }
                else {
                    if (!inRaid) {
                        if (opsel == 0) stats.Caps += stats.Weapon1.SalePrice;
                        if (opsel == 1) stats.Caps += stats.Weapon2.SalePrice;
                    }
                    else {
                        if (opsel == 0) {
                            DropName = stats.Weapon1.Name;
                            DropType = stats.Weapon1.Type;
                            DropLoad = stats.Weapon1.Loaded;
                            DropRes = stats.Weapon1.Reserve;
                        }
                        if (opsel == 1) {
                            DropName = stats.Weapon2.Name;
                            DropType = stats.Weapon2.Type;
                            DropLoad = stats.Weapon2.Loaded;
                            DropRes = stats.Weapon2.Reserve;
                        }
                    }

                    stats.RemoveWeapon(opsel);
                }

                RefreshItems(stats.Backpack);
                RefreshWeapons(stats.Weapon1, stats.Weapon2);

                if (Contains.size() == 0) WeaponCategory = true;

                if (opsel > 0) opsel--;
                DiscardClock.restart();
            }

            RefreshItems(stats.Backpack, true);
            RefreshWeapons(stats.Weapon1, stats.Weapon2, true);
        }
        // Purchase item from vendor
        else {
            if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                if (!WeaponCategory || WeaponCategory && (pstats->Weapon1.Name == "" || pstats->Weapon2.Name == "")) {
                    string name;
                    if (!WeaponCategory) name = stats.Backpack[opsel].Name;
                    if (WeaponCategory) {
                        if (opsel == 0) name = stats.Weapon1.Name;
                        if (opsel == 1) name = stats.Weapon2.Name;
                    }
                    Discarding = true;
                    Discard.setString("Purchasing " + name + " in " + to_string(2 - (int)DiscardClock.getElapsedTime().asSeconds()) + "...");
                }
                else Discard.setString("You are already carrying two weapons.");
            }
            else {
                Discarding = false;
                DiscardClock.restart();
            }

            if (Discarding && DiscardClock.getElapsedTime().asSeconds() > 2) {
                if (!WeaponCategory) {
                    if (pstats->Caps >= stats.Backpack[opsel].Price && pstats->Backpack.size() < 12) {
                        for (int i = 0; i != GlobalItems.size(); i++) {
                            if (GlobalItems[i].Name == stats.Backpack[opsel].Name) {
                                pstats->Caps -= stats.Backpack[opsel].Price;
                                pstats->AddToBackpack(GlobalItems[i]);
                                break;
                            }
                        }
                    }
                }
                else {
                    if (opsel == 0) {
                        if (pstats->Caps >= stats.Weapon1.Price) {
                            for (int i = 0; i != GlobalWeapons.size(); i++) {
                                if (GlobalWeapons[i].Name == stats.Weapon1.Name) {
                                    pstats->Caps -= stats.Weapon1.Price;
                                    pstats->EquipWeapon(GlobalWeapons[i]);
                                    break;
                                }
                            }
                        }
                    }
                    if (opsel == 1) {
                        if (pstats->Caps >= stats.Weapon2.Price) {
                            for (int i = 0; i != GlobalWeapons.size(); i++) {
                                if (GlobalWeapons[i].Name == stats.Weapon2.Name) {
                                    pstats->Caps -= stats.Weapon2.Price;
                                    pstats->EquipWeapon(GlobalWeapons[i]);
                                    break;
                                }
                            }
                        }
                    }
                }

                if (Contains.size() == 0) WeaponCategory = true;

                DiscardClock.restart();
            }
        }

        // Left/Right/Up/Down selection
        if (Keyboard::isKeyPressed(Keyboard::A) || Left()) {
            opsel--;
            highlight.play();
            if (!WeaponCategory) {
                if (opsel < 0) opsel = Contains.size() - 1;
            }
            else if (opsel < 0) opsel = 1;
            prompt = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::D) || Right()) {
            opsel++;
            highlight.play();
            if (!WeaponCategory) {
                if (opsel > Contains.size() - 1) opsel = 0;
            }
            else if (opsel > 1) opsel = 0;
            prompt = true;
        }

        if (!WeaponCategory) {
            if (Keyboard::isKeyPressed(Keyboard::S) || Down()) {
                opsel += 6;
                highlight.play();
                if (opsel > Contains.size() - 1) opsel = 0;
                prompt = true;
            }

            if (Keyboard::isKeyPressed(Keyboard::W) || Up()) {
                opsel -= 6;
                highlight.play();
                if (opsel < 0) opsel = Contains.size() - 1;
                prompt = true;
            }
        }

        if (!WeaponCategory) {
            CheckItemHighlight(opsel);

            if (stats.Backpack.size() == 0) SelectedHint = "";
            else SelectedHint = stats.Backpack[opsel].Name + "\n\n" + stats.Backpack[opsel].Hint;
        }
        else {
            CheckWeaponHighlight(opsel);

            if (opsel == 0) {
                if (stats.Weapon1.Name != "") SelectedHint = stats.Weapon1.Name + "\n\n" + stats.Weapon1.Hint;
                else SelectedHint = "Available Weapon Slot\n\nCan't really do anything with nothing,\nright?";
            }
            else if (opsel == 1) {
                if (stats.Weapon2.Name != "") SelectedHint = stats.Weapon2.Name + "\n\n" + stats.Weapon2.Hint;
                else SelectedHint = "Available Weapon Slot\n\nCan't really do anything with nothing,\nright?";
            }
        }
    }

    bool CheckDrop() {
        if (DropName != "") return true;
    }

    vector<InventoryItem> Contains;
    Text Discard, Hint;
    InventoryWeapon* Slot1;
    InventoryWeapon* Slot2;
    int xPos = -112;
    int yPos = 172;
    bool WeaponCategory = true;
    bool Discarding = false;
    Clock DiscardClock;
    string DropName, DropType, SelectedHint = "";
    int DropLoad = 0, DropRes = 0;

    SoundBuffer UseBuffer;
    Sound UseSnd;
};

#endif // INVENTORY_HEADER
