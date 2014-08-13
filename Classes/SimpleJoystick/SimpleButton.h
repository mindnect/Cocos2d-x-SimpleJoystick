#ifndef __SIMPLE_BUTTON_H__
#define __SIMPLE_BUTTON_H__

#include "cocos2d.h"

class SimpleButton : public cocos2d::Node
{
public:
	static SimpleButton* create(const cocos2d::Size&);
	CC_SYNTHESIZE(bool, _isEnabled, Enable);
	CC_SYNTHESIZE(bool, _isHoldable, IsHoldable);
	CC_SYNTHESIZE(bool, _isToggleable, IsToggleable);
	CC_SYNTHESIZE(bool, _isKeyboard, IsKeyboard);
	CC_SYNTHESIZE(cocos2d::EventKeyboard::KeyCode, _keyCode, KeyCode);
	CC_SYNTHESIZE_READONLY(float, _buttonRadius, ButtonRadius);

	CC_SYNTHESIZE_READONLY(bool, _isActive, IsActive);
	CC_SYNTHESIZE_READONLY(bool, _isPressed, IsPressed);

	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, _defaultSprite, DefaultSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, _activatedSprite, ActivatedSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, _pressedSprite, PressedSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, _disabledSprite, DisabledSprite);

	void setButtonRadius(float radius);
	void setDefaultSprite(cocos2d::Sprite *aSprite);
	void setActivatedSprite(cocos2d::Sprite *aSprite);
	void setDisabledSprite(cocos2d::Sprite *aSprite);
	void setPressedSprite(cocos2d::Sprite *aSprite);

private:
	float _rateLimit;
	float _radiusSq;

public:
	bool init(const cocos2d::Size& size);
	void limiter(float delta);

protected:
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();	
	void update(float);
	
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void touchBegin();
	void touchEnd();

};

#endif