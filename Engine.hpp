// Engine assets (UI, audio, etc.)

#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Controller.hpp"
#include "Player.hpp"
#include "math.h"

using namespace sf;
using namespace std;

string Dir_BindsKBM = "Engine\\Binds_KBM\\";
string Dir_BindsXBONE = "Engine\\Binds_XBONE\\";
string Dir_Fonts = "Engine\\Fonts\\";
string Dir_Assets = "Engine\\Assets\\";
string Dir_Sounds = "Engine\\Sounds\\";
string Dir_Music = "Engine\\Music\\";

Font BarlowB;

// Sound, skips having to make a SoundBuffer on seperate line
struct Audio : public Sound {
    Audio(string buffer, string type = "sound") {
        if (type == "sound") Buffer.loadFromFile(Dir_Sounds + buffer + ".ogg");
        if (type == "music") Buffer.loadFromFile(Dir_Music + buffer + ".ogg");
        this->setBuffer(Buffer);
    }

    SoundBuffer Buffer;
    bool Playing = false;
};

// Functions

// Scrolling in text effect
void ScrollTextIn(Text& obj, bool start, string text = "") {
    if (start) obj.setString("");
    else {
        if (obj.getString().getSize() != text.length()) {
            string newText = "";
            for (int i = 0; i != obj.getString().getSize(); i++) {
                newText += text[i];
            }
            obj.setString(newText + text[obj.getString().getSize()]);
        }
    }
}

// Fancy text outlines
void OutlineText(Text& text, bool white = false) {
    if (!white) text.setOutlineColor(Color(0, 0, 0, 100));
    else text.setOutlineColor(Color(255, 255, 255, 30));
    text.setOutlineThickness(1);
}

// Scrolling Header effect
void ScrollHeader(bool& prompt, Text& head, Text& hint, Audio& audio) {
    prompt = true;
    ScrollTextIn(head, true);
    ScrollTextIn(hint, true);
    audio.play();
}

// Images, skip Texture
struct Picture : public RectangleShape {
    Picture(float width, float height, string dir = "none") : RectangleShape(Vector2f(width, height)) {
        if (dir != "none") LoadImage(dir);
        else this->setFillColor(Color::Transparent);
    }

    void LoadImage(string dir) {
        Tex.loadFromFile(Dir_Assets + dir + ".png");
        this->setTexture(&Tex);
        this->setFillColor(Color::White);
    }

    void Blank() { this->setFillColor(Color::Transparent); }
    void Reposition(float x, float y) { this->setPosition(x, y); }

    Texture Tex;
};

// UI
// Keyboard/Controller prompt
struct KeyPrompt : public RectangleShape {
    KeyPrompt(string key, string button, string text, float height) : RectangleShape(Vector2f(height, height)) {
        KeyImage = new Texture;
        ButtonImage = new Texture;
        KeyImage->loadFromFile(Dir_BindsKBM + "Keyboard_White_" + key + ".png");
        ButtonImage->loadFromFile(Dir_BindsXBONE + "XboxOne_" + button + ".png");
        this->setTexture(KeyImage);
        PromptText.setString(text);
        PromptText.setFont(BarlowB);
        PromptText.setCharacterSize(height - 15);
        Reposition(0, 0);
        OutlineText(PromptText, true);
    }

    void CheckInput(string method) {
        if (method == "kb") this->setTexture(KeyImage);
        if (method == "ctrl") this->setTexture(ButtonImage);
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        PromptText.setPosition(x + this->getSize().x + 3, y + 5);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(PromptText);
    }

    Text PromptText;
    Texture* KeyImage;
    Texture* ButtonImage;
};

struct KeyBind {
    KeyBind(string key, string button, string text) {
        Key = key;
        Button = button;
        Text = text;
    }

    string Key, Button, Text;
};

// Generate key prompts in bottom right
void GenerateMenuPrompts(vector<KeyBind> binds, vector<KeyPrompt>& prompts, Vector2f winSize, string inputMethod) {
    for (int i = 0; i != prompts.size(); i++) {
        delete prompts[i].KeyImage;
        delete prompts[i].ButtonImage;
    }
    prompts.clear();
    float lastX = winSize.x - 15;
    for (int i = 0; i != binds.size(); i++) {
        prompts.push_back(KeyPrompt(binds[i].Key, binds[i].Button, binds[i].Text, 37));
        float subamt = 52;
        if (binds[i].Text == "") subamt = 35;
        lastX -= subamt + prompts.back().PromptText.getGlobalBounds().width;
        prompts.back().Reposition(lastX, winSize.y - 43);
        prompts.back().CheckInput(inputMethod);
    }
}

// Left/Right selectors
struct Selector : public RectangleShape {
    Selector(string name, vector<string>& values, int num) : RectangleShape(Vector2f(600, 40)) {
        this->setFillColor(Color(50, 50, 50));
        Frame.setSize(Vector2f(240, 40));
        Frame.setFillColor(Color(220, 0, 0));
        Title.setFont(BarlowB);
        Title.setString(name);
        Title.setCharacterSize(25);
        Selection.setFont(BarlowB);
        Selection.setCharacterSize(25);
        Fraction.setFont(BarlowB);
        Fraction.setCharacterSize(15);
        SelectNum = num;

        OutlineText(Title);
        OutlineText(Selection);
        OutlineText(Fraction);

        for (int i = 0; i != values.size(); i++) Add(values[i]);
        Reposition(0, 0);
    }

    void Add(string option) {
        Options.push_back(option);
        if (Options.size() == 1) Selection.setString(Options[0]);
        Fraction.setString(to_string(Selected + 1) + "/" + to_string(Options.size()));
    }

    void Increment(bool more, int num) {
        if (num == SelectNum) {
            if (more) {
                Selected++;
                if (Selected > Options.size() - 1) Selected = 0;
            }
            else {
                Selected--;
                if (Selected < 0) Selected = Options.size() - 1;
            }

            Selection.setString(Options[Selected]);
            Fraction.setString(to_string(Selected + 1) + "/" + to_string(Options.size()));
        }
    }

    bool CheckHighlight(int amount) {
        if (amount == SelectNum) {
            this->setOutlineThickness(2);
            this->setFillColor(Color(220, 0, 0));
            return true;
        }
        else {
            this->setOutlineThickness(0);
            this->setFillColor(Color(50, 50, 50));
        }
    }

    void Active(int sel, bool& prompt, Audio& select) {
        if (CheckHighlight(sel)) {
            if (Keyboard::isKeyPressed(Keyboard::A) || Left()) {
                Increment(false, sel);
                select.play();
                prompt = true;
            }

            if (Keyboard::isKeyPressed(Keyboard::D) || Right()) {
                Increment(true, sel);
                select.play();
                prompt = true;
            }
        }
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        Frame.setPosition(x, y);
        Title.setPosition(x + 15, y + 5);
        Selection.setPosition(x + 265, y + 5);
        Fraction.setPosition(x + 560, y + 12);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(Frame);
        window.draw(Title);
        window.draw(Selection);
        window.draw(Fraction);
    }

    RectangleShape Frame;
    Text Title, Selection, Fraction;
    vector<string> Options;
    int Selected = 0;
    int SelectNum;
};

// Values/ statuses
struct Value : public RectangleShape {
    Value(string name, float width, float height, float width2) : RectangleShape(Vector2f(width, height)) {
        this->setFillColor(Color(50, 50, 50));
        Frame.setSize(Vector2f(width2, height));
        Frame.setFillColor(Color(220, 0, 0));
        Title.setFont(BarlowB);
        Title.setString(name);
        Title.setCharacterSize(25);
        Val.setFont(BarlowB);
        Val.setCharacterSize(25);

        OutlineText(Title);
        OutlineText(Val);

        Reposition(0, 0);
    }

    void SetValue(string value) {
        Val.setString(value);
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        Frame.setPosition(x, y);
        Title.setPosition(x + 15, y + 5);
        Val.setPosition(x + Frame.getSize().x + 15, y + 5);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(Frame);
        window.draw(Title);
        window.draw(Val);
    }

    RectangleShape Frame;
    Text Title, Val;
};

// Menu buttons, 2 styles
struct MenuPlayButton : public RectangleShape {
    MenuPlayButton(string title, string img, int num, bool large = false) {
        this->setFillColor(Color(50, 50, 50));

        Tex.loadFromFile(Dir_Assets + img + ".png");
        if (large) {
            Picture.setSize(Vector2f(500, 150));
            this->setSize(Vector2f(700, 150));
        }
        else {
            this->setSize(Vector2f(Vector2f(600, 80)));
            Picture.setSize(Vector2f(400, 80));
        }

        Picture.setFillColor(Color(100, 100, 100));
        Picture.setTexture(&Tex);

        Header.setString(title);
        Header.setFont(BarlowB);
        Header.setCharacterSize(25);

        OutlineText(Header);

        SelectNum = num;
    }

    bool CheckHighlight(int amount) {
        if (amount == SelectNum) {
            this->setOutlineThickness(2);
            this->setFillColor(Color(220, 0, 0));
            Picture.setFillColor(Color::White);
            return true;
        }
        else {
            this->setOutlineThickness(0);
            this->setFillColor(Color(50, 50, 50));
            Picture.setFillColor(Color(100, 100, 100));
        }
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        Picture.setPosition(x + 200, y);
        Header.setPosition(x + 15, y + 10);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(Picture);
        window.draw(Header);
    }

    Text Header;
    RectangleShape Picture;
    Texture Tex;
    int SelectNum;
};

// Ammo counter in bottom right
struct AmmoCount : public RectangleShape {
    AmmoCount() : RectangleShape(Vector2f(134, 52)) {
        this->setSize(Vector2f(134, 52));
        Counter.setFont(BarlowB);
        Counter.setCharacterSize(17);
        Counter.setString("Ammunition Count");
        OutlineText(Counter);

        ReloadPrompt = new KeyPrompt("R", "X", "Reload", 32);
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        ReloadPrompt->Reposition(x - 100, y - 2);
    }

    void Routine(PlayerStats& stats, string inputMethod) {
        ReloadPrompt->CheckInput(inputMethod);

        if (stats.Holding->Name != "") {
            WeaponTex.loadFromFile(Dir_Assets + "Items\\" + stats.Holding->Name + "_White.png");
            this->setTexture(&WeaponTex);

            Holding = true;

            string amount;
            float perc = (float)stats.Holding->Loaded/(float)stats.Holding->Capacity;

            LowAmmo = false;
            if (perc >= 1) amount = "Full";
            if (perc < 1 && perc >= 0.7) amount = "Almost Full";
            if (perc < 0.7 && perc >= 0.3) amount = "Around Half";
            if (perc < 0.3 && perc > 0) {
                amount = "Almost Empty";
                LowAmmo = true;
            }
            if (perc <= 0) {
                amount = "Empty";
                LowAmmo = true;
            }

            if (stats.Holding->Reserve <= 0) LowAmmo = false;

            if (perc >= 0.3) Counter.setFillColor(Color::White);
            else Counter.setFillColor(Color(220, 0, 0));

            Counter.setString(amount);
            Counter.setPosition(this->getPosition().x - Counter.getGlobalBounds().width - 10, this->getPosition().y + 25);
        }
        else {
            WeaponTex.loadFromFile(Dir_Assets + "Items\\Unarmed_White.png");
            Holding = false;
        }

        this->setTexture(&WeaponTex);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        if (Holding) {
            window.draw(Counter);
            if (LowAmmo) ReloadPrompt->Draw(window);
        }
    }

    Texture WeaponTex;
    Text Counter;
    bool Holding, LowAmmo = false;
    KeyPrompt* ReloadPrompt = nullptr;
};

// Literally a sine graph
struct SineGraph : public RectangleShape {
    SineGraph(float x, float y, float a, float s, float r) : RectangleShape(Vector2f(x, y)) {
        Amplitude = a;
        Stretch = s;
        Rate = r;
    }

    void Routine() {
        if (Points.size() < this->getSize().x) {
            for (int i = 0; i != Rate; i++) {
                float y = -Amplitude*sin(Stretch*x*3.14/180);

                Points.push_back(RectangleShape(Vector2f(2, 2)));
                Points.back().setPosition(this->getPosition().x + x, this->getPosition().y + y + this->getSize().y/2);
                Points.back().setFillColor(PointColor);

                x++;
            }
        }
        else {
            for (int i = 0; i != Points.size(); i++) {
                if (Points[i].getFillColor().a > 20) {
                    Points[i].setFillColor(Color(PointColor.r, PointColor.g, PointColor.b, Points[i].getFillColor().a - 20));
                }
                else {
                    Points.clear();
                    x = 0;
                    break;
                }
            }
        }
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        Points.clear();
        x = 0;
    }

    void Draw(RenderWindow& window) {
        for (int i = 0; i != Points.size(); i++) window.draw(Points[i]);
    }

    vector<RectangleShape> Points;
    float Amplitude, Stretch, Rate, x = 0;
    Color PointColor = Color::White;
};

// Progress bar 0 - 100%
struct ProgressBar : public RectangleShape {
    ProgressBar(float x, float y) : RectangleShape(Vector2f(x, y)) {
        Bar.setFillColor(Color(220, 0, 0));
        this->setFillColor(Color(50, 50, 50));
    }

    void SetValue(float perc) {
        Bar.setSize(Vector2f(this->getSize().x * perc, this->getSize().y));
    }

    void Reposition(float x, float y) {
        this->setPosition(x, y);
        Bar.setPosition(x, y);
    }

    void Draw(RenderWindow& window) {
        window.draw(*this);
        window.draw(Bar);
    }

    RectangleShape Bar;
};

#endif // ENGINE_HEADER
