// Box2D Game Assets (testing)

#ifndef ASSETS_HEADER
#define ASSETS_HEADER

#include <SFML/Graphics.hpp>
#include "physics.h"

using namespace sf;

// old box2d testing stuff
/*
struct Base : public RectangleShape {
    Base(b2World &world, float x, float y, float width, float height, b2BodyType type = b2_dynamicBody) : RectangleShape(Vector2f(width, height)) {
        b2BodyDef bodyDef;
        bodyDef.position.Set((x + width/2.0)/pixels_per_meter, (y + height/2.0)/pixels_per_meter);
        bodyDef.type = type;
        bodyDef.linearDamping = 0.1;
        b2PolygonShape b2shape;
        b2shape.SetAsBox(width/pixels_per_meter/2.0, height/pixels_per_meter/2.0);
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 0;
        fixtureDef.shape = &b2shape;

        body = world.CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);

        this->setPosition(x, y);
        this->setOrigin(width/2, height/2);
    }

    void UpdatePos() {
        this->setPosition(body->GetPosition().x*32, body->GetPosition().y*32);
        this->setRotation(body->GetAngle()*180/3.14);
    }

    b2Body* body;
};

struct Weld {
    Weld(b2World& world, Base* a, Base* b) {
        Def.bodyA = a->body;
        Def.bodyB = b->body;
        Def.collideConnected = false;

        Def.Initialize(a->body, b->body, b2Vec2(0, 0));
        Joint = (b2WeldJoint*)world.CreateJoint(&Def);
    }

    b2WeldJoint* Joint;
    b2WeldJointDef Def;
};

struct Revolute {
    Revolute(b2World& world, Base* a, Base* b) {
        Def.bodyA = a->body;
        Def.bodyB = b->body;
        Def.collideConnected = false;

        Def.Initialize(a->body, b->body, a->body->GetWorldCenter());

        Def.enableMotor = true;
        //Def.motorSpeed = -50;
        Def.maxMotorTorque = 40;

        Def.lowerAngle = -45*3.14/180;
        Def.upperAngle = 45*3.14/180;
        Joint = (b2RevoluteJoint*)world.CreateJoint(&Def);
    }

    b2RevoluteJoint* Joint;
    b2RevoluteJointDef Def;
};

struct Motor {
    Motor(b2World& world, Base* a, Base* b) {
        Def.bodyA = a->body;
        Def.bodyB = b->body;
        Def.collideConnected = false;

        Def.Initialize(a->body, b->body);
        Def.maxTorque = 1;
        Def.angularOffset = 1;
        Joint = (b2MotorJoint*)world.CreateJoint(&Def);
    }

    b2MotorJoint* Joint;
    b2MotorJointDef Def;
};

struct Rope {
    Rope(b2World& world, Base* a, Base* b) {
        Def.bodyA = a->body;
        Def.bodyB = b->body;
        Def.collideConnected = false;

        Def.maxLength = 2;
        Def.localAnchorA = b2Vec2(5/32, 0);
        //Def.localAnchorB = b2Vec2(1, 0);
        //Def.localAnchorB = b2Vec2(0, 2);
        Joint = (b2RopeJoint*)world.CreateJoint(&Def);
    }

    b2RopeJoint* Joint;
    b2RopeJointDef Def;
};

struct Distance {
    Distance(b2World& world, Base* a, Base* b) {
        Def.bodyA = a->body;
        Def.bodyB = b->body;
        Def.collideConnected = false;
        Def.length = 4;
        a->body->ApplyAngularImpulse(5, true);
        Def.Initialize(a->body, b->body, a->body->GetPosition(), b->body->GetPosition());

        //Def.localAnchorA = b2Vec2(5/32, 0);
        //Def.localAnchorB = b2Vec2(0, 2);
        Joint = (b2DistanceJoint*)world.CreateJoint(&Def);
    }

    b2DistanceJoint* Joint;
    b2DistanceJointDef Def;
};

struct Lightbulb : public Base {
    Lightbulb(b2World& world, float x = 0, float y = 0) : Base(world, x, y, 40, 40) {
        Bulb = new Base(world, x - 30, y + 45, 100, 100);
        Bulb->setFillColor(Color::Yellow);
        Bulb->setOutlineThickness(5);
        Bulb->setOutlineColor(Color::Black);
        Bulb->body->GetFixtureList()->SetDensity(0);
        Bulb->body->ResetMassData();
        this->setFillColor(Color(50, 50, 50));
        this->setOutlineThickness(5);
        this->setOutlineColor(Color::Black);

        physics::setCollisionID(Bulb->body, -1);
        physics::setCollisionID(body, -1);

        Anchor = new Base(world, x + 15, y - 60, 10, 10, b2_staticBody);
        Anchor->body->SetTransform(Anchor->body->GetPosition(), 45*3.14/180);

        BulbWeld = new Weld(world, this, Bulb);
        AnchorRope = new Rope(world, Anchor, this);
    }

    void Update() {
        Bulb->UpdatePos();
        this->UpdatePos();
        Anchor->UpdatePos();
    }

    void Draw(RenderWindow& window) {
        window.draw(*Bulb);
        window.draw(*Anchor);
        window.draw(*this);
    }

    Base* Anchor;
    Base* Bulb;
    Weld* BulbWeld;
    Rope* AnchorRope;
};

*/

// Map objects base with collision and scaling
struct ObjectBase : public Sprite {
    ObjectBase(string obj, bool collide = true, bool scaled = true) {
        Tex.loadFromFile("Engine\\Assets\\" + obj);
        ID = obj;
        this->setTexture(Tex);

        if (scaled) this->scale(2, 2);
    }

    Texture Tex;
    bool Collision;
    string ID;
};

// Extract points used in game
struct ExtractFlare : public RectangleShape {
    ExtractFlare(float x, float y, float w, float h, string name) : RectangleShape(Vector2f(w, h)) {
        this->setFillColor(Color::Green);
        this->setPosition(x, y);

        Name = name;
    }

    string Name;
};

#endif // ASSETS_HEADER
