#include "SimpleButton.h"

using namespace cocos2d;

bool SimpleButton::init(const Size& size)
{
	_defaultSprite = nullptr;
	_disabledSprite = nullptr;
	_activatedSprite = nullptr;
	_pressedSprite = nullptr;

	_isEnabled = true;
	_isActive = false;
	_isPressed = false;
	_isHoldable = false;
	_isToggleable = false;
	
	_rateLimit = 1.0f / 120.0f;

	_isKeyboard = false;
	_keyCode = EventKeyboard::KeyCode::KEY_NONE;

	this->setButtonRadius(size.width / 2);
	this->setContentSize(size);
	return true;
}

void SimpleButton::onEnterTransitionDidFinish()
{
	Node::onEnterTransitionDidFinish();
	if (_isKeyboard){
		auto keyListener = EventListenerKeyboard::create();
		keyListener->onKeyPressed = CC_CALLBACK_2(SimpleButton::onKeyPressed, this);
		keyListener->onKeyReleased = CC_CALLBACK_2(SimpleButton::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	}
	else{
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);

		touchListener->onTouchBegan = CC_CALLBACK_2(SimpleButton::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(SimpleButton::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(SimpleButton::onTouchEnded, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(SimpleButton::onTouchCancelled, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}

	this->scheduleUpdate();
	if (!this->_isEnabled) this->_defaultSprite->setVisible(false);
}

void SimpleButton::onExit()
{
	Node::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}


void SimpleButton::limiter(float delta)
{
	_isActive = false;
	_isPressed = false;
	
	this->unschedule(schedule_selector(SimpleButton::limiter));
}

bool SimpleButton::onTouchBegan(Touch *touch, Event *event)
{
	if (_isEnabled){
		if (!_isActive){
			auto loc = this->convertTouchToNodeSpace(touch);
			if (loc.x > -_buttonRadius && loc.x < _buttonRadius && loc.y > -_buttonRadius && loc.y < _buttonRadius) {
				float dSq = loc.getLengthSq();
				if (_radiusSq > dSq){
					this->touchBegin();
					return true;
				}
			}
		}
	}

	return false;
}

void SimpleButton::touchBegin()
{
	if (_isActive) return;
	
	_isActive = true;
	if (!_isHoldable && !_isToggleable){
		_isPressed = true;
		this->schedule(schedule_selector(SimpleButton::limiter), _rateLimit);
	}

	if (_isHoldable) _isPressed = true;
	if (_isToggleable) _isPressed = !_isPressed;
}

void SimpleButton::onTouchMoved(Touch *touch, Event *event)
{
	if (_isActive){
		auto loc = this->convertTouchToNodeSpace(touch);
		if (loc.x > -_buttonRadius && loc.x < _buttonRadius && loc.y > -_buttonRadius && loc.y < _buttonRadius) {
			float dSq = loc.getLengthSq();
			
			// inside
			if (_radiusSq > dSq){
				if (_isHoldable) _isPressed = true;
			}
			// out of range
			else{
				if (_isHoldable) _isPressed = false;
				_isActive = false;
			}
		}
	}
}

void SimpleButton::onTouchEnded(Touch *touch, Event *event)
{
	this->touchEnd();
}

void SimpleButton::onTouchCancelled(Touch *touch, Event *event)
{
	this->onTouchEnded(touch, event);
}

void SimpleButton::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == _keyCode){
		this->touchBegin();
	}
}

void SimpleButton::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == _keyCode){
		this->touchEnd();
	}
}

void SimpleButton::touchEnd()
{
	if (_isActive){
		if (_isHoldable){
			_isPressed = false;
			_isActive = false;
		}
		if (_isToggleable){
			_isActive = false;
		}
		
	}
}

void SimpleButton::update(float)
{
	if (_isEnabled) {
		if (_disabledSprite) _disabledSprite->setVisible(false);

		// button is active
		if (_isActive) {
			_defaultSprite->setVisible(false);
			if (_pressedSprite) {
				_pressedSprite->setVisible(true);
			}
		}
		// 작동 중 아님
		else {
			_pressedSprite->setVisible(false);
			// 눌려져 있으면
			if (_isPressed) {
				_activatedSprite->setVisible(true);
			}
			// 눌려져 있지 않으면
			else {
				_activatedSprite->setVisible(false);
				// 기본 스프라이트 보여줌
				if (_defaultSprite) {
					_defaultSprite->setVisible(true);
				}
			}
		}
	}
	// 작동중 아님
	else {
		_defaultSprite->setVisible(false);
		if (_disabledSprite) {
			_disabledSprite->setVisible(true);
		}
	}
}

void SimpleButton::setDefaultSprite(cocos2d::Sprite *aSprite)
{
	if (_defaultSprite) {
		_defaultSprite->removeFromParent();
	}

	_defaultSprite = aSprite;
	this->addChild(aSprite, 0);
}

void SimpleButton::setActivatedSprite(cocos2d::Sprite *aSprite)
{
	if (_activatedSprite) {
		_activatedSprite->removeFromParent();
	}

	this->_activatedSprite = aSprite;
	this->_activatedSprite->setVisible(false);
	this->addChild(aSprite, 1);
}

void SimpleButton::setPressedSprite(cocos2d::Sprite *aSprite)
{
	if (_pressedSprite) {
		_pressedSprite->removeFromParent();
	}

	this->_pressedSprite = aSprite;
	this->_pressedSprite->setVisible(false);
	this->addChild(aSprite, 2);
}

void SimpleButton::setDisabledSprite(cocos2d::Sprite *aSprite)
{
	if (_disabledSprite) {
		_disabledSprite->removeFromParent();
	}
	this->_disabledSprite = aSprite;
	this->_disabledSprite->setVisible(false);
	this->addChild(aSprite, 3);
}

SimpleButton* SimpleButton::create(const Size& size)
{
	auto ret = new SimpleButton();
	ret->init(size);
	ret->autorelease();
	return ret;
}

void SimpleButton::setButtonRadius(float radius)
{
	_buttonRadius = radius;
	_radiusSq = radius*radius;
}



