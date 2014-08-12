#ifndef __SIMPLE_JOYSTICK__
#define __SIMPLE_JOYSTICK__

#include "cocos2d.h"

class SimpleJoystick : public cocos2d::Node
{
public:
	static SimpleJoystick* create(const cocos2d::Size&);

	CC_SYNTHESIZE(bool, _isEnable,		IsEnable);
	CC_SYNTHESIZE(bool, _isAutoCenter,	IsAutoCenter);
	CC_SYNTHESIZE(int,	_numberOfDirections, NumberOfDirections);
	CC_SYNTHESIZE(bool, _isActive,		IsActive);
	CC_SYNTHESIZE(bool, _isKeyboard,	IsKeyboard);

	CC_SYNTHESIZE(bool, _isDPad, IsDPad);

	void setJoystickRadius(float r);
	void setThumbRadius(float r);
	void setDeadRadius(float r);

	void setBackgroundSprite(cocos2d::Sprite *aSprite);
	void setThumbSprite(cocos2d::Sprite *aSprite);

	CC_SYNTHESIZE_READONLY(cocos2d::Point, _stickPosition, StickPosition);
	CC_SYNTHESIZE_READONLY(cocos2d::Point, _velocity, Velocity);
	CC_SYNTHESIZE_READONLY(float, _degree, Degree);

	CC_SYNTHESIZE_READONLY(float, _JoystickRadius, JoystickRadius);
	CC_SYNTHESIZE_READONLY(float, _deadRadius, DeadRadius);
	CC_SYNTHESIZE_READONLY(float, _thumbRadius, ThumbRadius);

	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, _backgroundSprite, BackgroundSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, _thumbSprite, ThumbSprite);

protected:
	float _joystickRadiusSq;
	float _deadRadiusSq;
	float _thumbRadiusSq;
	bool _keyAr[255];

public:
	bool init(const cocos2d::Size&);
	void updateVelocity(cocos2d::Point point);

protected:
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	void update(float delta);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};
#endif
