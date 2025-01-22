/*

    DESOCRETE
    Brandon Luu 2020

    Game info is in the "Game Information" Google document

*/

#include <iostream>
#include "Engine.hpp"
#include "Assets.hpp"
#include "Player.hpp"
#include "Inventory.hpp"
#include "Weapons.hpp"
#include "Bullets.hpp"
#include "Loot.hpp"
#include "AI.hpp"
#include "physics.h"

// Maps
#include "Forest.hpp"

using namespace std;
using namespace sf;

int main()
{
    srand(time(0));
    Vector2f WinSize = Vector2f(1280, 768);
    RenderWindow window(VideoMode(WinSize.x, WinSize.y), "DESOCRETE - Loading assets, please wait!");

    Image GameIcon; GameIcon.loadFromFile("Engine\\Assets\\Items\\Cola Quantum.png");

    View GameView;

    b2World world(b2Vec2(0, 9.8));
    physics::createBox(world,0,570,800,30,b2_staticBody);

    Font BarlowSB, Overseer;
    BarlowSB.loadFromFile(Dir_Fonts + "Barlow-SemiBold.ttf");
    BarlowB.loadFromFile(Dir_Fonts + "Barlow-Bold.ttf");
    Overseer.loadFromFile(Dir_Fonts + "Overseer.otf");

    // * User Interface * //
    vector<KeyPrompt> Menu_KeyPrompts;

    // Main Menu
    Text LoadingText("Loading assets, get ready!", BarlowB, 20); LoadingText.setPosition(20, WinSize.y - 37);
    Picture LoadingWheel(96, 48, "VaultTecLogo"); LoadingWheel.setPosition(WinSize.x - 111, WinSize.y - 53);
    window.draw(LoadingWheel); window.display();

    Texture Menu_BackgroundImage; Menu_BackgroundImage.loadFromFile(Dir_Assets + "Menu_Background.png");
    Texture Menu_BackgroundImage2; Menu_BackgroundImage2.loadFromFile(Dir_Assets + "Menu_Background2.png");
    string Menu_CurrentBackground = "desert";
    if (rand() % 2 == 0) Menu_CurrentBackground = "forest";
    bool Menu_BackgroundReverse = false;
    Clock Menu_BackgroundClock;

    Sprite Menu_Background(Menu_BackgroundImage);
    Sprite Menu_Background2(Menu_BackgroundImage2);

    RectangleShape Menu_LogoBanner(Vector2f(0, 200)); Menu_LogoBanner.setFillColor(Color(50, 50, 50, 150));
    Text Menu_Logo("DESOCRETE", Overseer, 150); OutlineText(Menu_Logo);

    Text Menu_Logo2("ALPHA", Overseer, 35);
    Menu_Logo2.setFillColor(Color(253, 220, 104)); OutlineText(Menu_Logo2);

    Text Menu_Credit("Brandon Luu 2020", BarlowB, 20); OutlineText(Menu_Credit, true);

    RectangleShape Menu_TopBar(Vector2f(WinSize.x, 50)); Menu_TopBar.setFillColor(Color::Black);
    RectangleShape Menu_BottomBar(Vector2f(WinSize.x, 50)); Menu_BottomBar.setFillColor(Color::Black);
    Text Menu_Header("", BarlowB, 70); OutlineText(Menu_Header);
    Text Menu_Hint("", BarlowB, 20); OutlineText(Menu_Hint, true);
    RectangleShape Menu_Fade(Vector2f(WinSize.x, WinSize.y)); Menu_Fade.setFillColor(Color::Transparent);

    vector<KeyBind> Menu_AcceptPrompts {
        KeyBind("Esc", "B", "No"),
        KeyBind("Enter_Alt", "A", "Yes"),
    };

    vector<KeyBind> Menu_StartPrompts {
        KeyBind("Esc", "B", "Quit Game"),
        KeyBind("Enter_Alt", "A", "Start Game"),
    };

    // Quit Game
    int Menu_OptionSelected = 0;

    // Settings
    Text Menu_SettingsHint("", BarlowB, 22); OutlineText(Menu_SettingsHint);

    vector<string> Menu_SettingsMoveList = {"WASD", "Arrow Keys"};
    vector<string> Menu_SettingsWinList = {"Windowed", "Borderless", "Fullscreen"};
    vector<string> Menu_SettingsResList = {"1280x768", "1280x1024","1920x1080"};
    vector<string> Menu_SettingsSFXList = {"On", "Off"};
    Selector Menu_SettingsMove("KBM Move Method", Menu_SettingsMoveList, 0);
    Selector Menu_SettingsWin("Window Mode", Menu_SettingsWinList, 1);
    Selector Menu_SettingsRes("Screen Resolution", Menu_SettingsResList, 2);
    Selector Menu_SettingsSFX("Sound + Music", Menu_SettingsSFXList, 3);
    int Menu_SettingsSMax = 3;

    vector<KeyBind> Menu_SettingsPrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("WASD", "Dpad_Full", "Scroll/Adjust"),
        KeyBind("Enter_Alt", "A", "Apply"),
    };

    // Home
    ContainerFrame Menu_DemoFrame("Attention!", 700, 120);
    Text Menu_DemoText("This is a demo version of DESCORETE Alpha.\nNo stats will be saved throughout or when exiting the game.", BarlowB, 22); OutlineText(Menu_DemoText);
    MenuPlayButton Menu_HomeRaid("Raid", "Menu_HomeRaid", 0, true);
    MenuPlayButton Menu_HomeCharacter("Character", "Menu_HomeCharacter", 1);
    MenuPlayButton Menu_HomeTraders("Traders", "Menu_HomeTraders", 2);
    MenuPlayButton Menu_HomeHideout("Hideout", "Menu_HomeHideout", 3);
    MenuPlayButton Menu_HomeSettings("Settings", "Menu_HomeSettings", 4);
    int Menu_PlaySMax = 4;

    Text Menu_HomeHint("", BarlowB, 22); OutlineText(Menu_HomeHint);

    vector<KeyBind> Menu_HomePrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("WASD", "Dpad_Full", "Scroll"),
        KeyBind("Enter_Alt", "A", "Select"),
    };

    // Raid
    vector<string> Menu_RaidMapList = {"Rozhok National Forest", "Demo Map"};
    vector<string> Menu_RaidTimeList = {"Day", "Night"};
    vector<string> Menu_RaidPrepList = {"Setting", "Insurance", "Deployment"};
    Selector Menu_RaidMap("Location", Menu_RaidMapList, 0);
    Selector Menu_RaidTime("Time", Menu_RaidTimeList, 1);
    Selector Menu_RaidPrep("Preparation", Menu_RaidPrepList, 0);
    int Menu_RaidSMax = 1;

    vector<KeyBind> Menu_RaidPrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("WASD", "Dpad_Full", "Scroll/Adjust"),
        KeyBind("Enter_Alt", "A", "Next")
    };

    RectangleShape Menu_RaidMapPreview(Vector2f(400, 250)); Menu_RaidMapPreview.setFillColor(Color(50, 50, 50));
    Text Menu_RaidMapHint("", BarlowB, 22); OutlineText(Menu_RaidMapHint);

    Value Menu_RaidInsure("Insure Items?", 370, 40, 230);
    Text Menu_RaidInsureHint("Insurance [ Not Available in ALPHA ]\n\nIf you die during the raid, you will randomly lose\none weapon as well as 50% of your inventory.\n\nInsurance will prevent this- you will get your lost\nitems back after 10 minutes of dying.\n\nCost: 0 Caps", BarlowB, 22); OutlineText(Menu_RaidInsureHint);

    KeyPrompt Menu_RaidInsureKey("R", "Y", "Toggle", 37);
    bool Menu_RaidInsureValue = false;

    Text Menu_RaidDeployHint("", BarlowB, 22); OutlineText(Menu_RaidDeployHint);

    vector<KeyBind> Menu_RaidDeployPrompts = {
        KeyBind("Esc", "B", "Back"),
    };

    // Character
    InventoryFrame Player_Inventory("Inventory", 670, 434);
    ContainerFrame Player_Vitals("Vitals & Preview", 450, 434);
    Text Player_WeaponHint("", BarlowB, 22);
    bool Player_InvSelWeapons = true;
    Clock Player_InvDiscardClock;
    bool Player_InvDiscording = false;

    vector<KeyBind> Menu_CharacterPrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("WASD", "Dpad_Full", "Scroll"),
        KeyBind("R", "Y", "Sell [HOLD]"),
        KeyBind("Tab", "X", "Swap [Weapon/Items]"),
        KeyBind("Enter_Alt", "A", "Use")
    };

    Picture Player_VitalsGuy(179, 236, "Menu_CharacterGuy");
    Value Player_VitalsHealth("Health", 200, 40, 110); Player_VitalsHealth.setFillColor(Color(100, 100, 100));
    SineGraph Player_VitalsHealthSin(180, 100, 50, 2.4, 3);
    Value Player_VitalsCaps("Caps", 200, 40, 110); Player_VitalsCaps.setFillColor(Color(100, 100, 100));

    ProgressBar Player_XPBar(450, 30);
    Text Player_XPText("Level 0: 0/100 XP", BarlowB, 20); OutlineText(Player_XPText);

    // Traders
    MenuPlayButton Menu_TradeDoctor("Doctor", "Menu_TraderDoctor", 0);
    MenuPlayButton Menu_TradeDealer("Dealer", "Menu_TraderDealer", 1);
    int Menu_TraderSMax = 1;

    vector<KeyBind> Menu_TraderPrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("WASD", "Dpad_Full", "Scroll"),
        KeyBind("Enter_Alt", "A", "Select")
    };

    // Trader Vendor
    vector<KeyBind> Menu_TraderVndrPrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("WASD", "Dpad_Full", "Scroll"),
        KeyBind("Tab", "X", "Swap [Weapon/Items]"),
        KeyBind("Enter_Alt", "A", "Buy [HOLD]")
    };

    PlayerStats VendorStats;
    Text Menu_TraderHint("", BarlowB, 22); OutlineText(Menu_TraderHint);
    InventoryFrame Menu_VendorInventory("Vendor", 670, 434);
    ContainerFrame Menu_VendorStats("Description", 450, 434);

    // Menu Audio
    Audio Menu_Highlight1("Menu_Highlight1");

    Audio Menu_Header1("Menu_Header1");
    Audio Menu_Header2("Menu_Header2");
    Audio Menu_Header3("Menu_Header3");

    Audio Menu_Select2("Menu_Select2");
    Audio Menu_Select3("Menu_Select3");

    Audio Menu_Scroll1("Menu_Scroll1");
    Audio Menu_Select1("Menu_Select1");

    Audio Game_Enter("Game_Enter");
    Audio Game_Exit("Game_Exit");

    // Music
    Music* Menu_CurrentMusic;
    Music Menu_Music1; Menu_Music1.openFromFile(Dir_Music + "Menu_Music1.ogg"); Menu_Music1.setVolume(15);
    Music Menu_Music2; Menu_Music2.openFromFile(Dir_Music + "Menu_Music2.ogg"); Menu_Music2.setVolume(60);
    bool MenuMusicPlaying = false;
    Clock MusicTimer;

    // Raid Start
    Text RaidStartText("Get Ready!", BarlowB, 42); OutlineText(RaidStartText);
    Text RaidStartTimer("", BarlowB, 25); OutlineText(RaidStartTimer);

    bool inMenu = true;
    bool RaidLoadingComplete = false;
    Clock RaidSpawnInClock;

    string inputMethod = "kb";
    string menuState = "menu";
    Clock uiClock;

    bool UpdateResolution = true;

    // Raid Summary
    RectangleShape Menu_FrontFade(Vector2f(WinSize.x, WinSize.y)); Menu_FrontFade.setFillColor(Color::Transparent);

    Audio Raid_Success("Raid_Success");
    Audio Raid_Failure("Raid_Failed");

    ContainerFrame Menu_RaidSummary("Summary", 500, 434);
    Value Menu_SummaryOutcome("Outcome", 500, 40, 190); Menu_SummaryOutcome.setFillColor(Color(100, 100, 100));
    Value Menu_SummaryTime("Survival Time", 500, 40, 190); Menu_SummaryTime.setFillColor(Color(100, 100, 100));
    Value Menu_SummaryKills("Kill Count", 500, 40, 190); Menu_SummaryKills.setFillColor(Color(100, 100, 100));
    Value Menu_SummaryXP("XP Earned", 500, 40, 190); Menu_SummaryXP.setFillColor(Color(100, 100, 100));

    vector<KeyBind> Menu_RaidSummaryPrompts = {
        KeyBind("Enter", "A", "Proceed")
    };

    // asset testing
    //Lightbulb bulb(world, 400, 300);

    // In Raid and Player Stats
    Picture Crosshair(50, 50, "Crosshair"); Crosshair.setOrigin(25, 25);

    Audio Inventory_Open("Inventory_Open");
    Audio Item_Pickup("Item_Pickup");

    bool PlayerDied = false;
    string RaidAftermath;
    Clock RaidExitDelay;
    PlayerStats CharStats;
    CharStats.Caps = 1500;

    // Just in case we run out of memory I'm gonna load the VO here instead of the PlayerModel constructor
    VOBuffer EnemyAVO("Enemy");
    VOBuffer Enemy2AVO("Enemy2");
    VOBuffer PlayerVO("Player");

    PlayerModel CharModel("Player", PlayerVO);

    int GainedXP = 0;
    int EnemiesKilled = 0;

    // Controls Help
    bool ControlsShowing = false;
    RectangleShape ControlsPanel(Vector2f(190, 45)); ControlsPanel.setFillColor(Color(50, 50, 50));
    RectangleShape ControlsHeader(Vector2f(190, 45)); ControlsHeader.setFillColor(Color(220, 0, 0));

    KeyPrompt ControlsOpen("M", "Dpad_Down", "Controls", 35);
    KeyPrompt ControlsWalk("WASD", "Left_Stick", "Move", 35);
    KeyPrompt ControlsSprint("Shift_Alt", "A", "Sprint (Hold)", 35);
    KeyPrompt ControlsInv("E", "Windows", "Inventory", 35);
    KeyPrompt ControlsMenu("Esc", "Menu", "Menu", 35);
    KeyPrompt ControlsShoot("Mouse_Left", "RB", "Shoot", 35);
    KeyPrompt ControlsAim("Mouse_Simple", "LB", "Aim", 35);
    KeyPrompt ControlsSwap("Tab", "Y", "Swap Weapons", 35);

    // Start the player with a weapon and a few items so they won't start with nothing
    CharStats.EquipWeapon(GlobalWeapons[0]);
    CharStats.AddToBackpack(GlobalItems[0]);
    CharStats.AddToBackpack(GlobalItems[0]);
    CharStats.AddToBackpack(GlobalItems[1]);

    vector<WorldItem> GroundLoot;

    Clock LootCooldown;

    Picture HurtFade(1280, 768, "HurtFade");

    string RaidMenuState = "playing";
    vector<KeyBind> Raid_InventoryPrompts = {
        KeyBind("E", "B", "Close"),
        KeyBind("WASD", "Dpad_Full", "Scroll"),
        KeyBind("R", "Y", "Sell/Discard [HOLD]"),
        KeyBind("Tab", "X", "Swap [Weapon/Items]"),
        KeyBind("Enter_Alt", "A", "Use")
    };

    // Escape Menu
    Text Raid_MenuHint("WARNING: Exiting the raid will count as a death.", BarlowB, 20); Raid_MenuHint.setFillColor(Color(220, 0, 0)); OutlineText(Raid_MenuHint, false);
    vector<KeyBind> Raid_GameMenuPrompts = {
        KeyBind("Esc", "B", "Back"),
        KeyBind("E", "X", "Options"),
        KeyBind("R", "Y", "Leave Raid")
    };

    AmmoCount WeaponAmmoCount;
    Clock SurvivalTime;

    vector<Bullet> Bullets;
    vector<Enemy> Enemies;
    Clock EnemySpawnTimer;

    bool ExtractsShowing = false;
    KeyPrompt ToggleExtracts("U", "Dpad_Left", "", 35);
    vector<Value> ExtractsAvail;
    Value ExtractPoints("      Find an extraction point", 460, 45, 320); ExtractPoints.SetValue("00:00:00");
    ExtractPoints.Frame.setFillColor(Color(50, 50, 50));
    Value ExtractTimeLeft("Extracting in ", 255, 40, 180);
    Clock ExtractTimer, RaidTimeLeft;
    bool Extracting = false;
    vector<ExtractFlare> Extracts;

    string LoadedMap = "none";
    ForestMap* Forest;
    DemoMap* Demo;

    vector<MapBarrier> MapBarriers;
    Vector2f LastValidPos;

    RectangleShape PlayerSafeSpawn(Vector2f(1024, 768)); PlayerSafeSpawn.setOrigin(512, 384);

    bool CheatMode = false; // change to true to cheat in items in inventory

    Picture inputMethodPic(0, 0, "InputMethod"); inputMethodPic.setSize(Vector2f(110, 50));

    // * Game Loop * //
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::JoystickButtonPressed || event.type == Event::JoystickMoved) inputMethod = "ctrl";
            if (event.type == Event::KeyPressed) inputMethod = "kb";
            if (event.type == Event::LostFocus && !inMenu && RaidMenuState != "menu") {
                RaidMenuState = "menu";
                bool dummyprompt = false;
                ScrollHeader(dummyprompt, Menu_Header, Menu_Hint, Menu_Select2);
                GenerateMenuPrompts(Raid_GameMenuPrompts, Menu_KeyPrompts, WinSize, inputMethod);
            }
        }

        // Input method checking for UI prompts
        for (int i = 0; i != Menu_KeyPrompts.size(); i++) {
            Menu_KeyPrompts[i].CheckInput(inputMethod);
        }

        if (inputMethod == "kb") inputMethodPic.setTextureRect(IntRect(110, 0, 110, 50));
        else if (inputMethod == "ctrl") inputMethodPic.setTextureRect(IntRect(0, 0, 110, 50));


        // All User Interface Stuff
        bool uiCooldown = uiClock.getElapsedTime().asSeconds() > 0.35;
        if (uiCooldown && inMenu) {
            bool promptUsed = false;
            if (inMenu) { // menu statuses
                // Start Screen
                if (menuState == "menu") {
                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        menuState = "home";
                        GenerateMenuPrompts(Menu_HomePrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Game_Enter.play();
                        ScrollTextIn(Menu_Header, true);
                        ScrollTextIn(Menu_Hint, true);
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "quit";
                        GenerateMenuPrompts(Menu_AcceptPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Game_Exit.play();
                        ScrollTextIn(Menu_Hint, true);
                        promptUsed = true;
                    }
                }

                // Quit
                else if (menuState == "quit" && !promptUsed) {
                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) window.close();
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "menu";
                        GenerateMenuPrompts(Menu_StartPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_LogoBanner.setSize(Vector2f(0, 200));
                        Menu_Scroll1.Playing = false;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select3);
                    }
                }

                // Settings
                else if (menuState == "settings" && !promptUsed) {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "home";
                        GenerateMenuPrompts(Menu_HomePrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    if (Keyboard::isKeyPressed(Keyboard::S) || Down()) {
                        Menu_OptionSelected++;
                        if (Menu_OptionSelected > Menu_SettingsSMax) Menu_OptionSelected = Menu_SettingsSMax;
                        Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::W) || Up()) {
                        Menu_OptionSelected--;
                        if (Menu_OptionSelected < 0) Menu_OptionSelected = 0;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    Menu_SettingsMove.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_SettingsWin.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_SettingsRes.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_SettingsSFX.Active(Menu_OptionSelected, promptUsed, Menu_Select1);

                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        UpdateResolution = true;
                        promptUsed = true;
                    }

                    if (Menu_SettingsMove.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> KBM Move Method [ Not Available in ALPHA ]\n\nChange between WASD and Arrow Keys to move\nyour character.");
                    if (Menu_SettingsWin.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> Window Mode\n\nPlay in Windowed, Borderless, or Fullscreen\nresolution.");
                    if (Menu_SettingsRes.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> Screen Resolution\n\nScale the window size.");
                    if (Menu_SettingsSFX.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> Sound + Music [ Not Available in ALPHA ]\n\nTurn the sound and music on or off.\n\nWhy would you want these off anyway?");
                }

                // Home
                else if (menuState == "home") {
                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        if (Menu_OptionSelected == 0) {
                            menuState = "raidSetting";
                            GenerateMenuPrompts(Menu_RaidPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        }
                        if (Menu_OptionSelected == 1) {
                            menuState = "character";
                            GenerateMenuPrompts(Menu_CharacterPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        }
                        if (Menu_OptionSelected == 2) {
                            menuState = "traders";
                            GenerateMenuPrompts(Menu_TraderPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        }
                        if (Menu_OptionSelected == 4) {
                            menuState = "settings";
                            GenerateMenuPrompts(Menu_SettingsPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        }

                        Menu_OptionSelected = 0;
                        if (rand() % 2 == 0) Menu_Header2.play();
                        else Menu_Header3.play();
                        ScrollTextIn(Menu_Header, true);
                        ScrollTextIn(Menu_Hint, true);
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "menu";
                        GenerateMenuPrompts(Menu_StartPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_LogoBanner.setSize(Vector2f(0, 200));
                        Menu_Scroll1.Playing = false;
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    if (Keyboard::isKeyPressed(Keyboard::S) || Down()) {
                        Menu_OptionSelected++;
                        if (Menu_OptionSelected > Menu_PlaySMax) Menu_OptionSelected = Menu_PlaySMax;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::W) || Up()) {
                        Menu_OptionSelected--;
                        if (Menu_OptionSelected < 0) Menu_OptionSelected = 0;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Menu_HomeRaid.CheckHighlight(Menu_OptionSelected)) Menu_HomeHint.setString("Begin a raid.\n\nScavenge loot by finding or killing enemies\nand extracting at various points.\n\nEnemies will become more deadly the longer\nyou're alive. Do whatever it takes to survive.");
                    if (Menu_HomeCharacter.CheckHighlight(Menu_OptionSelected)) Menu_HomeHint.setString("Manage your vitals and inventory.\n\nBetter make sure you're in tip-top shape\nbefore going in.");
                    if (Menu_HomeTraders.CheckHighlight(Menu_OptionSelected)) Menu_HomeHint.setString("Buy and sell with traders.\n\nAccept and fulfill quests to gain trust,\nunlocking more items to buy.");
                    if (Menu_HomeHideout.CheckHighlight(Menu_OptionSelected)) Menu_HomeHint.setString("[ Not Available in ALPHA ]\n\nEnter and upgrade your personal hideout.\n\nUnlocking and upgrading areas grants\naccess to helpful stations.");
                    if (Menu_HomeSettings.CheckHighlight(Menu_OptionSelected)) Menu_HomeHint.setString("Tweak game and video settings.");
                }

                // Raid Setting
                else if (menuState == "raidSetting") {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "home";
                        GenerateMenuPrompts(Menu_HomePrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    if (Keyboard::isKeyPressed(Keyboard::S) || Down()) {
                        Menu_OptionSelected++;
                        if (Menu_OptionSelected > Menu_RaidSMax) Menu_OptionSelected = Menu_RaidSMax;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::W) || Up()) {
                        Menu_OptionSelected--;
                        if (Menu_OptionSelected < 0) Menu_OptionSelected = 0;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        menuState = "raidInsurance";
                        Menu_RaidPrep.Increment(true, Menu_OptionSelected);
                        Player_Inventory.RefreshItems(CharStats.Backpack);
                        Player_Inventory.RefreshWeapons(CharStats.Weapon1, CharStats.Weapon2);
                        Player_Inventory.CheckWeaponHighlight(2);

                        string val = "No";
                        if (Menu_RaidInsureValue) val = "Yes";
                        Menu_RaidInsure.SetValue(val);

                        Menu_Select2.play();
                        promptUsed = true;
                    }

                    Menu_RaidMap.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_RaidTime.Active(Menu_OptionSelected, promptUsed, Menu_Select1);

                    if (Menu_RaidMap.Selected == 0) Menu_RaidMapHint.setString("Rozhok National Forest\n\nA popular tourist spot. Lots of trees covering the\nland make for great cover and for scavenging.\nLoot scattered everywhere.");
                    if (Menu_RaidMap.Selected == 1) Menu_RaidMapHint.setString("Demo Map\n\nAsset testing");

                    Menu_RaidMap.CheckHighlight(Menu_OptionSelected);
                    Menu_RaidTime.CheckHighlight(Menu_OptionSelected);
                }

                // Raid Insurance
                else if (menuState == "raidInsurance") {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "raidSetting";
                        Menu_RaidPrep.Increment(false, Menu_OptionSelected);
                        Menu_OptionSelected = 0;
                        Menu_Select2.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::R) || Y()) {
                        Menu_RaidInsureValue = !Menu_RaidInsureValue;
                        Menu_Select1.play();
                        string val = "No";
                        if (Menu_RaidInsureValue) val = "Yes";
                        Menu_RaidInsure.SetValue(val);
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        menuState = "raidDeployment";
                        GenerateMenuPrompts(Menu_RaidDeployPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_RaidPrep.Increment(true, Menu_OptionSelected);
                        ScrollTextIn(Menu_Hint, true);
                        Menu_RaidDeployHint.setString("DEPLOYING TO LOCATION...\n\n" + Menu_RaidMap.Selection.getString() + " - " + Menu_RaidTime.Selection.getString() + "\nItems Insured: " + Menu_RaidInsure.Val.getString());

                        // Insurance Testing
                        if (Menu_RaidInsureValue) {
                            for (int i = 0; i != CharStats.Backpack.size(); i++) {
                                CharStats.Backpack[i].Insured = true;
                            }
                        }

                        if (Menu_RaidMap.Selected == 0) LoadedMap = "forest";
                        if (Menu_RaidMap.Selected == 1) LoadedMap = "demo";

                        Menu_Select2.play();
                        promptUsed = true;
                    }

                    Menu_RaidInsureKey.CheckInput(inputMethod);
                }

                // Raid Deployment
                else if (menuState == "raidDeployment") {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "raidInsurance";
                        GenerateMenuPrompts(Menu_RaidPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_RaidPrep.Increment(false, Menu_OptionSelected);
                        Menu_OptionSelected = 0;
                        Menu_Select2.play();
                        promptUsed = true;
                    }

                    RaidSpawnInClock.restart();

                    //WorldItem testcola("Cola", "item");
                    //GroundLoot.push_back(testcola);
                    //ForestMap_Load(RaidLoadingComplete, Enemies, MapObjs);

                    if (LoadedMap == "forest") {
                        Forest = new ForestMap(RaidLoadingComplete, Enemies, Extracts, GroundLoot, MapBarriers, CharModel);
                        PlayerSafeSpawn.setPosition(CharModel.getPosition());

                        // loading enemes here instead because the way i have VOs setup :((
                        for (int i = 0; i != 10; i++) {
                            VOBuffer* SelectedVO;
                            if (rand() % 2 == 0) SelectedVO = &EnemyAVO;
                            else SelectedVO = &Enemy2AVO;
                            Enemies.push_back(Enemy("Enemy", *SelectedVO, Vector2f(rand() % (int)Forest->Ground->getGlobalBounds().width, rand() % (int)Forest->Ground->getGlobalBounds().height)));

                            while (Enemies.back().getGlobalBounds().intersects(PlayerSafeSpawn.getGlobalBounds())) { // prevent instant death on spawn by enemy
                                Enemies.back().setPosition(Vector2f(rand() % (int)Forest->Ground->getGlobalBounds().width, rand() % (int)Forest->Ground->getGlobalBounds().height));
                            }
                        }
                    }

                    if (LoadedMap == "demo") Demo = new DemoMap(RaidLoadingComplete, Enemies, Extracts, GroundLoot, MapBarriers, CharModel);

                    ExtractsAvail.clear();

                    for (int i = 0; i != rand() % Extracts.size() + 1; i++) {
                        ExtractsAvail.push_back(Value(Extracts[i].Name, 460, 45, 320));
                        ExtractsAvail.back().SetValue(". . .");
                        ExtractsAvail.back().Reposition(ExtractTimeLeft.getPosition().x, ExtractsAvail.size() * 45);
                    }
                }

                // Character
                else if (menuState == "character") {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "home";
                        GenerateMenuPrompts(Menu_HomePrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    // Give player stuff testing
                    if (Keyboard::isKeyPressed(Keyboard::F) && CheatMode) {
                        CharStats.EquipWeapon(GlobalWeapons[rand() % GlobalWeapons.size()]);

                        Player_Inventory.RefreshWeapons(CharStats.Weapon1, CharStats.Weapon2);
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::G) && CheatMode) {
                        CharStats.AddToBackpack(GlobalItems[rand() % GlobalItems.size()]);

                        Player_Inventory.RefreshItems(CharStats.Backpack);

                        promptUsed = true;
                    }

                    Player_Inventory.Active(CharStats, Menu_OptionSelected, Menu_Highlight1, promptUsed);
                    Player_WeaponHint.setString(Player_Inventory.SelectedHint);

                    Player_XPText.setString("Level " + to_string(CharStats.Level) + ": " + to_string(CharStats.CurrentXP) + "/" + to_string((CharStats.Level * 100 + 100)) + " XP");
                    Player_XPBar.SetValue((float)CharStats.CurrentXP / (float)(CharStats.Level * 100 + 100));
                }

                // Traders
                else if (menuState == "traders") {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "home";
                        GenerateMenuPrompts(Menu_HomePrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        VendorStats.Backpack.clear();
                        Menu_VendorInventory.WeaponCategory = true;
                        Menu_VendorInventory.Hint.setString("");
                        VendorStats.Weapon1.Remove();
                        VendorStats.Weapon2.Remove();

                        if (Menu_OptionSelected == 0) {
                            menuState = "traders_doctor";
                            Menu_VendorInventory.Hint.setString("This vendor cannot sell weapons.");
                            for (int i = 0; i != GlobalItems.size(); i++) {
                                VendorStats.AddToBackpack(GlobalItems[i]);
                            }
                        }
                        if (Menu_OptionSelected == 1) {
                            menuState = "traders_dealer";
                            VendorStats.EquipWeapon(GlobalWeapons[rand() % GlobalWeapons.size()]);
                            VendorStats.EquipWeapon(GlobalWeapons[rand() % GlobalWeapons.size()]);
                        }

                        Menu_VendorInventory.RefreshWeapons(VendorStats.Weapon1, VendorStats.Weapon2);
                        Menu_VendorInventory.RefreshItems(VendorStats.Backpack);
                        GenerateMenuPrompts(Menu_TraderVndrPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    if (Keyboard::isKeyPressed(Keyboard::S) || Down()) {
                        Menu_OptionSelected++;
                        if (Menu_OptionSelected > Menu_TraderSMax) Menu_OptionSelected = Menu_TraderSMax;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::W) || Up()) {
                        Menu_OptionSelected--;
                        if (Menu_OptionSelected < 0) Menu_OptionSelected = 0;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Menu_TradeDealer.CheckHighlight(Menu_OptionSelected)) Menu_TraderHint.setString("Dealer\n\nWhy use a knife when you can blast someone?");
                    if (Menu_TradeDoctor.CheckHighlight(Menu_OptionSelected)) Menu_TraderHint.setString("Doctor\n\nYou never know if you break your legs.");
                }

                // Trade Vendors
                else if (menuState == "traders_doctor" || menuState == "traders_dealer") {
                    if (Keyboard::isKeyPressed(Keyboard::Tab) || X()) {
                        if (Player_Inventory.Contains.size() > 0) {
                            Menu_Select1.play();
                            Player_InvSelWeapons = !Player_InvSelWeapons;
                            Player_Inventory.CheckItemHighlight(-1);
                            Player_Inventory.CheckWeaponHighlight(-1);
                            Menu_OptionSelected = 0;
                            promptUsed = true;
                        }
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        menuState = "traders";
                        GenerateMenuPrompts(Menu_TraderPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    Menu_VendorInventory.Active(VendorStats, Menu_OptionSelected, Menu_Highlight1, promptUsed, true, &CharStats);
                    Player_WeaponHint.setString(Menu_VendorInventory.SelectedHint);
                }
                // Raid Summary
                else if (menuState == "summary") {
                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        menuState = "home";
                        GenerateMenuPrompts(Menu_HomePrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }
                }
            }

            if (promptUsed) uiClock.restart();
        }

        if (inMenu) {
            if (!Menu_BackgroundReverse) {
                Menu_Background.move(-0.1, 0);
                Menu_Background2.move(-0.1, 0);
            }
            else { // this doesnt seem to be working
                Menu_Background.move(0.1, 0);
                Menu_Background2.move(0.1, 0);
            }

            if (Menu_BackgroundClock.getElapsedTime().asSeconds() > 5) {
                Menu_BackgroundReverse != Menu_BackgroundReverse;
                Menu_BackgroundClock.restart();
            }

            if (!MenuMusicPlaying && menuState != "raidStart") { // menu music loop
                int num = rand() % 2;
                if (num == 0) {
                    Menu_Music1.play();
                    Menu_CurrentMusic = &Menu_Music1;
                }
                else if (num == 1) {
                    Menu_Music2.play();
                    Menu_CurrentMusic = &Menu_Music2;
                }

                MenuMusicPlaying = true;
            }

            if (Menu_CurrentMusic->getStatus() == Sound::Stopped && MusicTimer.getElapsedTime().asSeconds() > 6) MenuMusicPlaying = false;
            if (Menu_CurrentMusic->getStatus() == Sound::Playing) MusicTimer.restart();

            if (menuState == "menu") {
                ScrollTextIn(Menu_Hint, false, "Welcome!");
                float a = Menu_Fade.getFillColor().a;
                float x = Menu_LogoBanner.getSize().x;
                if (a > 10) Menu_Fade.setFillColor(Color(0, 0, 0, a - 10));
                if (x < WinSize.x) {
                    Menu_LogoBanner.setSize(Vector2f(x + 100, 200));
                    if (!Menu_Scroll1.Playing) {
                        Menu_Scroll1.play();
                        Menu_Scroll1.Playing = true;
                    }
                }
            }

            if (menuState != "menu") {
                if (menuState == "settings") {
                    ScrollTextIn(Menu_Hint, false, "Adjust game settings.");
                    ScrollTextIn(Menu_Header, false, "Settings");
                }

                if (menuState == "home") {
                    ScrollTextIn(Menu_Hint, false, "Your adventure, your story.");
                    ScrollTextIn(Menu_Header, false, "Home");
                }

                if (menuState == "raidSetting" || menuState == "raidInsurance" || menuState == "raidDeployment") {
                    if (menuState == "raidDeployment") ScrollTextIn(Menu_Hint, false, "Loading raid...");
                    else ScrollTextIn(Menu_Hint, false, "Prepare for escape.");
                    ScrollTextIn(Menu_Header, false, "Raid");
                }

                if (menuState == "character") {
                    ScrollTextIn(Menu_Hint, false, "Preparation is key.");
                    ScrollTextIn(Menu_Header, false,  "Character");

                    Player_VitalsHealth.SetValue(to_string(CharStats.Health));
                    Player_VitalsCaps.SetValue(to_string(CharStats.Caps));

                    if (CharStats.Health >= 75) Player_VitalsHealthSin.PointColor = Color(76, 209, 55);
                    else if (CharStats.Health >= 25) Player_VitalsHealthSin.PointColor = Color(251, 197, 49);
                    else Player_VitalsHealthSin.PointColor = Color(232, 65, 24);

                    float r = (CharStats.Health/(float)100);
                    Player_VitalsHealthSin.Amplitude = (int)(50 * r);
                    Player_VitalsHealthSin.Rate = (int)(3/r);
                    Player_VitalsHealthSin.Stretch = (int)(2.4/r);

                    Player_VitalsHealthSin.Routine();
                    Player_XPText.setString("Level " + to_string(CharStats.Level) + ": " + to_string(CharStats.CurrentXP) + "/" + to_string((CharStats.Level * 100 + 100)) + " XP");
                    Player_XPBar.SetValue((float)CharStats.CurrentXP / (float)(CharStats.Level * 100 + 100));

                    if (Keyboard::isKeyPressed(Keyboard::Y) && CheatMode) CharStats.CurrentXP++;
                }

                if (menuState == "traders") {
                    ScrollTextIn(Menu_Hint, false, "Guns? Meds? You name it.");
                    ScrollTextIn(Menu_Header, false, "Traders");
                }

                if (menuState == "traders_doctor") {
                    ScrollTextIn(Menu_Hint, false, "Don't kill yourself out there.");
                    ScrollTextIn(Menu_Header, false, "Doctor");
                }

                if (menuState == "traders_dealer") {
                    ScrollTextIn(Menu_Hint, false, "Need more firepower?");
                    ScrollTextIn(Menu_Header, false, "Dealer");
                }

                if (menuState == "summary") {
                    string a = "";
                    if (RaidAftermath == "leave") a = "Missing In Action";
                    if (RaidAftermath == "extract") a = "Extract Successful";
                    if (RaidAftermath == "killed") a = "Killed In Action";
                    ScrollTextIn(Menu_Header, false, "Raid Ended");
                    ScrollTextIn(Menu_Hint, true);

                    Menu_SummaryOutcome.SetValue(a);
                    Menu_SummaryXP.SetValue(to_string(GainedXP));

                    float b = Menu_FrontFade.getFillColor().a;
                    if (b > 10) Menu_FrontFade.setFillColor(Color(0, 0, 0, b - 10));
                }

                if (menuState == "quit") ScrollTextIn(Menu_Hint, false, "You are about to quit the game. Continue?");

                float a = Menu_Fade.getFillColor().a;
                if (a < 100) Menu_Fade.setFillColor(Color(0, 0, 0, a + 10));
            }

            // Enter the Raid
            if (RaidLoadingComplete) {
                menuState = "raidStart";
                if (Menu_CurrentMusic->getVolume() > 1) Menu_CurrentMusic->setVolume(Menu_CurrentMusic->getVolume() - 1);

                int itime = 7000 - RaidSpawnInClock.getElapsedTime().asMilliseconds();
                int seconds = (itime / 1000) % 60;
                int milliseconds = itime % 100;

                string sectext = to_string(seconds);
                if (seconds < 10) sectext = "0" + sectext;

                RaidStartTimer.setString(sectext + '.' + to_string(milliseconds));
                RaidStartTimer.setPosition(WinSize.x/2 - RaidStartTimer.getGlobalBounds().width/2, WinSize.y/2);

                if (itime <= 0) {
                    inMenu = false;
                    Menu_CurrentMusic->stop();

                    Menu_FrontFade.setFillColor(Color::Black);
                    GainedXP = 0;
                    EnemiesKilled = 0;
                    SurvivalTime.restart();
                    RaidTimeLeft.restart();
                    EnemySpawnTimer.restart();
                }
            }
        }

        // Update all UI if resolution change
        if (UpdateResolution) {
            int Res = Menu_SettingsRes.Selected;
            int Win = Menu_SettingsWin.Selected;
            int WinNew;
            if (Res == 0) WinSize = Vector2f(1280, 768);
            if (Res == 1) WinSize = Vector2f(1280, 1024);
            if (Res == 2) WinSize = Vector2f(1920, 1080);
            if (Win == 0) WinNew = 4;
            if (Win == 1) WinNew = 0;
            if (Win == 2) WinNew = 8;

            window.create(VideoMode(WinSize.x, WinSize.y), "DESOCRETE [ Demo Release ]", WinNew);
            window.setFramerateLimit(60); // maybe bump this up to 75fps? 60fps feels somewhat meh
            window.setIcon(GameIcon.getSize().x, GameIcon.getSize().y, GameIcon.getPixelsPtr());

            Vector2f WinCenter = Vector2f(WinSize.x/2, WinSize.y/2);

            Menu_DemoText.setPosition(WinCenter.x - 335, WinCenter.y + 200);
            Menu_DemoFrame.Reposition(WinCenter.x - 350, WinCenter.y + 150);
            Menu_LogoBanner.setPosition(0, WinCenter.y - 90);
            Menu_Logo.setPosition(40, WinCenter.y - 100);
            Menu_Logo2.setPosition(40, WinCenter.y + 50);
            Menu_BottomBar.setPosition(0, WinSize.y - 50);
            Menu_Header.setPosition(WinCenter.x - 590, WinCenter.y - 304);
            Menu_Hint.setPosition(20, WinSize.y - 37);
            Menu_Credit.setPosition(20, 14);

            LoadingWheel.setPosition(WinSize.x - 111, WinSize.y - 48);

            // Settings Menu
            Menu_SettingsHint.setPosition(WinCenter.x + 90, WinCenter.y - 204);
            Menu_SettingsMove.Reposition(WinCenter.x - 590, WinCenter.y - 204);
            Menu_SettingsWin.Reposition(WinCenter.x - 590, WinCenter.y - 162);
            Menu_SettingsRes.Reposition(WinCenter.x - 590, WinCenter.y - 120);
            Menu_SettingsSFX.Reposition(WinCenter.x - 590, WinCenter.y - 78);

            // Home Menu
            Menu_HomeRaid.Reposition(WinCenter.x - 590, WinCenter.y - 204);
            Menu_HomeCharacter.Reposition(WinCenter.x - 590, WinCenter.y - 52);
            Menu_HomeTraders.Reposition(WinCenter.x - 590, WinCenter.y + 30);
            Menu_HomeHideout.Reposition(WinCenter.x - 590, WinCenter.y + 112);
            Menu_HomeSettings.Reposition(WinCenter.x - 590, WinCenter.y + 194);
            Menu_HomeHint.setPosition(WinCenter.x + 150, WinCenter.y - 204);

            // Raid Menu
            Menu_RaidMap.Reposition(WinCenter.x - 590, WinCenter.y - 204);
            Menu_RaidTime.Reposition(WinCenter.x - 590, WinCenter.y - 162);
            Menu_RaidPrep.Reposition(WinCenter.x - 590, WinCenter.y + 266);

            Menu_RaidMapPreview.setPosition(WinCenter.x - 590, WinCenter.y - 84);
            Menu_RaidMapHint.setPosition(WinCenter.x - 175, WinCenter.y - 84);

            Menu_RaidInsureHint.setPosition(WinCenter.x + 110, WinCenter.y - 204);
            Menu_RaidInsure.Reposition(WinCenter.x + 160, WinCenter.y + 136);
            Menu_RaidInsureKey.Reposition(WinCenter.x + 160, WinCenter.y + 181);

            Menu_RaidDeployHint.setPosition(WinCenter.x - 590, WinCenter.y - 204);

            // Character & Inventory
            Player_Inventory.Reposition(WinCenter.x - 590, WinCenter.y - 204);
            Player_Vitals.Reposition(WinCenter.x + 130, WinCenter.y - 204);
            Player_VitalsGuy.Reposition(WinCenter.x + 370, WinCenter.y - 6);
            Player_VitalsHealth.Reposition(WinCenter.x + 130, WinCenter.y - 4);
            Player_VitalsHealthSin.Reposition(WinCenter.x + 130, WinCenter.y + 51);
            Player_VitalsCaps.Reposition(WinCenter.x + 130, WinCenter.y + 166);
            Player_WeaponHint.setPosition(WinCenter.x + 155, WinCenter.y - 142);
            Player_XPBar.Reposition(WinCenter.x + 130, WinCenter.y - 254);
            Player_XPText.setPosition(WinCenter.x + 145, WinCenter.y - 251);

            // Trader Menu
            Menu_TraderHint.setPosition(WinCenter.x + 60, WinCenter.y - 204);
            Menu_TradeDoctor.Reposition(WinCenter.x - 590, WinCenter.y - 204);
            Menu_TradeDealer.Reposition(WinCenter.x - 590, WinCenter.y - 122);

            // Trader Vendor
            Menu_VendorInventory.Reposition(WinCenter.x - 590, WinCenter.y - 204);
            Menu_VendorStats.Reposition(WinCenter.x + 130, WinCenter.y - 204);

            // Other
            Menu_Fade.setSize(WinSize);
            Menu_TopBar.setSize(Vector2f(WinSize.x, 50));
            Menu_BottomBar.setSize(Vector2f(WinSize.x, 50));

            inputMethodPic.Reposition(WinSize.x - 120, WinSize.y - 110);

            Menu_BackgroundClock.restart();
            Menu_Background.setPosition(0, 0);
            Menu_Background2.setPosition(0, 0);
            Menu_Background.setScale(1.45 * WinSize.y/768, 1.45 * WinSize.y/768);
            Menu_Background2.setScale(1.45 * WinSize.y/768, 1.45 * WinSize.y/768);

            // Raid Start
            RaidStartText.setPosition(WinCenter.x - RaidStartText.getGlobalBounds().width/2, WinCenter.y - 70);

            // Raid Summary
            Menu_RaidSummary.Reposition(50, WinCenter.y - 204);
            Menu_SummaryOutcome.Reposition(50, WinCenter.y - 134);
            Menu_SummaryTime.Reposition(50, WinCenter.y - 54);
            Menu_SummaryKills.Reposition(50, WinCenter.y - 14);
            Menu_SummaryXP.Reposition(50, WinCenter.y + 26);

            Menu_FrontFade.setSize(WinSize);

            // In Raid
            WeaponAmmoCount.Reposition(WinSize.x - 144, WinSize.y - 52);
            ExtractTimeLeft.Reposition(0, WinSize.y - 40);
            ExtractPoints.Reposition(0, 0);
            ToggleExtracts.Reposition(5, 5);

            Raid_MenuHint.setPosition(20, WinSize.y - 90);

            HurtFade.setSize(Vector2f(WinSize.x, WinSize.y));

            ControlsPanel.setPosition(WinSize.x - 190, 0);
            ControlsHeader.setPosition(WinSize.x - 190, 0);
            ControlsOpen.Reposition(WinSize.x - 185, 5);
            ControlsWalk.Reposition(WinSize.x - 185, 50);
            ControlsSprint.Reposition(WinSize.x - 185, 80);
            ControlsInv.Reposition(WinSize.x - 185, 110);
            ControlsAim.Reposition(WinSize.x - 185, 140);
            ControlsShoot.Reposition(WinSize.x - 185, 170);
            ControlsSwap.Reposition(WinSize.x - 185, 200);
            ControlsMenu.Reposition(WinSize.x - 185, 230);

            if (menuState == "settings") GenerateMenuPrompts(Menu_SettingsPrompts, Menu_KeyPrompts, WinSize, inputMethod);
            else GenerateMenuPrompts(Menu_StartPrompts, Menu_KeyPrompts, WinSize, inputMethod);

            GameView.setSize(Vector2f(WinSize.x, WinSize.y));
            GameView.zoom(1.3);

            UpdateResolution = false;
        }

        // XP thing I shoved here randomly
        if (CharStats.CurrentXP >= (CharStats.Level * 100 + 100)) {
            CharStats.CurrentXP -= CharStats.Level * 100 + 100;
            CharStats.Caps += CharStats.Level * 50 + 50;
            CharStats.Level++;
        }

        if (!inMenu) {
        // In Raid
        // this is where the magic happens
            GameView.setCenter(CharModel.getPosition().x + 81, CharModel.getPosition().y + 162);
            CharStats.Routine();

            if (RaidMenuState == "playing" && !PlayerDied) CharModel.Routine(CharStats, Bullets, &window, &GameView);

            bool touching = false;
            for (int i = 0; i != MapBarriers.size(); i++) MapBarriers[i].Routine(CharModel, touching);
            if (!touching) LastValidPos = CharModel.getPosition();
            else CharModel.setPosition(LastValidPos);

            // Player Death Check
            if (CharStats.Health <= 0) {
                PlayerDied = true;
                Extracting = false;
                RaidAftermath = "killed";
                RaidMenuState = "playing";

                if (PlayerDied) CharModel.DeathAnimation();
            }

            // Extracts
            for (int i = 0; i != Extracts.size(); i++) {
                if (CharModel.getGlobalBounds().intersects(Extracts[i].getGlobalBounds())) {
                    if (CharStats.Health > 0) Extracting = true;
                    else Extracting = false;
                }
                else {
                    ExtractTimer.restart();
                    Extracting = false;
                }
            }

            if (Extracting) {
                if (ExtractTimer.getElapsedTime().asSeconds() >= 5) {
                    PlayerDied = true;
                    RaidAftermath = "extract";
                    RaidMenuState = "playing";
                }

                string t = "";
                for (int i = 0; i != 3; i++) t += (to_string(5 - ExtractTimer.getElapsedTime().asSeconds())[i]);

                if (ExtractTimer.getElapsedTime().asSeconds() >= 5) t = "0.0";

                ExtractTimeLeft.SetValue(t + "s");

                for (int i = 0; i != ExtractsAvail.size(); i++) {
                    for (int x = 0; x != Extracts.size(); x++) {
                        if ((string)ExtractsAvail[i].Title.getString() == Extracts[x].Name) ExtractsAvail[i].SetValue(t + "s");
                    }
                }
            }
            else {
                for (int i = 0; i != ExtractsAvail.size(); i++) ExtractsAvail[i].SetValue(". . .");
            }

            if (EnemySpawnTimer.getElapsedTime().asSeconds() > 60) {
                VOBuffer* SelectedVO;
                if (rand() % 2 == 0) SelectedVO = &EnemyAVO;
                else SelectedVO = &Enemy2AVO;
                Enemies.push_back(Enemy("Enemy", *SelectedVO, Vector2f(rand() % (int)Forest->Ground->getGlobalBounds().width, rand() % (int)Forest->Ground->getGlobalBounds().height)));

                EnemySpawnTimer.restart();
            }

            // Enemy AI
            for (int x = 0; x != Enemies.size(); x++) {
                bool aitouch = false;
                for (int i = 0; i != MapBarriers.size(); i++) MapBarriers[i].Routine(Enemies[i], aitouch);
                if (aitouch) {
                    Enemies[x].xGoal = rand() % 2501;
                    Enemies[x].yGoal = rand() % 2501;

                    if (rand() % 2 == 0) Enemies[x].xGoal = -Enemies[x].xGoal;
                    if (rand() % 2 == 0) Enemies[x].yGoal = -Enemies[x].yGoal;

                    Enemies[x].xGoal += Enemies[x].BasePosition.x;
                    Enemies[x].yGoal += Enemies[x].BasePosition.y;
                }

                if (Enemies[x].Stats.Health > 0) {
                    Enemies[x].Routine(Enemies[x].Stats, Bullets, &window, &GameView, true);
                    Enemies[x].AI(CharModel, Bullets);
                }
                else {
                    Enemies[x].DeathAnimation();

                    if (!Enemies[x].LootDropped) {
                        EnemiesKilled++;
                        Enemies[x].LootDropped = true;
                        if (rand() % 10 > 0) GroundLoot.push_back(WorldItem(GlobalItems[rand() % GlobalItems.size()].Name, "item", Vector2f(Enemies[x].getPosition().x, Enemies[x].getPosition().y + 150)));
                        GroundLoot.push_back(WorldItem(Enemies[x].Stats.Holding->Name, Enemies[x].Stats.Holding->Type, Vector2f(Enemies[x].getPosition().x - 150, Enemies[x].getPosition().y + 150), Enemies[x].Stats.Holding->Loaded, Enemies[x].Stats.Holding->Reserve));
                    }
                }

                Enemies[x].setTexture(*Enemies[x].Sheet);

                for (int i = 0; i < Bullets.size(); i++) {
                    if (Bullets[i].getGlobalBounds().intersects(Enemies[x].getGlobalBounds())) {
                        if (Bullets[i].ImpactType == "none") {
                            Bullets[i].setPosition(Enemies[x].getPosition().x + Enemies[x].getGlobalBounds().width/2, Enemies[x].getPosition().y + Enemies[x].getGlobalBounds().height/2);
                            Enemies[x].Stats.Health -= Bullets[i].Damage;

                            if (Enemies[x].Stats.Health > 0) {
                                if (rand() % 10 > 5) {
                                    int r = rand() % 2;
                                    while (r == Enemies[x].LastHurtVO) r = rand() % 2;
                                    if (r == 0) Enemies[x].Hurt1.play();
                                    if (r == 1) Enemies[x].Hurt2.play();
                                    if (r == 2) Enemies[x].Hurt3.play();
                                    Enemies[x].LastHurtVO = r;
                                }
                            }
                            else if (!Enemies[x].DeathShout) {
                                if (rand() % 2 == 0) Enemies[x].Dead1.play();
                                else Enemies[x].Dead2.play();
                                Enemies[x].DeathShout = true;
                            }

                            Bullets[i].ImpactType = "blood";
                        }
                    }
                }
            }

            // Bullet behaviour
            for (int i = 0; i < Bullets.size(); i++) {
                for (int x = 0; x != MapBarriers.size(); x++) {
                    if (Bullets[i].getGlobalBounds().intersects(MapBarriers[x].getGlobalBounds()) && Bullets[i].ImpactType == "none") {
                        Bullets[i].ImpactType = "object";
                    }
                }

                if (Bullets[i].getGlobalBounds().intersects(CharModel.getGlobalBounds())) {
                    if (Bullets[i].ImpactType == "none") {
                        Bullets[i].setPosition(CharModel.getPosition().x + CharModel.getGlobalBounds().width/2, CharModel.getPosition().y + CharModel.getGlobalBounds().height/2);
                        CharStats.Health -= Bullets[i].Damage;
                        if (CharStats.Health > 0) {
                            if (rand() % 10 > 5) {
                                int r = rand() % 3;
                                while (r == CharModel.LastHurtVO) r = rand() % 3;
                                if (r == 0) CharModel.Hurt1.play();
                                if (r == 1) CharModel.Hurt2.play();
                                if (r == 2) CharModel.Hurt3.play();
                                CharModel.LastHurtVO = r;
                            }
                        }
                        else if (!CharModel.DeathShout) {
                            if (rand() % 2 == 0) CharModel.Dead1.play();
                            else CharModel.Dead2.play();
                            CharModel.DeathShout = true;
                        }

                        Bullets[i].ImpactType = "blood";
                    }
                }

                Bullets[i].Routine();
                if (Bullets[i].Lifetime.getElapsedTime().asSeconds() > 3 && Bullets[i].ImpactType != "none" || Bullets[i].Destroy) {
                    delete Bullets[i].ImpactTex;
                    delete Bullets[i].ImpactBuffer;
                    Bullets.erase(Bullets.begin() + i);
                }
            }

            WeaponAmmoCount.Routine(CharStats, inputMethod);

            if (CharStats.Health > 100) CharStats.Health = 100;

            float Hurt = CharStats.Health;
            if (Hurt >= 100) Hurt = 100;
            HurtFade.setFillColor(Color(255, 255, 255, 255 - (255 * (Hurt)/(float)100)));

            float c = Menu_FrontFade.getFillColor().a;
            if (c > 5) Menu_FrontFade.setFillColor(Color(0, 0, 0, c - 5));

            // raid time
            int t = 900000 - RaidTimeLeft.getElapsedTime().asMilliseconds();
            int seconds = (int) (t / 1000) % 60;
            int minutes = (int) ((t / (1000*60)) % 60);

            string mintext = to_string(minutes);
            if (minutes < 10) mintext = "0" + mintext;

            string sectext = to_string(seconds);
            if (seconds < 10) sectext = "0" + sectext;

            string milltext = "", millbase = to_string(t);
            for (int i = 0; i != 3; i++) milltext += millbase[i];

            ExtractPoints.SetValue(mintext + ":" + sectext + "." + milltext);

            if (t <= 0) {
                if (t <= 0) ExtractPoints.SetValue("--:--.---");
                PlayerDied = true;
                RaidAftermath = "leave";
            }

            bool AlreadyLooted = false;
            for (int i = 0; i < GroundLoot.size(); i++) {
                if (GroundLoot[i].Routine(CharModel, CharStats, inputMethod) && LootCooldown.getElapsedTime().asSeconds() > 0.5 && RaidMenuState == "playing") {
                    bool success = false;
                    // Check items
                    for (int x = 0; x != GlobalItems.size(); x++) {
                        if (GlobalItems[x].Name == GroundLoot[i].Name && CharStats.Backpack.size() < 12 && !AlreadyLooted) {
                            CharStats.AddToBackpack(Item(GlobalItems[x].Name, GlobalItems[x].Type, GlobalItems[x].Price, GlobalItems[i].Hint));
                            success = true;
                            AlreadyLooted = true;
                        }
                    }

                    // Check weapons
                    for (int x = 0; x != GlobalWeapons.size(); x++) {
                        if (GlobalWeapons[x].Name == GroundLoot[i].Name && !AlreadyLooted) {
                            if (CharStats.Weapon1.Name == "" || CharStats.Weapon2.Name == "") {
                                CharStats.EquipWeapon(GlobalWeapons[x], true, GroundLoot[i].Loaded, GroundLoot[i].Reserve);
                                success = true;
                                AlreadyLooted = true;
                            }
                        }
                    }

                    if (success) {
                        Item_Pickup.play();
                        delete GroundLoot[i].Tex;
                        delete GroundLoot[i].Prompt;
                        GroundLoot.erase(GroundLoot.begin() + i);
                        LootCooldown.restart();
                    }
                }
            }

            bool promptUsed = false;
            bool uiCooldown = uiClock.getElapsedTime().asSeconds() > 0.35;

            ControlsOpen.CheckInput(inputMethod);
            ControlsWalk.CheckInput(inputMethod);
            ControlsSprint.CheckInput(inputMethod);
            ControlsInv.CheckInput(inputMethod);
            ControlsShoot.CheckInput(inputMethod);
            ControlsAim.CheckInput(inputMethod);
            ControlsSwap.CheckInput(inputMethod);
            ControlsMenu.CheckInput(inputMethod);
            ToggleExtracts.CheckInput(inputMethod);

            if (uiCooldown && !PlayerDied) {
                // Activate escape menu
                if (RaidMenuState == "playing") {
                    if (Keyboard::isKeyPressed(Keyboard::E) || Back()) { // Open Inventory
                        Player_Inventory.RefreshItems(CharStats.Backpack, true);
                        Player_Inventory.RefreshWeapons(CharStats.Weapon1, CharStats.Weapon2, true);
                        RaidMenuState = "inventory";
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Inventory_Open);
                        GenerateMenuPrompts(Raid_InventoryPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Escape) || Start()) {
                        if (RaidMenuState == "menu") RaidMenuState = "playing";
                        else {
                            RaidMenuState = "menu";
                            ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Header3);
                        }
                        GenerateMenuPrompts(Raid_GameMenuPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::M) || DPadDown()) {
                        ControlsShowing = !ControlsShowing;
                        if (ControlsShowing) ControlsPanel.setSize(Vector2f(190, 270));
                        else ControlsPanel.setSize(Vector2f(190, 45));
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::U) || DPadLeft()) {
                        ExtractsShowing = !ExtractsShowing;
                        if (ExtractsShowing) ExtractPoints.Frame.setFillColor(Color(220, 0, 0));
                        else ExtractPoints.Frame.setFillColor(Color(50, 50, 50));
                        promptUsed = true;
                    }
                }

                // Inventory functionailty
                if (RaidMenuState == "inventory" && !promptUsed) {
                    // Discard items onto ground as loot
                    if (Player_Inventory.CheckDrop()) {
                        WorldItem newItem(Player_Inventory.DropName, Player_Inventory.DropType, Vector2f(CharModel.getPosition().x + (rand() % 100), CharModel.getPosition().y + 200), Player_Inventory.DropLoad, Player_Inventory.DropRes);
                        GroundLoot.push_back(newItem);
                        Player_Inventory.DropName = "";
                        Player_Inventory.DropType = "";
                        Player_Inventory.DropLoad = 0;
                        Player_Inventory.DropRes = 0;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::E) || B()) {
                        RaidMenuState = "playing";
                        promptUsed = true;
                    }

                    // Inventory management active
                    Player_Inventory.Active(CharStats, Menu_OptionSelected, Menu_Highlight1, promptUsed, false, &CharStats, true);
                    Player_WeaponHint.setString(Player_Inventory.SelectedHint);
                }
                else if (RaidMenuState == "menu" && !promptUsed) {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        RaidMenuState = "playing";
                        Menu_Select2.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::R) || Y()) {
                        RaidMenuState = "leave";
                        GenerateMenuPrompts(Menu_AcceptPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Game_Exit.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::E) || X()) {
                        RaidMenuState = "settings";
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Header1);
                        GenerateMenuPrompts(Menu_SettingsPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        promptUsed = true;
                    }
                }
                else if (RaidMenuState == "leave" && !promptUsed) {
                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        PlayerDied = true;
                        RaidAftermath = "leave";
                        RaidMenuState = "playing";
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        RaidMenuState = "menu";
                        GenerateMenuPrompts(Raid_GameMenuPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }
                }
                else if (RaidMenuState == "settings" && !promptUsed) {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) || B()) {
                        RaidMenuState = "menu";
                        GenerateMenuPrompts(Raid_GameMenuPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        Menu_OptionSelected = 0;
                        ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Menu_Select2);
                    }

                    if (Keyboard::isKeyPressed(Keyboard::S) || Down()) {
                        Menu_OptionSelected++;
                        if (Menu_OptionSelected > Menu_SettingsSMax) Menu_OptionSelected = Menu_SettingsSMax;
                        Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::W) || Up()) {
                        Menu_OptionSelected--;
                        if (Menu_OptionSelected < 0) Menu_OptionSelected = 0;
                        else Menu_Highlight1.play();
                        promptUsed = true;
                    }

                    Menu_SettingsMove.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_SettingsWin.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_SettingsRes.Active(Menu_OptionSelected, promptUsed, Menu_Select1);
                    Menu_SettingsSFX.Active(Menu_OptionSelected, promptUsed, Menu_Select1);

                    if (Keyboard::isKeyPressed(Keyboard::Return) || A()) {
                        UpdateResolution = true;
                        promptUsed = true;
                    }

                    if (Menu_SettingsMove.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> KBM Move Method\n\nChange between WASD and Arrow Keys to move\nyour character.");
                    if (Menu_SettingsWin.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> Window Mode\n\nPlay in Windowed, Borderless, or Fullscreen\nresolution.");
                    if (Menu_SettingsRes.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> Screen Resolution\n\nScale the window size.");
                    if (Menu_SettingsSFX.CheckHighlight(Menu_OptionSelected)) Menu_SettingsHint.setString("> Sound + Music\n\nTurn the sound and music on or off.\n\nWhy would you want these off anyway?");
                }

                if (promptUsed) uiClock.restart();
            }

            float a = Menu_Fade.getFillColor().a;
            if (RaidMenuState != "playing") {
                if (a < 100) Menu_Fade.setFillColor(Color(0, 0, 0, a + 10));
            }
            else if (!PlayerDied) {
                if (a > 10) Menu_Fade.setFillColor(Color(0, 0, 0, a - 10));
            }

            if (RaidMenuState == "menu") {
                ScrollTextIn(Menu_Header, false, "Game Menu");
                ScrollTextIn(Menu_Hint, false, "Select an option.");
            }

            if (RaidMenuState == "leave") {
                ScrollTextIn(Menu_Hint, false, "Leaving the raid will count as a death. Are you sure you want to leave?");
            }

            if (RaidMenuState == "settings") {
                ScrollTextIn(Menu_Hint, false, "Adjust game settings.");
                ScrollTextIn(Menu_Header, false, "Settings");
            }

            if (RaidMenuState == "inventory") {
                ScrollTextIn(Menu_Header, false, "Inventory");

                if (Keyboard::isKeyPressed(Keyboard::G) && CharStats.Health > 0) CharStats.Health -= 1;

                Player_VitalsHealth.SetValue(to_string(CharStats.Health));
                Player_VitalsCaps.SetValue(to_string(CharStats.Caps));

                if (CharStats.Health >= 75) Player_VitalsHealthSin.PointColor = Color(76, 209, 55);
                else if (CharStats.Health >= 25) Player_VitalsHealthSin.PointColor = Color(251, 197, 49);
                else Player_VitalsHealthSin.PointColor = Color(232, 65, 24);

                float r = (CharStats.Health/(float)100);
                if (r <= 0.01) r = 0.01; // sine crash fix
                Player_VitalsHealthSin.Amplitude = (int)(50 * r);
                Player_VitalsHealthSin.Rate = (int)(3/r);
                Player_VitalsHealthSin.Stretch = (int)(2.4/r);

                Player_VitalsHealthSin.Routine();
            }

            float b = Menu_Fade.getFillColor().a;
            if (PlayerDied) {
                if (b < 255) {
                    Menu_Fade.setFillColor(Color(0, 0, 0, b + 1));
                    RaidExitDelay.restart();
                }
                else {
                    if (RaidExitDelay.getElapsedTime().asSeconds() > 3) {
                        PlayerDied = false;
                        inMenu = true;
                        menuState = "summary";
                        RaidLoadingComplete = false;
                        Menu_LogoBanner.setSize(Vector2f(0, 200));
                        Menu_FrontFade.setFillColor(Color::Black);
                        CharStats.Health = 100;
                        CharModel.DeathShout = false;

                        int t = SurvivalTime.getElapsedTime().asMilliseconds();
                        int seconds = (int) (t / 1000) % 60;
                        int minutes = (int) ((t / (1000*60)) % 60);

                        string mintext = to_string(minutes);
                        if (minutes < 10) mintext = "0" + mintext;

                        string sectext = to_string(seconds);
                        if (seconds < 10) sectext = "0" + sectext;

                        string milltext = "", millbase = to_string(t);KeyPrompt ControlsShoot("Mouse_Left", "RB", "Shoot", 35);
                        for (int i = 0; i != 3; i++) milltext += millbase[i];

                        Menu_SummaryTime.SetValue(mintext + ":" + sectext + "." + milltext);
                        Menu_SummaryKills.SetValue(to_string(EnemiesKilled));

                        if (!Extracting) {
                            if (rand() % 2 == 0) CharStats.RemoveWeapon(0);
                            else CharStats.RemoveWeapon(1);
                            for (int i = 0; i != CharStats.Backpack.size()/2; i++) CharStats.Backpack.erase(CharStats.Backpack.begin() + i);
                            Player_Inventory.RefreshItems(CharStats.Backpack);
                            Player_Inventory.RefreshWeapons(CharStats.Weapon1, CharStats.Weapon2);

                            CharStats.CurrentXP += 100;
                            GainedXP += 100;
                        }
                        else {
                            CharStats.CurrentXP += 300;
                            GainedXP += 300;
                        }

                        Enemies.clear();
                        GroundLoot.clear();
                        Extracts.clear();
                        MapBarriers.clear();

                        if (LoadedMap == "forest") delete Forest;
                        if (LoadedMap == "demo") delete Demo;
                        LoadedMap = "none";

                        Menu_BackgroundClock.restart();
                        Menu_Background.setPosition(0, 0);
                        Menu_Background2.setPosition(0, 0);

                        GenerateMenuPrompts(Menu_RaidSummaryPrompts, Menu_KeyPrompts, WinSize, inputMethod);
                        if (Extracting) ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Raid_Success);
                        else ScrollHeader(promptUsed, Menu_Header, Menu_Hint, Raid_Failure);
                        Menu_Fade.setFillColor(Color(0, 0, 0, 0));

                    }
                }
            }

            // Map Routine
            if (LoadedMap == "forest") Forest->Routine();
        }

        // Crosshair functionality
        if (!inMenu && RaidMenuState == "playing") {
            if (inputMethod == "kb") Crosshair.setPosition(Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y));
            else if (inputMethod == "ctrl") {
                if (LB()) {
                    if (LStickLeft()) Crosshair.move(-10, 0);
                    if (LStickRight()) Crosshair.move(10, 0);
                    if (LStickUp()) Crosshair.move(0, -10);
                    if (LStickDown()) Crosshair.move(0, 10);
                }

                Mouse::setPosition(Vector2i(Crosshair.getPosition().x, Crosshair.getPosition().y), window);
            }

            window.setMouseCursorVisible(false);
        }
        else window.setMouseCursorVisible(true);

        Vector2f xhairpos = Crosshair.getPosition();
        if (xhairpos.x <= 0) Crosshair.setPosition(0, xhairpos.y);
        if (xhairpos.x >= WinSize.x) Crosshair.setPosition(WinSize.x, xhairpos.y);
        if (xhairpos.y <= 0) Crosshair.setPosition(xhairpos.x, 0);
        if (xhairpos.y >= WinSize.y) Crosshair.setPosition(xhairpos.x, WinSize.y);

        window.clear();
        world.Step((float32)1/60, 8, 8);

        // in menu
        if (inMenu) {
            if (Menu_CurrentBackground != "forest") window.draw(Menu_Background);
            if (Menu_CurrentBackground != "desert") window.draw(Menu_Background2);

            window.draw(Menu_TopBar);
            window.draw(Menu_BottomBar);
            window.draw(Menu_Fade);
            window.draw(inputMethodPic);

            if (menuState == "menu") {
                window.draw(Menu_Credit);
                window.draw(Menu_LogoBanner);
                window.draw(Menu_Logo);
                window.draw(Menu_Logo2);
                Menu_DemoFrame.Draw(window);
                window.draw(Menu_DemoText);
            }

            if (menuState != "menu" && menuState != "raidStart") {
                window.draw(Menu_Header);
            }

            if (menuState == "home") {
                window.draw(Menu_Credit);
                window.draw(Menu_HomeHint);
                Menu_HomeRaid.Draw(window);
                Menu_HomeCharacter.Draw(window);
                Menu_HomeTraders.Draw(window);
                Menu_HomeHideout.Draw(window);
                Menu_HomeSettings.Draw(window);
            }

            if (menuState == "settings") {
                window.draw(Menu_SettingsHint);
                Menu_SettingsMove.Draw(window);
                Menu_SettingsWin.Draw(window);
                Menu_SettingsRes.Draw(window);
                Menu_SettingsSFX.Draw(window);
            }

            if (menuState == "raidSetting") {
                Menu_RaidMap.Draw(window);
                Menu_RaidTime.Draw(window);
                Menu_RaidPrep.Draw(window);
                window.draw(Menu_RaidMapPreview);
                window.draw(Menu_RaidMapHint);
            }

            if (menuState == "raidInsurance") {
                Player_Inventory.Draw(window);
                Menu_RaidPrep.Draw(window);
                Menu_RaidInsure.Draw(window);
                window.draw(Menu_RaidInsureHint);
                Menu_RaidInsureKey.Draw(window);
            }

            if (menuState == "raidDeployment") {
                window.draw(Menu_RaidMapPreview);
                Menu_RaidPrep.Draw(window);
                window.draw(Menu_RaidDeployHint);
            }

            if (menuState == "character") {
                Player_Inventory.Draw(window);
                Player_Vitals.Draw(window);
                window.draw(Player_VitalsGuy);
                Player_VitalsHealth.Draw(window);
                Player_VitalsHealthSin.Draw(window);
                Player_VitalsCaps.Draw(window);
                window.draw(Player_WeaponHint);
                Player_XPBar.Draw(window);
                window.draw(Player_XPText);
            }

            if (menuState == "traders_dealer" || menuState == "traders_doctor") {
                if (menuState == "traders_doctor") Menu_VendorInventory.Draw(window);
                else Menu_VendorInventory.Draw(window);
                Menu_VendorStats.Draw(window);
                window.draw(Player_WeaponHint);
            }

            if (menuState == "traders") {
                window.draw(Menu_TraderHint);
                Menu_TradeDoctor.Draw(window);
                Menu_TradeDealer.Draw(window);
            }

            if (menuState != "raidStart") {
                window.draw(Menu_Hint);
                for (int i = 0; i != Menu_KeyPrompts.size(); i++) {
                    Menu_KeyPrompts[i].Draw(window);
                }
            }
            else {
                window.draw(RaidStartText);
                window.draw(RaidStartTimer);
            }

            if (menuState == "summary") {
                Menu_RaidSummary.Draw(window);
                Menu_SummaryOutcome.Draw(window);
                Menu_SummaryTime.Draw(window);
                Menu_SummaryKills.Draw(window);
                Menu_SummaryXP.Draw(window);
                window.draw(Menu_FrontFade);
            }
        }
        // In game
        else {
            window.setView(GameView);
            if (LoadedMap == "forest") Forest->Draw(window);
            if (LoadedMap == "demo") Demo->Draw(window);
            //for (int i = 0; i != MapBarriers.size(); i++) window.draw(MapBarriers[i]);
            //for (int i = 0; i != Extracts.size(); i++) window.draw(Extracts[i]);
            for (int i = 0; i != Bullets.size(); i++) window.draw(Bullets[i]);
            for (int i = 0; i != GroundLoot.size(); i++) GroundLoot[i].Draw(window);
            for (int i = 0; i != Enemies.size(); i++) Enemies[i].Draw(window);

            CharModel.Draw(window);

            window.setView(window.getDefaultView());
            window.draw(HurtFade);

            // Controls
            if (RaidMenuState == "playing") {
                window.draw(ControlsPanel);
                if (ControlsShowing) {
                    ControlsWalk.Draw(window);
                    ControlsSprint.Draw(window);
                    ControlsInv.Draw(window);
                    ControlsAim.Draw(window);
                    ControlsShoot.Draw(window);
                    ControlsSwap.Draw(window);
                    ControlsMenu.Draw(window);
                    window.draw(ControlsHeader);
                }
                ControlsOpen.Draw(window);
                ExtractPoints.Draw(window);
                for (int i = 0; i != ExtractsAvail.size(); i++) {
                    if (ExtractsShowing) ExtractsAvail[i].Draw(window);
                }
                if (Extracting) ExtractTimeLeft.Draw(window);
                ToggleExtracts.Draw(window);

                window.draw(Crosshair);
            }
            WeaponAmmoCount.Draw(window);
            window.draw(inputMethodPic);

            window.draw(Menu_Fade);

            if (PlayerDied && Menu_Fade.getFillColor().a >= 255) window.draw(LoadingWheel);

            if (RaidMenuState != "playing") {
                window.draw(Menu_TopBar);
                window.draw(Menu_BottomBar);
                for (int i = 0; i != Menu_KeyPrompts.size(); i++) Menu_KeyPrompts[i].Draw(window);

                if (RaidMenuState != "leave") window.draw(Menu_Header);
                window.draw(Menu_Hint);

                if (RaidMenuState == "menu" || RaidMenuState == "leave") {
                    window.draw(Raid_MenuHint);
                }

                if (RaidMenuState == "settings") {
                    window.draw(Menu_SettingsHint);
                    Menu_SettingsMove.Draw(window);
                    Menu_SettingsWin.Draw(window);
                    Menu_SettingsRes.Draw(window);
                    Menu_SettingsSFX.Draw(window);
                }
                if (RaidMenuState == "inventory") {
                    Player_Inventory.Draw(window);
                    Player_Vitals.Draw(window);
                    window.draw(Player_VitalsGuy);
                    Player_VitalsHealth.Draw(window);
                    Player_VitalsHealthSin.Draw(window);
                    Player_VitalsCaps.Draw(window);
                    window.draw(Player_WeaponHint);
                    Player_XPBar.Draw(window);
                    window.draw(Player_XPText);
                }
            }

            window.draw(Menu_FrontFade);
        }

        window.display();
    }

    return 0;
}
