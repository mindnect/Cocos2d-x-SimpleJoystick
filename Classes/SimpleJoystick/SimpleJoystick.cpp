#include "SimpleJoystick.h"

using namespace cocos2d;

// enum class to number
template <typename E>
typename std::underlying_type<E>::type to_underlying(E e) {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

SimpleJoystick* SimpleJoystick::create(const cocos2d::Size& size)
{
	auto ret = new SimpleJoystick();
	ret->init(size);
	ret->autorelease();
	return ret;
}

bool SimpleJoystick::init(const Size& size)
{
	_stickPosition = Point::ZERO;
	_degree = 0.0f;
	_velocity = Point::ZERO;
	_isAutoCenter = true;
	_isDPad = false;
	_isActive = false;
	_numberOfDirections = 4;
	_deadRadius = 0.0f;
	_isKeyboard = false;

	_backgroundSprite = nullptr;
	_thumbSprite = nullptr;

	std::fill(_keyAr, _keyAr+255, false);		// init key array with false

	this->setContentSize(size);
	this->setJoystickRadius(size.width/2);
	this->setThumbRadius(size.width/2);

	return true;
}

void SimpleJoystick::onEnterTransitionDidFinish()
{
	Node::onEnterTransitionDidFinish();

	if (_isKeyboard){
		auto keyListener = EventListenerKeyboard::create();
		keyListener->onKeyPressed = CC_CALLBACK_2(SimpleJoystick::onKeyPressed, this);
		keyListener->onKeyReleased = CC_CALLBACK_2(SimpleJoystick::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	}
	else{
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);

		touchListener->onTouchBegan = CC_CALLBACK_2(SimpleJoystick::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(SimpleJoystick::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(SimpleJoystick::onTouchEnded, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(SimpleJoystick::onTouchCancelled, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}
	this->scheduleUpdate();
}

void SimpleJoystick::onExit()
{
	Node::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

void SimpleJoystick::update(float delta)
{
	if (_isKeyboard){
		Vec2 k = Vec2::ZERO;
		bool check = false;
		if (_keyAr[to_underlying(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)]){
			k.x += _JoystickRadius; check = true;
		}
		if (_keyAr[to_underlying(EventKeyboard::KeyCode::KEY_LEFT_ARROW)]){
			k.x -= _JoystickRadius; check = true;
		}
		if (_keyAr[to_underlying(EventKeyboard::KeyCode::KEY_UP_ARROW)]){
			k.y += _JoystickRadius; check = true;
		}
		if (_keyAr[to_underlying(EventKeyboard::KeyCode::KEY_DOWN_ARROW)]){
			k.y -= _JoystickRadius; check = true;
		}
		
		if (check){
			this->updateVelocity(k + _stickPosition);
		}
		else{
			this->updateVelocity(k);
		}

	}

	if (_thumbSprite) {
		_thumbSprite->setPosition(_stickPosition);
	}
}

void SimpleJoystick::updateVelocity(Point point)
{
	// Calculate distance and angle from the center.
	float dx = point.x;
	float dy = point.y;
	float dSq = point.getLengthSq();
	
	if(dSq < _deadRadiusSq && _isDPad){
		_velocity = Point::ZERO;
		_degree = 0.0f;
		_stickPosition = point;
		_isActive = false;
		return;
	}
	else{
		_isActive = true;
	}
	
	float angle = atan2f(dy, dx); // in radians

	//if(angle < 0){
	//	angle += SJ_PI_X_2;
	//}
	
	if(_isDPad){
		float anglePerSector = CC_DEGREES_TO_RADIANS(360.0f / _numberOfDirections);
		angle = roundf(angle/anglePerSector) * anglePerSector;
	}

	float cosAngle;
	float sinAngle;
	

	// NOTE: Velocity goes from -1.0 to 1.0.
	if (dSq > _thumbRadiusSq || _isDPad) {		
		// out of radius
		cosAngle = cosf(angle);
		sinAngle = sinf(angle);
		dx = cosAngle * _thumbRadius;
		dy = sinAngle * _thumbRadius;
	}
		
	_velocity = Vec2(dx, dy) / _thumbRadius;
	

	_degree = CC_DEGREES_TO_RADIANS(angle);
	_stickPosition = Vec2(dx, dy);// Update the thumb's position

	// fixing floating value non zero problem
	if (fabs(_velocity.x) < FLT_EPSILON) _velocity.x = 0;
	if (fabs(_velocity.y) < FLT_EPSILON) _velocity.y = 0;
	
	//if (_degree < 0) _degree += 360.0;
}

void SimpleJoystick::setJoystickRadius(float r)
{
	_JoystickRadius = r;
	_joystickRadiusSq = r*r;
}

void SimpleJoystick::setDeadRadius(float r)
{
	_deadRadius = r;
	_deadRadiusSq = r*r;
}

bool SimpleJoystick::onTouchBegan(Touch *touch, Event *event)
{
	auto loc = this->convertTouchToNodeSpace(touch);

	//Do a fast rect check before doing a circle hit check:
	if (loc.x > -_JoystickRadius || loc.x < _JoystickRadius || loc.y > -_JoystickRadius || loc.y < _JoystickRadius){
		float dSq = loc.getLengthSq();
		if (dSq < _joystickRadiusSq){
			this->updateVelocity(loc);
			return true;
		}
	}
	return false;
}

void SimpleJoystick::onTouchMoved(Touch *touch, Event *event)
{
	auto loc = this->convertTouchToNodeSpace(touch);
	this->updateVelocity(loc);
}

void SimpleJoystick::onTouchEnded(Touch *touch, Event *event)
{
	auto loc = Vec2::ZERO;
	if(!_isAutoCenter){
		loc = this->convertTouchToNodeSpace(touch);
	}
	this->updateVelocity(loc);
	_isActive = false;
}

void SimpleJoystick::onTouchCancelled(Touch *touch, Event *event)
{
	this->onTouchEnded(touch, event);
}

void SimpleJoystick::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	_keyAr[to_underlying(keyCode)] = true;
}

void SimpleJoystick::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	_keyAr[to_underlying(keyCode)] = false;

}

void SimpleJoystick::setThumbRadius(float r)
{
	_thumbRadius = r;
	_thumbRadiusSq = r * r;
}

void SimpleJoystick::setBackgroundSprite(cocos2d::Sprite *aSprite)
{
	if (_backgroundSprite){
		_backgroundSprite->removeFromParent();
	}
	_backgroundSprite = aSprite;
	this->addChild(_backgroundSprite, 0);
}

void SimpleJoystick::setThumbSprite(cocos2d::Sprite *aSprite)
{
	if (_thumbSprite){
		_thumbSprite->removeFromParent();
	}

	_thumbSprite = aSprite;
	this->addChild(_thumbSprite, 1);
}