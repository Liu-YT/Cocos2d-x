#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	//good miner text
	auto title = Sprite::create("gold-miner-text.png");
	title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 160));
	this->addChild(title, 2);
	
	//mouth
	auto mouth = Sprite::createWithSpriteFrameName("miner-face-whistle-0.png");
	Animate* mouthAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouthAnimation"));
	mouth->runAction(RepeatForever::create(mouthAnimate));
	mouth->setPosition(Vec2(161 + origin.x, visibleSize.height / 2 + 45 + origin.y));
	this->addChild(mouth, 1);
	
	//leg
	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(114 + origin.x, origin.y + 98.5);
	this->addChild(leg, 1);

	//stone
	auto gold = Sprite::create("menu-start-gold.png");
	gold->setPosition(visibleSize.width - 200 + origin.x, origin.y + 120);
	this->addChild(gold, 2);

	//start MenuItem
	auto startMenuItem = MenuItemImage::create(
		"start-0.png",
		"start-1.png",
		CC_CALLBACK_1(MenuScene::start, this));
	startMenuItem->setPosition(Point(760, 175));
	auto menu = Menu::create(startMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);
	
	//cave
	auto cave = Sprite::createWithSpriteFrameName("cave-0.png");
	Animate* caveAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("caveAnimation"));
	cave->runAction(RepeatForever::create(caveAnimate));
	cave->setPosition(Vec2(670 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(cave, 1);
	return true;
}

void MenuScene::start(Ref* pSender) {
	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene, Color3B(255,255,255)));
}
