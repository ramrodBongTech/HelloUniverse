#include "stdafx.h"
#include "Controller.h"

#include <iostream> 
#include <string>
#define M_PI 3.141592654
using namespace std;

Controller::Controller()
{
	lastDpadState = DPadState::DCENTER;

	lastLStickState = LStickState::LCENTER;
	lastRStickState = RStickState::RCENTER;

	lastLTriggerState = LTriggerState::LTUP;
	lastRTriggerState = RTriggerState::RTUP;

	rightStickAngleVector = sf::Vector2f(0, 0);
	rightStickAngle = 0;
	previosRightStickAngle = 0;
	previosRightStickVector = sf::Vector2f(0, 0);

	leftStickVector = sf::Vector2f(0, 0);
	previosLeftStickVector = sf::Vector2f(0, 0);

	aHold = false;
	bHold = false;
	xHold = false;
	yHold = false;
	lbHold = false;
	rbHold = false;
	backHold = false;
	startHold = false;
	l3Hold = false;
	r3Hold = false;
}


Controller::~Controller()
{
}

void Controller::update(sf::Event Event)
{
	switch (Event.type)
	{
	case sf::Event::JoystickConnected:
		std::cout << "Joystick connected!" << std::endl;
		break;

	case sf::Event::JoystickDisconnected:
		std::cout << "Joystick disconnected!" << std::endl;
		break;

	//case sf::Event::JoystickButtonPressed:
	//	std::cout << "Button pressed!" << std::endl;
	//	break;

	//case sf::Event::JoystickButtonReleased:
	//	std::cout << "Button released!" << std::endl;
	//	break;

	//case sf::Event::JoystickMoved:
	//	std::cout << Event.joystickMove.position << std::endl;
	//	break;
	}

	if (sf::Joystick::isConnected(0))
	{
		std::cout << sf::Joystick::ButtonCount << std::endl;
	}

	LeftThumbStickDirection.x = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::X);
	LeftThumbStickDirection.y = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::Y);

	RightThumbStickDirection.x = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::U);
	RightThumbStickDirection.y = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::R);

	DPadDirection.x = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::PovX);
	DPadDirection.y = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::PovY);

	TriggerPosition.x = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::Z); // Left +
	TriggerPosition.y = sf::Joystick::getAxisPosition(connectedController, sf::Joystick::Z); // Right -


	//cout << "RS ANGLE \t = \t " << getRightThumbByAngle() << endl;
	//std::cout << "X  \t = \t " << getLeftStickVelocity().x << "\t = \t" << "Y  \t = \t " << getLeftStickVelocity().y << endl;

	getButtonValue();

	getDPadValue();
	//controller.getLStickValue();
	//controller.getRStickValue();
	getLeftStickVelocity();
	getRightStickByAngle();

	getLTriggerValue();
	getRTriggerValue();
}

void Controller::getButtonValue()
{
	// A
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::A) && aHold == false)
	{ 
		std::cout << "A \t = \t " << Buttons::A << std::endl;
		aHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::A)) { aHold = false; }

	// B
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::B) && bHold == false)
	{ 
		std::cout << "B \t = \t " << Buttons::B << std::endl;
		bHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::B)) { bHold = false; }

	// X
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::X) && xHold == false)
	{ 
		std::cout << "X \t = \t " << Buttons::X << std::endl; 
		xHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::X)) { xHold = false; }

	// Y
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::Y) && yHold == false)
	{ 
		std::cout << "Y \t = \t " << Buttons::Y << std::endl; 
		yHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::Y)) { yHold = false; }

	// LB
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::LB) && lbHold == false)
	{ 
		std::cout << "LB \t = \t " << Buttons::LB << std::endl; 
		lbHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::LB)) { lbHold = false; }

	// RB
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::RB) && rbHold == false)
	{ 
		std::cout << "RB \t = \t " << Buttons::RB << std::endl; 
		rbHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::RB)) { rbHold = false; }

	// BACK
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::BACK) && backHold == false)
	{ 
		std::cout << "BACK \t = \t " << Buttons::BACK << std::endl;
		backHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::BACK)) { backHold = false; }

	// START
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::START) && startHold == false)
	{ 
		std::cout << "START \t = \t " << Buttons::START << std::endl;
		startHold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::START)) { startHold = false; }

	// L3
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::L3) && l3Hold == false)
	{ 
		std::cout << "L3 \t = \t " << Buttons::L3 << std::endl; 
		l3Hold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::L3)) { l3Hold = false; }

	// R3
	if (sf::Joystick::isButtonPressed(connectedController, Buttons::R3) && r3Hold == false)
	{ 
		std::cout << "R3 \t = \t " << Buttons::R3 << std::endl; 
		r3Hold = true;
	}
	else if (!sf::Joystick::isButtonPressed(connectedController, Buttons::R3)) { r3Hold = false; }
}

void Controller::getDPadValue()
{
	if (DPadDirection.x == 100 && lastDpadState != DPadState::DRIGHT)
	{
		std::cout << "DPad \t = \t RIGHT" << std::endl;

		dpadState = DPadState::DRIGHT;
		lastDpadState = DPadState::DRIGHT;
	}
	else if (DPadDirection.x == -100 && lastDpadState != DPadState::DLEFT)
	{
		std::cout << "DPad \t = \t LEFT" << std::endl;

		dpadState = DPadState::DLEFT;
		lastDpadState = DPadState::DLEFT;
	}
	else if (DPadDirection.y == 100 && lastDpadState != DPadState::DUP)
	{
		std::cout << "DPad \t = \t UP" << std::endl;

		dpadState = DPadState::DUP;
		lastDpadState = DPadState::DUP;
	}
	else if (DPadDirection.y == -100 && lastDpadState != DPadState::DDOWN)
	{
		std::cout << "DPad \t = \t DOWN" << std::endl;

		dpadState = DPadState::DDOWN;
		lastDpadState = DPadState::DDOWN;
	}
	else if (DPadDirection.x == 0 && DPadDirection.y == 0 && lastDpadState != DPadState::DCENTER)
	{
		//std::cout << "DPad \t = \t CENTRE" << std::endl;

		dpadState = DPadState::DCENTER;
		lastDpadState = DPadState::DCENTER;
	}
}

void Controller::getLStickValue()
{
	if ((LeftThumbStickDirection.x >= 25 ) && lastLStickState != LStickState::LRIGHT)
	{
		std::cout << "L Stick \t = \t RIGHT" << std::endl;

		lStickState = LStickState::LRIGHT;
		lastLStickState = LStickState::LRIGHT;
	}
	else if (LeftThumbStickDirection.x <= -25 && lastLStickState != LStickState::LLEFT)
	{
		std::cout << "L Stick \t = \t LEFT" << std::endl;

		lStickState = LStickState::LLEFT;
		lastLStickState = LStickState::LLEFT;
	}
	else if (LeftThumbStickDirection.y <= -25 && lastLStickState != LStickState::LUP)
	{
		std::cout << "L Stick \t = \t UP" << std::endl;

		lStickState = LStickState::LUP;
		lastLStickState = LStickState::LUP;
	}
	else if (LeftThumbStickDirection.y >= 25 && lastLStickState != LStickState::LDOWN)
	{
		std::cout << "L Stick \t = \t DOWN" << std::endl;

		lStickState = LStickState::LCENTER;
		lastLStickState = LStickState::LDOWN;
	}
	else if (LeftThumbStickDirection.x <= 25 && LeftThumbStickDirection.y <= 25 &&
		LeftThumbStickDirection.x >= -25 && LeftThumbStickDirection.y >= -25 &&
		lastLStickState != LStickState::LCENTER)
	{
		//std::cout << "L Stick \t = \t CENTRE" << std::endl;

		lStickState = LStickState::LCENTER;
		lastLStickState = LStickState::LCENTER;
	}
}

void Controller::getRStickValue()
{
	if ((RightThumbStickDirection.x >= 25) && lastRStickState != RStickState::RRIGHT)
	{
		std::cout << "R Stick \t = \t RIGHT" << std::endl;

		rStickState = RStickState::RRIGHT;
		lastRStickState = RStickState::RRIGHT;
	}
	else if (RightThumbStickDirection.x <= -25 && lastRStickState != RStickState::RLEFT)
	{
		std::cout << "R Stick \t = \t LEFT" << std::endl;

		rStickState = RStickState::RLEFT;
		lastRStickState = RStickState::RLEFT;
	}
	else if (RightThumbStickDirection.y <= -25 && lastRStickState != RStickState::RUP)
	{
		std::cout << "R Stick \t = \t UP" << std::endl;

		rStickState = RStickState::RUP;
		lastRStickState = RStickState::RUP;
	}
	else if (RightThumbStickDirection.y >= 25 && lastRStickState != RStickState::RDOWN)
	{
		std::cout << "R Stick \t = \t DOWN" << std::endl;

		rStickState = RStickState::RDOWN;
		lastRStickState = RStickState::RDOWN;
	}
	else if (RightThumbStickDirection.x <= 25 && RightThumbStickDirection.y <= 25 &&
		RightThumbStickDirection.x >= -25 && RightThumbStickDirection.y >= -25 &&
		lastRStickState != RStickState::RCENTER)
	{
		//std::cout << "R Stick \t = \t CENTRE" << std::endl;

		rStickState = RStickState::RCENTER;
		lastRStickState = RStickState::RCENTER;
	}
}

void Controller::getLTriggerValue()
{
	if (TriggerPosition.x >= 10 && TriggerPosition.x <= 70 && lastLTriggerState != LTriggerState::LTSOFT)
	{
		std::cout << "LT  \t = \t SOFT" << std::endl;

		lTriggerState = LTriggerState::LTSOFT;
		lastLTriggerState = LTriggerState::LTSOFT;
	}

	else if (TriggerPosition.x >= 70 && lastLTriggerState != LTriggerState::LTHARD)
	{
		std::cout << "LT  \t = \t HARD" << std::endl;

		lTriggerState = LTriggerState::LTHARD;
		lastLTriggerState = LTriggerState::LTHARD;
	}

	else if (TriggerPosition.x <= 10 && lastLTriggerState != LTriggerState::LTUP)
	{
		//std::cout << "LT  \t = \t  UP" << std::endl;

		lTriggerState = LTriggerState::LTUP;
		lastLTriggerState = LTriggerState::LTUP;
	}
}

void Controller::getRTriggerValue()
{
	if (TriggerPosition.y <= -10 && TriggerPosition.x >= -70 && lastRTriggerState != RTriggerState::RTSOFT)
	{
		std::cout << "RT  \t = \t SOFT" << std::endl;

		rTriggerState = RTriggerState::RTSOFT;
		lastRTriggerState = RTriggerState::RTSOFT;
	}

	else if (TriggerPosition.y <= -70 && lastRTriggerState != RTriggerState::RTHARD)
	{
		std::cout << "RT  \t = \t HARD" << std::endl;

		rTriggerState = RTriggerState::RTHARD;
		lastRTriggerState = RTriggerState::RTHARD;
	}

	else if (TriggerPosition.y >= -10 && lastRTriggerState != RTriggerState::RTUP)
	{
		//std::cout << "RT \t = \t UP" << std::endl;

		rTriggerState = RTriggerState::RTUP;
		lastRTriggerState = RTriggerState::RTUP;
	}
}

sf::Vector2f Controller::normalise(sf::Vector2f vector)
{
	auto magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);

	if (magnitude != 0)
	{
		return vector /= magnitude;
	}

	else
	{
		vector = sf::Vector2f(0, 0);
		return vector;
	}
}

float Controller::getRightStickByAngle()
{
	rightStickAngleVector = normalise(RightThumbStickDirection);

	rightStickAngle = 180 -180 * (atan2(rightStickAngleVector.x, rightStickAngleVector.y)) / M_PI;
	if (rightStickAngle < 0)
	{
		rightStickAngle += 360;
	}

	// Detect when stick has moved off centre
	if (RightThumbStickDirection.x >= 50 || RightThumbStickDirection.x <= -50
		|| RightThumbStickDirection.y >= 50 || RightThumbStickDirection.y <= -50)
	{
		if (rightStickAngle <= previosRightStickAngle - 2 || rightStickAngle >= previosRightStickAngle + 2)
		{
			cout << "RS ANGLE \t = \t " << rightStickAngle << endl;
			previosRightStickAngle = rightStickAngle;
			return previosRightStickAngle;
		}

		else return previosRightStickAngle;
	}

	else return previosRightStickAngle;
}

sf::Vector2f Controller::getRightStickByVector()
{
	rightStickVector = normalise(RightThumbStickDirection);


	if (rightStickVector.x <= previosRightStickVector.x - 0.1f || rightStickVector.x >= previosRightStickVector.x + 0.1f
		|| rightStickVector.y <= previosRightStickVector.y - 0.1f || rightStickVector.y >= previosRightStickVector.y + 0.1f)
	{
		std::cout << "X  \t = \t " << rightStickVector.x << "\t = \t" << "Y  \t = \t " << rightStickVector.y << endl;
		previosRightStickVector = rightStickVector;
	}

	return rightStickVector;
}

sf::Vector2f Controller::getLeftStickVelocity()
{
	float speed = 10.0f;

	leftStickVector = normalise(LeftThumbStickDirection);

	int x = ceil(leftStickVector.x * 100);
	int y = ceil(leftStickVector.y * 100);

	leftStickVector = sf::Vector2f((x / 100.0f) * speed, (y / 100.0f) * speed);

	if (LeftThumbStickDirection.x >= 25 || LeftThumbStickDirection.x <= -25
		|| LeftThumbStickDirection.y >= 25 || LeftThumbStickDirection.y <= -25)
	{
		if (leftStickVector.x <= previosLeftStickVector.x - 0.1f || leftStickVector.x >= previosLeftStickVector.x + 0.1f
			|| leftStickVector.y <= previosLeftStickVector.y - 0.1f || leftStickVector.y >= previosLeftStickVector.y + 0.1f)
		{
			std::cout << "X  \t = \t " << leftStickVector.x << "\t = \t" << "Y  \t = \t " << leftStickVector.y << endl;
			previosLeftStickVector = leftStickVector;
		}

		return leftStickVector;
	}

	else
	{
		return sf::Vector2f(0, 0);
	}
}