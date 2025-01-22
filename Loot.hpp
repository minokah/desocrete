#ifndef LOOT_HEADER
#define LOOT_HEADER

#include "Player.hpp"
#include "Engine.hpp"

// Global loot, press "F" to pickup when in world
struct WorldItem : public Sprite {
    WorldItem(string name, string type, Vector2f pos = Vector2f(0, 0), int load = 0, int res = 0) {
        Name = name;
        Type = type;
        Loaded = load;
        Reserve = res;

        Tex = new Texture;
        Tex->loadFromFile(Dir_Assets + "Loot\\" + name + ".png");
        this->setTexture(*Tex);
        this->setPosition(pos);

        Prompt = new KeyPrompt("F", "X", "Take " + name, 37);
        Prompt->Reposition(this->getPosition().x, this->getPosition().y - 15);
    }

    bool Routine(PlayerModel& model, PlayerStats& stats, string inputMethod) {
        Prompt->CheckInput(inputMethod);

        if (stats.Weapon1.Name != "" && stats.Weapon2.Name != "" && Type != "item" || stats.Backpack.size() >= 12 && Type == "item") {
            Prompt->PromptText.setFillColor(Color(220, 0, 0));
            Prompt->PromptText.setString("Slots Full");
        }
        else {
            Prompt->PromptText.setFillColor(Color::White);
            Prompt->PromptText.setString("Take " + Name);
        }

        if (this->getGlobalBounds().intersects(model.getGlobalBounds())) {
            Active = true;

            if (Keyboard::isKeyPressed(Keyboard::F) || X()) return true;
        }
        else Active = false;

        return false;
    }

    void Draw(RenderWindow& window) {
        this->setTexture(*Tex);
        window.draw(*this);
        if (Active) Prompt->Draw(window);
    }

    KeyPrompt* Prompt;
    Texture* Tex;
    string Name, Type;
    bool Active = false;
    int Loaded, Reserve;
};

#endif // LOOT_HEADER
