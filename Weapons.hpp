// Weapons and Items List

// shoving tons of stuff into constructors got messy so now everything is all over the place

#ifndef WEAPONS_HEADER
#define WEAPONS_HEADER

#include "Player.hpp"

vector<Item> GlobalWeapons = {
    Item("AR-12", "ar", 200, "It goes pew pew pew", Vector2f(0, 145), Vector2f(180, 145)),
    Item("Auto-6", "shotgun", 200, "Probably hurts to get hit by this", Vector2f(-10, 140), Vector2f(185, 140)),
    Item("Trumpet", "special", 1000, "doot", Vector2f(-10, 135), Vector2f(190, 135)),
    Item("BnL Repeater", "lever", 200, "Makes you feel like a cowboy", Vector2f(-20, 140), Vector2f(195, 140)),
    Item("HCR200", "sniper", 200, "Careful where you point that thing man!", Vector2f(-35, 130), Vector2f(210, 130))
};

vector<Item> GlobalItems {
    Item("Nuka-Cola", "item", 20, "Nuka-Cola: the perfect beverage!\n(May include radiation) [+15 HP]"),
    Item("Cola Quantum", "item", 80, "Nuka-Cola Quantum: it looks like\nit's... glowing? [+50 HP]"),
};

#endif // WEAPONS_HEADER
