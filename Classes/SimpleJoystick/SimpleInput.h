#ifndef __SIMPLE_INPUT_H__ 
#define __SIMPLE_INPUT_H__

#include "cocos2d.h"
#include "Library/Library.h"

#include "SimpleButton.h"
#include "SimpleJoystick.h"

using namespace cocos2d;

/*
	auto buttonAttk = SimpleImput::createAttkButton();
	this->addChild(buttonAttk,1);

	auto buttonDefense = SimpleImput::createJumpButton();
	this->addChild(buttonDefense, 1);

	auto joystick = SimpleImput::createJoystick();
	this->addChild(joystick, 1);
*/
	
namespace SimpleImput{
	static SimpleJoystick* createJoystick()
{
		int xOffset = 12;
		int yOffset = 12;

		auto size = Size(82.0f, 82.0f);
		auto pos= Vec2(
			VisibleRect::getOrigin().x + (size.width / 2) + xOffset,
			VisibleRect::getOrigin().y + (size.height / 2) + yOffset);

		// 조이스틱 객체
		SimpleJoystick *joystick = SimpleJoystick::create(size);
		joystick->setName("Joystick");
		
		joystick->setNumberOfDirections(8);
		joystick->setDeadRadius(8.0);
		joystick->setThumbRadius(41 - 10);
		//joystick->setIsDPad(true);
		joystick->setIsKeyboard(true);

		joystick->setBackgroundSprite(CCSprite::create("Joystick/joystick_background.png"));
		joystick->setThumbSprite(CCSprite::create("Joystick/joystick_thumb.png"));
		joystick->setPosition(pos);

		return joystick;
	}

	static SimpleButton* createAttkButton()
	{
		int xOffset = 64;
		int yOffset = 4;

		Size size = Size(48.0f, 48.0f);
		Point pos = Point(
			VisibleRect::rightBottom().x - (size.width / 2) - xOffset,
			VisibleRect::rightBottom().y + (size.height / 2) + yOffset);

		// 버튼 객체
		auto button = SimpleButton::create(size);
		button->setName("AttackButton");

		button->setPosition(pos);
		button->setIsHoldable(true);
		button->setIsKeyboard(true);
		button->setKeyCode(EventKeyboard::KeyCode::KEY_Z);

		button->setDefaultSprite(CCSprite::create("Joystick/button1_Default.png"));
		button->setActivatedSprite(CCSprite::create("Joystick/button1_Pressed.png"));
		button->setPressedSprite(CCSprite::create("Joystick/button1_Pressed.png"));
		button->setDisabledSprite(CCSprite::create("Joystick/button1_Disabled.png"));

		return button;
	}

	static SimpleButton* createJumpButton()
	{
		int xOffset = 4;
		int yOffset = 4;

		Size size = Size(48.0f, 48.0f);
		Point pos = Point(
			VisibleRect::rightBottom().x - (size.width / 2) - xOffset,
			VisibleRect::rightBottom().y + (size.height / 2) + yOffset);

		// 버튼 객체
		auto button = SimpleButton::create(size);
		button->setName("DefenseButton");

		button->setPosition(pos);
		button->setIsHoldable(true);
		button->setIsKeyboard(true);
		button->setKeyCode(EventKeyboard::KeyCode::KEY_X);

		// 버튼 스킨
		button->setDefaultSprite(CCSprite::create("Joystick/button2_Default.png"));
		button->setActivatedSprite(CCSprite::create("Joystick/button2_Pressed.png"));
		button->setPressedSprite(CCSprite::create("Joystick/button2_Pressed.png"));
		button->setDisabledSprite(CCSprite::create("Joystick/button2_Disabled.png"));

		return button;
	}
}

#endif
