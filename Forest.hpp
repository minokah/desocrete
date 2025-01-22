// Forest Map

// Used for map loading

#ifndef MAP_FOREST_HEADER
#define MAP_FOREST_HEADER

#include "AI.hpp"
#include "Loot.hpp"

struct MapBarrier : public RectangleShape {
    MapBarrier(float w, float h, float x, float y) : RectangleShape(Vector2f(w, h)) {
        this->setFillColor(Color::Yellow);
        this->setPosition(x, y);
    }

    void Routine(PlayerModel& model, bool& touching) {
        if (model.getGlobalBounds().intersects(this->getGlobalBounds())) touching = true;
    }
};

struct ForestMap { // loading the map a different way than last year because i don't have the collision creator tool
    ForestMap(bool& status, vector<Enemy>& enemies, vector<ExtractFlare>& extracts, vector<WorldItem>& loot, vector<MapBarrier>& barriers, PlayerModel& model) {
        Ground = new ObjectBase("Forest\\Map.jpg", false, true);
        Water = new ObjectBase("Forest\\Water.jpg", false, true);
        Water->setPosition(0, -Water->getGlobalBounds().height/2);

        TopFence = new ObjectBase("Forest\\TopFence.png", true, true);
        barriers.push_back(MapBarrier(TopFence->getGlobalBounds().width, TopFence->getGlobalBounds().height, 0, 0));

        BottomFence = new ObjectBase("Forest\\BottomFence.png", true, true);
        BottomFence->setPosition(0, Ground->getGlobalBounds().height - BottomFence->getGlobalBounds().height);
        barriers.push_back(MapBarrier(BottomFence->getGlobalBounds().width, BottomFence->getGlobalBounds().height, 0, Ground->getGlobalBounds().height - BottomFence->getGlobalBounds().height));

        LeftFence = new ObjectBase("Forest\\LeftFence.jpg", true, true);
        barriers.push_back(MapBarrier(LeftFence->getGlobalBounds().width, LeftFence->getGlobalBounds().height, 0, 0));

        RightFence = new ObjectBase("Forest\\LeftFence.jpg", true, true);
        RightFence->setPosition(Ground->getGlobalBounds().width - RightFence->getGlobalBounds().width, 0);
        barriers.push_back(MapBarrier(RightFence->getGlobalBounds().width, RightFence->getGlobalBounds().height, Ground->getGlobalBounds().width - RightFence->getGlobalBounds().width, 0));

        Boat = new ObjectBase("Forest\\Boat.png", false, true);
        Boat->setPosition(800, -Boat->getGlobalBounds().height + 50);

        extracts.push_back(ExtractFlare(800, 100, Boat->getGlobalBounds().width, 250, "Riverside Boat"));

        for (int i = 0; i != 15; i++) loot.push_back(WorldItem(GlobalItems[rand() % 2].Name, "item"));
        for (int i = 0; i != 10; i++) {
            Item newWeapon = GlobalWeapons[rand() % GlobalWeapons.size()];

            // lazy!!!
            int Reserve = 0;
            int Loaded = 0;

            string name = newWeapon.Name;
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

            loot.push_back(WorldItem(newWeapon.Name, newWeapon.Type, Vector2f(rand() % (int)Ground->getGlobalBounds().width, rand() % (int)Ground->getGlobalBounds().height), Loaded, Reserve));
        }

        model.setPosition(Ground->getGlobalBounds().width/2, Ground->getGlobalBounds().height/2);

        status = true;
    }

    void Routine() {
        Water->move(0, -1);
        if (Water->getPosition().y < -Water->getGlobalBounds().height) Water->setPosition(0, -Water->getGlobalBounds().height/2);
    }

    void Draw(RenderWindow& window) {
        window.draw(*Water);
        window.draw(*Ground);
        window.draw(*LeftFence);
        window.draw(*RightFence);
        window.draw(*TopFence);
        window.draw(*BottomFence);
        window.draw(*Boat);
    }

    ~ForestMap() {
        delete Ground;
        delete Water;
        delete TopFence;
        delete LeftFence;
        delete RightFence;
        delete BottomFence;
        delete Boat;
    }

    ObjectBase* Ground;
    ObjectBase* Water;
    ObjectBase* TopFence, *LeftFence, *BottomFence, *RightFence;
    ObjectBase* Boat;
};

struct DemoMap {
    DemoMap(bool& status, vector<Enemy>& enemies, vector<ExtractFlare>& extracts, vector<WorldItem>& loot, vector<MapBarrier>& barriers, PlayerModel& model) {
        Ground = new ObjectBase("DemoMap\\Map.png", false);

        extracts.push_back(ExtractFlare(400, 100, 100, 100, "demo extract lol"));
        loot.push_back(WorldItem(GlobalItems[rand() % 2].Name, "item", Vector2f(700, 100)));

        barriers.push_back(MapBarrier(100, 100, 0, 600));

        model.setPosition(500, 500);

        status = true;
    }

    void Draw(RenderWindow& window) {
        window.draw(*Ground);
    }

    ObjectBase* Ground;
};

#endif // MAP_FOREST_HEADER
