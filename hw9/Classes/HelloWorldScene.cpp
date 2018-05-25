#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));


    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	CCDictionary* message = CCDictionary::createWithContentsOfFile("info.xml");    //读取xml文件，文件在Resources目录下
	//name
	auto name = message->valueForKey("name");    //根据key，获取value
	const char* myName = name->getCString();    //将value转化为字符串
	auto nameLabel = Label::createWithTTF(myName, "fonts/GB2312.ttf", 24);
	nameLabel->setColor(Color3B(255, 0, 0));
	//id
	auto id = message->valueForKey("id");    //根据key，获取value
	const char* myId = id->getCString();    //将value转化为字符串
    auto idLabel = Label::createWithTTF(myId, "fonts/Marker Felt.ttf", 24);
	//auto idLabel = Label::createWithTTF("16340158", "fonts/Marker Felt.ttf", 24);

	auto clickMeLabel = Label::createWithTTF("ClickMe", "fonts/Marker Felt.ttf", 40);
	//log(myId);
    if (nameLabel == nullptr || idLabel == nullptr || clickMeLabel == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // name position the label on the center of the screen
       nameLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - nameLabel->getContentSize().height));

	   // id position the label on the center of the screen
	   idLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		   origin.y + visibleSize.height - idLabel->getContentSize().height - nameLabel->getContentSize().height));

	   //clickme posistion the label on the lower right corner
	   clickMeLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		   origin.y + clickMeLabel->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(nameLabel, 1);
		this->addChild(idLabel, 2);
		this->addChild(clickMeLabel, 3);


		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch *t, Event *e) {
			if (clickMeLabel->cocos2d::Node::getBoundingBox().containsPoint(t->getLocation()) && clickMeLabel->getPosition()!= Point(100,100))
			{
				//cocos2d::Node:: 这个不能省略  
				clickMeLabel->runAction(MoveTo::create(1, Vec2(100,100)));
			}
			else {
				clickMeLabel->runAction(MoveTo::create(1, Vec2(origin.x + visibleSize.width / 2,origin.y + clickMeLabel->getContentSize().height)));
			}
			return false;
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, clickMeLabel);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("earth.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
