// using xbox one controller bindings

#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

bool A() { if (Joystick::isButtonPressed(0, 0)) return true; }
bool B() { if (Joystick::isButtonPressed(0, 1)) return true; }
bool X() { if (Joystick::isButtonPressed(0, 2)) return true; }
bool Y() { if (Joystick::isButtonPressed(0, 3)) return true; }
bool LB() { if (Joystick::isButtonPressed(0, 4)) return true; }
bool RB() { if (Joystick::isButtonPressed(0, 5)) return true; }

Vector2f DPadPos() { return Vector2f(Joystick::getAxisPosition(0, Joystick::PovX), Joystick::getAxisPosition(0, Joystick::PovY)); }
Vector2f LStickPos() { return Vector2f(Joystick::getAxisPosition(0, Joystick::X), Joystick::getAxisPosition(0, Joystick::Y)); }
Vector2f RStickPos() { return Vector2f(Joystick::getAxisPosition(0, Joystick::U), Joystick::getAxisPosition(0, Joystick::V)); }
float LTriggerPos() { return Joystick::getAxisPosition(0, Joystick::Z); }
float RTriggerPos() { return Joystick::getAxisPosition(0, Joystick::R); }

bool LTrigger() { if (LTriggerPos() > 0.5) return true; }
bool RTrigger() { if (RTriggerPos() > 0.5) return true; }

bool LStickUp() { if (LStickPos().y < -50) return true; }
bool LStickDown() { if (LStickPos().y > 50) return true; }
bool LStickLeft() { if (LStickPos().x < -50) return true; }
bool LStickRight() { if (LStickPos().x > 50) return true; }

bool RStickUp() { if (RStickPos().y < -50) return true; }
bool RStickDown() { if (RStickPos().y > 50) return true; }
bool RStickLeft() { if (RStickPos().x < -50) return true; }
bool RStickRight() { if (RStickPos().x > 50) return true; }

bool DPadDown() { if (DPadPos().y < -50) return true; }
bool DPadUp() { if (DPadPos().y > 50) return true; }
bool DPadLeft() { if (DPadPos().x < -50) return true; }
bool DPadRight() { if (DPadPos().x > 50) return true; }

bool Down() { if (DPadDown() || LStickDown()) return true; }
bool Up() { if (DPadUp() || LStickUp()) return true; }
bool Left() { if (DPadLeft() || LStickLeft()) return true; }
bool Right() { if (DPadRight() || LStickRight()) return true; }

bool Back() { if (Joystick::isButtonPressed(0, 6)) return true; }
bool Start() { if (Joystick::isButtonPressed(0, 7)) return true; }

#endif // CONTROLLER_HEADER
