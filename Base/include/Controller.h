#pragma once

#include "SFML/Graphics.hpp"  
#include "SFML/OpenGL.hpp" 

class Controller
{
public:
	Controller();
	~Controller();

	const int connectedController = 0;

	void update(sf::Event Event);

	void getDPadValue();
	void getButtonValue();
	void getLStickValue();
	void getRStickValue();
	void getLTriggerValue();
	void getRTriggerValue();

	sf::Vector2f normalise(sf::Vector2f); 
	float getRightStickByAngle();
	sf::Vector2f getRightStickByVector();

	sf::Vector2f getLeftStickVelocity();

	enum Buttons{
		A = 0, B = 1, X = 2, Y = 3,
		LB = 4, RB = 5,
		BACK = 6, START = 7,
		L3 = 8, R3 = 9
	};

	enum DPadState { DLEFT, DRIGHT, DUP, DDOWN, DCENTER };
	enum LStickState { LLEFT, LRIGHT, LUP, LDOWN, LCENTER };
	enum RStickState { RLEFT, RRIGHT, RUP, RDOWN, RCENTER };

	enum LTriggerState { LTSOFT, LTHARD, LTUP, };
	enum RTriggerState { RTSOFT, RTHARD, RTUP };

	DPadState dpadState, lastDpadState;
	LStickState lStickState, lastLStickState;
	RStickState rStickState, lastRStickState;
	LTriggerState lTriggerState, lastLTriggerState;
	RTriggerState rTriggerState, lastRTriggerState;

	sf::Vector2f LeftThumbStickDirection, RightThumbStickDirection, DPadDirection, TriggerPosition;

private:

	sf::Vector2f rightStickAngleVector;
	float rightStickAngle;
	float previosRightStickAngle;
	sf::Vector2f rightStickVector;
	sf::Vector2f previosRightStickVector;

	sf::Vector2f leftStickVector;
	sf::Vector2f previosLeftStickVector;

	bool aHold;
	bool bHold;
	bool xHold;
	bool yHold;
	bool lbHold;
	bool rbHold;
	bool backHold;
	bool startHold;
	bool l3Hold;
	bool r3Hold;
};

