#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void updateTime(float dt);

	void Xfun(cocos2d::Ref * pSpender);

	void Yfun(cocos2d::Ref * pSpender);

	void MoveUp(cocos2d::Ref * pSender);

	void MoveDown(cocos2d::Ref * pSender);

	void MoveRight(cocos2d::Ref * pSender);

	void MoveLeft(cocos2d::Ref * pSender);
        
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::ProgressTimer* pT;
	bool acting;
};
